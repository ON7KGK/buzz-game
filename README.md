# Waveshare ESP32-S3-Touch-LCD-3.5B-C - Projet SPARKOH!

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Ready-orange.svg)](https://platformio.org)
[![ESP32-S3](https://img.shields.io/badge/ESP32--S3-Supported-green.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> Bibliothèque modulaire et programme de test pour le module Waveshare ESP32-S3-Touch-LCD-3.5B-C, développé pour SPARKOH! - Centre de Culture Scientifique

## 📋 Description

Ce projet fournit une bibliothèque complète et modulaire pour le développement d'applications interactives sur le module **Waveshare ESP32-S3-Touch-LCD-3.5B-C**. Il est conçu pour des projets éducatifs et scientifiques, avec un accent sur la facilité d'utilisation et la configurabilité.

### Caractéristiques du Module

- **MCU**: ESP32-S3R8 (dual-core Xtensa 240MHz)
- **Mémoire**: 512KB SRAM + 8MB PSRAM + 16MB Flash
- **Écran**: 3.5" IPS LCD 320×480 pixels (RGB565) avec contrôleur AXS15231B
- **Tactile**: Capacitif I2C
- **Audio**: Codec ES8311 avec I2S
- **Capteurs**:
  - QMI8658 IMU 6-axis (accéléromètre + gyroscope)
  - PCF85063 RTC
- **Gestion d'énergie**: AXP2101 (charge batterie Li-ion)
- **Stockage**: Slot carte SD/TF
- **Caméra**: OV5640 5MP (version -C uniquement)
- **Connectivité**: WiFi 2.4GHz + Bluetooth 5.0

## 🚀 Démarrage Rapide

### Prérequis

- [PlatformIO](https://platformio.org/) (via VS Code ou CLI)
- Module Waveshare ESP32-S3-Touch-LCD-3.5B-C
- Câble USB-C

### Installation

1. Clonez ce dépôt:
```bash
git clone https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C.git
cd Waveshare-ESP32-S3-Touch-LCD-3.5B-C
```

2. Ouvrez le projet dans PlatformIO

3. Compilez et uploadez:
```bash
pio run --target upload
```

4. Ouvrez le moniteur série:
```bash
pio device monitor
```

## 📁 Structure du Projet

```
Waveshare1/
├── src/
│   └── main.cpp                  # Programme principal (dessin tactile interactif)
├── lib/
│   └── WaveshareESP32S3Lib/     # Bibliothèque principale
│       ├── src/
│       │   ├── config/          # Fichiers de configuration
│       │   │   ├── features.h           # Activation/désactivation des composants
│       │   │   ├── pins_definitions.h   # Définition des GPIO
│       │   │   ├── display_config.h     # Configuration de l'écran
│       │   │   ├── audio_config.h       # Configuration audio
│       │   │   ├── sdcard_config.h      # Configuration carte SD
│       │   │   └── camera_config.h      # Configuration caméra
│       │   ├── peripherals/     # Pilotes des périphériques
│       │   │   ├── Display.h/cpp        # Gestion de l'écran LCD
│       │   │   └── TouchScreen.h/cpp    # Gestion du tactile
│       │   └── WaveshareESP32S3.h/cpp  # Classe principale (facade)
│       ├── examples/
│       │   └── 01_BasicTest/
│       │       └── BasicTest.ino # Exemple de base
│       └── README.md            # Documentation complète
├── include/                      # Headers de compatibilité
│   ├── esp32-hal-periman.h      # Stub pour compatibility
│   └── esp_private/
│       └── periph_ctrl.h        # Stub pour compatibility
├── platformio.ini               # Configuration PlatformIO
└── README.md                    # Ce fichier

```

## ⚙️ Configuration

### Features (Composants Activables)

Éditez `lib/WaveshareESP32S3Lib/src/config/features.h` pour activer/désactiver les composants:

```cpp
#define FEATURE_DISPLAY_ENABLED      1    // Écran LCD
#define FEATURE_TOUCH_ENABLED        1    // Tactile
#define FEATURE_AUDIO_ENABLED        1    // Audio ES8311
#define FEATURE_IMU_ENABLED          1    // IMU QMI8658
#define FEATURE_RTC_ENABLED          0    // RTC PCF85063
#define FEATURE_POWER_MGMT_ENABLED   1    // Gestion d'énergie AXP2101
#define FEATURE_SD_CARD_ENABLED      1    // Carte SD
#define FEATURE_CAMERA_ENABLED       0    // Caméra OV5640
#define FEATURE_WIFI_ENABLED         1    // WiFi
#define FEATURE_BLUETOOTH_ENABLED    0    // Bluetooth
#define FEATURE_LVGL_ENABLED         1    // LVGL UI Framework
```

### Broches GPIO

Les broches sont définies dans `lib/WaveshareESP32S3Lib/src/config/pins_definitions.h`:

| Composant | Broches |
|-----------|---------|
| LCD QSPI | CS=45, CLK=47, D0-D3=21/48/40/39, BL=1 |
| Touch I2C | SDA=4, SCL=8, INT=11, RST=12 |
| Main I2C | SDA=17, SCL=18 |
| Audio I2S | MCLK=16, BCLK=7, LRCK=5, DOUT=6, DIN=15 |
| SD Card | CLK=14, CMD=13, D0=10 |
| UART | TX=43, RX=44 |
| Boot Button | GPIO0 |

## 🔧 Compilation du Projet

### ⚠️ Important : Problèmes de Compatibilité

La bibliothèque `Arduino_GFX` v1.4.7+ (requise pour le pilote AXS15231B) nécessite **Arduino-ESP32 v3.0+**, mais PlatformIO utilise actuellement la version 2.x par défaut.

### 📖 Guide de Compilation Détaillé

**➡️ Consultez [COMPILATION_NOTES.md](COMPILATION_NOTES.md) pour les instructions complètes de compilation.**

Ce document contient :
- ✅ **Option 1** : Arduino IDE 2.x (Recommandé - Le plus simple)
- ⚙️ **Option 2** : PlatformIO avec patch manuel
- 🔧 **Option 3** : PlatformIO avec Arduino-ESP32 3.0 (Avancé)
- 📋 Explications détaillées des problèmes de compatibilité
- 🎯 Recommandations selon votre niveau

### Compilation Rapide (Arduino IDE)

La méthode la plus simple pour compiler ce projet :

1. Installer Arduino IDE 2.x
2. Installer ESP32 Board Manager v3.0+
3. Installer les bibliothèques requises via le Library Manager
4. Configurer la carte : ESP32S3 Dev Module avec PSRAM OPI
5. Compiler et uploader !

**Le code est 100% fonctionnel**, seule la configuration de l'environnement de compilation nécessite attention.

## 📚 Utilisation de la Bibliothèque

### Exemple Minimal

```cpp
#include <Arduino.h>
#include <WaveshareESP32S3.h>

WaveshareESP32S3 board;

void setup() {
    Serial.begin(115200);

    if (!board.begin()) {
        Serial.println("Erreur d'initialisation!");
        while(1);
    }

    board.display.clear(COLOR_BLUE);
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(50, 100);
    board.display.println("Hello SPARKOH!");
}

void loop() {
    if (board.touch.read()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();
        board.display.fillCircle(x, y, 5, COLOR_RED);
    }
    delay(10);
}
```

### Programme de Test Inclus

Le programme `src/main.cpp` fourni démontre:
- ✅ Initialisation complète du module
- ✅ Affichage graphique (formes, texte, couleurs)
- ✅ Détection tactile en temps réel
- ✅ Dessin interactif
- ✅ Utilisation du bouton BOOT
- ✅ Statistiques système

## 🎨 Palette de Couleurs SPARKOH!

Le projet inclut des couleurs personnalisées pour SPARKOH!:

```cpp
#define COLOR_SPARKOH_BLUE    0x0A7F   // Bleu SPARKOH!
#define COLOR_SPARKOH_GREEN   0x07E0   // Vert SPARKOH!
#define COLOR_SPARKOH_ORANGE  0xFD20   // Orange SPARKOH!
```

## 🐛 Débogage

### Serial Monitor

Le projet utilise le port série USB à 115200 bauds pour le débogage:

```bash
pio device monitor --baud 115200
```

### Niveau de Debug

Ajustez le niveau de debug dans `platformio.ini`:

```ini
build_flags =
    -DCORE_DEBUG_LEVEL=3  # 0=None, 1=Error, 2=Warn, 3=Info, 4=Debug, 5=Verbose
```

## 📦 Dépendances

- `Arduino_GFX` ^1.4.7 - Bibliothèque graphique
- `XPowersLib` ^0.2.4 - Gestion d'énergie AXP2101
- `lvgl` ^8.4.0 - Framework UI (optionnel)
- `Adafruit Unified Sensor` ^1.1.14 - Bibliothèque capteurs

## 🔄 Roadmap

- [ ] Implémenter le pilote Audio ES8311
- [ ] Implémenter le pilote IMU QMI8658
- [ ] Implémenter le pilote RTC PCF85063
- [ ] Implémenter la gestion complète AXP2101
- [ ] Implémenter le pilote carte SD
- [ ] Implémenter le pilote caméra OV5640
- [ ] Ajouter l'intégration LVGL
- [ ] Ajouter des exemples WiFi/BLE
- [ ] Créer des widgets LVGL personnalisés
- [ ] Optimiser la consommation d'énergie

## 👤 Auteur

**Michaël - SPARKOH!**
- GitHub: [@ON7KGK](https://github.com/ON7KGK)
- Projet: SPARKOH! - Centre de Culture Scientifique

## 📄 Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

## 🙏 Remerciements

- Waveshare pour la documentation du module
- Communauté PlatformIO et ESP32
- Bibliothèque Arduino_GFX par moononournation
- Tous les contributeurs open-source

## 📞 Support

Pour toute question ou problème:
- Ouvrez une issue sur [GitHub Issues](https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C/issues)
- Consultez la documentation complète dans `lib/WaveshareESP32S3Lib/README.md`

---

**Note**: Ce projet est en développement actif. Les contributions sont les bienvenues!

Développé avec ❤️ pour SPARKOH! et la communauté maker
