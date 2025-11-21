/**
 * @file features.h
 * @brief Configuration des fonctionnalités du module ESP32-S3-Touch-LCD-3.5B-C
 * 
 * Ce fichier permet d'activer ou désactiver les différents composants
 * pour optimiser l'utilisation de la mémoire selon vos besoins.
 * 
 * Décommentez les lignes pour ACTIVER les fonctionnalités
 * Commentez les lignes pour DÉSACTIVER les fonctionnalités
 */

#ifndef WAVESHARE_FEATURES_H
#define WAVESHARE_FEATURES_H

// ============================================================================
// AFFICHAGE ET TACTILE (Recommandé : toujours activé)
// ============================================================================
#define FEATURE_DISPLAY_ENABLED         // Active l'écran LCD 3.5" (AXS15231B)
#define FEATURE_TOUCH_ENABLED            // Active le tactile capacitif

// ============================================================================
// AUDIO
// ============================================================================
#define FEATURE_AUDIO_ENABLED            // Active le codec audio ES8311
// #define FEATURE_MICROPHONE_ENABLED    // Active le microphone intégré
// #define FEATURE_SPEAKER_ENABLED       // Active le haut-parleur

// ============================================================================
// CAPTEURS
// ============================================================================
#define FEATURE_IMU_ENABLED              // Active l'IMU 6 axes QMI8658
// #define FEATURE_RTC_ENABLED           // Active l'horloge temps réel PCF85063

// ============================================================================
// GESTION D'ALIMENTATION
// ============================================================================
#define FEATURE_POWER_MGMT_ENABLED       // Active la gestion d'alimentation AXP2101
// #define FEATURE_BATTERY_MONITOR       // Active le monitoring de la batterie

// ============================================================================
// STOCKAGE
// ============================================================================
#define FEATURE_SD_CARD_ENABLED          // Active la carte SD/TF

// ============================================================================
// CAMÉRA (Uniquement pour version -C)
// ============================================================================
// #define FEATURE_CAMERA_ENABLED        // Active la caméra OV5640 (5MP)

// ============================================================================
// CONNECTIVITY
// ============================================================================
#define FEATURE_WIFI_ENABLED             // Active le WiFi
// #define FEATURE_BLUETOOTH_ENABLED     // Active le Bluetooth 5

// ============================================================================
// INTERFACE GRAPHIQUE
// ============================================================================
#define FEATURE_LVGL_ENABLED             // Active LVGL pour l'interface graphique
// #define FEATURE_LVGL_DEMO             // Active les démos LVGL

// ============================================================================
// BOUTONS
// ============================================================================
#define FEATURE_BUTTONS_ENABLED          // Active les boutons PWR et BOOT

// ============================================================================
// DEBUGGING
// ============================================================================
#define FEATURE_SERIAL_DEBUG             // Active le debug série
#define DEBUG_LEVEL 1                    // 0=OFF, 1=ERROR, 2=WARN, 3=INFO, 4=DEBUG, 5=VERBOSE

// ============================================================================
// CONFIGURATION AVANCÉE
// ============================================================================

// Taille du buffer pour l'affichage (en pixels)
#define DISPLAY_BUFFER_SIZE (320 * 480)

// Fréquence I2C (Hz)
#define I2C_FREQUENCY 400000

// Volume par défaut (0-21)
#define DEFAULT_AUDIO_VOLUME 15

// Luminosité par défaut de l'écran (0-100%)
#define DEFAULT_BRIGHTNESS 80

// Timeout pour la SD card (ms)
#define SD_CARD_TIMEOUT 5000

#endif // WAVESHARE_FEATURES_H
