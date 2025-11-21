/**
 * @file audio_config.h
 * @brief Configuration du codec audio ES8311
 */

#ifndef WAVESHARE_AUDIO_CONFIG_H
#define WAVESHARE_AUDIO_CONFIG_H

#include "pins_definitions.h"

// ============================================================================
// ES8311 CODEC CONFIGURATION
// ============================================================================

// Adresse I2C du codec
#define AUDIO_CODEC_ADDR            ES8311_ADDR

// ============================================================================
// I2S CONFIGURATION
// ============================================================================

// Port I2S à utiliser
#define I2S_PORT_NUM                I2S_NUM_0

// Fréquence d'échantillonnage (Hz)
// Valeurs communes: 8000, 16000, 22050, 44100, 48000
#define AUDIO_SAMPLE_RATE           44100

// Bits par échantillon
// Valeurs: 16, 24, 32
#define AUDIO_BITS_PER_SAMPLE       16

// Mode I2S
#define AUDIO_I2S_MODE              (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX)

// Format de communication
#define AUDIO_COMM_FORMAT           (I2S_COMM_FORMAT_STAND_I2S)

// Format des données
#define AUDIO_CHANNEL_FORMAT        I2S_CHANNEL_FMT_RIGHT_LEFT

// ============================================================================
// BUFFER AUDIO
// ============================================================================

// Nombre de buffers DMA
#define AUDIO_DMA_BUF_COUNT         8

// Taille de chaque buffer DMA (en samples)
#define AUDIO_DMA_BUF_LEN           1024

// Taille totale du buffer (bytes)
#define AUDIO_BUFFER_SIZE           (AUDIO_DMA_BUF_COUNT * AUDIO_DMA_BUF_LEN * 2)

// ============================================================================
// VOLUME
// ============================================================================

// Volume par défaut (0-100)
#define AUDIO_DEFAULT_VOLUME        70

// Volume minimum
#define AUDIO_MIN_VOLUME            0

// Volume maximum
#define AUDIO_MAX_VOLUME            100

// Pas d'ajustement du volume
#define AUDIO_VOLUME_STEP           5

// ============================================================================
// MICROPHONE
// ============================================================================

#ifdef FEATURE_MICROPHONE_ENABLED

// Gain du microphone (0-8)
#define MIC_GAIN                    4

// Activer le filtre passe-haut
#define MIC_HPF_ENABLE              true

// Activer l'AGC (Automatic Gain Control)
#define MIC_AGC_ENABLE              true

#endif // FEATURE_MICROPHONE_ENABLED

// ============================================================================
// SPEAKER / AMPLIFICATEUR
// ============================================================================

#ifdef FEATURE_SPEAKER_ENABLED

// Activer l'amplificateur au démarrage
#define SPEAKER_ENABLE_ON_BOOT      true

// Délai avant activation (ms)
#define SPEAKER_ENABLE_DELAY        100

#endif // FEATURE_SPEAKER_ENABLED

// ============================================================================
// FORMATS AUDIO SUPPORTÉS
// ============================================================================

// MP3
#define SUPPORT_MP3                 true

// WAV
#define SUPPORT_WAV                 true

// AAC
#define SUPPORT_AAC                 false

// OGG
#define SUPPORT_OGG                 false

// ============================================================================
// ENREGISTREMENT
// ============================================================================

// Durée maximum d'enregistrement (secondes)
#define MAX_RECORD_TIME             300   // 5 minutes

// Format d'enregistrement par défaut
#define RECORD_FORMAT               "WAV"

// Qualité d'enregistrement (bits)
#define RECORD_BITS                 16

// Fréquence d'enregistrement
#define RECORD_SAMPLE_RATE          16000  // 16kHz pour économiser l'espace

#endif // WAVESHARE_AUDIO_CONFIG_H
