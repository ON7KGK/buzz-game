/**
 * @file display_config.h
 * @brief Configuration avancée de l'affichage LCD AXS15231B
 */

#ifndef WAVESHARE_DISPLAY_CONFIG_H
#define WAVESHARE_DISPLAY_CONFIG_H

#include "pins_definitions.h"

// ============================================================================
// PARAMÈTRES DE L'ÉCRAN
// ============================================================================

// Résolution
#define DISPLAY_WIDTH               LCD_WIDTH
#define DISPLAY_HEIGHT              LCD_HEIGHT

// Orientation par défaut
// 0 = Portrait (320x480)
// 1 = Paysage (480x320)
// 2 = Portrait inversé (320x480)
// 3 = Paysage inversé (480x320)
#define DISPLAY_DEFAULT_ROTATION    0

// Profondeur de couleur
#define DISPLAY_COLOR_DEPTH         16    // 16-bit (RGB565)

// ============================================================================
// RÉTROÉCLAIRAGE
// ============================================================================

// Luminosité par défaut (0-255)
#define BACKLIGHT_DEFAULT           204   // ~80%
#define BACKLIGHT_MIN               25    // ~10%
#define BACKLIGHT_MAX               255   // 100%

// Fréquence PWM pour le rétroéclairage (Hz)
#define BACKLIGHT_PWM_FREQ          5000
#define BACKLIGHT_PWM_CHANNEL       0
#define BACKLIGHT_PWM_RESOLUTION    8     // 8-bit = 0-255

// ============================================================================
// BUFFER D'AFFICHAGE
// ============================================================================

// Utiliser un double buffer pour des animations fluides
#define USE_DOUBLE_BUFFER           true

// Taille du buffer (en pixels)
// Pour économiser la RAM, vous pouvez réduire cette valeur
// Valeurs communes: 320*10, 320*20, 320*40, 320*480 (full screen)
#define BUFFER_HEIGHT               40
#define BUFFER_SIZE                 (LCD_WIDTH * BUFFER_HEIGHT)

// ============================================================================
// QSPI CONFIGURATION
// ============================================================================

// Fréquence du bus QSPI (Hz)
// Valeurs testées: 40MHz, 60MHz, 80MHz
// Plus rapide = meilleur framerate, mais peut être instable
#define QSPI_FREQUENCY              80000000L  // 80 MHz

// Mode QSPI
#define QSPI_MODE                   0

// ============================================================================
// PERFORMANCE
// ============================================================================

// Activer le DMA pour des transferts plus rapides
#define ENABLE_DMA                  true

// Taille du buffer DMA
#define DMA_BUFFER_SIZE             4096

// ============================================================================
// TACTILE
// ============================================================================

// Nombre de points de contact maximum
#define TOUCH_MAX_POINTS            1

// Seuil de pression (si supporté)
#define TOUCH_PRESSURE_THRESHOLD    50

// Inverser les axes X/Y si nécessaire
#define TOUCH_SWAP_XY               false
#define TOUCH_INVERT_X              false
#define TOUCH_INVERT_Y              false

// Calibration du tactile
// Ajuster ces valeurs si le tactile n'est pas précis
#define TOUCH_CAL_X_MIN             0
#define TOUCH_CAL_X_MAX             LCD_WIDTH
#define TOUCH_CAL_Y_MIN             0
#define TOUCH_CAL_Y_MAX             LCD_HEIGHT

// ============================================================================
// LVGL CONFIGURATION (si activé)
// ============================================================================
#ifdef FEATURE_LVGL_ENABLED

// Taille du buffer LVGL (pixels)
#define LVGL_BUFFER_SIZE            (LCD_WIDTH * 40)

// Utiliser 2 buffers pour LVGL
#define LVGL_USE_DOUBLE_BUFFER      true

// Fréquence de rafraîchissement (ms)
#define LVGL_TICK_PERIOD_MS         5

// Taille de la pile pour la tâche LVGL
#define LVGL_TASK_STACK_SIZE        4096

// Priorité de la tâche LVGL
#define LVGL_TASK_PRIORITY          1

// Core sur lequel exécuter LVGL (0 ou 1)
#define LVGL_TASK_CORE              1

#endif // FEATURE_LVGL_ENABLED

// ============================================================================
// COULEURS PRÉDÉFINIES (RGB565)
// ============================================================================

#define COLOR_BLACK                 0x0000
#define COLOR_WHITE                 0xFFFF
#define COLOR_RED                   0xF800
#define COLOR_GREEN                 0x07E0
#define COLOR_BLUE                  0x001F
#define COLOR_YELLOW                0xFFE0
#define COLOR_CYAN                  0x07FF
#define COLOR_MAGENTA               0xF81F
#define COLOR_ORANGE                0xFD20
#define COLOR_PURPLE                0x8010
#define COLOR_GRAY                  0x8410
#define COLOR_LIGHT_GRAY            0xC618
#define COLOR_DARK_GRAY             0x4208

// Couleurs SPARKOH! (pour tes projets au musée)
#define COLOR_SPARKOH_BLUE          0x0357  // Bleu principal
#define COLOR_SPARKOH_GREEN         0x07E0  // Vert
#define COLOR_SPARKOH_ORANGE        0xFC00  // Orange

#endif // WAVESHARE_DISPLAY_CONFIG_H
