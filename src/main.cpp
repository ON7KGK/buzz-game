/**
 * @file main.cpp
 * @brief Buzz Wire Game - Jeu d'adresse électronique
 *
 * Le joueur doit déplacer un anneau métallique le long d'un serpentin
 * sans le toucher. Si le contact se produit, le joueur perd.
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 * @version 3.0.0
 */

#include <Arduino.h>
#include "init.h"
#include "fonts.h"
#include "drivers/LEDStrip.h"
#include "config/display_config.h"

// Définition du rouge plus lumineux (RGB565: rouge max + un peu de vert)
#define BRIGHT_RED RGB565(255, 32, 0)

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX
// ═══════════════════════════════════════════════════════════════════════════

Display display;
AudioDriver audio;
SDCard sd;
Touch touch;

// Bandeaux LED SK9822
LEDStrip led1(21, 38, 60);  // LED1: GPIO21 (DI), GPIO38 (CI), 60 LEDs
LEDStrip led2(39, 40, 60);  // LED2: GPIO39 (DI), GPIO40 (CI), 60 LEDs - Côté 1 du carré
LEDStrip led3(41, 42, 60);  // LED3: GPIO41 (DI), GPIO42 (CI), 60 LEDs - Côté 2 du carré
LEDStrip led4(45, 46, 60);  // LED4: GPIO45 (DI), GPIO46 (CI), 60 LEDs - Côté 3 du carré

// ═══════════════════════════════════════════════════════════════════════════
// CONFIGURATION GPIO
// ═══════════════════════════════════════════════════════════════════════════

#define PIN_PLOT_GAUCHE    17   // Plot de départ gauche (broche 16)
#define PIN_PLOT_DROIT     18   // Plot de départ droit (broche 18)
#define PIN_ANNEAU         43   // Anneau métallique - touchette (broche 27 - TX)

// ═══════════════════════════════════════════════════════════════════════════
// ÉTATS DU JEU
// ═══════════════════════════════════════════════════════════════════════════

enum EtatJeu {
    ATTENTE_DEMARRAGE,  // En attente du positionnement du manche
    PRET_GAUCHE,        // Manche positionné à gauche, prêt à démarrer
    PRET_DROIT,         // Manche positionné à droite, prêt à démarrer
    JEU_EN_COURS,       // Jeu en cours
    VICTOIRE,           // Joueur a gagné
    DEFAITE,            // Joueur a perdu (touché le serpentin)
    TIMEOUT             // Temps écoulé (120 secondes)
};

// ═══════════════════════════════════════════════════════════════════════════
// VARIABLES GLOBALES
// ═══════════════════════════════════════════════════════════════════════════

EtatJeu etatActuel = ATTENTE_DEMARRAGE;
EtatJeu etatPrecedent = ATTENTE_DEMARRAGE; // Pour détecter les changements d'état
uint8_t coteDepart = 0;           // 0=aucun, 1=gauche, 2=droite
unsigned long compteur = 0;        // Compteur en millisecondes
unsigned long tempsDebut = 0;      // Temps du début du jeu
unsigned long tempsMessage = 0;    // Temps d'affichage des messages
unsigned long tempsRainbow = 0;    // Temps pour animation rainbow LED2
uint8_t luminositeLED1 = 200;      // Luminosité LED1 (0-255, ajustable)
uint8_t luminositeLED234 = 200;   // Luminosité LED2-4 (0-255, ajustable)

const unsigned long TIMEOUT_JEU = 60000;         // 60 secondes
const unsigned long DELAI_MESSAGE = 7000;        // ⚙️ CONFIGURABLE: Délai avant redémarrage auto (en ms)
const unsigned long DELAI_ABANDON = 2000;        // 2 secondes pour détecter abandon
const unsigned long INTERVALLE_RAINBOW = 1;      // 1ms entre mises à jour rainbow (vitesse LED2)

// Configuration du moniteur série
const bool MONITEUR_ACTIF = true;                // true = affichage des infos de debug, false = désactivé

