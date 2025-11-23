/**
 * @file sd_config.h
 * @brief Configuration de la carte SD/TF
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef SD_CONFIG_H
#define SD_CONFIG_H

// ============================================================================
// PINS SD CARD (SD_MMC Interface - 1-bit mode)
// ============================================================================
#define SD_MMC_CLK              11      // Clock
#define SD_MMC_CMD              10      // Command
#define SD_MMC_D0               9       // Data 0
// Mode 1-bit uniquement (D1, D2, D3 non utilisés)

// ============================================================================
// CONFIGURATION SD
// ============================================================================
#define SD_MODE_1BIT            true    // Mode 1-bit (true) ou 4-bit (false)
#define SD_FREQUENCY_MHZ        20      // Fréquence du bus SD (MHz)

// ============================================================================
// CHEMINS RÉPERTOIRES
// ============================================================================
#define SD_AUDIO_PATH           "/audio"        // Fichiers audio (.mp3, .wav)
#define SD_DATA_PATH            "/data"         // Données de l'application
#define SD_CONFIG_PATH          "/config"       // Fichiers de configuration
#define SD_LOGS_PATH            "/logs"         // Fichiers de logs

// ============================================================================
// LIMITES
// ============================================================================
#define SD_MAX_FILENAME_LENGTH  64      // Longueur max du nom de fichier
#define SD_MAX_PATH_LENGTH      256     // Longueur max du chemin complet

#endif // SD_CONFIG_H
