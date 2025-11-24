/**
 * @file audio_config.h
 * @brief Configuration du codec audio ES8311 et I2S
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef AUDIO_CONFIG_H
#define AUDIO_CONFIG_H

// ============================================================================
// PINS AUDIO (I2S Interface)
// ============================================================================
#define I2S_MCLK                44      // Master Clock
#define I2S_BCLK                13      // Bit Clock
#define I2S_LRCK                15      // Left/Right Clock (WS)
#define I2S_DOUT                16      // Data Out (Playback)
#define I2S_DIN                 14      // Data In (Recording)
#define PA_CTRL                 11      // Power Amplifier Control

// Codec I2C
#define ES8311_ADDR             0x18    // Adresse I2C du codec ES8311

// ============================================================================
// CONFIGURATION I2S
// ============================================================================
#define AUDIO_SAMPLE_RATE       44100   // Hz
#define AUDIO_BITS_PER_SAMPLE   16      // bits
#define AUDIO_DMA_BUF_COUNT     8
#define AUDIO_DMA_BUF_LEN       1024

// ============================================================================
// VOLUME
// ============================================================================
#define AUDIO_DEFAULT_VOLUME    2      // 0-100

// ============================================================================
// FORMATS SUPPORTÉS
// ============================================================================
#define SUPPORT_MP3             true
#define SUPPORT_WAV             true
#define SUPPORT_AAC             false

#endif // AUDIO_CONFIG_H
