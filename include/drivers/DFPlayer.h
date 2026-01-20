/**
 * @file DFPlayer.h
 * @brief Driver pour DFPlayer Mini MP3 Player
 *
 * Le DFPlayer Mini est connecté via Serial sur les broches:
 * - GPIO 47 (ESP32 TX) -> DFPlayer RX
 * - GPIO 48 (ESP32 RX) -> DFPlayer TX
 * Avec conversion de niveaux 3.3V <-> 5V
 *
 * Structure des fichiers MP3 sur la carte SD du DFPlayer:
 * /01/001.mp3  -> Victoire
 * /01/002.mp3  -> Défaite (touché)
 * /01/003.mp3  -> Timeout
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef DFPLAYER_H
#define DFPLAYER_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

// Numéros des pistes MP3 (dans le dossier /01/)
#define MP3_VICTOIRE    1
#define MP3_DEFAITE     2
#define MP3_TIMEOUT     3

class DFPlayerDriver {
public:
    DFPlayerDriver() : _initialized(false), _volume(25) {}

    /**
     * @brief Initialise le DFPlayer Mini
     * @param rxPin Broche RX de l'ESP32 (connectée au TX du DFPlayer)
     * @param txPin Broche TX de l'ESP32 (connectée au RX du DFPlayer)
     * @return true si l'initialisation a réussi
     */
    bool begin(uint8_t rxPin, uint8_t txPin) {
        Serial.println("DFPlayer: Initialisation...");

        // Initialiser le port série pour le DFPlayer
        _serial.begin(9600, SERIAL_8N1, rxPin, txPin);

        // Délai pour stabilisation du module
        delay(500);

        // Initialisation avec timeout (comme dans le code Arduino validé)
        if (!_player.begin(_serial)) {
            Serial.println("DFPlayer: Erreur d'initialisation!");
            return false;
        }

        _initialized = true;
        Serial.println("DFPlayer: Initialisé avec succès");

        // Configuration du volume uniquement
        _player.volume(_volume);
        delay(100);

        return true;
    }

    /**
     * @brief Vérifie si le DFPlayer est initialisé
     */
    bool isInitialized() {
        return _initialized;
    }

    /**
     * @brief Règle le volume (0-30)
     */
    void setVolume(uint8_t volume) {
        _volume = constrain(volume, 0, 30);
        if (_initialized) {
            _player.volume(_volume);
        }
    }

    /**
     * @brief Joue le son de victoire
     */
    void playVictoire() {
        if (_initialized) {
            Serial.println("DFPlayer: Lecture VICTOIRE");
            _player.playFolder(1, MP3_VICTOIRE);
        }
    }

    /**
     * @brief Joue le son de défaite (touché)
     */
    void playDefaite() {
        if (_initialized) {
            Serial.println("DFPlayer: Lecture DEFAITE");
            _player.playFolder(1, MP3_DEFAITE);
        }
    }

    /**
     * @brief Joue le son de timeout
     */
    void playTimeout() {
        if (_initialized) {
            Serial.println("DFPlayer: Lecture TIMEOUT");
            _player.playFolder(1, MP3_TIMEOUT);
        }
    }

    /**
     * @brief Joue un fichier spécifique
     * @param folder Numéro du dossier (1-99)
     * @param track Numéro de la piste (1-255)
     */
    void play(uint8_t folder, uint8_t track) {
        if (_initialized) {
            _player.playFolder(folder, track);
        }
    }

    /**
     * @brief Arrête la lecture
     */
    void stop() {
        if (_initialized) {
            _player.stop();
        }
    }

    /**
     * @brief Vérifie si une lecture est en cours
     */
    bool isPlaying() {
        if (_initialized) {
            return _player.readState() == 1;
        }
        return false;
    }

private:
    HardwareSerial _serial{1};  // Utiliser UART1
    DFRobotDFPlayerMini _player;
    bool _initialized;
    uint8_t _volume;
};

#endif // DFPLAYER_H
