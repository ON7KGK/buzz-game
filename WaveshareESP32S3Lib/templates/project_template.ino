/**
 * @file project_template.ino
 * @brief Template de projet pour Waveshare ESP32-S3-Touch-LCD-3.5B-C
 * 
 * Utilisez ce template comme point de départ pour vos projets.
 * Copiez ce fichier et modifiez-le selon vos besoins.
 * 
 * @author Votre Nom
 * @date YYYY-MM-DD
 * @version 1.0.0
 */

#include <WaveshareESP32S3.h>

// =============================================================================
// CONFIGURATION
// =============================================================================

// Créer l'instance du module
WaveshareESP32S3 board;

// Constantes de votre projet
const char* PROJECT_NAME = "Mon Projet SPARKOH!";
const char* VERSION = "1.0.0";

// Variables globales
// TODO: Ajoutez vos variables ici

// =============================================================================
// SETUP - Exécuté une fois au démarrage
// =============================================================================

void setup() {
    // Initialiser le port série
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.printf("║ %s\n", PROJECT_NAME);
    Serial.printf("║ Version %s\n", VERSION);
    Serial.println("╚════════════════════════════════════════╝\n");
    
    // Initialiser le module Waveshare
    Serial.println("Initialisation du module...");
    if (!board.begin()) {
        Serial.println("❌ ERREUR: Échec de l'initialisation!");
        Serial.println("Vérifiez les connexions et redémarrez.");
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("✅ Module initialisé avec succès!\n");
    
    // TODO: Votre code d'initialisation ici
    setupUI();
    setupInteractions();
    
    Serial.println("✅ Setup terminé!\n");
}

// =============================================================================
// LOOP - Exécuté en boucle
// =============================================================================

void loop() {
    // TODO: Votre code principal ici
    
    // Gérer les interactions tactiles
    handleTouch();
    
    // Gérer les boutons
    handleButtons();
    
    // Mettre à jour l'affichage
    updateDisplay();
    
    // Petit délai pour ne pas surcharger le CPU
    delay(10);
}

// =============================================================================
// FONCTIONS D'INTERFACE UTILISATEUR
// =============================================================================

/**
 * @brief Configure l'interface utilisateur initiale
 */
void setupUI() {
    // Effacer l'écran avec une couleur de fond
    board.display.clear(COLOR_BLACK);
    
    // TODO: Dessinez votre interface initiale
    
    // Exemple: Afficher un titre
    board.display.setTextSize(3);
    board.display.setTextColor(COLOR_SPARKOH_BLUE);
    board.display.setCursor(20, 50);
    board.display.println(PROJECT_NAME);
    
    // Exemple: Instructions
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(20, 150);
    board.display.println("Touchez l'ecran pour commencer");
    
    Serial.println("[UI] Interface initialisée");
}

/**
 * @brief Configure les interactions (boutons, zones tactiles, etc.)
 */
void setupInteractions() {
    // TODO: Définissez vos zones interactives
    
    Serial.println("[Interactions] Zones configurées");
}

/**
 * @brief Met à jour l'affichage
 */
void updateDisplay() {
    // TODO: Mettez à jour les éléments dynamiques de l'écran
    
    // Exemple: Afficher le temps écoulé
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 1000) {  // Mise à jour chaque seconde
        lastUpdate = millis();
        
        // Votre code de mise à jour ici
    }
}

// =============================================================================
// GESTION DES INTERACTIONS
// =============================================================================

/**
 * @brief Gère les événements tactiles
 */
void handleTouch() {
    if (board.touch.touched()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();
        
        Serial.printf("[Touch] Point: (%d, %d)\n", x, y);
        
        // TODO: Gérer les zones tactiles
        // Exemple:
        // if (isTouchInButton(x, y, button1)) {
        //     onButton1Pressed();
        // }
        
        // Feedback visuel temporaire
        board.display.fillCircle(x, y, 5, COLOR_RED);
    }
}

/**
 * @brief Gère les boutons physiques
 */
void handleButtons() {
    // Bouton BOOT
    static bool lastBootState = false;
    bool currentBootState = board.bootButtonPressed();
    
    if (currentBootState && !lastBootState) {
        // Appui détecté
        Serial.println("[Button] BOOT pressé");
        onBootButtonPressed();
    }
    
    lastBootState = currentBootState;
}

// =============================================================================
// CALLBACKS ET HANDLERS
// =============================================================================

/**
 * @brief Appelé quand le bouton BOOT est pressé
 */
void onBootButtonPressed() {
    Serial.println("[Event] Reset de l'affichage");
    setupUI();
}

/**
 * @brief Vérifie si un point est dans un rectangle
 */
bool isTouchInRect(uint16_t x, uint16_t y, uint16_t rectX, uint16_t rectY, 
                   uint16_t rectW, uint16_t rectH) {
    return (x >= rectX && x <= rectX + rectW && 
            y >= rectY && y <= rectY + rectH);
}

// =============================================================================
// FONCTIONS UTILITAIRES
// =============================================================================

/**
 * @brief Affiche un message de debug
 */
void debugPrint(const char* tag, const char* message) {
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.printf("[%s] %s\n", tag, message);
    #endif
}

/**
 * @brief Convertit RGB en RGB565
 */
uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return board.display.color565(r, g, b);
}

// =============================================================================
// NOTES ET TODO
// =============================================================================

/*
 * TODO Liste:
 * - [ ] Implémenter la logique principale
 * - [ ] Ajouter les graphismes
 * - [ ] Tester les interactions tactiles
 * - [ ] Ajouter du son si nécessaire
 * - [ ] Optimiser les performances
 * - [ ] Documenter le code
 * 
 * Notes:
 * - Mémoire PSRAM disponible pour grandes images
 * - Utiliser double buffering pour animations fluides
 * - Tester sur batterie pour consommation
 */
