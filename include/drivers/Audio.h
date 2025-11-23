/**
 * @file Audio.h
 * @brief Classe pour gérer l'audio ES8311 et lecture MP3/WAV
 *
 * Utilise ESP32-audioI2S avec codec ES8311
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include <Arduino.h>
#include <Audio.h>  // ESP32-audioI2S library
#include <SD_MMC.h>
#include <Wire.h>
#include "es8311.h"
#include "esp_check.h"
#include "config/audio_config.h"
#include "features.h"

#define TAG "Audio"

// Configuration ES8311
#define EXAMPLE_SAMPLE_RATE     (16000)
#define EXAMPLE_MCLK_MULTIPLE   (256)
#define EXAMPLE_MCLK_FREQ_HZ    (EXAMPLE_SAMPLE_RATE * EXAMPLE_MCLK_MULTIPLE)
#define EXAMPLE_VOICE_VOLUME    (75)

class AudioDriver {
public:
    /**
     * @brief Constructeur
     */
    AudioDriver() : audio(nullptr), initialized(false) {
        audio = new Audio();
    }

    /**
     * @brief Destructeur
     */
    ~AudioDriver() {
        if (audio) delete audio;
    }

    /**
     * @brief Initialise l'audio ES8311 + I2S
     * @return true si succès, false sinon
     */
    bool begin() {
        #if !FEATURE_AUDIO_ENABLED
        return false;
        #endif

        Serial.println("🔊 Init codec ES8311...");

        // Initialiser le codec ES8311 via I2C
        if (!initES8311()) {
            Serial.println("❌ Échec init ES8311");
            return false;
        }

        Serial.println("✓ Codec ES8311 initialisé");

        // Configuration I2S
        audio->setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT, I2S_MCLK);
        audio->setVolume(DEFAULT_AUDIO_VOLUME / 5);  // 0...21 (scale from 0-100)

        initialized = true;
        Serial.println("✓ Audio I2S configuré");
        return true;
    }

    /**
     * @brief Joue un fichier MP3 ou WAV depuis la carte SD
     * @param filename Nom du fichier (ex: "/audio/tada.mp3")
     * @return true si la lecture démarre
     */
    bool play(const char* filename) {
        if (!initialized || !audio) {
            Serial.println("❌ Audio non initialisé");
            return false;
        }

        // Vérifier si le fichier existe
        if (!SD_MMC.exists(filename)) {
            Serial.printf("❌ Fichier non trouvé: %s\n", filename);
            return false;
        }

        // Lancer la lecture
        audio->connecttoFS(SD_MMC, filename);
        Serial.printf("♪ Lecture: %s\n", filename);
        return true;
    }

    /**
     * @brief Met à jour la lecture (à appeler dans loop())
     */
    void loop() {
        if (initialized && audio) {
            audio->loop();
        }
    }

    /**
     * @brief Arrête la lecture en cours
     */
    void stop() {
        if (audio) {
            audio->stopSong();
        }
    }

    /**
     * @brief Vérifie si de l'audio est en cours de lecture
     * @return true si lecture en cours
     */
    bool isPlaying() {
        return (audio && audio->isRunning());
    }

    /**
     * @brief Définit le volume
     * @param vol Volume 0-100
     */
    void setVolume(uint8_t vol) {
        if (audio) {
            // ESP32-audioI2S utilise 0-21
            uint8_t scaledVol = map(constrain(vol, 0, 100), 0, 100, 0, 21);
            audio->setVolume(scaledVol);
        }
    }

    /**
     * @brief Accès direct à l'objet Audio pour fonctions avancées
     */
    Audio* getAudio() {
        return audio;
    }

private:
    Audio* audio;
    bool initialized;

    /**
     * @brief Initialise le codec ES8311
     */
    bool initES8311() {
        es8311_handle_t es_handle = es8311_create(I2C_NUM_0, ES8311_ADDRRES_0);
        if (!es_handle) {
            Serial.println("❌ ES8311 create failed");
            return false;
        }

        const es8311_clock_config_t es_clk = {
            .mclk_inverted = false,
            .sclk_inverted = false,
            .mclk_from_mclk_pin = true,
            .mclk_frequency = EXAMPLE_MCLK_FREQ_HZ,
            .sample_frequency = EXAMPLE_SAMPLE_RATE
        };

        esp_err_t ret = es8311_init(es_handle, &es_clk, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16);
        if (ret != ESP_OK) {
            Serial.println("❌ ES8311 init failed");
            return false;
        }

        ret = es8311_voice_volume_set(es_handle, EXAMPLE_VOICE_VOLUME, NULL);
        if (ret != ESP_OK) {
            Serial.println("⚠️  ES8311 volume set failed");
        }

        ret = es8311_microphone_config(es_handle, false);
        if (ret != ESP_OK) {
            Serial.println("⚠️  ES8311 microphone config failed");
        }

        return true;
    }
};

#endif // AUDIO_DRIVER_H
