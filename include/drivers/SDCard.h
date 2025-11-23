/**
 * @file SDCard.h
 * @brief Classe pour gérer la carte SD/TF
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef SDCARD_DRIVER_H
#define SDCARD_DRIVER_H

#include <Arduino.h>
#include <SD_MMC.h>
#include <SD.h>
#include "config/sd_config.h"
#include "features.h"

class SDCard {
public:
    /**
     * @brief Constructeur
     */
    SDCard() : initialized(false), cardSize(0) {}

    /**
     * @brief Initialise la carte SD
     * @return true si succès, false sinon
     */
    bool begin() {
        #if !FEATURE_SD_ENABLED
        return false;
        #endif

        // Init SD_MMC en mode 1-bit
        if (!SD_MMC.begin("/sdcard", true)) {  // true = mode 1-bit
            Serial.println("❌ Carte SD_MMC non détectée");
            return false;
        }

        uint8_t cardType = SD_MMC.cardType();
        if (cardType == CARD_NONE) {
            Serial.println("❌ Aucune carte SD insérée");
            return false;
        }

        // Monter aussi via SD pour ESP8266Audio
        if (!SD.begin()) {
            Serial.println("⚠️  SD (pour audio) non monté, mais SD_MMC OK");
        }

        // Afficher les infos
        cardSize = SD_MMC.cardSize() / (1024 * 1024);
        uint64_t usedSize = SD_MMC.usedBytes() / (1024 * 1024);

        Serial.printf("✓ Carte SD détectée: %lluMB (utilisé: %lluMB)\n", cardSize, usedSize);

        initialized = true;
        return true;
    }

    /**
     * @brief Vérifie si un fichier existe
     * @param path Chemin du fichier
     * @return true si le fichier existe
     */
    bool exists(const char* path) {
        if (!initialized) return false;
        return SD_MMC.exists(path);
    }

    /**
     * @brief Ouvre un fichier en lecture
     * @param path Chemin du fichier
     * @return Objet File (vérifier avec .available())
     */
    File open(const char* path) {
        if (!initialized) return File();
        return SD_MMC.open(path, FILE_READ);
    }

    /**
     * @brief Ouvre un fichier en écriture
     * @param path Chemin du fichier
     * @return Objet File (vérifier avec .available())
     */
    File openWrite(const char* path) {
        if (!initialized) return File();
        return SD_MMC.open(path, FILE_WRITE);
    }

    /**
     * @brief Crée un répertoire
     * @param path Chemin du répertoire
     * @return true si succès
     */
    bool mkdir(const char* path) {
        if (!initialized) return false;
        return SD_MMC.mkdir(path);
    }

    /**
     * @brief Supprime un fichier
     * @param path Chemin du fichier
     * @return true si succès
     */
    bool remove(const char* path) {
        if (!initialized) return false;
        return SD_MMC.remove(path);
    }

    /**
     * @brief Retourne la taille de la carte (MB)
     */
    uint64_t getCardSize() {
        return cardSize;
    }

    /**
     * @brief Retourne l'espace utilisé (MB)
     */
    uint64_t getUsedSize() {
        if (!initialized) return 0;
        return SD_MMC.usedBytes() / (1024 * 1024);
    }

    /**
     * @brief Retourne l'espace libre (MB)
     */
    uint64_t getFreeSize() {
        if (!initialized) return 0;
        return (SD_MMC.totalBytes() - SD_MMC.usedBytes()) / (1024 * 1024);
    }

    /**
     * @brief Liste les fichiers d'un répertoire
     * @param dirname Chemin du répertoire
     * @param levels Profondeur de récursion
     */
    void listDir(const char* dirname, uint8_t levels = 1) {
        if (!initialized) return;

        Serial.printf("Listing directory: %s\n", dirname);

        File root = SD_MMC.open(dirname);
        if (!root) {
            Serial.println("Failed to open directory");
            return;
        }
        if (!root.isDirectory()) {
            Serial.println("Not a directory");
            return;
        }

        File file = root.openNextFile();
        while (file) {
            if (file.isDirectory()) {
                Serial.print("  DIR : ");
                Serial.println(file.name());
                if (levels) {
                    listDir(file.path(), levels - 1);
                }
            } else {
                Serial.print("  FILE: ");
                Serial.print(file.name());
                Serial.print("\tSIZE: ");
                Serial.println(file.size());
            }
            file = root.openNextFile();
        }
    }

private:
    bool initialized;
    uint64_t cardSize;
};

#endif // SDCARD_DRIVER_H
