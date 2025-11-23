/**
 * @file main.cpp
 * @brief Programme principal - LOGIQUE UNIQUEMENT
 *
 * Architecture modulaire pour projets SPARKOH!
 * - Configuration dans include/config/
 * - Drivers dans include/drivers/
 * - Features activables dans include/features.h
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 * @version 2.0.0
 */

#include <Arduino.h>
#include <Wire.h>
#include "features.h"
#include "config/hardware_config.h"
#include "config/i2c_config.h"
#include "drivers/Display.h"
#include "drivers/Audio.h"
#include "drivers/SDCard.h"

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX (gérés par les drivers)
// ═══════════════════════════════════════════════════════════════════════════

Display display;
AudioDriver audio;
SDCard sd;

// ═══════════════════════════════════════════════════════════════════════════
// SETUP - INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    // ─────────────────────────────────────────────────────────────────────
    // 1. INITIALISATION SÉRIE
    // ─────────────────────────────────────────────────────────────────────
    Serial.begin(115200);
    delay(1000);

    #if SHOW_STARTUP_BANNER
    Serial.println("\n");
    Serial.println("╔═══════════════════════════════════════════════════════════╗");
    Serial.println("║                                                           ║");
    Serial.println("║        SPARKOH! - ESP32-S3 Touch LCD 3.5\"                ║");
    Serial.println("║        Architecture Modulaire v2.0                       ║");
    Serial.println("║                                                           ║");
    Serial.println("╚═══════════════════════════════════════════════════════════╝");
    Serial.println();
    Serial.printf("Module: %s\n", MODULE_NAME);
    Serial.printf("Version: %s\n", MODULE_VERSION);
    Serial.printf("MCU: %s @ %dMHz\n", MCU_MODEL, MCU_FREQ_MHZ);
    Serial.printf("Flash: %dMB | PSRAM: %dMB\n", FLASH_SIZE_MB, PSRAM_SIZE_MB);
    Serial.println();
    #endif

    // ─────────────────────────────────────────────────────────────────────
    // 2. INITIALISATION DISPLAY
    // ─────────────────────────────────────────────────────────────────────
    #if FEATURE_DISPLAY_ENABLED
    Serial.println("🖥️  Initialisation Display...");
    if (display.begin()) {
        Serial.println("✅ Display OK");
    } else {
        Serial.println("❌ Échec Display");
        while(1) { delay(1000); }  // Blocage si display critique
    }
    #endif

    // ─────────────────────────────────────────────────────────────────────
    // 3. INITIALISATION CARTE SD
    // ─────────────────────────────────────────────────────────────────────
    #if FEATURE_SD_ENABLED
    Serial.println("💾 Initialisation Carte SD...");
    if (sd.begin()) {
        Serial.println("✅ Carte SD OK");
        #if SD_DEBUG_ENABLED
        sd.listDir("/audio", 0);
        #endif
    } else {
        Serial.println("⚠️  Carte SD non disponible");
    }
    #endif

    // ─────────────────────────────────────────────────────────────────────
    // 4. INITIALISATION I2C (pour ES8311 et autres périphériques)
    // ─────────────────────────────────────────────────────────────────────
    Serial.println("🔌 Initialisation I2C...");
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.println("✅ I2C OK");

    // ─────────────────────────────────────────────────────────────────────
    // 5. INITIALISATION AUDIO
    // ─────────────────────────────────────────────────────────────────────
    #if FEATURE_AUDIO_ENABLED
    Serial.println("🔊 Initialisation Audio...");
    if (audio.begin()) {
        Serial.println("✅ Audio OK");
        audio.setVolume(DEFAULT_AUDIO_VOLUME);
    } else {
        Serial.println("⚠️  Audio non disponible");
    }
    #endif

    Serial.println();
    Serial.println("═══════════════════════════════════════════════════════════");
    Serial.println("  INITIALISATION TERMINÉE");
    Serial.println("═══════════════════════════════════════════════════════════");
    Serial.println();

    // ═════════════════════════════════════════════════════════════════════
    // LOGIQUE DE L'APPLICATION
    // ═════════════════════════════════════════════════════════════════════

    // 1. Afficher écran blanc
    display.clear(COLOR_WHITE);
    Serial.println("✓ Écran blanc affiché");

    delay(500);

    // 2. Afficher "Coucou" en noir
    display.printCentered("Coucou", SCREEN_HEIGHT / 2, COLOR_BLACK, 4);
    Serial.println("✓ Texte 'Coucou' affiché");

    delay(500);

    // 3. Jouer le fichier audio
    #if FEATURE_AUDIO_ENABLED && FEATURE_SD_ENABLED
    if (sd.exists("/audio/tada.mp3")) {
        Serial.println("♪ Lecture de /audio/tada.mp3");
        audio.play("/audio/tada.mp3");
    } else {
        Serial.println("⚠️  Fichier /audio/tada.mp3 non trouvé");
        Serial.println("   Assurez-vous que le fichier existe sur la carte SD");
    }
    #endif

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
    // ═════════════════════════════════════════════════════════════════════
    // LOGIQUE DE VOTRE APPLICATION ICI
    // ═════════════════════════════════════════════════════════════════════

    // Mettre à jour la lecture audio
    #if FEATURE_AUDIO_ENABLED
    audio.loop();
    #endif

    // Votre code ici...
    // Exemples:
    // - Lire le tactile
    // - Gérer des boutons
    // - Afficher des animations
    // - Lire des capteurs
    // - Envoyer des données
    // etc.

    delay(10);  // Délai pour ne pas surcharger le CPU
}
