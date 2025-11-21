/**
 * @file TouchScreen.h
 * @brief Gestion du tactile capacitif
 */

#ifndef WAVESHARE_TOUCHSCREEN_H
#define WAVESHARE_TOUCHSCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include "../config/features.h"

#ifdef FEATURE_TOUCH_ENABLED

#include "../config/pins_definitions.h"
#include "../config/display_config.h"

struct TouchPoint {
    uint16_t x;
    uint16_t y;
    bool pressed;
    uint8_t pressure;  // 0-255 si supporté
};

class WaveshareTouch {
public:
    WaveshareTouch();
    ~WaveshareTouch();
    
    /**
     * @brief Initialise le contrôleur tactile
     * @param wire Interface I2C à utiliser
     * @return true si succès, false sinon
     */
    bool begin(TwoWire& wire = Wire);
    
    /**
     * @brief Termine et libère les ressources
     */
    void end();
    
    /**
     * @brief Lit l'état du tactile
     * @return true si un point tactile est détecté
     */
    bool read();
    
    /**
     * @brief Obtient le point tactile actuel
     * @return Structure TouchPoint
     */
    TouchPoint getPoint();
    
    /**
     * @brief Vérifie si l'écran est touché
     * @return true si touché, false sinon
     */
    bool touched();
    
    /**
     * @brief Obtient la coordonnée X
     */
    uint16_t getX() { return _point.x; }
    
    /**
     * @brief Obtient la coordonnée Y
     */
    uint16_t getY() { return _point.y; }
    
    /**
     * @brief Vérifie si initialisé
     */
    bool isInitialized() { return _initialized; }
    
    /**
     * @brief Calibre le tactile
     */
    void calibrate(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax);

private:
    TwoWire* _wire;
    bool _initialized;
    TouchPoint _point;
    
    uint16_t _calXMin, _calXMax, _calYMin, _calYMax;
    
    void reset();
    bool readData();
    void applyCalibration();
};

#endif // FEATURE_TOUCH_ENABLED

#endif // WAVESHARE_TOUCHSCREEN_H
