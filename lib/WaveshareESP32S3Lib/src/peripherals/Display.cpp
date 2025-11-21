/**
 * @file Display.cpp
 * @brief Implémentation de la gestion de l'écran LCD
 */

#include "Display.h"

#ifdef FEATURE_DISPLAY_ENABLED

WaveshareDisplay::WaveshareDisplay() 
    : _bus(nullptr), 
      _display(nullptr), 
      _gfx(nullptr),
      _initialized(false),
      _brightness(DEFAULT_BRIGHTNESS),
      _rotation(DISPLAY_DEFAULT_ROTATION) {
}

WaveshareDisplay::~WaveshareDisplay() {
    end();
}

bool WaveshareDisplay::begin() {
    if (_initialized) {
        return true;
    }
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Display] Initialisation...");
    #endif
    
    // Initialiser le bus QSPI
    _bus = new Arduino_ESP32QSPI(
        LCD_QSPI_CS,   // CS
        LCD_QSPI_CLK,  // SCK
        LCD_QSPI_D0,   // D0
        LCD_QSPI_D1,   // D1
        LCD_QSPI_D2,   // D2
        LCD_QSPI_D3    // D3
    );
    
    if (_bus == nullptr) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.println("[Display] Erreur: échec création bus QSPI");
        #endif
        return false;
    }
    
    // Initialiser le contrôleur d'affichage AXS15231B
    _display = new Arduino_AXS15231B(
        _bus, 
        LCD_RST,        // RST (non utilisé)
        _rotation,      // Rotation
        false,          // IPS
        LCD_WIDTH, 
        LCD_HEIGHT
    );
    
    if (_display == nullptr) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.println("[Display] Erreur: échec création display");
        #endif
        delete _bus;
        return false;
    }
    
    // Créer le canvas pour le double buffering
    _gfx = new Arduino_Canvas(
        LCD_WIDTH, 
        LCD_HEIGHT, 
        _display, 
        0, 
        0, 
        _rotation
    );
    
    if (_gfx == nullptr) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.println("[Display] Erreur: échec création canvas");
        #endif
        delete _display;
        delete _bus;
        return false;
    }
    
    // Initialiser l'affichage
    if (!_gfx->begin()) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.println("[Display] Erreur: échec begin()");
        #endif
        delete _gfx;
        delete _display;
        delete _bus;
        return false;
    }
    
    // Initialiser le rétroéclairage
    initBacklight();
    
    // Effacer l'écran
    clear(COLOR_BLACK);
    
    _initialized = true;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Display] Initialisé avec succès");
    Serial.printf("[Display] Résolution: %dx%d\n", width(), height());
    #endif
    
    return true;
}

void WaveshareDisplay::end() {
    if (!_initialized) {
        return;
    }
    
    setBrightness(0);  // Éteindre le rétroéclairage
    
    if (_gfx) {
        delete _gfx;
        _gfx = nullptr;
    }
    
    if (_display) {
        delete _display;
        _display = nullptr;
    }
    
    if (_bus) {
        delete _bus;
        _bus = nullptr;
    }
    
    _initialized = false;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Display] Terminé");
    #endif
}

void WaveshareDisplay::power(bool on) {
    if (!_initialized) return;
    
    if (on) {
        setBrightness(_brightness);
    } else {
        setBrightness(0);
    }
}

void WaveshareDisplay::clear(uint16_t color) {
    if (!_initialized || !_gfx) return;
    
    _gfx->fillScreen(color);
}

void WaveshareDisplay::setRotation(uint8_t rotation) {
    if (!_initialized || !_gfx) return;
    
    _rotation = rotation % 4;
    _gfx->setRotation(_rotation);
}

uint8_t WaveshareDisplay::getRotation() {
    return _rotation;
}

void WaveshareDisplay::initBacklight() {
    // Configurer le PWM pour le rétroéclairage
    ledcSetup(BACKLIGHT_PWM_CHANNEL, BACKLIGHT_PWM_FREQ, BACKLIGHT_PWM_RESOLUTION);
    ledcAttachPin(LCD_BL, BACKLIGHT_PWM_CHANNEL);
    
    // Définir la luminosité initiale
    setBrightness(_brightness);
}

