# WaveshareESP32S3 Library

Bibliothèque complète pour le module **Waveshare ESP32-S3-Touch-LCD-3.5B-C** optimisée pour PlatformIO.

Développée par **Michaël** pour **SPARKOH!** 🚀

## 📋 Caractéristiques

Cette bibliothèque fournit une interface simple et modulaire pour tous les composants du module Waveshare:

- ✅ **Écran LCD 3.5"** (320×480, AXS15231B, QSPI)
- ✅ **Tactile capacitif** (I2C)
- 🔊 **Codec audio ES8311** (I2S)
- 📊 **IMU 6 axes QMI8658** (accéléromètre + gyroscope)
- ⏰ **RTC PCF85063** (horloge temps réel)
- 🔋 **Gestion d'alimentation AXP2101**
- 💾 **Carte SD/TF**
- 📷 **Caméra OV5640 5MP** (version -C uniquement)
- 🔘 **Boutons PWR et BOOT**
- 📡 **WiFi et Bluetooth 5**

## 🎯 Installation

### PlatformIO (Recommandé)

1. Copiez le dossier `WaveshareESP32S3Lib` dans le répertoire `lib/` de votre projet
2. Ou clonez depuis GitHub:
```bash
cd lib/
git clone https://github.com/votre-username/waveshare-esp32s3-lib.git
```

3. Utilisez le fichier `platformio.ini` fourni comme référence

### Arduino IDE

1. Copiez le dossier `WaveshareESP32S3Lib/src` dans votre dossier `libraries/` Arduino
2. Renommez-le en `WaveshareESP32S3`
3. Redémarrez Arduino IDE

## 🚀 Démarrage rapide

```cpp
#include <WaveshareESP32S3.h>

WaveshareESP32S3 board;

void setup() {
    Serial.begin(115200);
    
    // Initialiser le module
    if (!board.begin()) {
        Serial.println("Erreur initialisation!");
        while(1);
    }
    
    // Utiliser l'écran
    board.display.clear(COLOR_BLUE);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setTextSize(2);
    board.display.setCursor(10, 10);
    board.display.println("Hello SPARKOH!");
}

void loop() {
    // Lire le tactile
    if (board.touch.touched()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();
        board.display.fillCircle(x, y, 5, COLOR_RED);
    }
}
```

## ⚙️ Configuration

### Activation des composants

Éditez `src/config/features.h` pour activer/désactiver les composants:

```cpp
#define FEATURE_DISPLAY_ENABLED      // Écran LCD
#define FEATURE_TOUCH_ENABLED        // Tactile
#define FEATURE_AUDIO_ENABLED        // Audio
// #define FEATURE_CAMERA_ENABLED    // Caméra (décommentez si besoin)
```

### Configuration des pins

Toutes les définitions de GPIO sont dans `src/config/pins_definitions.h`.  
⚠️ **NE MODIFIEZ PAS** sauf si vous savez ce que vous faites!

### Configuration des périphériques

