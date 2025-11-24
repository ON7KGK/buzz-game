/**
 * @file LEDStrip.h
 * @brief Driver pour bandeaux LED SK9822 (compatible APA102)
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Arduino.h>

class LEDStrip {
public:
    LEDStrip(uint8_t dataPin, uint8_t clockPin, uint16_t numLeds);
    ~LEDStrip();

    // Initialisation
    bool begin();

    // Contrôle de base
    void clear();
    void show();
    void setBrightness(uint8_t brightness); // 0-255

    // Définir couleur d'une LED (index, R, G, B)
    void setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

    // Remplir tout le bandeau avec une couleur
    void fill(uint8_t r, uint8_t g, uint8_t b);

    // Effets spéciaux
    void rainbow(uint16_t firstPixelHue = 0);
    void updateRainbow(); // Pour animation continue

    // Getters
    uint16_t getNumLeds() const { return _numLeds; }

private:
    uint8_t _dataPin;
    uint8_t _clockPin;
    uint16_t _numLeds;
    uint8_t _brightness;
    uint8_t* _pixels;
    uint16_t _rainbowHue;

    void startFrame();
    void endFrame();
    void writeByte(uint8_t byte);
    uint32_t wheel(byte wheelPos);
};

#endif // LED_STRIP_H
