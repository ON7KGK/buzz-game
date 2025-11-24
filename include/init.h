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

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS GLOBAUX (accessibles depuis main.cpp)
// ═══════════════════════════════════════════════════════════════════════════

extern Display display;
extern AudioDriver audio;
extern SDCard sd;
extern Touch touch;

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
    #if FEATURE_AUDIO_ENABLED
    if (audio.begin()) {
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

inline bool initHardware() {
    initSerial();

    if (!initDisplay()) {
        return false;
    }

    initSDCard();
    initI2C();
    initAudio();
    initTouch();

    return true;
}

#endif // INIT_H
