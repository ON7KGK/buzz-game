/**
 * @file WaveshareESP32S3.cpp
 * @brief Implémentation de la classe principale
 */

#include "WaveshareESP32S3.h"

WaveshareESP32S3::WaveshareESP32S3() 
    : _initialized(false) {
}

WaveshareESP32S3::~WaveshareESP32S3() {
    end();
}

bool WaveshareESP32S3::begin() {
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("\n===========================================");
    Serial.println("  Waveshare ESP32-S3-Touch-LCD-3.5B-C");
    Serial.println("  Bibliothèque v" + String(LIBRARY_VERSION));
    Serial.println("  Par Michaël - SPARKOH!");
    Serial.println("===========================================\n");
    #endif
    
    // Initialiser le port série si debug activé
    #ifdef FEATURE_SERIAL_DEBUG
    if (!Serial) {
        Serial.begin(115200);
        delay(1000);
    }
    Serial.println("[Board] Démarrage de l'initialisation...");
    #endif
    
    // Initialiser les bus I2C
    initI2C();
    
    // Initialiser l'affichage
    #ifdef FEATURE_DISPLAY_ENABLED
    Serial.println("[Board] Initialisation de l'écran...");
    if (!display.begin()) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.println("[Board] ERREUR: Échec initialisation écran!");
        #endif
        return false;
    }
    Serial.println("[Board] Écran initialisé ✓");
    #endif
    
    // Initialiser le tactile
    #ifdef FEATURE_TOUCH_ENABLED
    Serial.println("[Board] Initialisation du tactile...");
    // Utiliser un bus I2C séparé pour le tactile
    Wire1.begin(TOUCH_SDA, TOUCH_SCL);
    if (!touch.begin(Wire1)) {
        #ifdef FEATURE_SERIAL_DEBUG
        Serial.println("[Board] ERREUR: Échec initialisation tactile!");
        #endif
        return false;
    }
    Serial.println("[Board] Tactile initialisé ✓");
    #endif
    
    // Initialiser le bouton BOOT
    pinMode(BTN_BOOT, INPUT_PULLUP);
    
    // TODO: Initialiser les autres périphériques selon features.h
    // - Audio (ES8311)
    // - IMU (QMI8658)
    // - RTC (PCF85063)
    // - Power Management (AXP2101)
    // - SD Card
    // - Camera (si version -C)
    
    _initialized = true;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("\n[Board] Initialisation terminée avec succès! 🎉");
    printInfo();
    #endif
    
    return true;
}

bool WaveshareESP32S3::beginBasic() {
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Board] Initialisation basique (Display + Touch)");
    #endif
    
    initI2C();
    
    #ifdef FEATURE_DISPLAY_ENABLED
    if (!display.begin()) {
        return false;
    }
    #endif
    
    #ifdef FEATURE_TOUCH_ENABLED
    Wire1.begin(TOUCH_SDA, TOUCH_SCL);
    if (!touch.begin(Wire1)) {
        return false;
    }
    #endif
    
    pinMode(BTN_BOOT, INPUT_PULLUP);
    
    _initialized = true;
    return true;
}

void WaveshareESP32S3::end() {
    if (!_initialized) {
        return;
    }
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Board] Arrêt du module...");
    #endif
    
    #ifdef FEATURE_DISPLAY_ENABLED
    display.end();
    #endif
    
    #ifdef FEATURE_TOUCH_ENABLED
    touch.end();
    #endif
    
    // TODO: Terminer les autres périphériques
    
    _initialized = false;
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Board] Module arrêté");
    #endif
}

void WaveshareESP32S3::initI2C() {
    // Bus I2C principal (capteurs, audio, RTC, power)
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_FREQUENCY);
    
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("[Board] Bus I2C principal initialisé");
    Serial.printf("[Board] SDA: GPIO%d, SCL: GPIO%d\n", I2C_SDA, I2C_SCL);
    #endif
}

