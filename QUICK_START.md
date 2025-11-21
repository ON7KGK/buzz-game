# 🚀 Démarrage Rapide - Waveshare ESP32-S3

> Guide ultra-rapide pour compiler et flasher votre module en 5 minutes !

## ✅ Prérequis

- ✅ VS Code installé
- ✅ Extension PlatformIO installée
- ✅ Module Waveshare ESP32-S3-Touch-LCD-3.5B-C
- ✅ Câble USB-C

## 📥 Étape 1: Cloner le Projet

```bash
git clone https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C.git
cd Waveshare-ESP32-S3-Touch-LCD-3.5B-C
```

## 🔨 Étape 2: Ouvrir dans VS Code

```bash
code .
```

Ou : `Fichier > Ouvrir le dossier` dans VS Code

## ⚡ Étape 3: Compiler

**Méthode 1** : Cliquez sur l'icône ✓ (checkmark) dans la barre PlatformIO en bas

**Méthode 2** : Terminal
```bash
pio run
```

**Temps de compilation** : ~8 secondes (après le premier téléchargement des dépendances)

## 📤 Étape 4: Flasher

1. **Connecter** le module via USB-C
2. **Cliquez** sur l'icône → (arrow) dans la barre PlatformIO

**Ou** en ligne de commande :
```bash
pio run --target upload
```

## 🎉 Étape 5: Tester !

Le module devrait :
1. Afficher l'écran de bienvenue SPARKOH!
2. Afficher un test de couleurs
3. Passer en mode dessin tactile

**Interactions** :
- ✏️ Touchez l'écran pour dessiner
- 🗑️ Appuyez sur le bouton BOOT pour effacer

## 📊 Moniteur Série

Pour voir les messages de debug :

```bash
pio device monitor
```

Ou cliquez sur l'icône 🔌 (plug) dans la barre PlatformIO

**Baudrate** : 115200

## 🎨 Personnaliser le Code

Le fichier principal est : **[src/main.cpp](src/main.cpp)**

Exemple minimal :

```cpp
#include <Arduino.h>
#include <WaveshareESP32S3.h>

WaveshareESP32S3 board;

void setup() {
    Serial.begin(115200);
    board.begin();

    // Votre code ici
    board.display.clear(COLOR_BLUE);
    board.display.setTextSize(3);
    board.display.setTextColor(COLOR_WHITE);
    board.display.setCursor(50, 100);
    board.display.println("Bonjour!");
}

void loop() {
    // Votre boucle ici
    if (board.touch.read()) {
        uint16_t x = board.touch.getX();
        uint16_t y = board.touch.getY();
        board.display.fillCircle(x, y, 10, COLOR_RED);
    }
    delay(10);
}
```

## 📁 Structure du Projet

```
src/main.cpp                          ← Votre code principal
lib/WaveshareESP32S3Lib/             ← Bibliothèque du module
  ├── src/config/                    ← Configurations
  │   ├── features.h                 ← Activer/désactiver composants
  │   ├── pins_definitions.h         ← Définition des GPIO
  │   └── display_config.h           ← Config écran
  └── src/peripherals/               ← Pilotes
      ├── Display.h/cpp              ← Gestion écran
      └── TouchScreen.h/cpp          ← Gestion tactile
```

## 🔧 Configuration des Composants

Éditez **[lib/WaveshareESP32S3Lib/src/config/features.h](lib/WaveshareESP32S3Lib/src/config/features.h)** :

```cpp
#define FEATURE_DISPLAY_ENABLED      1    // Écran
#define FEATURE_TOUCH_ENABLED        1    // Tactile
#define FEATURE_AUDIO_ENABLED        1    // Audio
#define FEATURE_IMU_ENABLED          1    // IMU
#define FEATURE_WIFI_ENABLED         1    // WiFi
#define FEATURE_LVGL_ENABLED         1    // LVGL
// ... etc
```

## 📚 Ressources

- **[README.md](README.md)** - Documentation complète
- **[COMPILATION_NOTES.md](COMPILATION_NOTES.md)** - Notes de compilation détaillées
- **[lib/WaveshareESP32S3Lib/README.md](lib/WaveshareESP32S3Lib/README.md)** - Doc de la bibliothèque
- **[lib/WaveshareESP32S3Lib/QUICKSTART.md](lib/WaveshareESP32S3Lib/QUICKSTART.md)** - Guide bibliothèque

## 🆘 Problèmes Courants

### ❌ Erreur "Device not found"

**Solution** : Vérifiez que le câble USB-C supporte les données (pas uniquement charge)

### ❌ Erreur "Permission denied" (Linux/Mac)

**Solution** :
```bash
# Linux
sudo usermod -a -G dialout $USER
# Puis redémarrer la session

# Mac
# Pas de config nécessaire normalement
```

### ❌ Compilation échoue

**Solution** : Supprimez le cache et recompilez
```bash
pio run --target clean
rm -rf .pio
pio run
```

### ❌ L'écran ne s'allume pas

**Vérifications** :
1. Module alimenté en 5V/2A minimum
2. Câble USB-C de bonne qualité
3. Vérifier les logs du moniteur série

## 💡 Prochaines Étapes

Une fois le module testé :

1. **Explorez les exemples** dans `lib/WaveshareESP32S3Lib/examples/`
2. **Lisez la documentation complète** dans le README
3. **Implémentez vos propres fonctions** :
   - Audio avec ES8311
   - IMU avec QMI8658
   - WiFi / Bluetooth
   - LVGL pour une UI avancée

## 🎯 Objectifs pour SPARKOH!

Ce module est parfait pour :
- 🎮 Interfaces tactiles interactives
- 📊 Affichage de données en temps réel
- 🎨 Projets créatifs et éducatifs
- 🤖 Prototypes IoT avec écran

## 📞 Support

- **GitHub Issues** : [https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C/issues](https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C/issues)
- **Discussions** : [https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C/discussions](https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C/discussions)

---

**🎉 Bon codage avec votre module Waveshare ESP32-S3 !**

*Développé avec ❤️ pour SPARKOH! - Centre de Culture Scientifique*
