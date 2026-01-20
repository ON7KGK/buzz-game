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
#include "drivers/Touch.h"
#include "drivers/DFPlayer.h"

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX (accessibles depuis main.cpp)
// ═══════════════════════════════════════════════════════════════════════════

extern Display display;
extern AudioDriver audio;
extern SDCard sd;
extern Touch touch;
extern DFPlayerDriver dfplayer;

// ═══════════════════════════════════════════════════════════════════════════
// FONCTIONS D'INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

inline void initSerial() {
    Serial.begin(115200);
    delay(100);
}

inline bool initDisplay() {
    #if FEATURE_DISPLAY_ENABLED
    if (!display.begin()) {
        return false;
    }
    return true;
    #else
    return true;
    #endif
}

inline bool initSDCard() {
    #if FEATURE_SD_ENABLED
    return sd.begin();
    #else
    return true;
    #endif
}

inline void initI2C() {
    Wire.begin(I2C_SDA, I2C_SCL);
}

inline bool initAudio() {
    #if FEATURE_I2S_AUDIO_ENABLED
    // DÉLAI CRITIQUE pour stabilisation I2S/MAX98357A/ES8311
    // Sans ce délai, l'initialisation audio peut échouer aléatoirement
    // Testé et validé pour une fiabilité à 100%
    delay(1500);

    if (audio.begin()) {
        // Délai post-initialisation pour stabiliser l'ampli
        delay(800);
        audio.setVolume(DEFAULT_AUDIO_VOLUME);
        return true;
    }
    return false;
    #else
    return true;
    #endif
}

inline bool initTouch() {
    #if FEATURE_DISPLAY_ENABLED
    return touch.begin();
    #else
    return true;
    #endif
}

inline bool initDFPlayer() {
    #if FEATURE_DFPLAYER_ENABLED
    // Initialiser le DFPlayer Mini pour les sons de victoire/défaite/timeout
    // Connexions: GPIO 47 (TX) -> DFPlayer RX, GPIO 48 (RX) -> DFPlayer TX
    if (dfplayer.begin(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN)) {
        dfplayer.setVolume(DFPLAYER_VOLUME);
        Serial.println("DFPlayer Mini: OK");
        return true;
    }
    Serial.println("DFPlayer Mini: ERREUR (non bloquant)");
    return false;  // Non bloquant - le jeu peut continuer sans DFPlayer
    #else
    return true;
    #endif
}

inline bool initHardware() {
    initSerial();

    // IMPORTANT: Initialiser I2C AVANT Display et Touch car ils en dépendent
    initI2C();

    if (!initDisplay()) {
        return false;
    }

    initSDCard();
    initAudio();
    initTouch();
    initDFPlayer();  // Initialiser le DFPlayer Mini (non bloquant)

    return true;
}

#endif // INIT_H