void WaveshareDisplay::setBrightness(uint8_t brightness) {
    if (brightness > 100) brightness = 100;
    
    _brightness = brightness;
    
    // Convertir 0-100 en 0-255
    uint8_t pwmValue = map(brightness, 0, 100, 0, 255);
    
    ledcWrite(BACKLIGHT_PWM_CHANNEL, pwmValue);
}

uint8_t WaveshareDisplay::getBrightness() {
    return _brightness;
}

void WaveshareDisplay::increaseBrightness() {
    uint8_t newBrightness = _brightness + 10;
    if (newBrightness > 100) newBrightness = 100;
    setBrightness(newBrightness);
}

void WaveshareDisplay::decreaseBrightness() {
    int16_t newBrightness = _brightness - 10;
    if (newBrightness < 0) newBrightness = 0;
    setBrightness(newBrightness);
}

// ============================================================================
// MÉTHODES DE DESSIN
// ============================================================================

void WaveshareDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawPixel(x, y, color);
}

void WaveshareDisplay::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawLine(x0, y0, x1, y1, color);
}

void WaveshareDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawRect(x, y, w, h, color);
}

void WaveshareDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->fillRect(x, y, w, h, color);
}

void WaveshareDisplay::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawCircle(x, y, r, color);
}

void WaveshareDisplay::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->fillCircle(x, y, r, color);
}

void WaveshareDisplay::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
                                    int16_t x2, int16_t y2, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

void WaveshareDisplay::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                    int16_t x2, int16_t y2, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void WaveshareDisplay::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, 
                                     int16_t r, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawRoundRect(x, y, w, h, r, color);
}

void WaveshareDisplay::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                     int16_t r, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->fillRoundRect(x, y, w, h, r, color);
}

// ============================================================================
// MÉTHODES DE TEXTE
// ============================================================================

void WaveshareDisplay::setTextColor(uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->setTextColor(color);
}

void WaveshareDisplay::setTextColor(uint16_t fgColor, uint16_t bgColor) {
    if (!_initialized || !_gfx) return;
    _gfx->setTextColor(fgColor, bgColor);
}

void WaveshareDisplay::setTextSize(uint8_t size) {
    if (!_initialized || !_gfx) return;
    _gfx->setTextSize(size);
}

void WaveshareDisplay::setCursor(int16_t x, int16_t y) {
    if (!_initialized || !_gfx) return;
    _gfx->setCursor(x, y);
}

void WaveshareDisplay::print(const char* text) {
    if (!_initialized || !_gfx) return;
    _gfx->print(text);
}

void WaveshareDisplay::print(int number) {
    if (!_initialized || !_gfx) return;
    _gfx->print(number);
}

void WaveshareDisplay::print(float number, int decimals) {
    if (!_initialized || !_gfx) return;
    _gfx->print(number, decimals);
}

void WaveshareDisplay::println(const char* text) {
    if (!_initialized || !_gfx) return;
    _gfx->println(text);
}

void WaveshareDisplay::println(int number) {
    if (!_initialized || !_gfx) return;
    _gfx->println(number);
}

void WaveshareDisplay::println(float number, int decimals) {
    if (!_initialized || !_gfx) return;
    _gfx->println(number, decimals);
}

// ============================================================================
// MÉTHODES D'IMAGES
// ============================================================================

void WaveshareDisplay::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, 
                                  int16_t w, int16_t h, uint16_t color) {
    if (!_initialized || !_gfx) return;
    _gfx->drawBitmap(x, y, bitmap, w, h, color);
}

void WaveshareDisplay::drawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap,
                                     int16_t w, int16_t h) {
    if (!_initialized || !_gfx) return;
    _gfx->draw16bitRGBBitmap(x, y, bitmap, w, h);
}

// ============================================================================
// UTILITAIRES
// ============================================================================

int16_t WaveshareDisplay::width() {
    if (!_initialized || !_gfx) return 0;
    return _gfx->width();
}

int16_t WaveshareDisplay::height() {
    if (!_initialized || !_gfx) return 0;
    return _gfx->height();
}

uint16_t WaveshareDisplay::color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

#endif // FEATURE_DISPLAY_ENABLED
