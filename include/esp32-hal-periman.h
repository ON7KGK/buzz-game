/**
 * @file esp32-hal-periman.h
 * @brief Compatibility shim for ESP32 HAL Peripheral Manager
 *
 * This file provides compatibility for older Arduino-ESP32 frameworks
 * that don't include the peripheral manager header.
 *
 * The peripheral manager is used for GPIO pin management in newer
 * versions of arduino-esp32. For older versions, we provide stub
 * definitions to allow compilation.
 */

#ifndef _ESP32_HAL_PERIMAN_H_
#define _ESP32_HAL_PERIMAN_H_

#ifdef __cplusplus
extern "C" {
#endif

// Stub definitions for compatibility
// These are no-ops for older frameworks that don't have periman support

typedef enum {
    ESP32_BUS_TYPE_INIT,
    ESP32_BUS_TYPE_GPIO,
    ESP32_BUS_TYPE_UART_RX,
    ESP32_BUS_TYPE_UART_TX,
    ESP32_BUS_TYPE_UART_CTS,
    ESP32_BUS_TYPE_UART_RTS,
    ESP32_BUS_TYPE_I2C_SDA,
    ESP32_BUS_TYPE_I2C_SCL,
    ESP32_BUS_TYPE_SPI_CLK,
    ESP32_BUS_TYPE_SPI_MOSI,
    ESP32_BUS_TYPE_SPI_MISO,
    ESP32_BUS_TYPE_SPI_CS,
    ESP32_BUS_TYPE_SPI_HD,
    ESP32_BUS_TYPE_SPI_WP,
    ESP32_BUS_TYPE_MAX
} peripheral_bus_type_t;

// Stub inline functions that do nothing
static inline bool perimanSetPinBus(uint8_t pin, peripheral_bus_type_t type, void* bus) {
    return true;  // Always succeed
}

static inline bool perimanClearPinBus(uint8_t pin) {
    return true;  // Always succeed
}

static inline void* perimanGetPinBus(uint8_t pin, peripheral_bus_type_t type) {
    return NULL;  // No bus assigned
}

static inline peripheral_bus_type_t perimanGetPinBusType(uint8_t pin) {
    return ESP32_BUS_TYPE_GPIO;  // Default to GPIO
}

#ifdef __cplusplus
}
#endif

#endif /* _ESP32_HAL_PERIMAN_H_ */
