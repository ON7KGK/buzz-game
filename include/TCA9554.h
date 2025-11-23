/*
 * TCA9554.h - Bibliothèque pour le contrôleur I/O TCA9554
 * Utilisé pour contrôler le reset de l'écran LCD
 */

#ifndef TCA9554_H
#define TCA9554_H

#include <Wire.h>

// Registres TCA9554
#define TCA9554_INPUT_REG       0x00
#define TCA9554_OUTPUT_REG      0x01
#define TCA9554_POLARITY_REG    0x02
#define TCA9554_CONFIG_REG      0x03

class TCA9554 {
public:
    TCA9554(uint8_t address) : _address(address) {}

    bool begin() {
        Wire.beginTransmission(_address);
        return (Wire.endTransmission() == 0);
    }

    // Configuration d'un pin (INPUT=1, OUTPUT=0)
    void pinMode1(uint8_t pin, uint8_t mode) {
        uint8_t config = readRegister(TCA9554_CONFIG_REG);
        if (mode == OUTPUT) {
            config &= ~(1 << pin);
        } else {
            config |= (1 << pin);
        }
        writeRegister(TCA9554_CONFIG_REG, config);
    }

    // Écriture sur un pin
    void write1(uint8_t pin, uint8_t value) {
        uint8_t output = readRegister(TCA9554_OUTPUT_REG);
        if (value) {
            output |= (1 << pin);
        } else {
            output &= ~(1 << pin);
        }
        writeRegister(TCA9554_OUTPUT_REG, output);
    }

    // Lecture d'un pin
    uint8_t read1(uint8_t pin) {
        uint8_t input = readRegister(TCA9554_INPUT_REG);
        return (input >> pin) & 0x01;
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

#endif
