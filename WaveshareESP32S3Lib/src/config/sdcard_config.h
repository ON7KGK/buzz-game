/**
 * @file sdcard_config.h
 * @brief Configuration de la carte SD/TF
 */

#ifndef WAVESHARE_SDCARD_CONFIG_H
#define WAVESHARE_SDCARD_CONFIG_H

#include "pins_definitions.h"

// ============================================================================
// CONFIGURATION SD_MMC
// ============================================================================

// Mode de fonctionnement
// true = 1-bit mode (économie de GPIO)
// false = 4-bit mode (plus rapide, nécessite D1, D2, D3)
#define SD_MMC_1BIT_MODE            true

// Point de montage
#define SD_MOUNT_POINT              "/sdcard"

// Nom de la partition
#define SD_PARTITION_NAME           "sdmmc"

// ============================================================================
// PERFORMANCE
// ============================================================================

// Fréquence du bus SD (Hz)
// Valeurs: 20000 (20kHz), 400000 (400kHz), 20000000 (20MHz)
#define SD_FREQUENCY                20000000  // 20 MHz

// Taille du buffer de lecture/écriture
#define SD_BUFFER_SIZE              4096

// Nombre maximum de fichiers ouverts simultanément
#define SD_MAX_OPEN_FILES           5

// ============================================================================
// TIMEOUTS
// ============================================================================

// Timeout pour l'initialisation (ms)
#define SD_INIT_TIMEOUT             5000

// Timeout pour les opérations de lecture/écriture (ms)
#define SD_RW_TIMEOUT               3000

// ============================================================================
// FORMAT
// ============================================================================

// Formatage automatique si la carte n'est pas formatée
#define SD_AUTO_FORMAT              false

// Type de système de fichiers
// "FAT", "FAT32", "exFAT"
#define SD_FS_TYPE                  "FAT32"

// Taille d'allocation par défaut (bytes)
#define SD_ALLOCATION_UNIT_SIZE     4096

// ============================================================================
// GESTION D'ERREURS
// ============================================================================

// Nombre de tentatives de réinitialisation en cas d'erreur
#define SD_MAX_RETRY_COUNT          3

// Délai entre les tentatives (ms)
#define SD_RETRY_DELAY              1000

// Activer le mode debug pour la SD
#define SD_DEBUG_MODE               false

// ============================================================================
// CHEMINS PRÉDÉFINIS
// ============================================================================

// Répertoires par défaut
#define SD_AUDIO_DIR                "/sdcard/audio"
#define SD_IMAGES_DIR               "/sdcard/images"
#define SD_VIDEO_DIR                "/sdcard/video"
#define SD_DATA_DIR                 "/sdcard/data"
#define SD_LOGS_DIR                 "/sdcard/logs"
#define SD_CONFIG_DIR               "/sdcard/config"

// ============================================================================
// EXTENSIONS DE FICHIERS SUPPORTÉES
// ============================================================================

// Audio
#define AUDIO_EXTENSIONS            {".mp3", ".wav", ".aac", ".ogg"}

// Images
#define IMAGE_EXTENSIONS            {".jpg", ".jpeg", ".png", ".bmp", ".gif"}

// Vidéo
#define VIDEO_EXTENSIONS            {".mp4", ".avi", ".mkv"}

// Texte
#define TEXT_EXTENSIONS             {".txt", ".log", ".csv", ".json"}

// ============================================================================
// FONCTIONNALITÉS
// ============================================================================

// Activer la création automatique des dossiers
#define SD_AUTO_CREATE_DIRS         true

// Activer le cache de lecture
#define SD_ENABLE_READ_CACHE        true

// Taille du cache de lecture (bytes)
#define SD_READ_CACHE_SIZE          8192

// Activer le cache d'écriture
#define SD_ENABLE_WRITE_CACHE       false

#endif // WAVESHARE_SDCARD_CONFIG_H
