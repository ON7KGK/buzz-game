# Waveshare ESP32-S3-Touch-LCD-3.5B - Projet SPARKOH!

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Ready-orange.svg)](https://platformio.org)
[![ESP32-S3](https://img.shields.io/badge/ESP32--S3-Supported-green.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> **Architecture modulaire v2.0** pour le module Waveshare ESP32-S3-Touch-LCD-3.5B, développé pour SPARKOH! - Centre de Culture Scientifique

## 📋 Description

Ce projet fournit une **architecture modulaire complète** pour le développement rapide d'applications interactives sur le module **Waveshare ESP32-S3-Touch-LCD-3.5B**.

### 🎯 Philosophie du Projet

**Concentrez-vous sur votre logique, pas sur le matériel !**

- ✅ Configuration centralisée dans `include/config/`
- ✅ Drivers réutilisables dans `include/drivers/`
- ✅ Features activables/désactivables dans `include/features.h`
- ✅ Code principal (`src/main.cpp`) **UNIQUEMENT pour votre logique**

---

## 🔧 Caractéristiques du Module

### Spécifications Matérielles

- **MCU**: ESP32-S3R8 (dual-core Xtensa 240MHz)
- **Mémoire**: 512KB SRAM + 8MB PSRAM + 16MB Flash
- **Écran**: 3.5" IPS LCD 320×480 pixels (RGB565) avec contrôleur AXS15231B
- **Tactile**: Capacitif I2C intégré
- **Audio**: Codec ES8311 avec I2S + amplificateur + haut-parleur
- **Stockage**: Slot carte SD/TF (mode 1-bit)
- **Connectivité**: WiFi 2.4GHz + Bluetooth 5.0 BLE

### Capteurs & Périphériques (optionnels)

- QMI8658 IMU 6-axis (accéléromètre + gyroscope)
- PCF85063 RTC (horloge temps réel)
- AXP2101 Power Management (gestion batterie Li-ion)

---

## 📁 Structure du Projet

```
Waveshare1/
├── include/
│   ├── features.h                    ← Active/désactive les composants
│   ├── config/                       ← Configuration matérielle
│   │   ├── hardware_config.h         (Infos module)
│   │   ├── display_config.h          (LCD, couleurs)
│   │   ├── audio_config.h            (ES8311, I2S)
│   │   ├── sd_config.h               (Carte SD)
│   │   ├── i2c_config.h              (Bus I2C)
│   │   └── gpio_config.h             (GPIO exposés)
│   └── drivers/                      ← Drivers réutilisables
│       ├── Display.h                 (Affichage LCD)
│       ├── Audio.h                   (Lecture MP3/WAV)
│       └── SDCard.h                  (Carte SD)
├── src/
│   └── main.cpp                      ← VOTRE LOGIQUE ICI
├── platformio.ini                    ← Config PlatformIO
└── README.md                         ← Ce fichier
```

---

## 🚀 Démarrage Rapide

### Prérequis

- [PlatformIO](https://platformio.org/) (via VS Code recommandé)
- Module Waveshare ESP32-S3-Touch-LCD-3.5B
- Câble USB-C
- (Optionnel) Carte SD avec fichier `/audio/tada.mp3` pour le test

### Installation

1. **Clonez ce dépôt**:
```bash
git clone https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C.git
cd Waveshare-ESP32-S3-Touch-LCD-3.5B-C
```

2. **Ouvrez le projet dans PlatformIO** (VS Code)

3. **Compilez et uploadez**:
```bash
platformio run --target upload
```

4. **Moniteur série**:
```bash
platformio device monitor
```

### Premier Test

Le code par défaut affiche :
1. ✅ Écran blanc
2. ✅ Texte "Coucou" en noir
3. ✅ Lecture du fichier `/audio/tada.mp3` (si présent sur la SD)

---

## 🎛️ Configuration

### Activer/Désactiver des Fonctionnalités

Éditez `include/features.h` :

```cpp
#define FEATURE_DISPLAY_ENABLED      true    // Écran LCD
#define FEATURE_TOUCH_ENABLED        true    // Tactile
#define FEATURE_AUDIO_ENABLED        true    // Audio
#define FEATURE_SD_ENABLED           true    // Carte SD

#define FEATURE_IMU_ENABLED          false   // IMU (futur)
#define FEATURE_RTC_ENABLED          false   // RTC (futur)
#define FEATURE_POWER_MGMT_ENABLED   false   // AXP2101 (futur)

#define USE_ADAFRUIT_FONTS           true    // Polices vectorielles
#define DEFAULT_BACKLIGHT_LEVEL      255     // Luminosité (0-255)
#define DEFAULT_AUDIO_VOLUME         70      // Volume (0-100)
```

---

## 🔌 GPIO Exposés (Connecteur 2x16 broches)

Le module expose un connecteur **2x16 broches** pour connecter vos capteurs, boutons, moteurs, LEDs, etc.

### Pinout du Connecteur

```
┌─────────────────────────────────────┐
│  1  BAT          5V         2       │
│  3  GND          GND        4       │
│  5  GPIO21       DN(GPIO19) 6       │
│  7  GPIO38       DP(GPIO20) 8       │
│  9  GPIO37       GPIO10     10      │
│  11 GPIO40       GPIO9      12      │
│  13 GPIO41       GPIO17     14      │
│  15 GPIO42       GPIO18     16      │
│  17 GPIO45       BOOT(GPIO0)18      │
│  19 GPIO46       RST         20     │
│  21 GPIO47       PWR         22     │
│  23 GPIO48       SCL(GPIO7)  24     │
│  25 RX(GPIO43)   SDA(GPIO8)  26     │
│  27 TX(GPIO44)   GND         28     │
│  29 GND          3V3         30     │
│  31 3V3          3V3         32     │
└─────────────────────────────────────┘
```

### GPIO Disponibles pour Vos Projets

**GPIO sûrs** (pas de conflit avec le module) :
- `GPIO 9, 10, 17, 18, 21, 37, 38, 40, 41, 42, 45, 46, 47, 48`

**GPIO réservés** (déjà utilisés par le module) :
- `GPIO 0-8` : LCD, Touch, I2C, Backlight
- `GPIO 11-16` : Audio I2S, SD Card
- `GPIO 19-20` : USB

### Utilisation des GPIO

Voir `include/config/gpio_config.h` pour les définitions :

```cpp
#define GPIO_EXT_21   21    // Exemple: Bouton
#define GPIO_EXT_38   38    // Exemple: LED
#define GPIO_EXT_40   40    // Exemple: Capteur
// ... etc
```

### Bus I2C (partagé)

Pour connecter des capteurs I2C externes :
- **SDA** : GPIO 8 (broche 26)
- **SCL** : GPIO 7 (broche 24)

⚠️ **Attention** : Ce bus est déjà utilisé par Touch, IMU, RTC, Power Management. Assurez-vous que votre capteur utilise une adresse I2C différente.

### Alimentation

- **5V** : Broche 2 (sortie régulée)
- **3V3** : Broches 30, 31, 32 (sortie régulée)
- **GND** : Broches 3, 4, 28, 29
- **BAT** : Broche 1 (connexion batterie Li-ion optionnelle)

---

## 📚 Utilisation des Drivers

### Display

```cpp
#include "drivers/Display.h"

Display display;

void setup() {
    display.begin();

    // Écran blanc
    display.clear(COLOR_WHITE);

    // Texte centré
    display.printCentered("Hello SPARKOH!", 240, COLOR_BLACK, 3);

    // Luminosité
    display.setBacklight(200);  // 0-255
}
```

### Audio

```cpp
#include "drivers/Audio.h"

Audio audio;

void setup() {
    audio.begin();
    audio.setVolume(70);  // 0-100
    audio.play("/audio/tada.mp3");
}

void loop() {
    audio.loop();  // Important !
}
```

### Carte SD

```cpp
#include "drivers/SDCard.h"

SDCard sd;

void setup() {
    sd.begin();

    // Vérifier fichier
    if (sd.exists("/data/config.txt")) {
        File file = sd.open("/data/config.txt");
        // Lire le fichier...
    }

    // Lister répertoire
    sd.listDir("/audio");
}
```

---

## 🎨 Exemples de Code

### Exemple 1 : Bouton + LED

```cpp
#include "drivers/Display.h"
#include "config/gpio_config.h"

Display display;

#define BTN_PIN  GPIO_EXT_21
#define LED_PIN  GPIO_EXT_38

void setup() {
    display.begin();
    pinMode(BTN_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    if (digitalRead(BTN_PIN) == LOW) {
        digitalWrite(LED_PIN, HIGH);
        display.printCentered("Bouton presse!", 240, COLOR_RED, 3);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
    delay(10);
}
```

### Exemple 2 : Lecteur MP3 avec sélection tactile

```cpp
#include "drivers/Display.h"
#include "drivers/Audio.h"
#include "drivers/SDCard.h"

Display display;
Audio audio;
SDCard sd;

void setup() {
    display.begin();
    sd.begin();
    audio.begin();

    display.clear(COLOR_BLACK);
    display.printCentered("Lecteur MP3", 100, COLOR_CYAN, 3);
    display.printCentered("Touchez pour jouer", 200, COLOR_WHITE, 2);
}

void loop() {
    // Votre logique tactile ici...
    // Si touche détectée:
    //   audio.play("/audio/song.mp3");

    audio.loop();
}
```

---

## 🔧 Compilation

### Build

```bash
platformio run
```

### Upload

```bash
platformio run --target upload
```

### Monitor série

```bash
platformio device monitor
```

### Nettoyer

```bash
platformio run --target clean
```

---

## 📦 Bibliothèques Utilisées

- `moononournation/GFX Library for Arduino@1.5.0` - Affichage LCD
- `adafruit/Adafruit GFX Library` - Polices vectorielles
- `earlephilhower/ESP8266Audio` - Lecture MP3/WAV
- `SD_MMC` - Carte SD (ESP32 natif)

---

## 🐛 Dépannage

### L'écran reste noir

1. Vérifiez l'alimentation (min 5V/2A)
2. Vérifiez le câble USB-C
3. Vérifiez que `FEATURE_DISPLAY_ENABLED = true` dans `features.h`

### Pas de son

1. Vérifiez que le fichier MP3 existe sur la SD
2. Vérifiez `FEATURE_AUDIO_ENABLED = true`
3. Augmentez le volume : `audio.setVolume(100);`

### Carte SD non détectée

1. Formatez la carte en FAT32
2. Réinsérez la carte
3. Vérifiez les logs série

---

## 📝 Notes de Développement

### Nouveaux Projets

Pour démarrer un nouveau projet :

1. Copiez ce projet
2. Éditez `include/features.h` selon vos besoins
3. Écrivez **uniquement votre logique** dans `src/main.cpp`
4. Les drivers et configs sont **réutilisables sans modification**

### Ajout de Nouveaux Drivers

1. Créez `include/drivers/VotreDriver.h`
2. Suivez le modèle des drivers existants
3. Ajoutez la config dans `include/config/`
4. Ajoutez le feature dans `include/features.h`

---

## 🤝 Contribution

Contributions bienvenues ! Créez une issue ou un pull request.

---

## 📄 Licence

MIT License - Voir LICENSE

---

## 👨‍💻 Auteur

**Michaël** - SPARKOH! Centre de Culture Scientifique

🤖 Développé avec l'assistance de [Claude Code](https://claude.com/claude-code)

---

## 🔗 Liens Utiles

- [Waveshare ESP32-S3-Touch-LCD-3.5B](https://www.waveshare.com/esp32-s3-touch-lcd-3.5b.htm)
- [Schéma électronique](https://files.waveshare.com/wiki/ESP32-S3-Touch-LCD-3.5/ESP32-S3-Touch-LCD-3.5-SCH.pdf)
- [PlatformIO](https://platformio.org/)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
