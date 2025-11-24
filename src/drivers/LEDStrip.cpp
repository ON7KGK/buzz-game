/**
 * @file LEDStrip.cpp
 * @brief Implémentation du driver pour bandeaux LED SK9822
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#include "drivers/LEDStrip.h"

LEDStrip::LEDStrip(uint8_t dataPin, uint8_t clockPin, uint16_t numLeds)
    : _dataPin(dataPin), _clockPin(clockPin), _numLeds(numLeds),
      _brightness(255), _pixels(nullptr), _rainbowHue(0) {
}

LEDStrip::~LEDStrip() {
    if (_pixels) {
        delete[] _pixels;
    }
}

bool LEDStrip::begin() {
    // Allouer mémoire pour les pixels (3 bytes par LED: R,G,B)
    _pixels = new uint8_t[_numLeds * 3];
    if (!_pixels) {
        return false;
    }

    // Initialiser les pins
    pinMode(_dataPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    digitalWrite(_dataPin, LOW);
    digitalWrite(_clockPin, LOW);

    // Éteindre toutes les LEDs
    clear();
    show();

    return true;
}

void LEDStrip::startFrame() {
    // Start frame: 32 bits à 0
    for (int i = 0; i < 4; i++) {
        writeByte(0x00);
    }
}

void LEDStrip::endFrame() {
    // End frame: au moins (n+1)/2 bits à 1
    // Pour simplifier, on envoie 4 bytes à 0xFF
    for (int i = 0; i < 4; i++) {
        writeByte(0xFF);
    }
}

void LEDStrip::writeByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        digitalWrite(_dataPin, (byte >> i) & 0x01);
        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
    }
}

void LEDStrip::clear() {
    memset(_pixels, 0, _numLeds * 3);
}

void LEDStrip::show() {
    startFrame();

    // Calculer la luminosité globale (5 bits: 0-31)
    uint8_t globalBrightness = (_brightness >> 3) | 0xE0;

    for (uint16_t i = 0; i < _numLeds; i++) {
        uint16_t offset = i * 3;

        // Frame LED: 0xE0 + luminosité (5 bits)
        writeByte(globalBrightness);

        // Ordre des couleurs pour SK9822: BGR
        writeByte(_pixels[offset + 2]); // B
        writeByte(_pixels[offset + 1]); // G
        writeByte(_pixels[offset]);     // R
    }

    endFrame();
}

void LEDStrip::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

void LEDStrip::setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= _numLeds) return;

    uint16_t offset = index * 3;
    _pixels[offset] = r;
    _pixels[offset + 1] = g;
    _pixels[offset + 2] = b;
}

void LEDStrip::fill(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < _numLeds; i++) {
        setPixel(i, r, g, b);
    }
}

uint32_t LEDStrip::wheel(byte wheelPos) {
    wheelPos = 255 - wheelPos;
    if (wheelPos < 85) {
        return ((255 - wheelPos * 3) << 16) | (0 << 8) | (wheelPos * 3);
    }
    if (wheelPos < 170) {
        wheelPos -= 85;
        return ((0) << 16) | ((wheelPos * 3) << 8) | (255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return ((wheelPos * 3) << 16) | ((255 - wheelPos * 3) << 8) | (0);
}

void LEDStrip::rainbow(uint16_t firstPixelHue) {
    for (uint16_t i = 0; i < _numLeds; i++) {
        uint16_t pixelHue = firstPixelHue + (i * 65536L / _numLeds);
        uint8_t wheelPos = (pixelHue >> 8) & 0xFF;

        uint32_t color = wheel(wheelPos);
        setPixel(i, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    }
}

void LEDStrip::updateRainbow() {
    _rainbowHue += 256; // Vitesse de rotation
    if (_rainbowHue >= 65536) {
        _rainbowHue = 0;
    }
    rainbow(_rainbowHue);
    show();
}
