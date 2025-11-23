/**
 * @file i2c_config.h
 * @brief Configuration du bus I2C principal
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef I2C_CONFIG_H
#define I2C_CONFIG_H

// ============================================================================
// PINS I2C PRINCIPAL
// ============================================================================
#define I2C_SDA                 8       // GPIO 8
#define I2C_SCL                 7       // GPIO 7

// ============================================================================
// CONFIGURATION I2C
// ============================================================================
#define I2C_FREQUENCY           400000  // 400kHz (Fast mode)
#define I2C_TIMEOUT_MS          1000    // Timeout en millisecondes

// ============================================================================
// ADRESSES I2C DES PÉRIPHÉRIQUES
// ============================================================================
#define TCA9554_ADDR            0x20    // I/O Expander (reset LCD)
#define TOUCH_ADDR              0x3B    // Touch controller (intégré AXS15231B)
#define ES8311_ADDR             0x18    // Audio codec
#define QMI8658_ADDR            0x6B    // IMU 6-axes
#define PCF85063_ADDR           0x51    // RTC
#define AXP2101_ADDR            0x34    // Power management

#endif // I2C_CONFIG_H
