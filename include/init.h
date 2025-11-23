/**
 * @file init.h
 * @brief Gestion centralisée de toutes les initialisations matérielles
 *
 * Ce fichier encapsule TOUTE la complexité d'initialisation.
 * Le main.cpp reste UNIQUEMENT focalisé sur la logique applicative.
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef INIT_H
#define INIT_H

#include <Arduino.h>
#include <Wire.h>
#include "features.h"
#include "config/hardware_config.h"
#include "config/i2c_config.h"
#include "drivers/Display.h"
#include "drivers/Audio.h"
#include "drivers/SDCard.h"

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX (accessibles depuis main.cpp)
// ═══════════════════════════════════════════════════════════════════════════

extern Display display;
extern AudioDriver audio;
extern SDCard sd;

// ═══════════════════════════════════════════════════════════════════════════
// FONCTIONS D'INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Affiche la bannière de démarrage
 */
inline void initBanner() {
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
}

/**
 * @brief Initialise le port série
 */
inline void initSerial() {
    Serial.begin(115200);
    delay(1000);
}

/**
 * @brief Initialise l'écran LCD
 * @return true si succès, false sinon
 */
inline bool initDisplay() {
    #if FEATURE_DISPLAY_ENABLED
    Serial.println("🖥️  Initialisation Display...");
    if (display.begin()) {
        Serial.println("✅ Display OK");
        return true;
    } else {
        Serial.println("❌ Échec Display");
        return false;
    }
    #else
    return true;
    #endif
}

/**
 * @brief Initialise la carte SD
 * @return true si succès, false sinon
 */
inline bool initSDCard() {
    #if FEATURE_SD_ENABLED
    Serial.println("💾 Initialisation Carte SD...");
    if (sd.begin()) {
        Serial.println("✅ Carte SD OK");
        #if SD_DEBUG_ENABLED
        sd.listDir("/audio", 0);
        #endif
        return true;
    } else {
        Serial.println("⚠️  Carte SD non disponible");
        return false;
    }
    #else
    return true;
    #endif
}

/**
 * @brief Initialise le bus I2C
 */
inline void initI2C() {
    Serial.println("🔌 Initialisation I2C...");
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.println("✅ I2C OK");
}

/**
 * @brief Initialise l'audio (ES8311 + I2S)
 * @return true si succès, false sinon
 */
inline bool initAudio() {
    #if FEATURE_AUDIO_ENABLED
    Serial.println("🔊 Initialisation Audio...");
    if (audio.begin()) {
        Serial.println("✅ Audio OK");
        audio.setVolume(DEFAULT_AUDIO_VOLUME);
        return true;
    } else {
        Serial.println("⚠️  Audio non disponible");
        return false;
    }
    #else
    return true;
    #endif
}

/**
 * @brief Initialise TOUS les périphériques dans le bon ordre
 * @return true si tous les périphériques critiques sont OK
 */
inline bool initHardware() {
    initSerial();
    initBanner();

    // Display (critique)
    if (!initDisplay()) {
        Serial.println("❌ ERREUR CRITIQUE: Display requis");
        return false;
    }

    // Carte SD (non-critique)
    initSDCard();

    // I2C (critique pour audio)
    initI2C();

    // Audio (critique)
    if (!initAudio()) {
        Serial.println("⚠️  Audio désactivé");
    }

    Serial.println();
    Serial.println("═══════════════════════════════════════════════════════════");
    Serial.println("  INITIALISATION TERMINÉE");
    Serial.println("═══════════════════════════════════════════════════════════");
    Serial.println();

    return true;
}

#endif // INIT_H
