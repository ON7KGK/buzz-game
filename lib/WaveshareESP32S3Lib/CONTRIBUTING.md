# 🤝 Guide de Contribution

Merci de votre intérêt pour contribuer à la bibliothèque WaveshareESP32S3!

## 🎯 Comment contribuer

### Signaler des bugs 🐛

Si vous trouvez un bug:

1. Vérifiez qu'il n'a pas déjà été signalé dans les Issues
2. Créez une nouvelle Issue avec:
   - Description claire du problème
   - Version de la bibliothèque
   - Configuration matérielle (module, version)
   - Code minimal pour reproduire
   - Messages d'erreur complets
   - Configuration PlatformIO/Arduino

### Proposer des améliorations ✨

Pour suggérer une nouvelle fonctionnalité:

1. Ouvrez une Issue avec le tag `enhancement`
2. Décrivez clairement:
   - Ce que vous voulez faire
   - Pourquoi c'est utile
   - Comment vous l'imagineriez
   - Exemples d'utilisation

### Contribuer du code 💻

1. **Fork** le repository
2. Créez une **branche** pour votre fonctionnalité
   ```bash
   git checkout -b feature/ma-fonctionnalite
   ```
3. **Développez** votre code en suivant nos conventions
4. **Testez** sur du matériel réel
5. **Documentez** (commentaires, README si nécessaire)
6. **Commit** avec des messages clairs
   ```bash
   git commit -m "feat: ajoute support audio ES8311"
   ```
7. **Push** vers votre fork
8. Créez une **Pull Request**

## 📝 Conventions de code

### Style C++

```cpp
// Indentation: 4 spaces (pas de tabs)
// Accolades: style K&R
// Noms de classes: PascalCase
// Noms de méthodes: camelCase
// Noms de variables: camelCase
// Constantes: UPPER_SNAKE_CASE

class MonComposant {
public:
    bool begin();
    void maMethode();
    
private:
    int _maVariable;
    static const int MA_CONSTANTE = 42;
};
```

### Commentaires

Utilisez le style Doxygen:

```cpp
/**
 * @brief Description courte
 * 
 * Description longue si nécessaire.
 * 
 * @param param1 Description du paramètre
 * @return Description du retour
 */
bool maFonction(int param1);
```

### Organisation des fichiers

```
src/
├── config/          # Fichiers .h de configuration uniquement
├── peripherals/     # Une classe par périphérique (.h + .cpp)
└── utils/           # Fonctions utilitaires
```

## 🧪 Tests

Avant de soumettre:

1. ✅ Compilez sans warnings
2. ✅ Testez sur ESP32-S3-Touch-LCD-3.5B-C réel
3. ✅ Vérifiez la consommation mémoire
4. ✅ Testez avec différentes configurations de `features.h`
5. ✅ Documentez les nouveaux paramètres

## 📚 Documentation

Toute nouvelle fonctionnalité doit inclure:

- ✅ Commentaires Doxygen dans le code
- ✅ Mise à jour du README.md
- ✅ Exemple d'utilisation (si pertinent)
- ✅ Mise à jour du CHANGELOG.md

## 🎨 Périphériques prioritaires

Contributions particulièrement bienvenues pour:

1. 🔊 **Audio ES8311** - Lecture/enregistrement
2. 📊 **IMU QMI8658** - Gestion accéléromètre/gyroscope
3. ⏰ **RTC PCF85063** - Horloge temps réel
4. 🔋 **AXP2101** - Gestion batterie
5. 💾 **SD Card** - Lecture/écriture fichiers
6. 📷 **Camera OV5640** - Capture photo/vidéo
7. 🎮 **LVGL** - Interface graphique avancée

### Template pour nouveau périphérique

```cpp
// MyPeripheral.h
#ifndef WAVESHARE_MYPERIPHERAL_H
#define WAVESHARE_MYPERIPHERAL_H

#include <Arduino.h>
#include "../config/features.h"

#ifdef FEATURE_MYPERIPHERAL_ENABLED

#include "../config/pins_definitions.h"
#include "../config/myperipheral_config.h"

class MyPeripheral {
public:
    MyPeripheral();
    ~MyPeripheral();
    
    bool begin();
    void end();
    
    bool isInitialized() { return _initialized; }

private:
    bool _initialized;
};

#endif // FEATURE_MYPERIPHERAL_ENABLED
#endif // WAVESHARE_MYPERIPHERAL_H
```

## 🔄 Process de Pull Request

1. Votre PR doit:
   - Avoir un titre clair
   - Référencer une Issue (si applicable)
   - Décrire les changements
   - Inclure des exemples de test

2. Le code sera reviewé pour:
   - Style et conventions
   - Fonctionnement correct
   - Impact mémoire
   - Documentation
   - Compatibilité

3. Après approbation:
   - Squash des commits si nécessaire
   - Merge dans main
   - Release si pertinent

## 📋 Checklist PR

- [ ] Mon code suit les conventions du projet
- [ ] J'ai commenté les parties complexes
- [ ] J'ai mis à jour la documentation
- [ ] Mon code compile sans warnings
- [ ] J'ai testé sur du matériel réel
- [ ] J'ai mis à jour CHANGELOG.md
- [ ] J'ai créé un exemple si nécessaire

## 💬 Communication

- **Issues** - Pour bugs et suggestions
- **Pull Requests** - Pour contributions code
- **Discussions** - Pour questions générales

## 🙏 Types de contributions

Toutes les contributions sont bienvenues:

- 💻 Code (fonctionnalités, corrections)
- 📚 Documentation (README, guides, tutoriels)
- 🎨 Exemples (projets d'utilisation)
- 🐛 Tests et rapports de bugs
- 🌍 Traductions
- 💡 Idées et suggestions

## 📜 Code of Conduct

Soyez respectueux et constructif. Ce projet est:
- 🤝 Inclusif
- 🎓 Éducatif
- 🆓 Open source

## 📧 Contact

Questions? Contactez:
- GitHub Issues (préféré)
- Email: votre-email@sparkoh.be

---

**Merci de contribuer au projet! 🎉**

Ensemble, rendons l'ESP32-S3 accessible à tous! 🚀
