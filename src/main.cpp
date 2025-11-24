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

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX
// ═══════════════════════════════════════════════════════════════════════════

Display display;
AudioDriver audio;
SDCard sd;
Touch touch;

// Bandeaux LED SK9822
LEDStrip led1(45, 46, 40);  // LED1: GPIO45 (DI), GPIO46 (CI), 40 LEDs
LEDStrip led2(47, 48, 40);  // LED2: GPIO47 (DI), GPIO48 (CI), 40 LEDs

// ═══════════════════════════════════════════════════════════════════════════
// CONFIGURATION GPIO
// ═══════════════════════════════════════════════════════════════════════════

#define PIN_PLOT_GAUCHE    38   // Plot de départ gauche
#define PIN_PLOT_DROIT     39   // Plot de départ droit
#define PIN_ANNEAU         40   // Anneau métallique (détecte contact avec serpentin GND)

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
uint8_t luminositeLED1 = 128;      // Luminosité LED1 (0-255, ajustable)

const unsigned long TIMEOUT_JEU = 60000;         // 60 secondes
const unsigned long DELAI_MESSAGE = 2000;        // 2 secondes pour messages
const unsigned long DELAI_ABANDON = 2000;        // 2 secondes pour détecter abandon
const unsigned long INTERVALLE_RAINBOW = 50;     // 50ms entre mises à jour rainbow

// Configuration du moniteur série
const bool MONITEUR_ACTIF = false;               // true = affichage des infos de debug, false = désactivé

unsigned long tempsAbandon = 0;    // Timer pour détecter manche abandonné
bool ecranAffiche = false;         // Flag pour éviter le clignotement
bool messageRejouerAffiche = false; // Flag pour message "Pour rejouer"

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
    static char dernierTexte[16] = "";

    // Calculer une valeur combinée pour détecter les changements
    unsigned long valeurActuelle = secondes * 10 + dixiemes;

    // N'afficher que si le dixième a changé (évite le clignotement)
    if (valeurActuelle == derniereDixieme) {
        return;
    }
    derniereDixieme = valeurActuelle;

    auto canvas = display.getCanvas();

    // Police FreeSansBold72pt7b pour le compteur
    canvas->setFont(&FreeSansBold72pt7b);
    canvas->setTextSize(1);

    char texte[16];
    sprintf(texte, "%lu.%lu", secondes, dixiemes);

    // Centrer parfaitement
    int16_t x1, y1;
    uint16_t w, h;

    // Effacer l'ancien texte en noir
    if (strlen(dernierTexte) > 0) {
        canvas->setTextColor(BLACK);
        canvas->getTextBounds(dernierTexte, 0, 0, &x1, &y1, &w, &h);
        int16_t x = (480 - w) / 2 - x1;
        int16_t y = (320 - h) / 2 - y1;
        canvas->setCursor(x, y);
        canvas->print(dernierTexte);
    }

    // Afficher le nouveau texte en jaune
    canvas->setTextColor(YELLOW);
    canvas->getTextBounds(texte, 0, 0, &x1, &y1, &w, &h);
    int16_t x = (480 - w) / 2 - x1;
    int16_t y = (320 - h) / 2 - y1;
    canvas->setCursor(x, y);
    canvas->print(texte);

    // Sauvegarder le texte actuel pour l'effacer la prochaine fois
    strcpy(dernierTexte, texte);

    display.flush();
}

