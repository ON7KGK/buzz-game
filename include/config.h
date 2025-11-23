/*
 * CONFIG.H - Configuration ESP32-S3-Touch-LCD-3.5B
 * Toutes les constantes et paramètres du projet
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// PINS LCD
// ============================================
#define LCD_QSPI_CS   12
#define LCD_QSPI_CLK  5
#define LCD_QSPI_D0   1
#define LCD_QSPI_D1   2
#define LCD_QSPI_D2   3
#define LCD_QSPI_D3   4
#define GFX_BL        6

// ============================================
// PINS BOUTONS
// ============================================
#define BTN_MODE_ROUGE  21  // Bouton pour mode rouge

// ============================================
// PINS I2C
// ============================================
#define I2C_SDA  8
#define I2C_SCL  7
#define TCA9554_ADDR  0x20

// ============================================
// ROTATION ÉCRAN
// ============================================
// 0 = Portrait USB bas
// 1 = Paysage USB droite
// 2 = Portrait USB haut
// 3 = Paysage USB gauche
#define ROTATION 1

// ============================================
// DIMENSIONS ÉCRAN
// ============================================
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 480

// ============================================
// COULEURS
// ============================================
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_ORANGE  0xFD20
#define COLOR_PURPLE  0x780F
#define COLOR_CYAN    0x07FF
#define COLOR_GRAY    0x7BEF

// ============================================
// ZONES D'AFFICHAGE
// ============================================
#define ZONE_COMPTEUR_Y     150
#define ZONE_COMPTEUR_H     150
#define ZONE_CONSIGNE_Y     350
#define ZONE_CONSIGNE_H     50

// ============================================
// TAILLES DE POLICE
// ============================================
#define TAILLE_COMPTEUR     12   // Grand chiffre central
#define TAILLE_CONSIGNE     2    // Texte de consigne
#define TAILLE_INFO         1    // Petites infos

// ============================================
// TIMING
// ============================================
#define INTERVALLE_COMPTEUR  1000  // ms entre chaque incrément
#define DEBOUNCE_DELAY       50    // ms pour anti-rebond bouton

// ============================================
// MODES DE FONCTIONNEMENT
// ============================================
enum Mode {
  MODE_STANDBY,
  MODE_VERT,
  MODE_ROUGE
};

#endif
