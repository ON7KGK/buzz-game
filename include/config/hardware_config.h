/**
 * @file hardware_config.h
 * @brief Configuration matérielle globale du module Waveshare ESP32-S3-Touch-LCD-3.5B
 *
 * ⚠️ NE PAS MODIFIER sauf si vous savez exactement ce que vous faites !
 * Ces définitions correspondent au schéma électronique du module.
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// ============================================================================
// INFORMATIONS MODULE
// ============================================================================
#define MODULE_NAME                 "Waveshare ESP32-S3-Touch-LCD-3.5B"
#define MODULE_VERSION              "1.0.0"
#define SCREEN_SIZE                 "3.5 inch"
#define SCREEN_RESOLUTION          "320x480 RGB565"

// ============================================================================
// SPÉCIFICATIONS ESP32-S3
// ============================================================================
#define MCU_MODEL                   "ESP32-S3R8"
#define MCU_CORES                   2
#define MCU_FREQ_MHZ                240
#define FLASH_SIZE_MB               16
#define PSRAM_SIZE_MB               8

// ============================================================================
// DFPLAYER MINI - LECTEUR MP3 EXTERNE
// ============================================================================
// Connexions avec conversion de niveaux 3.3V <-> 5V:
// GPIO 47 (ESP32 RX) <- DFPlayer TX
// GPIO 48 (ESP32 TX) -> DFPlayer RX
#define DFPLAYER_RX_PIN             47
#define DFPLAYER_TX_PIN             48
#define DFPLAYER_VOLUME             22   // Volume par défaut (0-30)

#endif // HARDWARE_CONFIG_H
