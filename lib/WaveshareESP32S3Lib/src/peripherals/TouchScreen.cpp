/**
 * @file TouchScreen.cpp
 * @brief Implémentation de la gestion du tactile
 */

#include "TouchScreen.h"

#ifdef FEATURE_TOUCH_ENABLED

WaveshareTouch::WaveshareTouch()
    : _wire(nullptr),
      _initialized(false),
      _calXMin(TOUCH_CAL_X_MIN),
      _calXMax(TOUCH_CAL_X_MAX),
      _calYMin(TOUCH_CAL_Y_MIN),
      _calYMax(TOUCH_CAL_Y_MAX) {
    
    _point.x = 0;
    _point.y = 0;
    _point.pressed = false;
    _point.pressure = 0;
}

WaveshareTouch::~WaveshareTouch() {
    end();
}

bool WaveshareTouch::begin(TwoWire& wire) {
    if (_initialized) {
        return true;
    }
    
    _wire = &wire;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Touch] Initialisation...");
    #endif
    
    // Configurer les pins
    pinMode(TOUCH_INT, INPUT_PULLUP);
    pinMode(TOUCH_RST, OUTPUT);
    
    // Reset du contrôleur tactile
    reset();
    
    // Vérifier la présence du contrôleur
    _wire->beginTransmission(TOUCH_ADDR);
    uint8_t error = _wire->endTransmission();
    
    if (error != 0) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.printf("[Touch] Erreur: contrôleur non détecté (erreur %d)\n", error);
        #endif
        return false;
    }
    
    _initialized = true;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Touch] Initialisé avec succès");
    Serial.printf("[Touch] Adresse I2C: 0x%02X\n", TOUCH_ADDR);
    #endif
    
    return true;
}

void WaveshareTouch::end() {
    if (!_initialized) {
        return;
    }
    
    _initialized = false;
    _wire = nullptr;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Touch] Terminé");
    #endif
}

void WaveshareTouch::reset() {
    // Séquence de reset
    digitalWrite(TOUCH_RST, LOW);
    delay(10);
    digitalWrite(TOUCH_RST, HIGH);
    delay(300);
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Touch] Reset effectué");
    #endif
}

bool WaveshareTouch::read() {
    if (!_initialized || !_wire) {
        return false;
    }
    
    return readData();
}

bool WaveshareTouch::readData() {
    // Lire les données du registre tactile
    _wire->beginTransmission(TOUCH_ADDR);
    _wire->write(0x02);  // Registre des données tactiles
    
    if (_wire->endTransmission() != 0) {
        _point.pressed = false;
        return false;
    }
    
    // Lire 6 bytes (nombre de points + coordonnées)
    uint8_t bytesRead = _wire->requestFrom((uint8_t)TOUCH_ADDR, (uint8_t)6);
    
    if (bytesRead != 6) {
        _point.pressed = false;
        return false;
    }
    
    uint8_t data[6];
    for (int i = 0; i < 6; i++) {
        data[i] = _wire->read();
    }
    
    // Extraire le nombre de points tactiles
    uint8_t touchPoints = data[0] & 0x0F;
    
    if (touchPoints == 0) {
        _point.pressed = false;
        return false;
    }
    
    // Extraire les coordonnées (big-endian)
    uint16_t rawX = ((data[1] & 0x0F) << 8) | data[2];
    uint16_t rawY = ((data[3] & 0x0F) << 8) | data[4];
    
    // Appliquer la calibration
    _point.x = rawX;
    _point.y = rawY;
    applyCalibration();
    
    // Pression (si disponible)
    _point.pressure = data[5];
    _point.pressed = true;
    
    return true;
}

TouchPoint WaveshareTouch::getPoint() {
    return _point;
}

bool WaveshareTouch::touched() {
    // Vérifier l'état de l'interrupt pin
    if (digitalRead(TOUCH_INT) == LOW) {
        return read();
    }
    
    _point.pressed = false;
    return false;
}

void WaveshareTouch::calibrate(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax) {
    _calXMin = xMin;
    _calXMax = xMax;
    _calYMin = yMin;
    _calYMax = yMax;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Touch] Calibration mise à jour:");
    Serial.printf("  X: %d - %d\n", xMin, xMax);
    Serial.printf("  Y: %d - %d\n", yMin, yMax);
    #endif
}

void WaveshareTouch::applyCalibration() {
    // Mapper les coordonnées brutes vers l'espace de l'écran
    _point.x = map(_point.x, _calXMin, _calXMax, 0, LCD_WIDTH);
    _point.y = map(_point.y, _calYMin, _calYMax, 0, LCD_HEIGHT);
    
    // S'assurer que les coordonnées restent dans les limites
    if (_point.x < 0) _point.x = 0;
    if (_point.x >= LCD_WIDTH) _point.x = LCD_WIDTH - 1;
    if (_point.y < 0) _point.y = 0;
    if (_point.y >= LCD_HEIGHT) _point.y = LCD_HEIGHT - 1;
    
    // Appliquer les inversions si nécessaire
    #if TOUCH_INVERT_X
    _point.x = LCD_WIDTH - 1 - _point.x;
    #endif
    
    #if TOUCH_INVERT_Y
    _point.y = LCD_HEIGHT - 1 - _point.y;
    #endif
    
    // Échanger X et Y si nécessaire
    #if TOUCH_SWAP_XY
    uint16_t temp = _point.x;
    _point.x = _point.y;
    _point.y = temp;
    #endif
}

#endif // FEATURE_TOUCH_ENABLED