unsigned long tempsAbandon = 0;    // Timer pour détecter manche abandonné
bool ecranAffiche = false;         // Flag pour éviter le clignotement
bool messageRejouerAffiche = false; // Flag pour message "Pour rejouer"
bool touchActif = false;           // Flag pour activer la détection touch après relâchement

// Variables pour l'effet de pulsation LED1 verte (victoire)
unsigned long tempsPulsation = 0;  // Timer pour la pulsation
uint8_t luminositeVictoire = 30;   // Luminosité actuelle pour la pulsation
bool pulsationMontante = true;     // Direction de la pulsation

// ═══════════════════════════════════════════════════════════════════════════
// FONCTIONS D'AFFICHAGE
// ═══════════════════════════════════════════════════════════════════════════

// Convertit une chaîne UTF-8 en ISO-8859-1 (Latin-1) pour l'affichage
String utf8ToLatin1(const char* utf8) {
    String result = "";
    const uint8_t* str = (const uint8_t*)utf8;

    while (*str) {
        if (*str < 0x80) {
            // ASCII standard (0x00-0x7F)
            result += (char)*str;
            str++;
        } else if ((*str & 0xE0) == 0xC0) {
            // Caractère UTF-8 sur 2 octets (0xC0-0xDF)
            uint16_t unicode = ((*str & 0x1F) << 6) | (*(str + 1) & 0x3F);
            if (unicode < 0x100) {
                result += (char)unicode;
            }
            str += 2;
        } else if ((*str & 0xF0) == 0xE0) {
            // Caractère UTF-8 sur 3 octets (ignoré pour Latin-1)
            str += 3;
        } else {
            // Autre encodage, ignorer
            str++;
        }
    }
    return result;
}

void afficherTexte(const char* texte1, const char* texte2 = nullptr) {
    display.clear(BLACK);
    auto canvas = display.getCanvas();

    canvas->setTextColor(YELLOW);
    canvas->setFont(&FreeSansBold18ptAccents7b);
    canvas->setTextSize(1);

    // Convertir UTF-8 vers Latin-1
    String ligne1 = utf8ToLatin1(texte1);

    // Afficher première ligne centrée
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(ligne1.c_str(), 0, 0, &x1, &y1, &w, &h);
    int16_t x = (480 - w) / 2 - x1;
    int16_t y = texte2 ? 140 : 160;  // Plus haut si 2 lignes
    canvas->setCursor(x, y);
    canvas->print(ligne1);

    // Afficher deuxième ligne si présente
    if (texte2) {
        String ligne2 = utf8ToLatin1(texte2);
        canvas->getTextBounds(ligne2.c_str(), 0, 0, &x1, &y1, &w, &h);
        x = (480 - w) / 2 - x1;
        y = 180;
        canvas->setCursor(x, y);
        canvas->print(ligne2);
    }

    display.flush();
}

