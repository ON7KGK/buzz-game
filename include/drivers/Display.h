/**
 * @file Display.h
 * @brief Classe pour gérer l'affichage LCD AXS15231B
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "config/display_config.h"
#include "config/i2c_config.h"
#include "features.h"

#if USE_ADAFRUIT_FONTS
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#endif

class TCA9554 {
public:
    TCA9554(uint8_t address) : _address(address) {}

    bool begin() {
        Wire.beginTransmission(_address);
        return (Wire.endTransmission() == 0);
    }

    void pinMode1(uint8_t pin, uint8_t mode) {
        uint8_t config = readRegister(0x03);
        if (mode == OUTPUT) {
            config &= ~(1 << pin);
        } else {
            config |= (1 << pin);
        }
        writeRegister(0x03, config);
    }

    void write1(uint8_t pin, uint8_t value) {
        uint8_t output = readRegister(0x01);
        if (value) {
            output |= (1 << pin);
        } else {
            output &= ~(1 << pin);
        }
        writeRegister(0x01, output);
    }

private:
    uint8_t _address;

    void writeRegister(uint8_t reg, uint8_t value) {
        Wire.beginTransmission(_address);
        Wire.write(reg);
        Wire.write(value);
        Wire.endTransmission();
    }

    uint8_t readRegister(uint8_t reg) {
        Wire.beginTransmission(_address);
        Wire.write(reg);
        Wire.endTransmission();
        Wire.requestFrom(_address, (uint8_t)1);
        return Wire.read();
    }
};

class Display {
public:
    /**
     * @brief Constructeur
     */
    Display() : tca(TCA9554_ADDR),
                bus(nullptr),
                gfx(nullptr),
                canvas(nullptr),
                initialized(false) {}

    /**
     * @brief Initialise l'affichage
     * @return true si succès, false sinon
     */
    bool begin() {
        #if !FEATURE_DISPLAY_ENABLED
        return false;
        #endif

        // Init I2C pour TCA9554
        Wire.begin(I2C_SDA, I2C_SCL);

        if (!tca.begin()) {
            Serial.println("❌ TCA9554 non détecté");
            return false;
        }

        // Reset LCD via TCA9554
        tca.pinMode1(1, OUTPUT);
        tca.write1(1, 1);
        delay(10);
        tca.write1(1, 0);
        delay(10);
        tca.write1(1, 1);
        delay(200);

        // Init bus QSPI
        bus = new Arduino_ESP32QSPI(
            LCD_QSPI_CS, LCD_QSPI_CLK,
            LCD_QSPI_D0, LCD_QSPI_D1,
            LCD_QSPI_D2, LCD_QSPI_D3
        );

        // Init driver AXS15231B
        gfx = new Arduino_AXS15231B(
            bus, -1, 0, false,
            SCREEN_WIDTH, SCREEN_HEIGHT
        );

        #if USE_DOUBLE_BUFFER
        // Init canvas (double buffer)
        canvas = new Arduino_Canvas(
            SCREEN_WIDTH, SCREEN_HEIGHT,
            gfx, 0, 0, SCREEN_ROTATION
        );
        #endif

        // Démarrer l'affichage
        #if USE_DOUBLE_BUFFER
        if (!canvas->begin()) {
        #else
        if (!gfx->begin()) {
        #endif
            Serial.println("❌ Échec initialisation LCD");
            return false;
        }

        // Init backlight
        pinMode(LCD_BL, OUTPUT);
        setBacklight(DEFAULT_BACKLIGHT_LEVEL);

        initialized = true;
        return true;
    }

    /**
     * @brief Efface l'écran avec une couleur
     * @param color Couleur RGB565
     */
    void clear(uint16_t color = COLOR_BLACK) {
        if (!initialized) return;
        #if USE_DOUBLE_BUFFER
        canvas->fillScreen(color);
        canvas->flush();
        #else
        gfx->fillScreen(color);
        #endif
    }

    /**
     * @brief Affiche du texte centré
     * @param text Texte à afficher
     * @param y Position verticale
     * @param color Couleur du texte
     * @param fontSize Taille de police (1-4)
     */
    void printCentered(const char* text, int16_t y, uint16_t color = COLOR_WHITE, uint8_t fontSize = 2) {
        if (!initialized) return;

        Arduino_GFX* display = USE_DOUBLE_BUFFER ? (Arduino_GFX*)canvas : gfx;

        #if USE_ADAFRUIT_FONTS
        // Utiliser les polices vectorielles
        const GFXfont* font = &FreeSans12pt7b;
        if (fontSize >= 3) font = &FreeSansBold18pt7b;
        if (fontSize >= 4) font = &FreeSansBold24pt7b;

        display->setFont(font);
        display->setTextColor(color);

        int16_t x1, y1;
        uint16_t w, h;
        display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

        int16_t x = (SCREEN_WIDTH - w) / 2 - x1;
        display->setCursor(x, y);
        display->print(text);
        #else
        // Utiliser les polices bitmap
        display->setTextSize(fontSize);
        display->setTextColor(color);

        int16_t x1, y1;
        uint16_t w, h;
        display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

        int16_t x = (SCREEN_WIDTH - w) / 2;
        display->setCursor(x, y);
        display->print(text);
        #endif

        #if USE_DOUBLE_BUFFER
        canvas->flush();
        #endif
    }

    /**
     * @brief Définit la luminosité du rétroéclairage
     * @param level Niveau 0-255 (0=éteint, 255=max)
     */
    void setBacklight(uint8_t level) {
        analogWrite(LCD_BL, level);
    }

    /**
     * @brief Récupère le canvas/gfx pour dessins personnalisés
     * @return Pointeur vers Arduino_GFX
     */
    Arduino_GFX* getCanvas() {
        #if USE_DOUBLE_BUFFER
        return canvas;
        #else
        return gfx;
        #endif
    }

    /**
     * @brief Flush le buffer (si double buffer activé)
     */
    void flush() {
        #if USE_DOUBLE_BUFFER
        if (canvas) canvas->flush();
        #endif
    }

private:
    TCA9554 tca;
    Arduino_DataBus* bus;
    Arduino_GFX* gfx;
    Arduino_Canvas* canvas;
    bool initialized;
};

#endif // DISPLAY_DRIVER_H