Chaque composant a son propre fichier de configuration:
- `display_config.h` - Écran (luminosité, rotation, couleurs)
- `audio_config.h` - Audio (volume, fréquence d'échantillonnage)
- `sdcard_config.h` - Carte SD (chemins, formats)
- `camera_config.h` - Caméra (résolution, framerate)

## 📚 Exemples

### 01_BasicTest
Test de base de l'écran et du tactile avec dessin interactif.

```bash
cd examples/01_BasicTest
pio run -t upload
```

## 📖 Documentation complète

### Classe `WaveshareESP32S3`

Classe principale pour gérer le module.

**Méthodes:**
- `begin()` - Initialise tous les composants
- `beginBasic()` - Initialise uniquement écran + tactile
- `end()` - Libère toutes les ressources
- `printInfo()` - Affiche les informations du module
- `testAll()` - Teste tous les périphériques
- `bootButtonPressed()` - Lit l'état du bouton BOOT

### Classe `WaveshareDisplay`

Gestion de l'écran LCD.

**Méthodes principales:**
- `begin()`, `end()` - Initialisation/terminaison
- `clear(color)` - Efface l'écran
- `setBrightness(0-100)` - Réglage luminosité
- `setRotation(0-3)` - Rotation de l'écran
- `drawPixel/Line/Rect/Circle/Triangle()` - Formes
- `print/println()` - Texte
- `setTextColor/Size()` - Style de texte

### Classe `WaveshareTouch`

Gestion du tactile capacitif.

**Méthodes principales:**
- `begin()` - Initialisation
- `read()` - Lit l'état tactile
- `touched()` - Vérifie si écran touché
- `getX/Y()` - Coordonnées du point tactile
- `calibrate()` - Calibration du tactile

## 🎨 Couleurs prédéfinies

```cpp
COLOR_BLACK, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_BLUE
COLOR_YELLOW, COLOR_CYAN, COLOR_MAGENTA, COLOR_ORANGE
COLOR_PURPLE, COLOR_GRAY, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY

// Couleurs SPARKOH!
COLOR_SPARKOH_BLUE
COLOR_SPARKOH_GREEN
COLOR_SPARKOH_ORANGE
```

## 🔧 Dépannage

### L'écran ne s'allume pas
- Vérifiez l'alimentation (batterie ou USB)
- Vérifiez que `FEATURE_DISPLAY_ENABLED` est défini
- Testez avec `board.display.setBrightness(100)`

### Le tactile ne répond pas
- Vérifiez que `FEATURE_TOUCH_ENABLED` est défini
- Le tactile utilise Wire1 (bus I2C séparé)
- Testez la calibration avec `touch.calibrate()`

### Erreur de compilation
- Vérifiez que toutes les bibliothèques sont installées
- Utilisez la partition `16M Flash(3MB APP/9.9MB FATFS)`
- CPU à 240MHz recommandé

### Problèmes mémoire
- Désactivez les composants non utilisés dans `features.h`
- Réduisez `BUFFER_SIZE` dans `display_config.h`
- Utilisez PSRAM pour les grandes images

## 📦 Structure du projet

```
WaveshareESP32S3Lib/
├── src/
│   ├── config/              # Fichiers de configuration
│   │   ├── features.h       # Activation des composants
│   │   ├── pins_definitions.h   # Définitions GPIO
│   │   ├── display_config.h     # Config écran
│   │   ├── audio_config.h       # Config audio
│   │   ├── sdcard_config.h      # Config SD
│   │   └── camera_config.h      # Config caméra
│   ├── peripherals/         # Classes des périphériques
│   │   ├── Display.h/.cpp
│   │   ├── TouchScreen.h/.cpp
│   │   ├── Audio.h/.cpp (TODO)
│   │   ├── IMU.h/.cpp (TODO)
│   │   └── ...
│   └── WaveshareESP32S3.h/.cpp  # Classe principale
├── examples/                # Exemples
├── library.json            # Métadonnées PlatformIO
├── platformio.ini          # Configuration exemple
└── README.md               # Ce fichier
```

## 🤝 Contribution

Les contributions sont les bienvenues! N'hésitez pas à:
- Signaler des bugs
- Proposer des améliorations
- Ajouter des exemples
- Compléter les périphériques manquants

## 📄 Licence

MIT License - Libre d'utilisation pour projets personnels et commerciaux.

## 👤 Auteur

**Michaël**  
Technicien multimédia @ SPARKOH!  
Belgique 🇧🇪

## 🙏 Remerciements

- Waveshare pour le module
- Communauté ESP32
- SPARKOH! pour le soutien

## 📞 Support

Pour questions ou support:
- GitHub Issues: [lien vers votre repo]
- Email: votre-email@exemple.com

---

**Développé avec ❤️ pour les projets éducatifs et interactifs de SPARKOH!**