void afficherCompteur() {
    // Convertir le compteur en secondes et dixièmes
    unsigned long secondes = compteur / 1000;
    unsigned long dixiemes = (compteur % 1000) / 100;
    static unsigned long derniereDixieme = 9999; // Valeur impossible au départ

    // Calculer une valeur combinée pour détecter les changements
    unsigned long valeurActuelle = secondes * 10 + dixiemes;

    // N'afficher que si le dixième a changé (évite le clignotement)
    if (valeurActuelle == derniereDixieme) {
        return;
    }
    derniereDixieme = valeurActuelle;

    auto canvas = display.getCanvas();

    // Effacer toute la zone centrale (rectangle noir fixe)
    canvas->fillRect(0, 60, 480, 200, BLACK);

    // Police FreeSansBold72pt7b pour le compteur
    canvas->setFont(&FreeSansBold72pt7b);
    canvas->setTextSize(1);

    // Format du texte à afficher
    char texte[16];
    sprintf(texte, "%02lu.%lu", secondes, dixiemes);

    int16_t x1, y1;
    uint16_t w, h;

    // Avant 10 secondes: masquer le premier 0 et décaler vers la gauche
    if (secondes < 10) {
        // Calculer position basée sur "9.9"
        canvas->getTextBounds("9.9", 0, 0, &x1, &y1, &w, &h);
        int16_t x = (480 - w) / 2 - x1;
        int16_t y = (320 - h) / 2 - y1;

        // Calculer la largeur d'un caractère "0"
        int16_t x1_zero, y1_zero;
        uint16_t w_zero, h_zero;
        canvas->getTextBounds("0", 0, 0, &x1_zero, &y1_zero, &w_zero, &h_zero);

        // Décaler la position de départ d'un caractère vers la gauche
        x -= (w_zero - x1_zero);

        // Afficher le premier 0 en noir (masqué)
        canvas->setTextColor(BLACK);
        canvas->setCursor(x, y);
        canvas->print("0");

        // Afficher le reste en jaune, décalé d'un caractère vers la gauche
        canvas->setTextColor(YELLOW);
        canvas->setCursor(x + w_zero - x1_zero, y);
        canvas->print(&texte[1]);  // Afficher à partir du 2e caractère
    }
    // Après 10 secondes: afficher normalement
    else {
        canvas->getTextBounds("00.0", 0, 0, &x1, &y1, &w, &h);
        int16_t x = (480 - w) / 2 - x1;
        int16_t y = (320 - h) / 2 - y1;

        canvas->setTextColor(YELLOW);
        canvas->setCursor(x, y);
        canvas->print(texte);
    }

    display.flush();
}

void afficherDefaite() {
    display.clear(BLACK);
    auto canvas = display.getCanvas();

    canvas->setTextColor(BRIGHT_RED);  // Utiliser rouge plus lumineux
    canvas->setFont(&FreeSansBold18ptAccents7b);
    canvas->setTextSize(1);

    // Message "OH! non tu as touché" centré verticalement et horizontalement
    String message = utf8ToLatin1("OH! non tu as touché");
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(message.c_str(), 0, 0, &x1, &y1, &w, &h);
    int16_t x = (480 - w) / 2 - x1;
    int16_t y = (320 - h) / 2 - y1;  // Centré verticalement
    canvas->setCursor(x, y);
    canvas->print(message);

    display.flush();
}

void afficherTimeout() {
    display.clear(BLACK);
    auto canvas = display.getCanvas();

    canvas->setTextColor(BRIGHT_RED);  // Utiliser rouge plus lumineux
    canvas->setFont(&FreeSansBold18ptAccents7b);
    canvas->setTextSize(1);

    // Message "Le temps est écoulé" centré verticalement et horizontalement
    String message = utf8ToLatin1("Le temps est écoulé");
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(message.c_str(), 0, 0, &x1, &y1, &w, &h);
    int16_t x = (480 - w) / 2 - x1;
    int16_t y = (320 - h) / 2 - y1;  // Centré verticalement
    canvas->setCursor(x, y);
    canvas->print(message);

    display.flush();
}

void afficherVictoire(unsigned long secondes, unsigned long dixiemes) {
    display.clear(BLACK);
    auto canvas = display.getCanvas();

    canvas->setTextColor(GREEN);

    // Grand "BRAVO !" en haut (police 54pt)
    canvas->setFont(&FreeSansBold54pt7b);
    canvas->setTextSize(1);

    String bravo = utf8ToLatin1("BRAVO !");
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(bravo.c_str(), 0, 0, &x1, &y1, &w, &h);
    int16_t x = (480 - w) / 2 - x1;
    int16_t y = 90 - y1;  // Première partie de la hauteur
    canvas->setCursor(x, y);
    canvas->print(bravo);

    // Message temps en dessous (police 18pt)
    canvas->setFont(&FreeSansBold18ptAccents7b);
    canvas->setTextSize(1);
    char messageTemps[64];
    sprintf(messageTemps, "Tu as gagné en %lu.%lu s", secondes, dixiemes);
    String temps = utf8ToLatin1(messageTemps);
    canvas->getTextBounds(temps.c_str(), 0, 0, &x1, &y1, &w, &h);
    x = (480 - w) / 2 - x1;
    canvas->setCursor(x, 220);  // Plus bas que le BRAVO!
    canvas->print(temps);

    display.flush();
}

