/**
 * @file main.cpp
 * @brief Programme principal - LOGIQUE UNIQUEMENT
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

// ═══════════════════════════════════════════════════════════════════════════
// SETUP - INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    // Initialiser tout le hardware (voir include/init.h)
    if (!initHardware()) {
        while(1) { delay(1000); }
    }

    // ─────────────────────────────────────────────────────────────────────
    // LOGIQUE APPLICATIVE - Démo des polices
    // ─────────────────────────────────────────────────────────────────────

    display.clear(WHITE);
    auto canvas = display.getCanvas();
    canvas->setTextColor(BLACK);
    int16_t y = 20;

    // Sans-Serif (moderne, lisible)
    canvas->setFont(&FreeSans9pt7b);
    canvas->setCursor(10, y); y += 25;
    canvas->println("FreeSans 9pt - Moderne");

    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setCursor(10, y); y += 30;
    canvas->println("FreeSansBold 12pt");

    canvas->setFont(&FreeSansBold18pt7b);
    canvas->setCursor(10, y); y += 40;
    canvas->println("Bold 18pt");

    canvas->setFont(&FreeSansBold24pt7b);
    canvas->setCursor(10, y); y += 50;
    canvas->println("Bold 24pt");

    // Serif (classique, élégant)
    y += 10;
    canvas->setFont(&FreeSerif12pt7b);
    canvas->setCursor(10, y); y += 30;
    canvas->println("FreeSerif 12pt - Classique");

    canvas->setFont(&FreeSerifBold18pt7b);
    canvas->setCursor(10, y); y += 40;
    canvas->println("SerifBold 18pt");

    // Mono (largeur fixe, code)
    y += 10;
    canvas->setFont(&FreeMono9pt7b);
    canvas->setCursor(10, y); y += 25;
    canvas->println("FreeMono 9pt - Code 123");

    canvas->setFont(&FreeMonoBold12pt7b);
    canvas->setCursor(10, y); y += 30;
    canvas->println("MonoBold 12pt");

    display.flush();

    // Jouer un son
    delay(2000);
    audio.play("/audio/touchette8.mp3");
}

// ═══════════════════════════════════════════════════════════════════════════
// LOOP - BOUCLE PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
    // Mettre à jour la lecture audio (nécessaire pour ESP32-audioI2S)
    audio.loop();

    // Petit délai pour éviter de saturer le CPU
    delay(1);
}
