/**
 * @file Touch.h
 * @brief Classe pour gérer l'écran tactile (AXS15231B intégré)
 *
 * Le contrôleur tactile génère une interruption sur la broche P2 du TCA9554.
 * Quand l'écran est touché, cette broche passe à LOW (actif bas).
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include "config/i2c_config.h"
#include "TCA9554.h"
#include "features.h"

class Touch {
public:
    Touch() : initialized(false), lastTouchState(false), lastChangeMs(0), tca(TCA9554_ADDR) {}

    bool begin() {
        #if !FEATURE_DISPLAY_ENABLED
        return false;
        #endif

        if (!tca.begin()) {
            return false;
        }

        tca.pinMode1(TOUCH_INT_TCA_PIN, INPUT);
        initialized = true;
        return true;
    }

    bool isTouched() {
        if (!initialized) {
            return false;
        }

        bool currentState = (tca.read1(TOUCH_INT_TCA_PIN) == LOW);
        unsigned long now = millis();

        if (currentState != lastTouchState) {
            if (now - lastChangeMs > 30) {
                lastChangeMs = now;
                lastTouchState = currentState;

                if (currentState) {
                    return true;
                }
            }
        }

        return false;
    }

    bool getTouch(int16_t* x, int16_t* y) {
        return false;
    }

private:
    bool initialized;
    bool lastTouchState;
    unsigned long lastChangeMs;
    TCA9554 tca;
};

#endif // TOUCH_DRIVER_H
