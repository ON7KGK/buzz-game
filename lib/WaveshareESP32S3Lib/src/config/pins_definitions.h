/**
 * @file pins_definitions.h
 * @brief Définitions des GPIO pour ESP32-S3-Touch-LCD-3.5B-C de Waveshare
 * 
 * Ce fichier contient TOUTES les définitions de pins du module.
 * NE PAS MODIFIER sauf si vous savez exactement ce que vous faites !
 * 
 * Référence: Waveshare ESP32-S3-Touch-LCD-3.5B-C Schematic
 */

#ifndef WAVESHARE_PINS_H
#define WAVESHARE_PINS_H

// ============================================================================
// AFFICHAGE LCD - AXS15231B (QSPI Interface)
// ============================================================================
#define LCD_QSPI_CS         12    // Chip Select
#define LCD_QSPI_CLK        5    // Clock
#define LCD_QSPI_D0         1    // Data 0
#define LCD_QSPI_D1         2    // Data 1
#define LCD_QSPI_D2         3    // Data 2
#define LCD_QSPI_D3         4    // Data 3
#define LCD_RST             -1    // Reset (non utilisé, géré par AXS15231B)
#define LCD_BL              6    // Backlight PWM (GPIO 46 sur certains modèles, tester aussi 1)

// Résolution de l'écran
#define LCD_WIDTH           320
#define LCD_HEIGHT          480
#define LCD_ROTATION        3     // 0, 1, 2, 3 (0=portrait, 1=paysage, etc.)

// ============================================================================
// TACTILE CAPACITIF - AXS15231B (I2C Interface)
// ============================================================================
#define TOUCH_SDA           -1     // I2C Data
#define TOUCH_SCL           -1     // I2C Clock
#define TOUCH_INT           -1    // Interrupt
#define TOUCH_RST           -1    // Reset
#define TOUCH_ADDR          0x3B  // Adresse I2C du contrôleur tactile

// ============================================================================
// BUS I2C PRINCIPAL (Capteurs et périphériques)
// ============================================================================
#define I2C_SDA             8    // I2C Data principale
#define I2C_SCL             7    // I2C Clock principale
#define SENSOR_SDA          8    // Alias pour les capteurs
#define SENSOR_SCL          7    // Alias pour les capteurs

// ============================================================================
// AUDIO - ES8311 Codec (I2S Interface)
// ============================================================================
#define I2S_MCLK            44    // Master Clock
#define I2S_BCLK            13     // Bit Clock
#define I2S_LRCK            15     // Left/Right Clock (WS)
#define I2S_DOUT            16     // Data Out (Playback)
#define I2S_DIN             14    // Data In (Recording)
#define ES8311_ADDR         0x18  // Adresse I2C du codec audio

// Pins supplémentaires audio
#define CODEC_ADC_I2S_BCK   41    // ADC Bit Clock
#define CODEC_ADC_I2S_WS    42    // ADC Word Select
#define CODEC_ADC_I2S_DOUT  2     // ADC Data Out

#define PA_CTRL             11    // Power Amplifier Control

// ============================================================================
// IMU 6-AXES - QMI8658 (I2C Interface)
// ============================================================================
#define IMU_SDA             SENSOR_SDA    // Utilise le bus I2C principal
#define IMU_SCL             SENSOR_SCL    // Utilise le bus I2C principal
#define IMU_INT1            3             // Interrupt 1
#define IMU_INT2            -1            // Interrupt 2 (non connecté)
#define QMI8658_ADDR        0x6B          // Adresse I2C de l'IMU

// ============================================================================
// RTC - PCF85063 (I2C Interface)
// ============================================================================
#define RTC_SDA             SENSOR_SDA    // Utilise le bus I2C principal
#define RTC_SCL             SENSOR_SCL    // Utilise le bus I2C principal
#define RTC_INT             -1            // Interrupt (optionnel)
#define PCF85063_ADDR       0x51          // Adresse I2C du RTC

