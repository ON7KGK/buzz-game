/**
 * @file WaveshareESP32S3.h
 * @brief Classe principale pour le module Waveshare ESP32-S3-Touch-LCD-3.5B-C
 * 
 * Cette classe regroupe tous les périphériques du module et permet
 * de les initialiser et les gérer facilement.
 * 
 * @author Michaël - SPARKOH!
 * @date 2025
 */

#ifndef WAVESHARE_ESP32S3_H
#define WAVESHARE_ESP32S3_H

#include <Arduino.h>
#include <Wire.h>

// Configuration
#include "config/features.h"
#include "config/pins_definitions.h"
#include "config/display_config.h"
#include "config/audio_config.h"
#include "config/sdcard_config.h"
#include "config/camera_config.h"

// Périphériques
#ifdef FEATURE_DISPLAY_ENABLED
#include "peripherals/Display.h"
#endif

#ifdef FEATURE_TOUCH_ENABLED
#include "peripherals/TouchScreen.h"
#endif

/**
 * @class WaveshareESP32S3
 * @brief Classe principale pour gérer tous les composants du module
 * 
 * Exemple d'utilisation:
 * @code
 * #include <WaveshareESP32S3.h>
 * 
 * WaveshareESP32S3 board;
 * 
 * void setup() {
 *     Serial.begin(115200);
 *     
 *     // Initialiser tout le module
 *     if (!board.begin()) {
 *         Serial.println("Erreur initialisation!");
 *         while(1);
 *     }
 *     
 *     // Utiliser l'écran
 *     board.display.clear(COLOR_BLUE);
 *     board.display.setTextColor(COLOR_WHITE);
 *     board.display.setCursor(10, 10);
 *     board.display.println("SPARKOH!");
 * }
 * 
 * void loop() {
 *     // Lire le tactile
 *     if (board.touch.touched()) {
 *         uint16_t x = board.touch.getX();
 *         uint16_t y = board.touch.getY();
 *         board.display.fillCircle(x, y, 5, COLOR_RED);
 *     }
 * }
 * @endcode
 */
class WaveshareESP32S3 {
public:
    /**
     * @brief Constructeur
     */
    WaveshareESP32S3();
    
    /**
     * @brief Destructeur
     */
    ~WaveshareESP32S3();
    
    // ========================================================================
    // INITIALISATION
    // ========================================================================
    
    /**
     * @brief Initialise le module complet
     * 
     * Cette méthode initialise tous les périphériques activés dans features.h
     * 
     * @return true si succès, false si erreur
     */
    bool begin();
    
    /**
     * @brief Initialise uniquement l'affichage et le tactile
     * @return true si succès
     */
    bool beginBasic();
    
    /**
     * @brief Termine et libère toutes les ressources
     */
    void end();
    
    // ========================================================================
    // PÉRIPHÉRIQUES
    // ========================================================================
    
    #ifdef FEATURE_DISPLAY_ENABLED
    WaveshareDisplay display;      ///< Écran LCD 3.5"
    #endif
    
    #ifdef FEATURE_TOUCH_ENABLED
    WaveshareTouch touch;          ///< Écran tactile capacitif
    #endif
    
    // Les autres périphériques seront ajoutés ici
    // audio, imu, rtc, sdcard, camera, power
    
    // ========================================================================
    // UTILITAIRES
    // ========================================================================
    
    /**
     * @brief Affiche les informations du module
     */
    void printInfo();
    
    /**
     * @brief Teste tous les périphériques
     * @return true si tous les tests passent
     */
    bool testAll();
    
    /**
     * @brief Lit l'état du bouton BOOT
     * @return true si pressé, false sinon
     */
    bool bootButtonPressed();
    
    /**
     * @brief Obtient la version de la bibliothèque
     */
    const char* getVersion() { return LIBRARY_VERSION; }
    
    /**
     * @brief Vérifie si le module est initialisé
     */
    bool isInitialized() { return _initialized; }

private:
    bool _initialized;
    
    static constexpr const char* LIBRARY_VERSION = "1.0.0";
    
    void initI2C();
    void printBanner();
};

#endif // WAVESHARE_ESP32S3_H
