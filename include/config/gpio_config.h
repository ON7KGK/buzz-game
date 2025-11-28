/**
 * @file gpio_config.h
 * @brief Configuration des GPIO exposés sur le connecteur du module
 *
 * Basé sur le pinout du connecteur 2x16 broches du module Waveshare
 * Ces GPIO sont disponibles pour vos projets (capteurs, boutons, moteurs, etc.)
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

// ============================================================================
// GPIO DISPONIBLES (Connecteur 2x16 broches)
// ============================================================================
// Selon le pinout de votre image :
//
// Côté GAUCHE (broches impaires 1-31):
// 1  = BAT     (Alimentation batterie)
// 3  = GND     (Masse)
// 5  = GPIO21
// 7  = GPIO38
// 9  = GPIO39
// 11 = GPIO40
// 13 = GPIO41
// 15 = GPIO42
// 17 = GPIO45
// 19 = GPIO46
// 21 = GPIO47
// 23 = GPIO48
// 25 = TX(GPIO44)
// 27 = RX(GPIO43)
// 29 = GND
// 31 = 3V3
//
// Côté DROIT (broches paires 2-32):
// 2  = 5V      (Alimentation 5V)
// 4  = GND     (Masse)
// 6  = DN(GPIO19)
// 8  = DP(GPIO20)
// 10 = GPIO11
// 12 = GPIO10
// 14 = GPIO9
// 16 = GPIO17
// 18 = GPIO18
// 20 = BOOT(GPIO0)
// 22 = RST
// 24 = PWR
// 26 = SCL(GPIO7)
// 28 = SDA(GPIO8)
// 30 = GND
// 32 = 3V3

// ============================================================================
// GPIO GÉNÉRAUX DISPONIBLES
// ============================================================================
#define GPIO_EXT_9              9       // Broche 14
#define GPIO_EXT_10             10      // Broche 12
#define GPIO_EXT_11             11      // Broche 10
#define GPIO_EXT_17             17      // Broche 16
#define GPIO_EXT_18             18      // Broche 18
#define GPIO_EXT_21             21      // Broche 5
#define GPIO_EXT_38             38      // Broche 7
#define GPIO_EXT_39             39      // Broche 9
#define GPIO_EXT_40             40      // Broche 11
#define GPIO_EXT_41             41      // Broche 13
#define GPIO_EXT_42             42      // Broche 15
#define GPIO_EXT_45             45      // Broche 17
#define GPIO_EXT_46             46      // Broche 19
#define GPIO_EXT_47             47      // Broche 21
#define GPIO_EXT_48             48      // Broche 23

// ============================================================================
// UART EXPOSÉ
// ============================================================================
#define GPIO_UART_TX            44      // Broche 25 - TX
#define GPIO_UART_RX            43      // Broche 27 - RX

// ============================================================================
// I2C PRINCIPAL (déjà utilisé par Touch, IMU, RTC, Power)
// ============================================================================
#define GPIO_I2C_SDA            8       // Broche 26 - SDA
#define GPIO_I2C_SCL            7       // Broche 24 - SCL

// ⚠️ ATTENTION: Ces GPIO sont déjà utilisés par les périphériques I2C internes
// Si vous voulez connecter vos propres capteurs I2C, utilisez le même bus

// ============================================================================
// USB (Programmation et Debug)
// ============================================================================
#define GPIO_USB_DN             19      // Broche 6 - USB D-
#define GPIO_USB_DP             20      // Broche 8 - USB D+

// ============================================================================
// BOUTONS ET CONTRÔLE
// ============================================================================
#define GPIO_BOOT               0       // Broche 18 - Bouton BOOT
// RST (broche 20) et PWR (broche 22) sont des signaux spéciaux

// ============================================================================
// ALIMENTATION DISPONIBLE
// ============================================================================
// BAT   (broche 1)  - Connexion batterie
// 5V    (broche 2)  - Sortie 5V régulée
// 3V3   (broches 30/31) - Sortie 3.3V régulée
// GND   (broches 3/4/28/29) - Masses

// ============================================================================
// RECOMMANDATIONS D'UTILISATION
// ============================================================================
/*
 * GPIO SÛRS pour vos projets (pas de conflit):
 * - GPIO 9, 10, 11, 17, 18, 21, 38, 39, 40, 41, 42, 45, 46, 47, 48
 *
 * GPIO À ÉVITER (déjà utilisés par le module):
 * - GPIO 0-8   : LCD, Touch, I2C, Backlight
 * - GPIO 12-16 : Audio I2S, SD Card
 * - GPIO 19-20 : USB
 *
 * Pour capteurs I2C:
 * - Utilisez GPIO 7 (SCL) et 8 (SDA) - bus partagé
 *
 * Pour UART externe:
 * - TX = GPIO 44 (broche 25)
 * - RX = GPIO 43 (broche 27)
 */

#endif // GPIO_CONFIG_H
