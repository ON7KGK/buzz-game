/**
 * @file BasicTest.ino
 * @brief Exemple de base pour tester le module Waveshare ESP32-S3-Touch-LCD-3.5B-C
 * 
 * Cet exemple montre comment:
 * - Initialiser le module
 * - Afficher du texte et des formes sur l'écran
 * - Lire les coordonnées tactiles
 * - Dessiner en temps réel avec le doigt
 * 
 * @author Michaël - SPARKOH!
 */

#include <WaveshareESP32S3.h>

// Créer une instance du module
WaveshareESP32S3 board;

// Variables pour le dessin
uint16_t lastX = 0, lastY = 0;
bool drawing = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n╔═══════════════════════════════════════╗");
    Serial.println("║  Waveshare ESP32-S3 Basic Test       ║");
    Serial.println("║  SPARKOH! - Michaël                   ║");
    Serial.println("╚═══════════════════════════════════════╝\n");
    
    // Initialiser le module complet
    if (!board.begin()) {
        Serial.println("❌ ERREUR: Échec de l'initialisation!");
        Serial.println("Vérifiez les connexions et redémarrez.");
        while (1) {
            delay(1000);
        }
    }
    
    // Afficher l'écran de bienvenue
    displayWelcomeScreen();
    
    delay(2000);
    
    // Préparer l'écran pour le dessin
    setupDrawingScreen();
    
    Serial.println("\n✅ Prêt! Touchez l'écran pour dessiner.");
    Serial.println("   Appuyez sur BOOT pour effacer.\n");
}

void loop() {
    // Vérifier le bouton BOOT pour effacer l'écran
    if (board.bootButtonPressed()) {
        Serial.println("🗑️  Effacement de l'écran...");
        setupDrawingScreen();
        delay(300);  // Debounce
    }
    
    // Lire le tactile
    if (board.touch.read()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();
        
        // Première touche
        if (!drawing) {
            drawing = true;
            lastX = x;
            lastY = y;
            Serial.printf("🎨 Début du dessin à (%d, %d)\n", x, y);
        }
        
        // Dessiner une ligne depuis le dernier point
        board.display.drawLine(lastX, lastY, x, y, COLOR_WHITE);
        
        // Mettre à jour la position
        lastX = x;
        lastY = y;
    } else {
        // Réinitialiser l'état de dessin
        if (drawing) {
            drawing = false;
            Serial.println("✋ Fin du dessin");
        }
    }
    
    delay(10);  // Petit délai pour éviter de surcharger
}

/**
 * @brief Affiche l'écran de bienvenue
 */
void displayWelcomeScreen() {
    board.display.clear(COLOR_SPARKOH_BLUE);
    
    // Titre
    board.display.setTextSize(3);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(20, 50);
    board.display.println("SPARKOH!");
    
    // Sous-titre
    board.display.setTextSize(2);
    board.display.setCursor(30, 100);
    board.display.println("ESP32-S3");
    board.display.setCursor(30, 130);
    board.display.println("Test Display");
    
    // Informations
    board.display.setTextSize(1);
    board.display.setCursor(10, 200);
    board.display.print("Resolution: ");
    board.display.print(board.display.width());
    board.display.print("x");
    board.display.println(board.display.height());
    
    board.display.setCursor(10, 220);
    board.display.print("Touch: ");
    if (board.touch.isInitialized()) {
        board.display.setTextColor(COLOR_GREEN);
        board.display.println("OK");
    } else {
        board.display.setTextColor(COLOR_RED);
        board.display.println("ERROR");
    }
    
    // Instructions
    board.display.setTextColor(COLOR_YELLOW);
    board.display.setTextSize(1);
    board.display.setCursor(10, 280);
    board.display.println("Touchez l'ecran");
    board.display.setCursor(10, 300);
    board.display.println("pour dessiner!");
    
    // Cadre décoratif
    board.display.drawRect(5, 5, board.display.width()-10, 
                          board.display.height()-10, COLOR_WHITE);
    board.display.drawRect(7, 7, board.display.width()-14, 
                          board.display.height()-14, COLOR_SPARKOH_ORANGE);
}

/**
 * @brief Prépare l'écran pour le dessin
 */
void setupDrawingScreen() {
    board.display.clear(COLOR_BLACK);
    
    // Titre en haut
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_SPARKOH_BLUE);
    board.display.setCursor(10, 10);
    board.display.println("Dessin Tactile");
    
    // Instructions
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_GRAY);
    board.display.setCursor(10, 40);
    board.display.println("Bouton BOOT = Effacer");
    
    // Ligne de séparation
    board.display.drawLine(0, 60, board.display.width(), 60, COLOR_GRAY);
    
    // Zone de dessin (petit indicateur)
    board.display.setTextColor(COLOR_DARK_GRAY);
    board.display.setCursor(10, board.display.height() - 20);
    board.display.println("Zone de dessin...");
}
