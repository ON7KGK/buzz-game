# Changelog

Tous les changements notables de ce projet seront documentés dans ce fichier.

## [1.0.0] - 2025-11-21

### Ajouté
- ✨ Première version de la bibliothèque WaveshareESP32S3
- 📺 Support complet de l'écran LCD 3.5" (AXS15231B, QSPI)
  - Dessin de formes (pixels, lignes, rectangles, cercles, triangles)
  - Affichage de texte avec tailles variables
  - Support des images bitmap RGB565
  - Contrôle du rétroéclairage PWM
  - Rotation de l'écran (0-3)
- 👆 Support du tactile capacitif
  - Lecture des coordonnées X/Y
  - Détection du toucher
  - Calibration possible
  - Support de l'interrupt pin
- ⚙️ Système de configuration modulaire
  - `features.h` pour activer/désactiver les composants
  - `pins_definitions.h` avec toutes les définitions GPIO
  - Fichiers de config par composant (display, audio, SD, camera)
- 📦 Support PlatformIO avec library.json
- 📚 Exemple BasicTest pour démarrer rapidement
- 📖 Documentation complète dans README.md
- 🎨 Couleurs prédéfinies incluant les couleurs SPARKOH!
- 🔍 Méthodes de test et debug intégrées

### Structure
- Configuration centralisée dans `src/config/`
- Classes périphériques dans `src/peripherals/`
- Classe principale `WaveshareESP32S3` pour orchestration

### À venir (TODO)
- 🔊 Support du codec audio ES8311
- 📊 Support de l'IMU QMI8658
- ⏰ Support du RTC PCF85063
- 🔋 Support de la gestion d'alimentation AXP2101
- 💾 Support de la carte SD
- 📷 Support de la caméra OV5640
- 🎮 Support LVGL pour interface graphique avancée
- 📡 Exemples WiFi et Bluetooth
- 🎵 Exemples audio (lecture/enregistrement)
- 🎥 Exemples caméra et streaming

### Notes
- Testé avec ESP32-S3-Touch-LCD-3.5B-C
- Nécessite Arduino_GFX v1.4.7+
- Compatible PlatformIO et Arduino IDE
- PSRAM recommandé pour meilleures performances

---

## Légende

- ✨ Nouvelle fonctionnalité
- 🐛 Correction de bug
- 📚 Documentation
- 🔧 Maintenance
- ⚡ Performance
- 🔒 Sécurité
- ⚠️ Deprecated