void afficherResultat(const char* ligne1, const char* ligne2, const char* ligne3) {
    display.clear(BLACK);
    auto canvas = display.getCanvas();

    canvas->setTextColor(YELLOW);
    canvas->setFont(&FreeSansBold18ptAccents7b);
    canvas->setTextSize(1);

    // Ligne 1 (gros message)
    String l1 = utf8ToLatin1(ligne1);
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(l1.c_str(), 0, 0, &x1, &y1, &w, &h);
    int16_t x = (480 - w) / 2 - x1;
    canvas->setCursor(x, 120);
    canvas->print(l1);

    // Ligne 2 (temps ou message)
    if (ligne2) {
        String l2 = utf8ToLatin1(ligne2);
        canvas->getTextBounds(l2.c_str(), 0, 0, &x1, &y1, &w, &h);
        x = (480 - w) / 2 - x1;
        canvas->setCursor(x, 160);
        canvas->print(l2);
    }

    // Ligne 3 (instruction rejouer)
    if (ligne3) {
        String l3 = utf8ToLatin1(ligne3);
        canvas->getTextBounds(l3.c_str(), 0, 0, &x1, &y1, &w, &h);
        x = (480 - w) / 2 - x1;
        canvas->setCursor(x, 200);
        canvas->print(l3);
    }

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

void mettreAJourLED2() {
    // Animation rainbow continue pour LED2
    unsigned long maintenant = millis();
    if (maintenant - tempsRainbow >= INTERVALLE_RAINBOW) {
        tempsRainbow = maintenant;
        led2.updateRainbow();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// MACHINE À ÉTATS - GESTION DU JEU
// ═══════════════════════════════════════════════════════════════════════════

void gererAttenteDepart() {
    bool pinGauche = digitalRead(PIN_PLOT_GAUCHE);
    bool pinDroit = digitalRead(PIN_PLOT_DROIT);
    bool pinAnneau = digitalRead(PIN_ANNEAU);

    // Cas 1: Manche sur plot gauche
    if (pinGauche == LOW && pinDroit == HIGH) {
        coteDepart = 1;
        etatActuel = PRET_GAUCHE;
        ecranAffiche = false; // Réinitialiser le flag
        afficherTexte("Appuie quand tu es prêt");
        tempsAbandon = 0;
        return;
    }

    // Cas 2: Manche sur plot droit
    if (pinGauche == HIGH && pinDroit == LOW) {
        coteDepart = 2;
        etatActuel = PRET_DROIT;
        ecranAffiche = false; // Réinitialiser le flag
        afficherTexte("Appuie quand tu es prêt");
        tempsAbandon = 0;
        return;
    }

    // Cas 3: Manche abandonné (touche le serpentin)
    if (pinGauche == HIGH && pinDroit == HIGH && pinAnneau == LOW) {
        // Démarrer le timer d'abandon si pas encore fait
        if (tempsAbandon == 0) {
            tempsAbandon = millis();
            ecranAffiche = false; // Permettre un nouvel affichage
        }
        // Si abandonné pendant 2+ secondes, afficher UNE SEULE FOIS
        else if (millis() - tempsAbandon >= DELAI_ABANDON && !ecranAffiche) {
            afficherTexte("Pour jouer, place le manche", "à gauche ou à droite");
            ecranAffiche = true;
        }
    } else {
        tempsAbandon = 0;
        if (ecranAffiche) {
            ecranAffiche = false;
        }
    }
}

void gererPretGauche() {
    bool pinGauche = digitalRead(PIN_PLOT_GAUCHE);

    // Le joueur a soulevé le manche du plot gauche
    if (pinGauche == HIGH) {
        etatActuel = JEU_EN_COURS;
        tempsDebut = millis();
        compteur = 0;
        display.clear(BLACK);  // Effacer l'écran au début du jeu
        afficherCompteur();
    }
}

void gererPretDroit() {
    bool pinDroit = digitalRead(PIN_PLOT_DROIT);

    // Le joueur a soulevé le manche du plot droit
    if (pinDroit == HIGH) {
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

    bool pinGauche = digitalRead(PIN_PLOT_GAUCHE);
    bool pinDroit = digitalRead(PIN_PLOT_DROIT);
    bool pinAnneau = digitalRead(PIN_ANNEAU);

    // Scénario B: Touché le serpentin (défaite)
    if (pinAnneau == LOW) {
        etatActuel = DEFAITE;
        tempsMessage = millis();
        messageRejouerAffiche = false; // Réinitialiser pour nouveau message
        led1Rouge();
        if (MONITEUR_ACTIF) Serial.println("[AUDIO] Lecture: /audio/touchette7.mp3");
        audio.play("/audio/touchette7.mp3");
        afficherResultat("OH! non tu as touché", nullptr, nullptr);
        return;
    }

    // Scénario C: Timeout 60 secondes
    if (compteur >= TIMEOUT_JEU) {
        etatActuel = TIMEOUT;
        tempsMessage = millis();
        messageRejouerAffiche = false; // Réinitialiser pour nouveau message
        led1Rouge();
        if (MONITEUR_ACTIF) Serial.println("[AUDIO] Lecture: /audio/erreur.mp3");
        audio.play("/audio/erreur.mp3");
        afficherResultat("Le temps est écoulé", nullptr, nullptr);
        return;
    }

    // Scénario A: Victoire (arrivée au côté opposé)
    bool victoire = false;
    if (coteDepart == 1 && pinDroit == LOW) {
        victoire = true;  // Parti de gauche, arrivé à droite
    } else if (coteDepart == 2 && pinGauche == LOW) {
        victoire = true;  // Parti de droite, arrivé à gauche
    }

    if (victoire) {
        etatActuel = VICTOIRE;
        tempsMessage = millis();
        messageRejouerAffiche = false; // Réinitialiser pour nouveau message
        led1Vert();
        if (MONITEUR_ACTIF) Serial.println("[AUDIO] Lecture: /audio/gagne2.mp3");
        audio.play("/audio/gagne2.mp3");

        unsigned long secondes = compteur / 1000;
        unsigned long dixiemes = (compteur % 1000) / 100;
        char ligne1[64];
        sprintf(ligne1, "Bravo, tu as gagné en %lu.%lu s", secondes, dixiemes);
        afficherResultat(ligne1, nullptr, nullptr);
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
    // Attendre 2 secondes avant d'afficher "Pour rejouer"
    if (millis() - tempsMessage >= DELAI_MESSAGE) {
        // Ajouter le message "Pour rejouer" UNE SEULE FOIS
        if (!messageRejouerAffiche) {
            if (etatActuel == VICTOIRE) {
                unsigned long secondes = compteur / 1000;
                unsigned long dixiemes = (compteur % 1000) / 100;
                char ligne1[64];
                sprintf(ligne1, "Bravo, tu as gagné en %lu.%lu s", secondes, dixiemes);
                afficherResultat(ligne1, nullptr, "Pour rejouer appuie sur l'écran");
            } else if (etatActuel == DEFAITE) {
                afficherResultat("OH! non tu as touché", nullptr, "Pour rejouer, appuie sur l'écran");
            } else if (etatActuel == TIMEOUT) {
                afficherResultat("Le temps est écoulé", nullptr, "Pour rejouer appuie sur l'écran");
            }
            messageRejouerAffiche = true;
        }

        // Détecter le touch pour redémarrer
        if (touch.isTouched()) {
            // Éteindre rouge/vert, rallumer blanc
            led1Blanc();

            // Réinitialiser les variables
            etatActuel = ATTENTE_DEMARRAGE;
            coteDepart = 0;
            compteur = 0;
            tempsAbandon = 0;
            ecranAffiche = false;
            messageRejouerAffiche = false;

            afficherTexte("Pour jouer, place le manche", "à gauche ou à droite");
            delay(300);  // Debounce touch
        }
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

    led1Blanc();
    led2.updateRainbow();

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
