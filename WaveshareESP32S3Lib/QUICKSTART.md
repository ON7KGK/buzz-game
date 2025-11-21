# 🚀 Guide de Démarrage Rapide

Guide pour démarrer rapidement avec votre module Waveshare ESP32-S3-Touch-LCD-3.5B-C.

## 📦 Prérequis

### Matériel nécessaire
- Module Waveshare ESP32-S3-Touch-LCD-3.5B-C
- Câble USB-C
- (Optionnel) Batterie lithium 3.7V
- (Optionnel) Carte SD/TF
- (Optionnel) Caméra OV5640 (pour version -C)

### Logiciels nécessaires
- **PlatformIO** (recommandé) OU **Arduino IDE**
- **VS Code** (pour PlatformIO)
- **Pilote USB-to-Serial** (généralement détecté automatiquement)

## ⚡ Installation en 5 minutes

### Option 1: PlatformIO (Recommandé)

1. **Installer VS Code et PlatformIO**
   ```bash
   # Télécharger VS Code: https://code.visualstudio.com/
   # Installer l'extension PlatformIO depuis VS Code
   ```

2. **Créer un nouveau projet**
   ```bash
   pio project init --board esp32-s3-devkitc-1
   ```

3. **Copier la bibliothèque**
   ```bash
   # Copier le dossier WaveshareESP32S3Lib dans lib/
   cp -r WaveshareESP32S3Lib/ votre-projet/lib/
   ```

4. **Configurer platformio.ini**
   ```ini
   [env:esp32s3]
   platform = espressif32
   board = esp32-s3-devkitc-1
   framework = arduino
   board_build.flash_size = 16MB
   board_build.partitions = default_16MB.csv
   board_build.arduino.memory_type = qio_opi
   board_build.psram = enabled
   
   lib_deps = 
       moononournation/GFX Library for Arduino@^1.4.7
       lewisxhe/XPowersLib@^0.2.4
   
   monitor_speed = 115200
   ```

5. **Tester avec l'exemple BasicTest**
   ```bash
   pio run -t upload
   pio device monitor
   ```

### Option 2: Arduino IDE

1. **Installer Arduino IDE**
   - Télécharger depuis https://www.arduino.cc/

2. **Installer le support ESP32**
   - Fichier → Préférences → URLs de gestionnaire de cartes supplémentaires
   - Ajouter: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - Outils → Type de carte → Gestionnaire de cartes
   - Installer "esp32 by Espressif Systems"

3. **Installer les bibliothèques**
   - Outils → Gérer les bibliothèques
   - Installer: "GFX Library for Arduino"
   - Installer: "XPowersLib"

4. **Copier la bibliothèque WaveshareESP32S3**
   ```bash
   # Linux/Mac
   cp -r WaveshareESP32S3Lib/src ~/Arduino/libraries/WaveshareESP32S3
   
   # Windows
   # Copier manuellement dans Documents/Arduino/libraries/
   ```

5. **Configurer la carte**
   - Outils → Type de carte → ESP32S3 Dev Module
   - Outils → Partition Scheme → 16M Flash (3MB APP/9.9MB FATFS)
   - Outils → PSRAM → OPI PSRAM

## 🎯 Premier programme

### Programme minimal

```cpp
#include <WaveshareESP32S3.h>

WaveshareESP32S3 board;

void setup() {
    Serial.begin(115200);
    
    // Initialiser le module
    board.begin();
    
    // Message de bienvenue
    board.display.clear(COLOR_BLACK);
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(50, 100);
    board.display.println("Hello!");
    board.display.setCursor(50, 130);
    board.display.println("SPARKOH!");
}

void loop() {
    // Rien à faire
    delay(1000);
}
```

### Programme interactif avec tactile

```cpp
#include <WaveshareESP32S3.h>

WaveshareESP32S3 board;
uint16_t color = COLOR_RED;

void setup() {
    Serial.begin(115200);
    board.begin();
    
    board.display.clear(COLOR_BLACK);
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_YELLOW);
    board.display.setCursor(10, 10);
    board.display.println("Touchez l'ecran!");
}

void loop() {
    if (board.touch.touched()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();
        
        // Dessiner un cercle
        board.display.fillCircle(x, y, 10, color);
        
        // Changer de couleur
        color = random(0xFFFF);
        
        Serial.printf("Touch: X=%d, Y=%d\n", x, y);
    }
    
    delay(10);
}
```

