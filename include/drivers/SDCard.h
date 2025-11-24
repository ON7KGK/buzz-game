/**
 * @file SDCard.h
 * @brief Classe pour gérer la carte SD/TF via SD_MMC
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef SDCARD_DRIVER_H
#define SDCARD_DRIVER_H

#include <Arduino.h>
#include <SD_MMC.h>
#include "config/sd_config.h"
#include "features.h"

class SDCard {
public:
    /**
     * @brief Constructeur
     */
    SDCard() : initialized(false), cardSize(0) {}

    /**
     * @brief Initialise la carte SD via SD_MMC
     * @return true si succès, false sinon
     */
    bool begin() {
        #if !FEATURE_SD_ENABLED
        return false;
        #endif

        // Configuration des pins SD_MMC (mode SDIO 1-bit)
        SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

        // Init SD_MMC en mode 1-bit
        if (!SD_MMC.begin("/sdmmc", true, false, 20000)) {
            return false;
        }

        uint8_t cardType = SD_MMC.cardType();
        if (cardType == CARD_NONE) {
            return false;
        }

        // Afficher les infos
        cardSize = SD_MMC.cardSize() / (1024 * 1024);
        uint64_t usedSize = SD_MMC.usedBytes() / (1024 * 1024);


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


        File root = SD_MMC.open(dirname);
        if (!root) {
            return;
        }
        if (!root.isDirectory()) {
            return;
        }

        File file = root.openNextFile();
        while (file) {
            if (file.isDirectory()) {
                if (levels) {
                    listDir(file.path(), levels - 1);
                }
            } else {
            }
            file = root.openNextFile();
        }
    }

private:
    bool initialized;
    uint64_t cardSize;
};

#endif // SDCARD_DRIVER_H
