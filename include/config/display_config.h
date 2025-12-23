/**
 * @file display_config.h
 * @brief Configuration de l'affichage LCD AXS15231B 3.5"
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

// ============================================================================
// PINS LCD (QSPI Interface)
// ============================================================================
#define LCD_QSPI_CS             12      // Chip Select
#define LCD_QSPI_CLK            5       // Clock
#define LCD_QSPI_D0             1       // Data 0
#define LCD_QSPI_D1             2       // Data 1
#define LCD_QSPI_D2             3       // Data 2
#define LCD_QSPI_D3             4       // Data 3
#define LCD_BL                  6       // Backlight PWM

// ============================================================================
// RÉSOLUTION ET ORIENTATION
// ============================================================================
#define SCREEN_WIDTH            320
#define SCREEN_HEIGHT           480

/**
 * Rotation de l'écran:
 * 0 = Portrait (USB en bas)     - 320x480
 * 1 = Paysage (USB à droite)    - 480x320
 * 2 = Portrait inversé (USB haut) - 320x480
 * 3 = Paysage inversé (USB gauche) - 480x320
 */
#define SCREEN_ROTATION         1       // Par défaut: paysage

// ============================================================================
// COULEURS PRÉDÉFINIES (RGB565)
// ============================================================================
#define COLOR_BLACK             0x0000
#define COLOR_WHITE             0xFFFF
#define COLOR_RED               0xF800
#define COLOR_BRIGHT_RED        0xFB00  // Rouge plus lumineux (rouge + un peu de vert)
#define COLOR_GREEN             0x07E0
#define COLOR_BLUE              0x001F
#define COLOR_YELLOW            0xFFE0
#define COLOR_ORANGE            0xFD20
#define COLOR_PURPLE            0x780F
#define COLOR_CYAN              0x07FF
#define COLOR_MAGENTA           0xF81F
#define COLOR_GRAY              0x8410
#define COLOR_LIGHT_GRAY        0xC618
#define COLOR_DARK_GRAY         0x4208

// Couleurs SPARKOH! (pour projets muséaux)
#define COLOR_SPARKOH_BLUE      0x0357  // Bleu principal
#define COLOR_SPARKOH_GREEN     0x07E0  // Vert
#define COLOR_SPARKOH_ORANGE    0xFC00  // Orange

#endif // DISPLAY_CONFIG_H
