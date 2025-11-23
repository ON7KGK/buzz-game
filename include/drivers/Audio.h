/**
 * @file Audio.h
 * @brief Classe pour gérer l'audio ES8311 et lecture MP3/WAV
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include <Arduino.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include <SD_MMC.h>
#include <SD.h>
#include "config/audio_config.h"
#include "features.h"

class Audio {
public:
    /**
     * @brief Constructeur
     */
    Audio() : source(nullptr),
              mp3(nullptr),
              wav(nullptr),
              out(nullptr),
              initialized(false),
              playing(false),
              volume(AUDIO_DEFAULT_VOLUME) {}

    /**
     * @brief Initialise l'audio
     * @return true si succès, false sinon
     */
    bool begin() {
        #if !FEATURE_AUDIO_ENABLED
        return false;
        #endif

        // Configuration I2S
        out = new AudioOutputI2S();
        out->SetPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
        out->SetGain(volume / 100.0);

        // Activer l'amplificateur
        pinMode(PA_CTRL, OUTPUT);
        digitalWrite(PA_CTRL, HIGH);
        delay(100);

        initialized = true;
        Serial.println("✓ Audio initialisé");
        return true;
    }

    /**
     * @brief Joue un fichier MP3 ou WAV depuis la carte SD
     * @param filename Nom du fichier (ex: "/audio/tada.mp3")
     * @return true si la lecture démarre
     */
    bool play(const char* filename) {
        if (!initialized) {
            Serial.println("❌ Audio non initialisé");
            return false;
        }

        // Arrêter la lecture en cours
        stop();

        // Vérifier si le fichier existe
        if (!SD_MMC.exists(filename)) {
            Serial.printf("❌ Fichier non trouvé: %s\n", filename);
            return false;
        }

        // Créer la source depuis SD
        source = new AudioFileSourceSD(filename);
        if (!source->isOpen()) {
            Serial.printf("❌ Impossible d'ouvrir: %s\n", filename);
            delete source;
            source = nullptr;
            return false;
        }

        // Déterminer le type de fichier
        String file = String(filename);
        file.toLowerCase();

        if (file.endsWith(".mp3")) {
            mp3 = new AudioGeneratorMP3();
            if (!mp3->begin(source, out)) {
                Serial.println("❌ Échec démarrage MP3");
                stop();
                return false;
            }
        } else if (file.endsWith(".wav")) {
            wav = new AudioGeneratorWAV();
            if (!wav->begin(source, out)) {
                Serial.println("❌ Échec démarrage WAV");
                stop();
                return false;
            }
        } else {
            Serial.println("❌ Format non supporté (MP3/WAV uniquement)");
            delete source;
            source = nullptr;
            return false;
        }

        playing = true;
        Serial.printf("♪ Lecture: %s\n", filename);
        return true;
    }

    /**
     * @brief Met à jour la lecture (à appeler dans loop())
     * @return true si la lecture continue, false si terminée
     */
    bool loop() {
        if (!playing) return false;

        bool isRunning = false;

        if (mp3 && mp3->isRunning()) {
            isRunning = mp3->loop();
        } else if (wav && wav->isRunning()) {
            isRunning = wav->loop();
        }

        if (!isRunning) {
            stop();
            Serial.println("♪ Lecture terminée");
            return false;
        }

        return true;
    }

    /**
     * @brief Arrête la lecture en cours
     */
    void stop() {
        if (mp3) {
            if (mp3->isRunning()) mp3->stop();
            delete mp3;
            mp3 = nullptr;
        }

        if (wav) {
            if (wav->isRunning()) wav->stop();
            delete wav;
            wav = nullptr;
        }

        if (source) {
            source->close();
            delete source;
            source = nullptr;
        }

        playing = false;
    }

    /**
     * @brief Vérifie si de l'audio est en cours de lecture
     * @return true si lecture en cours
     */
    bool isPlaying() {
        return playing && (mp3 || wav);
    }

    /**
     * @brief Définit le volume
     * @param vol Volume 0-100
     */
    void setVolume(uint8_t vol) {
        volume = constrain(vol, 0, 100);
        if (out) {
            out->SetGain(volume / 100.0);
        }
    }

    /**
     * @brief Récupère le volume actuel
     * @return Volume 0-100
     */
    uint8_t getVolume() {
        return volume;
    }

    /**
     * @brief Désactive l'audio (économie d'énergie)
     */
    void disable() {
        stop();
        digitalWrite(PA_CTRL, LOW);
    }

    /**
     * @brief Réactive l'audio
     */
    void enable() {
        digitalWrite(PA_CTRL, HIGH);
        delay(100);
    }

private:
    AudioFileSourceSD* source;
    AudioGeneratorMP3* mp3;
    AudioGeneratorWAV* wav;
    AudioOutputI2S* out;
    bool initialized;
    bool playing;
    uint8_t volume;
};

#endif // AUDIO_DRIVER_H