// ============================================================================
// GESTION D'ALIMENTATION - AXP2101 (I2C Interface)
// ============================================================================
#define POWER_SDA           SENSOR_SDA    // Utilise le bus I2C principal
#define POWER_SCL           SENSOR_SCL    // Utilise le bus I2C principal
#define POWER_INT           -1            // Interrupt (optionnel)
#define AXP2101_ADDR        0x34          // Adresse I2C du AXP2101

// ============================================================================
// CARTE SD/TF (SD_MMC Interface)
// ============================================================================
#define SD_MMC_CLK          11    // Clock
#define SD_MMC_CMD          10    // Command
#define SD_MMC_D0           9    // Data 0
// Mode 1-bit uniquement pour ce module
// D1, D2, D3 non utilisés

// ============================================================================
// CAMÉRA OV5640 (Uniquement pour version -C)
// ============================================================================
// Interface DVP (Digital Video Port)
#define CAM_XCLK            -1    // External Clock (géré par ESP32)
#define CAM_PCLK            -1    // Pixel Clock
#define CAM_VSYNC           -1    // Vertical Sync
#define CAM_HSYNC           -1    // Horizontal Sync

// Pins de données (D0-D7)
#define CAM_D0              -1
#define CAM_D1              -1
#define CAM_D2              -1
#define CAM_D3              -1
#define CAM_D4              -1
#define CAM_D5              -1
#define CAM_D6              -1
#define CAM_D7              -1

// I2C pour config caméra
#define CAM_SDA             SENSOR_SDA
#define CAM_SCL             SENSOR_SCL
#define CAM_PWDN            -1    // Power Down
#define CAM_RESET           -1    // Reset

// ============================================================================
// BOUTONS
// ============================================================================
#define BTN_BOOT            -1     // Bouton BOOT (GPIO0)
#define BTN_PWR             -1    // Bouton PWR (connecté au AXP2101)

// ============================================================================
// GPIO EXPOSÉS (Connecteur d'extension)
// ============================================================================
// Ces GPIO sont disponibles sur le connecteur 2.54mm
#define GPIO_EXT_1          9     // GPIO disponible 2
#define GPIO_EXT_3          20    // GPIO disponible 3
#define GPIO_EXT_4          35    // GPIO disponible 4
#define GPIO_EXT_5          36    // GPIO disponible 5
#define GPIO_EXT_6          37    // GPIO disponible 6
#define GPIO_EXT_7          38    // GPIO disponible 7

// UART exposé
#define UART_TX             43    // TX exposé
#define UART_RX             44    // RX exposé

// ============================================================================
// USB
// ============================================================================
#define USB_DN              19    // USB D- (aussi disponible comme GPIO)
#define USB_DP              20    // USB D+ (aussi disponible comme GPIO)

// ============================================================================
// LED et indicateurs
// ============================================================================
// Pas de LED intégrée sur ce module, utiliser LCD_BL pour indication

// ============================================================================
// NOTES IMPORTANTES
// ============================================================================
/*
 * GPIO réservés par l'ESP32-S3 et non disponibles:
 * - GPIO 0: Bouton BOOT (utilisation spéciale)
 * - GPIO 19-20: USB D-/D+ (si USB activé)
 * - GPIO 26-32: Pas d'entrées/sorties (réservés)
 * - GPIO 33-37: PSRAM/Flash QSPI (ne pas utiliser)
 * 
 * I2C Bus principal (GPIO 17/18):
 * - AXP2101 (Power Management) @ 0x34
 * - QMI8658 (IMU) @ 0x6B
 * - PCF85063 (RTC) @ 0x51
 * - ES8311 (Audio Codec) @ 0x18
 * 
 * I2C Bus tactile (GPIO 4/8):
 * - AXS15231B Touch Controller @ 0x3B
 * 
 * QSPI Display (GPIO 45, 47, 21, 48, 40, 39):
 * - AXS15231B LCD Controller
 * 
 * I2S Audio:
 * - ES8311 utilise GPIO 5, 6, 7, 15, 16
 * 
 * SD Card:
 * - Utilise GPIO 10, 13, 14 en mode 1-bit
 */

#endif // WAVESHARE_PINS_H
