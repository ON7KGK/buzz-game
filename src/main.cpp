/**
 * @file main.cpp
 * @brief Programme principal pour le module Waveshare ESP32-S3-Touch-LCD-3.5B-C
 *
 * Cet exemple démontre:
 * - L'initialisation complète du module
 * - L'affichage graphique sur l'écran LCD 3.5"
 * - La détection tactile capacitive
 * - Le dessin interactif en temps réel
 * - L'utilisation du bouton BOOT
 *
 * Module: Waveshare ESP32-S3-Touch-LCD-3.5B-C
 * - ESP32-S3R8 (dual-core 240MHz, 8MB PSRAM, 16MB Flash)
 * - Écran IPS 3.5" 320x480 RGB565
 * - Touch capacitif I2C
 * - ES8311 Audio codec
 * - QMI8658 IMU 6-axis
 * - AXP2101 Power Management
 * - SD Card slot
 * - OV5640 Camera 5MP (version -C)
 *
 * @author Michaël - SPARKOH!
 * @date 2025
 * @version 1.0.0
 */

#include <Arduino.h>
#include <WaveshareESP32S3.h>

// ═══════════════════════════════════════════════════════════════════════════
// CONFIGURATION GLOBALE
// ═══════════════════════════════════════════════════════════════════════════

// Instance principale du module Waveshare
WaveshareESP32S3 board;

// Variables pour le système de dessin
uint16_t lastX = 0, lastY = 0;
bool drawing = false;

// Compteurs pour statistiques
unsigned long touchCount = 0;
unsigned long frameCount = 0;
unsigned long lastStatsTime = 0;

// ═══════════════════════════════════════════════════════════════════════════
// PROTOTYPES DES FONCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void displayWelcomeScreen();
void setupDrawingScreen();
void displayStats();
void testColorPalette();