// ═══════════════════════════════════════════════════════════════════════════
// FONCTIONS DE CONTRÔLE LED
// ═══════════════════════════════════════════════════════════════════════════

void led1Blanc() {
    led1.setBrightness(luminositeLED1);
    led1.fill(255, 255, 255);
    led1.show();
}

void led1Vert() {
    led1.setBrightness(255);
    led1.fill(0, 255, 0);
    led1.show();
}

void led1Rouge() {
    led1.setBrightness(255);
    led1.fill(255, 0, 0);
    led1.show();
}

void led1Bleu() {
    led1.setBrightness(luminositeLED1);
    led1.fill(0, 0, 255);
    led1.show();
}

// Effet de pulsation pour la LED1 verte (victoire)
void pulserLED1Vert() {
    unsigned long maintenant = millis();

    // Mise à jour toutes les 5ms pour un effet plus rapide
    if (maintenant - tempsPulsation >= 5) {
        tempsPulsation = maintenant;

        // Augmenter ou diminuer la luminosité
        if (pulsationMontante) {
            luminositeVictoire += 10;  // Vitesse de montée
            if (luminositeVictoire >= 250) {
                luminositeVictoire = 250;
                pulsationMontante = false;  // Inverser la direction
            }
        } else {
            luminositeVictoire -= 10;  // Vitesse de descente
            if (luminositeVictoire <= 30) {
                luminositeVictoire = 30;
                pulsationMontante = true;  // Inverser la direction
            }
        }

        // Appliquer la luminosité
        led1.setBrightness(luminositeVictoire);
        led1.fill(0, 255, 0);
        led1.show();
    }
}

// Variable globale pour le rainbow synchronisé (partagée entre LED2-4)
uint16_t rainbowHueGlobal = 0;

