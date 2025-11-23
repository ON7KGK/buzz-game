/**
 * @file features.h
 * @brief Configuration des fonctionnalités activées/désactivées
 *
 * Modifiez ce fichier pour activer ou désactiver les composants
 * en fonction de votre projet spécifique.
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef FEATURES_H
#define FEATURES_H

// ============================================================================
// FONCTIONNALITÉS PRINCIPALES (Priorité 1)
// ============================================================================

/**
 * @brief Activer l'affichage LCD
 * Si désactivé, toutes les fonctions d'affichage seront ignorées
 */
#define FEATURE_DISPLAY_ENABLED         true

/**
 * @brief Activer l'écran tactile capacitif
 * Si désactivé, la détection tactile ne sera pas initialisée
 */
#define FEATURE_TOUCH_ENABLED           true

/**
 * @brief Activer l'audio (ES8311 codec)
 * Si désactivé, pas de son ni de lecture MP3
 */
#define FEATURE_AUDIO_ENABLED           true

/**
 * @brief Activer la carte SD/TF
 * Si désactivé, impossible de lire/écrire sur la carte SD
 */
#define FEATURE_SD_ENABLED              true

// ============================================================================
// FONCTIONNALITÉS SECONDAIRES (Priorité 2 - Pour projets futurs)
// ============================================================================

/**
 * @brief Activer l'IMU 6-axes QMI8658
 * Accéléromètre + Gyroscope pour détection de mouvement
 */
#define FEATURE_IMU_ENABLED             false

/**
 * @brief Activer le RTC (Real-Time Clock) PCF85063
 * Horloge temps réel avec batterie de secours
 */
#define FEATURE_RTC_ENABLED             false

/**
 * @brief Activer la gestion d'alimentation AXP2101
 * Power management, batterie, charge, etc.
 */
#define FEATURE_POWER_MGMT_ENABLED      false

// ============================================================================
// OPTIONS D'AFFICHAGE
// ============================================================================

/**
 * @brief Utiliser les polices vectorielles Adafruit GFX
 * true = Polices élégantes et scalables
 * false = Polices bitmap intégrées (plus rapides mais moins jolies)
 */
#define USE_ADAFRUIT_FONTS              true

/**
 * @brief Activer le mode double buffer pour animations fluides
 * Consomme plus de RAM mais améliore la fluidité
 */
#define USE_DOUBLE_BUFFER               true

/**
 * @brief Niveau de luminosité par défaut du rétroéclairage (0-255)
 * 255 = 100%, 128 = 50%, 0 = éteint
 */
#define DEFAULT_BACKLIGHT_LEVEL         255

// ============================================================================
// OPTIONS AUDIO
// ============================================================================

/**
 * @brief Volume par défaut (0-100)
 * 0 = muet, 100 = volume maximum
 */
#define DEFAULT_AUDIO_VOLUME            70

/**
 * @brief Activer les logs audio pour debug
 */
#define AUDIO_DEBUG_ENABLED             false

// ============================================================================
// OPTIONS CARTE SD
// ============================================================================

/**
 * @brief Chemin racine des fichiers audio sur la carte SD
 */
#define SD_AUDIO_PATH                   "/audio"

/**
 * @brief Activer les logs SD pour debug
 */
#define SD_DEBUG_ENABLED                false

// ============================================================================
// OPTIONS DEBUG GLOBALES
// ============================================================================

/**
 * @brief Niveau de debug série (0=aucun, 1=erreurs, 2=warnings, 3=info, 4=verbose)
 */
#define DEBUG_LEVEL                     3

/**
 * @brief Activer le banner de démarrage stylisé
 */
#define SHOW_STARTUP_BANNER             true

#endif // FEATURES_H
