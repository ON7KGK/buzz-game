/**
 * @file main.cpp
 * @brief Programme principal - Compteur tactile avec audio
 *
 * Architecture modulaire pour projets SPARKOH!
 * - Toute l'initialisation hardware est dans include/init.h
 * - Ce fichier contient UNIQUEMENT la logique applicative
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 * @version 2.0.0
 */

#include <Arduino.h>
#include "init.h"
#include "fonts.h"

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX (définis dans init.h)
// ═══════════════════════════════════════════════════════════════════════════

Display display;
AudioDriver audio;
SDCard sd;
Touch touch;

// ═══════════════════════════════════════════════════════════════════════════
// VARIABLES GLOBALES
// ═══════════════════════════════════════════════════════════════════════════

int compteur = 0;

// ═══════════════════════════════════════════════════════════════════════════
// FONCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void afficherCompteur() {
    display.clear(BLACK);
    auto canvas = display.getCanvas();

    // Texte jaune, grosse police
    canvas->setTextColor(YELLOW);
    canvas->setFont(&FreeSansBold24pt7b);

    // Convertir le compteur en string
    char texte[16];
    sprintf(texte, "%d", compteur);

    // Calculer la position pour centrer
    int16_t x1, y1;
    uint16_t w, h;
    canvas->setTextSize(4);
    canvas->getTextBounds(texte, 0, 0, &x1, &y1, &w, &h);

    // Centrer horizontalement et verticalement
    int16_t x = (480 - w) / 2 - x1;
    int16_t y = (320 / 2) + (h / 2) - y1;

    canvas->setCursor(x, y);
    canvas->setTextSize(4);
    canvas->print(texte);

    display.flush();
}

// ═══════════════════════════════════════════════════════════════════════════
// SETUP - INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    if (!initHardware()) {
        while(1) { delay(1000); }
    }

    afficherCompteur();
}

// ═══════════════════════════════════════════════════════════════════════════
// LOOP - BOUCLE PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
    if (touch.isTouched()) {
        compteur++;
        afficherCompteur();
        audio.play("/audio/gagne1.mp3");
    }

    audio.loop();
    delay(10);
}