void mettreAJourLED2() {
    // Animation rainbow continue pour LED2, LED3, LED4 (3 côtés du carré)
    unsigned long maintenant = millis();
    if (maintenant - tempsRainbow >= 3) {  // Plus rapide : 3ms
        tempsRainbow = maintenant;

        // Incrémenter la teinte globale
        rainbowHueGlobal += 256;
        if (rainbowHueGlobal >= 65536) {
            rainbowHueGlobal = 0;
        }

        // Total de LEDs pour le carré (3 côtés × 60 LEDs)
        const uint16_t totalLeds = 180;

        // Fonction wheel locale
        auto wheel = [](byte wheelPos) -> uint32_t {
            wheelPos = 255 - wheelPos;
            if (wheelPos < 85) {
                return ((255 - wheelPos * 3) << 16) | (0 << 8) | (wheelPos * 3);
            }
            if (wheelPos < 170) {
                wheelPos -= 85;
                return ((0) << 16) | ((wheelPos * 3) << 8) | (255 - wheelPos * 3);
            }
            wheelPos -= 170;
            return ((wheelPos * 3) << 16) | ((255 - wheelPos * 3) << 8) | (0);
        };

        // Appliquer luminosité aux LED2-4
        led2.setBrightness(luminositeLED234);
        led3.setBrightness(luminositeLED234);
        led4.setBrightness(luminositeLED234);

        // LED2 - Côté 1 (LEDs 0-59)
        for (uint16_t i = 0; i < 60; i++) {
            uint16_t pixelHue = rainbowHueGlobal + (i * 65536L / totalLeds);
            uint8_t wheelPos = (pixelHue >> 8) & 0xFF;
            uint32_t color = wheel(wheelPos);
            led2.setPixel(i, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
        }

        // LED3 - Côté 2 (LEDs 60-119)
        for (uint16_t i = 0; i < 60; i++) {
            uint16_t pixelHue = rainbowHueGlobal + ((i + 60) * 65536L / totalLeds);
            uint8_t wheelPos = (pixelHue >> 8) & 0xFF;
            uint32_t color = wheel(wheelPos);
            led3.setPixel(i, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
        }

        // LED4 - Côté 3 (LEDs 120-179)
        for (uint16_t i = 0; i < 60; i++) {
            uint16_t pixelHue = rainbowHueGlobal + ((i + 120) * 65536L / totalLeds);
            uint8_t wheelPos = (pixelHue >> 8) & 0xFF;
            uint32_t color = wheel(wheelPos);
            led4.setPixel(i, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
        }

        // Afficher tous les strips en même temps
        led2.show();
        led3.show();
        led4.show();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// MACHINE À ÉTATS - GESTION DU JEU
// ═══════════════════════════════════════════════════════════════════════════

void gererAttenteDepart() {
    bool pinGauche = digitalRead(PIN_PLOT_GAUCHE);
    bool pinDroit = digitalRead(PIN_PLOT_DROIT);

    // VMA452: HIGH = contact, LOW = libre
    // Cas 1: Anneau touche plot gauche
    if (pinGauche == HIGH) {
        delay(10);  // Debounce
        if (digitalRead(PIN_PLOT_GAUCHE) == HIGH) {
            coteDepart = 1;
            etatActuel = PRET_GAUCHE;
            led1Bleu();
            afficherTexte("Rejoins l'autre côté", "sans toucher");
            tempsAbandon = 0;
            ecranAffiche = false;
            return;
        }
    }

    // Cas 2: Anneau touche plot droit
    if (pinDroit == HIGH) {
        delay(10);  // Debounce
        if (digitalRead(PIN_PLOT_DROIT) == HIGH) {
            coteDepart = 2;
            etatActuel = PRET_DROIT;
            led1Bleu();
            afficherTexte("Rejoins l'autre côté", "sans toucher");
            tempsAbandon = 0;
            ecranAffiche = false;
            return;
        }
    }

    // Cas 3: Anneau libre (tous LOW) - afficher message après délai
    if (pinGauche == LOW && pinDroit == LOW) {
        if (tempsAbandon == 0) {
            tempsAbandon = millis();
            ecranAffiche = false;
        } else if (millis() - tempsAbandon >= DELAI_ABANDON && !ecranAffiche) {
            afficherTexte("Pour jouer, place le manche", "à gauche ou à droite");
            ecranAffiche = true;
        }
    } else {
        tempsAbandon = 0;
    }
}

void gererPretGauche() {
    bool pinGauche = digitalRead(PIN_PLOT_GAUCHE);

    // Le joueur a soulevé le manche du plot gauche (optocoupleur désactivé = LOW)
    if (pinGauche == LOW) {
        etatActuel = JEU_EN_COURS;
        tempsDebut = millis();
        compteur = 0;
        display.clear(BLACK);  // Effacer l'écran au début du jeu
        afficherCompteur();
    }
}

void gererPretDroit() {
    bool pinDroit = digitalRead(PIN_PLOT_DROIT);

    // Le joueur a soulevé le manche du plot droit (optocoupleur désactivé = LOW)
    if (pinDroit == LOW) {
        etatActuel = JEU_EN_COURS;
        tempsDebut = millis();
        compteur = 0;
        display.clear(BLACK);  // Effacer l'écran au début du jeu
        afficherCompteur();
    }
}

void gererJeuEnCours() {
    // Mettre à jour le compteur
    compteur = millis() - tempsDebut;

    // Délai de stabilisation: ignorer les détections pendant les 100 premières ms
    if (compteur < 100) {
        // Afficher le compteur mais ne pas détecter de touchette
        static unsigned long dernierAffichage = 0;
        if (millis() - dernierAffichage >= 100) {
            dernierAffichage = millis();
            afficherCompteur();
        }
        return;
    }

    bool pinGauche = digitalRead(PIN_PLOT_GAUCHE);
    bool pinDroit = digitalRead(PIN_PLOT_DROIT);
    bool pinAnneau = digitalRead(PIN_ANNEAU);

    // Scénario B: Touché le serpentin (défaite) - optocoupleur activé = HIGH
    // Debounce: vérifier 2 fois à 10ms d'intervalle pour éviter les faux positifs
    if (pinAnneau == HIGH) {
        delay(10);  // Attendre 10ms
        if (digitalRead(PIN_ANNEAU) == HIGH) {  // Vérifier à nouveau
            etatActuel = DEFAITE;
            tempsMessage = millis();
            messageRejouerAffiche = false; // Réinitialiser pour nouveau message
            led1Rouge();
            audio.play("/audio/touchette7.mp3");
            afficherDefaite();
            return;
        }
    }

    // Scénario C: Timeout 60 secondes
    if (compteur >= TIMEOUT_JEU) {
        etatActuel = TIMEOUT;
        tempsMessage = millis();
        messageRejouerAffiche = false; // Réinitialiser pour nouveau message
        led1Rouge();
        audio.play("/audio/erreur.mp3");
        afficherTimeout();
        return;
    }

    // Scénario A: Victoire (arrivée au côté opposé) - optocoupleur activé = HIGH
    bool victoire = false;
    if (coteDepart == 1 && pinDroit == HIGH) {
        victoire = true;  // Parti de gauche, arrivé à droite
    } else if (coteDepart == 2 && pinGauche == HIGH) {
        victoire = true;  // Parti de droite, arrivé à gauche
    }

    if (victoire) {
        etatActuel = VICTOIRE;
        tempsMessage = millis();
        messageRejouerAffiche = false; // Réinitialiser pour nouveau message
        led1Vert();
        audio.play("/audio/gagne2.mp3");

        unsigned long secondes = compteur / 1000;
        unsigned long dixiemes = (compteur % 1000) / 100;
        afficherVictoire(secondes, dixiemes);
        return;
    }

    // Afficher le compteur en temps réel
    static unsigned long dernierAffichage = 0;
    if (millis() - dernierAffichage >= 100) {  // Mise à jour tous les 100ms
        dernierAffichage = millis();
        afficherCompteur();
    }
}

void gererFinDePartie() {
    // Si victoire, animer la LED1 verte avec effet de pulsation
    if (etatActuel == VICTOIRE) {
        pulserLED1Vert();
    }

    // Attendre le délai configuré puis redémarrer automatiquement
    if (millis() - tempsMessage >= DELAI_MESSAGE) {
        // Éteindre rouge/vert, rallumer blanc
        led1Blanc();

        // Réinitialiser les variables
        etatActuel = ATTENTE_DEMARRAGE;
        coteDepart = 0;
        compteur = 0;
        tempsAbandon = 0;
        ecranAffiche = false;
        messageRejouerAffiche = false;

        // Réinitialiser les variables de pulsation
        luminositeVictoire = 30;
        pulsationMontante = true;

        afficherTexte("Pour jouer, place le manche", "à gauche ou à droite");
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// SETUP - INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    // Initialiser le hardware
    if (!initHardware()) {
        while(1) { delay(1000); }
    }

    // Initialiser les GPIO
    pinMode(PIN_PLOT_GAUCHE, INPUT_PULLUP);
    pinMode(PIN_PLOT_DROIT, INPUT_PULLUP);
    pinMode(PIN_ANNEAU, INPUT_PULLUP);

    // Initialiser les LEDs
    led1.begin();
    led2.begin();
    led3.begin();
    led4.begin();

    led1Blanc();
    // Initialiser le rainbow du carré (LED2-4) sera fait dans loop()

    // Afficher message initial
    afficherTexte("Pour jouer, place le manche", "à gauche ou à droite");

    Serial.println("=== BUZZ WIRE GAME ===");
    Serial.println("Systeme pret !");
}

// ═══════════════════════════════════════════════════════════════════════════
// LOOP - BOUCLE PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
    // Animation LED2 rainbow continue
    mettreAJourLED2();

    // Machine à états
    switch (etatActuel) {
        case ATTENTE_DEMARRAGE:
            gererAttenteDepart();
            break;

        case PRET_GAUCHE:
            gererPretGauche();
            break;

        case PRET_DROIT:
            gererPretDroit();
            break;

        case JEU_EN_COURS:
            gererJeuEnCours();
            break;

        case VICTOIRE:
        case DEFAITE:
        case TIMEOUT:
            gererFinDePartie();
            break;
    }

    // Boucle audio
    audio.loop();

    delay(10);
}
