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
        Serial.println("❌ Initialisation échouée - arrêt");
        while(1) { delay(1000); }
    }

    // ─────────────────────────────────────────────────────────────────────
    // LOGIQUE APPLICATIVE - Test simple
    // ─────────────────────────────────────────────────────────────────────

    // 1. Afficher écran blanc + texte
    display.clear(WHITE);
    display.printCentered("Coucou", BLACK, 3);
    Serial.println("✓ Écran blanc affiché");
    Serial.println("✓ Texte 'Coucou' affiché");

    // 2. Jouer un fichier audio
    if (audio.play("/audio/tada.mp3")) {
        Serial.println("♪ Lecture de /audio/tada.mp3");
    } else {
        Serial.println("⚠️  Fichier /audio/tada.mp3 non trouvé");
        Serial.println("   Assurez-vous que le fichier existe sur la carte SD");
    }

    Serial.println();
    Serial.println("═══════════════════════════════════════════════════════════");
    Serial.println("  PRÊT - En attente...");
    Serial.println("═══════════════════════════════════════════════════════════");
    Serial.println();
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