void WaveshareESP32S3::printInfo() {
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("\n╔═══════════════════════════════════════════╗");
    Serial.println("║    INFORMATIONS DU MODULE                 ║");
    Serial.println("╚═══════════════════════════════════════════╝");
    
    Serial.print("  Version bibliothèque: ");
    Serial.println(LIBRARY_VERSION);
    
    Serial.print("  ESP32-S3 Fréquence: ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");
    
    Serial.print("  Flash: ");
    Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
    Serial.println(" MB");
    
    Serial.print("  PSRAM: ");
    Serial.print(ESP.getPsramSize() / (1024 * 1024));
    Serial.println(" MB");
    
    Serial.print("  RAM libre: ");
    Serial.print(ESP.getFreeHeap() / 1024);
    Serial.println(" KB");
    
    Serial.println("\n  Périphériques actifs:");
    
    #ifdef FEATURE_DISPLAY_ENABLED
    Serial.printf("    ✓ Écran LCD %dx%d\n", LCD_WIDTH, LCD_HEIGHT);
    #endif
    
    #ifdef FEATURE_TOUCH_ENABLED
    Serial.println("    ✓ Écran tactile");
    #endif
    
    #ifdef FEATURE_AUDIO_ENABLED
    Serial.println("    ✓ Audio ES8311");
    #endif
    
    #ifdef FEATURE_IMU_ENABLED
    Serial.println("    ✓ IMU QMI8658");
    #endif
    
    #ifdef FEATURE_RTC_ENABLED
    Serial.println("    ✓ RTC PCF85063");
    #endif
    
    #ifdef FEATURE_SD_CARD_ENABLED
    Serial.println("    ✓ Carte SD");
    #endif
    
    #ifdef FEATURE_CAMERA_ENABLED
    Serial.println("    ✓ Caméra OV5640");
    #endif
    
    Serial.println("╚═══════════════════════════════════════════╝\n");
    #endif
}

bool WaveshareESP32S3::testAll() {
    #ifdef FEATURE_SERIAL_DEBUG
    Serial.println("\n[Board] Démarrage des tests...");
    #endif
    
    bool allPassed = true;
    
    // Test de l'écran
    #ifdef FEATURE_DISPLAY_ENABLED
    Serial.print("[Test] Écran LCD... ");
    if (display.isInitialized()) {
        display.clear(COLOR_RED);
        delay(500);
        display.clear(COLOR_GREEN);
        delay(500);
        display.clear(COLOR_BLUE);
        delay(500);
        display.clear(COLOR_BLACK);
        Serial.println("✓ PASSÉ");
    } else {
        Serial.println("✗ ÉCHOUÉ");
        allPassed = false;
    }
    #endif
    
    // Test du tactile
    #ifdef FEATURE_TOUCH_ENABLED
    Serial.print("[Test] Tactile... ");
    if (touch.isInitialized()) {
        Serial.println("✓ PASSÉ");
        Serial.println("        Touchez l'écran pour tester...");
        
        unsigned long startTime = millis();
        bool touched = false;
        
        while (millis() - startTime < 5000 && !touched) {
            if (touch.touched()) {
                uint16_t x = touch.getX();
                uint16_t y = touch.getY();
                Serial.printf("        Point tactile: X=%d, Y=%d\n", x, y);
                touched = true;
            }
            delay(10);
        }
        
        if (!touched) {
            Serial.println("        ⚠ Aucun contact détecté");
        }
    } else {
        Serial.println("✗ ÉCHOUÉ");
        allPassed = false;
    }
    #endif
    
    // Test du bouton BOOT
    Serial.print("[Test] Bouton BOOT... ");
    if (!bootButtonPressed()) {
        Serial.println("✓ PASSÉ");
    } else {
        Serial.println("⚠ BOUTON PRESSÉ");
    }
    
    // TODO: Ajouter tests pour autres périphériques
    
    Serial.println("\n[Board] Tests terminés!");
    return allPassed;
}

bool WaveshareESP32S3::bootButtonPressed() {
    return digitalRead(BTN_BOOT) == LOW;
}
