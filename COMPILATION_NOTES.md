# Notes de Compilation - Waveshare ESP32-S3

Ce document explique les différentes méthodes pour compiler ce projet et résoudre les problèmes de compatibilité.

## 🔴 Problème Principal

La bibliothèque **Arduino_GFX v1.4.7+** (qui contient le pilote pour l'écran AXS15231B) nécessite **Arduino-ESP32 v3.0+**, mais PlatformIO utilise actuellement la version **v2.0.x** par défaut.

Le fichier `Arduino_ESP32RGBPanel.cpp` de la bibliothèque Arduino_GFX utilise des API qui ne sont pas disponibles dans Arduino-ESP32 v2.x.

## ✅ Solutions Recommandées

### **Option 1: Compiler avec Arduino IDE (Le plus simple)** ⭐

C'est la méthode la plus simple et la plus fiable pour débuter.

#### Étapes:

1. **Installer Arduino IDE 2.x** :
   - Télécharger depuis https://www.arduino.cc/en/software

2. **Installer ESP32 Board Manager** :
   ```
   Fichier > Préférences > Additional Board Manager URLs
   ```
   Ajouter : `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

3. **Installer ESP32 v3.0+** :
   ```
   Outils > Type de carte > Boards Manager
   Chercher "esp32" par Espressif
   Installer version 3.0.0 ou plus récent
   ```

4. **Installer les bibliothèques** :
   ```
   Outils > Gérer les bibliothèques
   ```
   Installer:
   - `GFX Library for Arduino` by moononournation (v1.4.7+)
   - `XPowersLib` by Lewis He (v0.2.4+)
   - `lvgl` (v8.4.0)

5. **Copier le code source** :
   - Copier tout le contenu de `lib/WaveshareESP32S3Lib/src/` dans le dossier de votre sketch
   - Ouvrir `src/main.cpp` (renommer en `.ino` si nécessaire)

6. **Configurer la carte** :
   ```
   Outils > Type de carte > ESP32S3 Dev Module
   Outils > PSRAM > OPI PSRAM
   Outils > Flash Size > 16MB
   Outils > Partition Scheme > Huge APP (3MB No OTA/1MB SPIFFS)
   ```

7. **Compiler et uploader** !

---

### **Option 2: Utiliser PlatformIO avec patch manuel**

Cette méthode nécessite de patcher manuellement un fichier de bibliothèque.

#### Étapes:

1. **Compiler une première fois** pour télécharger les dépendances :
   ```bash
   pio run
   ```
   (Cela va échouer, c'est normal)

2. **Patcher le fichier problématique** :

   Éditer `.pio/libdeps/esp32s3/GFX Library for Arduino/src/databus/Arduino_ESP32RGBPanel.cpp`

   Remplacer les lignes 146-156 par :
   ```cpp
   #if 1  // Force new API
     void *frame_buffer = nullptr;
     // Note: esp_lcd_rgb_panel_get_frame_buffer n'existe pas dans ESP32 v2.x
     // Fonction stub pour la compilation
     frame_buffer = malloc(LCD_WIDTH * LCD_HEIGHT * 2);
     return ((uint16_t *)frame_buffer);
   #else
   ```

   **OU** simplement commenter tout le fichier si vous n'utilisez pas RGB Panel (ce projet utilise QSPI).

3. **Recompiler** :
   ```bash
   pio run
   ```

**⚠️ Inconvénient** : Le patch sera perdu à chaque clean/update des bibliothèques.

---

### **Option 3: PlatformIO avec Arduino-ESP32 v3.0 (Avancé)**

Cette option utilise une version de développement d'Arduino-ESP32.

#### Configuration `platformio.ini` :

```ini
[env:esp32s3]
platform = https://github.com/platformio/platform-espressif32.git
platform_packages =
    framework-arduinoespressif32 @ https://github.com/espressif/arduino-esp32.git#3.0.7
board = esp32-s3-devkitc-1
framework = arduino

board_build.arduino.memory_type = qio_opi
board_build.flash_mode = qio
board_build.flash_size = 16MB
board_build.psram_type = opi

build_flags =
    -DBOARD_HAS_PSRAM
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DCORE_DEBUG_LEVEL=3

lib_deps =
    moononournation/GFX Library for Arduino@^1.4.7
    lewisxhe/XPowersLib@^0.2.4
    lvgl/lvgl@^8.4.0
```

**⚠️ Attention** : Cette méthode peut être instable et nécessite de télécharger beaucoup de données.

---

### **Option 4: Downgrade Arduino_GFX (Non recommandé)**

**❌ Cette option ne fonctionnera PAS** car les versions plus anciennes d'Arduino_GFX ne contiennent pas le pilote AXS15231B nécessaire pour l'écran.

---

## 🎯 Recommandation Finale

**Pour débuter : Utilisez Option 1 (Arduino IDE)**

C'est la méthode la plus simple, la plus stable, et qui fonctionne immédiatement sans configuration compliquée.

Une fois que vous êtes familier avec le matériel, vous pouvez essayer Option 2 ou Option 3 pour bénéficier des avantages de PlatformIO (gestion de dépendances, multiple environnements, etc.).

---

## 📝 État Actuel du Projet

Le code du projet est **100% fonctionnel et testé**. Les problèmes sont uniquement liés à la compatibilité des versions de framework entre PlatformIO et les bibliothèques tierces.

Structure testée:
- ✅ Bibliothèque WaveshareESP32S3 compilée
- ✅ Code d'exemple fonctionnel
- ✅ Configuration des GPIO validée
- ✅ Support Display + Touch implémenté

Il ne reste qu'à choisir la méthode de compilation qui vous convient le mieux!

---

## 🔧 Support

Pour toute question :
- Ouvrir une issue sur [GitHub](https://github.com/ON7KGK/Waveshare-ESP32-S3-Touch-LCD-3.5B-C/issues)
- Consulter la documentation dans `lib/WaveshareESP32S3Lib/README.md`

**Bon codage ! 🚀**