## 🎨 Exemples de dessin

### Dessiner des formes

```cpp
void dessinerFormes() {
    // Rectangle
    board.display.drawRect(10, 10, 100, 50, COLOR_RED);
    board.display.fillRect(120, 10, 100, 50, COLOR_BLUE);
    
    // Cercle
    board.display.drawCircle(60, 150, 40, COLOR_GREEN);
    board.display.fillCircle(170, 150, 40, COLOR_YELLOW);
    
    // Triangle
    board.display.drawTriangle(50, 250, 100, 300, 0, 300, COLOR_CYAN);
    
    // Ligne
    board.display.drawLine(0, 350, 320, 350, COLOR_MAGENTA);
}
```

### Afficher du texte

```cpp
void afficherTexte() {
    // Petit texte
    board.display.setTextSize(1);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(10, 10);
    board.display.println("Petit texte");
    
    // Texte moyen
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_YELLOW);
    board.display.setCursor(10, 30);
    board.display.println("Moyen");
    
    // Grand texte
    board.display.setTextSize(3);
    board.display.setTextColor(COLOR_RED);
    board.display.setCursor(10, 60);
    board.display.println("GRAND!");
    
    // Texte avec fond
    board.display.setTextSize(2);
    board.display.setTextColor(COLOR_BLACK, COLOR_WHITE);
    board.display.setCursor(10, 110);
    board.display.println("Avec fond");
}
```

## 🔧 Configuration personnalisée

### Activer/Désactiver des composants

Éditez `src/config/features.h`:

```cpp
// Activer (décommenter)
#define FEATURE_DISPLAY_ENABLED
#define FEATURE_TOUCH_ENABLED
#define FEATURE_AUDIO_ENABLED

// Désactiver (commenter)
// #define FEATURE_CAMERA_ENABLED
// #define FEATURE_IMU_ENABLED
```

### Changer la luminosité par défaut

Éditez `src/config/display_config.h`:

```cpp
#define DEFAULT_BRIGHTNESS 80  // 0-100%
```

### Changer l'orientation de l'écran

```cpp
void setup() {
    board.begin();
    board.display.setRotation(1);  // 0=portrait, 1=paysage
}
```

## 🐛 Dépannage courant

### Problème: L'écran reste noir
**Solution:**
```cpp
board.display.setBrightness(100);  // Luminosité au max
board.display.clear(COLOR_WHITE);   // Tester avec du blanc
```

### Problème: Le tactile ne répond pas
**Solution:**
```cpp
// Vérifier dans le Serial Monitor
if (board.touch.isInitialized()) {
    Serial.println("Tactile OK");
} else {
    Serial.println("Tactile ERROR");
}
```

### Problème: Erreur de compilation "Out of memory"
**Solution:**
- Désactiver les composants non utilisés dans `features.h`
- Réduire `BUFFER_SIZE` dans `display_config.h`
- Utiliser la partition 16M Flash

### Problème: Port COM non détecté
**Solution:**
- Maintenir le bouton BOOT en branchant le câble USB
- Installer les drivers CH340/CP210x si nécessaire
- Essayer un autre câble USB-C

## 📚 Prochaines étapes

1. ✅ Tester l'exemple BasicTest
2. 📖 Lire la documentation complète (README.md)
3. 🎨 Créer votre premier projet interactif
4. 🔊 Ajouter du son avec le codec audio
5. 📷 Utiliser la caméra (si version -C)
6. 🌐 Connecter au WiFi
7. 🎮 Créer une interface LVGL

## 💡 Conseils pour SPARKOH!

Pour vos projets éducatifs:

```cpp
// Utiliser les couleurs SPARKOH!
board.display.clear(COLOR_SPARKOH_BLUE);
board.display.setTextColor(COLOR_SPARKOH_ORANGE);

// Interface enfant-friendly
board.display.setTextSize(3);  // Gros texte
board.display.fillCircle(x, y, 30, color);  // Gros boutons

// Feedback tactile
if (board.touch.touched()) {
    // Jouer un son
    // Vibrer (si moteur ajouté)
    // Changer de couleur
}
```

## 🎉 C'est parti!

Vous êtes prêt à créer des projets interactifs incroyables! 🚀

Pour plus d'aide:
- 📖 README.md - Documentation complète
- 💬 GitHub Issues - Support et questions
- 📧 Contact: votre-email@sparkoh.be

---

**Bon développement! 🎨🔬⚡**