// ═══════════════════════════════════════════════════════════════════════════
// SETUP - INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    // Initialisation du port série
    Serial.begin(115200);
    delay(1000);

    // Affichage du banner de démarrage
    Serial.println("\n");
    Serial.println("╔═══════════════════════════════════════════════════════════╗");
    Serial.println("║                                                           ║");
    Serial.println("║        Waveshare ESP32-S3-Touch-LCD-3.5B-C               ║");
    Serial.println("║        Programme de Test Complet                         ║");
    Serial.println("║                                                           ║");
    Serial.println("║        SPARKOH! - Centre de Culture Scientifique         ║");
    Serial.println("║        Développé par: Michaël                            ║");
    Serial.println("║        Version: 1.0.0                                    ║");
    Serial.println("║                                                           ║");
    Serial.println("╚═══════════════════════════════════════════════════════════╝");
    Serial.println();

    // Informations système
    Serial.println("📋 INFORMATIONS SYSTÈME:");
    Serial.printf("   • MCU: %s\n", "ESP32-S3R8");
    Serial.printf("   • Fréquence CPU: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("   • Flash: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
    Serial.printf("   • PSRAM: %d KB\n", ESP.getPsramSize() / 1024);
    Serial.printf("   • Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);
    Serial.println();

    // Initialisation du module Waveshare
    Serial.println("🔧 INITIALISATION DU MODULE...");
    Serial.println();

    if (!board.begin()) {
        Serial.println("❌ ERREUR CRITIQUE: Échec de l'initialisation!");
        Serial.println("   Vérifiez:");
        Serial.println("   - Les connexions du module");
        Serial.println("   - L'alimentation (min 5V/2A)");
        Serial.println("   - Les câbles USB");
        Serial.println();
        Serial.println("⚠️  Le système va redémarrer dans 5 secondes...");

        // Boucle infinie avec clignotement d'erreur
        while (1) {
            delay(500);
        }
    }

    Serial.println("✅ Module initialisé avec succès!");
    Serial.println();

    // Afficher les informations du module
    board.printInfo();
    Serial.println();

    // Test de la palette de couleurs
    Serial.println("🎨 Test de la palette de couleurs...");
    testColorPalette();
    delay(1500);

    // Écran de bienvenue
    Serial.println("🖥️  Affichage de l'écran de bienvenue...");
    displayWelcomeScreen();
    delay(3000);

    // Préparation de l'écran de dessin
    Serial.println("🎨 Préparation de l'écran de dessin...");
    setupDrawingScreen();

    // Prêt!
    Serial.println();
    Serial.println("╔═══════════════════════════════════════════════════════════╗");
    Serial.println("║  ✅ SYSTÈME PRÊT                                         ║");
    Serial.println("║                                                           ║");
    Serial.println("║  📱 Touchez l'écran pour dessiner                        ║");
    Serial.println("║  🔘 Bouton BOOT pour effacer                             ║");
    Serial.println("║  📊 Statistiques en temps réel sur le Serial             ║");
    Serial.println("╚═══════════════════════════════════════════════════════════╝");
    Serial.println();

    lastStatsTime = millis();
}

// ═══════════════════════════════════════════════════════════════════════════
// LOOP - BOUCLE PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
    // Vérifier le bouton BOOT pour effacer l'écran
    if (board.bootButtonPressed()) {
        Serial.println("🗑️  Effacement de l'écran...");
        setupDrawingScreen();
        touchCount = 0;
        frameCount = 0;
        delay(300);  // Debounce
    }

    // Lire l'état du tactile
    if (board.touch.read()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();

        // Ignorer la zone d'en-tête (première touche après le titre)
        if (y < 70) {
            drawing = false;
            delay(10);
            return;
        }

        // Début d'un nouveau trait
        if (!drawing) {
            drawing = true;
            lastX = x;
            lastY = y;
            touchCount++;
            Serial.printf("🎨 Dessin #%lu démarré à (%d, %d)\n", touchCount, x, y);
        }

        // Dessiner une ligne depuis le dernier point
        board.display.drawLine(lastX, lastY, x, y, COLOR_WHITE);

        // Option: dessiner un point plus épais
        board.display.fillCircle(x, y, 2, COLOR_WHITE);

        // Mettre à jour la position précédente
        lastX = x;
        lastY = y;

        frameCount++;
    } else {
        // Fin du trait
        if (drawing) {
            drawing = false;
            Serial.printf("✋ Dessin #%lu terminé (%lu points)\n", touchCount, frameCount);
        }
    }

    // Afficher les statistiques toutes les 5 secondes
    if (millis() - lastStatsTime > 5000) {
        displayStats();
        lastStatsTime = millis();
    }

    delay(10);  // Délai pour éviter la surcharge CPU
}

// ═══════════════════════════════════════════════════════════════════════════
// FONCTIONS D'AFFICHAGE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Affiche l'écran de bienvenue avec logo SPARKOH!
 */
void displayWelcomeScreen() {
    board.display.clear(COLOR_SPARKOH_BLUE);

    // Cadre décoratif externe
    board.display.drawRect(5, 5, board.display.width()-10,
                          board.display.height()-10, COLOR_WHITE);
    board.display.drawRect(7, 7, board.display.width()-14,
                          board.display.height()-14, COLOR_SPARKOH_ORANGE);

    // Logo SPARKOH!
    board.display.setTextSize(4);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(30, 60);
    board.display.println("SPARKOH!");

    // Sous-titre
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_SPARKOH_ORANGE);
    board.display.setCursor(40, 120);
    board.display.println("ESP32-S3");

    board.display.setTextColor(COLOR_YELLOW);
    board.display.setCursor(25, 150);
    board.display.println("Touch LCD 3.5\"");

    // Ligne de séparation
    board.display.drawLine(30, 190, board.display.width()-30, 190, COLOR_WHITE);

    // Informations techniques
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_WHITE);

    board.display.setCursor(20, 210);
    board.display.print("Resolution: ");
    board.display.print(board.display.width());
    board.display.print(" x ");
    board.display.println(board.display.height());

    board.display.setCursor(20, 230);
    board.display.print("Tactile: ");
    if (board.touch.isInitialized()) {
        board.display.setTextColor(COLOR_GREEN);
        board.display.println("OK");
    } else {
        board.display.setTextColor(COLOR_RED);
        board.display.println("ERREUR");
    }

    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(20, 250);
    board.display.print("Luminosite: ");
    board.display.print(board.display.getBrightness());
    board.display.println("%");

    // Instructions
    board.display.setTextColor(COLOR_YELLOW);
    board.display.setTextSize(2);
    board.display.setCursor(30, 300);
    board.display.println("Pret a");
    board.display.setCursor(30, 330);
    board.display.println("dessiner!");

    // Version
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_GRAY);
    board.display.setCursor(20, board.display.height() - 30);
    board.display.print("v");
    board.display.println(board.getVersion());
}

/**
 * @brief Configure l'écran pour le mode dessin
 */
void setupDrawingScreen() {
    board.display.clear(COLOR_BLACK);

    // Titre en haut
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_SPARKOH_BLUE);
    board.display.setCursor(20, 10);
    board.display.println("Dessin Tactile");

    // Instructions
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_GRAY);
    board.display.setCursor(20, 40);
    board.display.println("BOOT = Effacer  |  Dessinez!");

    // Ligne de séparation
    board.display.drawLine(0, 65, board.display.width(), 65, COLOR_SPARKOH_ORANGE);
    board.display.drawLine(0, 67, board.display.width(), 67, COLOR_SPARKOH_ORANGE);

    // Petit guide visuel dans les coins
    board.display.fillCircle(10, 80, 3, COLOR_DARK_GRAY);
    board.display.fillCircle(board.display.width()-10, 80, 3, COLOR_DARK_GRAY);
}

/**
 * @brief Affiche les statistiques système sur le Serial
 */
void displayStats() {
    Serial.println("📊 STATISTIQUES:");
    Serial.printf("   • Dessins réalisés: %lu\n", touchCount);
    Serial.printf("   • Points dessinés: %lu\n", frameCount);
    Serial.printf("   • Heap libre: %d KB\n", ESP.getFreeHeap() / 1024);
    Serial.printf("   • Uptime: %lu secondes\n", millis() / 1000);
    Serial.println();
}

/**
 * @brief Test de la palette de couleurs
 */
void testColorPalette() {
    board.display.clear(COLOR_BLACK);

    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(50, 20);
    board.display.println("Test Couleurs");

    // Bandes de couleurs
    int barHeight = 40;
    int y = 80;

    board.display.fillRect(0, y, board.display.width(), barHeight, COLOR_RED);
    y += barHeight;
    board.display.fillRect(0, y, board.display.width(), barHeight, COLOR_GREEN);
    y += barHeight;
    board.display.fillRect(0, y, board.display.width(), barHeight, COLOR_BLUE);
    y += barHeight;
    board.display.fillRect(0, y, board.display.width(), barHeight, COLOR_YELLOW);
    y += barHeight;
    board.display.fillRect(0, y, board.display.width(), barHeight, COLOR_MAGENTA);
    y += barHeight;
    board.display.fillRect(0, y, board.display.width(), barHeight, COLOR_CYAN);

    // Couleurs SPARKOH!
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(10, board.display.height() - 60);
    board.display.println("Palette SPARKOH!:");

    int boxSize = 30;
    int startX = 10;
    y = board.display.height() - 40;

    board.display.fillRect(startX, y, boxSize, boxSize, COLOR_SPARKOH_BLUE);
    board.display.fillRect(startX + boxSize + 5, y, boxSize, boxSize, COLOR_SPARKOH_GREEN);
    board.display.fillRect(startX + (boxSize + 5) * 2, y, boxSize, boxSize, COLOR_SPARKOH_ORANGE);
}
