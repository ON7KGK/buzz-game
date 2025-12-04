# Câblage avec Optocoupleurs - Solution Professionnelle

## Matériel requis

- **1× Module Velleman VMA452** (4 optocoupleurs) ou équivalent
- **OU 3× Optocoupleurs individuels** type 4N25, PC817, etc.
- **3× Résistances de limitation** 220Ω-470Ω pour LEDs optocoupleurs

## Schéma de câblage

```
┌─────────────────────────────────────────────────────────────┐
│ ESP32-S3 Module                                             │
│                                                             │
│  GPIO 43 ←─────── [Opto 1 OUT]                            │
│  GPIO 17 ←─────── [Opto 2 OUT]                            │
│  GPIO 18 ←─────── [Opto 3 OUT]                            │
│                                                             │
│  3.3V ────────────→ HVCC (optocoupleurs)                   │
│  GND ─────────────→ HGND (optocoupleurs)                   │
└─────────────────────────────────────────────────────────────┘

Côté isolé (circuit de jeu):

ANNEAU (manche) ──→ 3.3V

STRUCTURE ──→ [LED Opto 1] ──[220Ω]──→ GND
   métallique

PLOT GAUCHE ──→ [LED Opto 2] ──[220Ω]──→ GND
   (isolé)

PLOT DROIT ──→ [LED Opto 3] ──[220Ω]──→ GND
   (isolé)
```

## Détails du câblage

### Côté ESP32 (isolé)
- **GPIO 43** (broche 27) ← Sortie Optocoupleur 1 (touchette structure)
- **GPIO 17** (broche 16) ← Sortie Optocoupleur 2 (plot gauche)
- **GPIO 18** (broche 18) ← Sortie Optocoupleur 3 (plot droit)
- **HVCC** ← 3.3V ESP32
- **HGND** ← GND ESP32

### Côté jeu (non isolé)
- **Anneau** → 3.3V (via manche mobile)
- **Structure métallique** → LED Opto 1 → Résistance 220Ω → GND
- **Plot gauche** (isolé) → LED Opto 2 → Résistance 220Ω → GND
- **Plot droit** (isolé) → LED Opto 3 → Résistance 220Ω → GND

## Fonctionnement

### Logique simplifiée (VMA452 avec correction)

**Tous les GPIO en INPUT_PULLUP en permanence**

Le module VMA452 utilise des **transistors de correction** qui inversent la logique standard:

| Situation | LED Opto | Sortie VMA452 | GPIO ESP32 |
|-----------|----------|---------------|------------|
| Anneau libre | Éteinte | LOW | LOW |
| Anneau touche plot/structure | Allumée | HIGH | HIGH |

⚠️ **Important**: OUT suit IN (pas d'inversion comme un optocoupleur classique)

### Phase "Attente démarrage"
1. Anneau libre → Tous GPIO = **LOW**
2. Anneau touche plot gauche → GPIO 17 = **HIGH** → Jeu prêt à gauche
3. Anneau touche plot droit → GPIO 18 = **HIGH** → Jeu prêt à droite

### Phase "Jeu en cours"
1. Anneau libre → GPIO 43 = **LOW**
2. Anneau touche structure → GPIO 43 = **HIGH** → **Touchette détectée!**
3. Anneau atteint plot opposé → GPIO opposé = **HIGH** → **Victoire!**

## Avantages de cette solution

✅ **Protection ESD complète** - Les décharges électrostatiques n'atteignent jamais l'ESP32
✅ **Isolation galvanique** - Aucun lien électrique entre jeu et électronique
✅ **Logique ultra-simple** - Pas de reconfiguration de GPIO
✅ **Fiable** - Signal propre, sans rebonds parasites
✅ **Détection claire** - LOW=contact, HIGH=libre (sans ambiguïté)
✅ **Robuste** - Gère tous les scénarios d'abandon et de redémarrage

## Tests de validation

### Test 1 : Démarrage à gauche
1. Poser l'anneau sur le plot gauche
2. → LED Opto 2 s'allume
3. → GPIO 17 = **HIGH**
4. → LED1 devient **bleue**
5. → Message "Rejoins l'autre côté sans toucher"
6. Soulever l'anneau → le jeu démarre

### Test 2 : Démarrage à droite
1. Poser l'anneau sur le plot droit
2. → LED Opto 3 s'allume
3. → GPIO 18 = **HIGH**
4. → LED1 devient **bleue**
5. → Message "Rejoins l'autre côté sans toucher"
6. Soulever l'anneau → le jeu démarre

### Test 3 : Touchette pendant le jeu
1. Démarrer une partie
2. Toucher la structure avec l'anneau
3. → LED Opto 1 s'allume
4. → GPIO 43 = **HIGH**
5. → LED1 devient **rouge**
6. → Son "touchette7.mp3"
7. → Message "OH! non tu as touché"

### Test 4 : Victoire
1. Démarrer à gauche
2. Atteindre le plot droit sans toucher
3. → LED Opto 3 s'allume
4. → GPIO 18 = **HIGH**
5. → LED1 devient **verte**
6. → Son "gagne2.mp3"
7. → Message "Bravo, tu as gagné en X.X s"

### Test 5 : Protection ESD
1. Décharger statique (friction pull) sur structure
2. → **Aucun effet sur l'ESP32** grâce à l'isolation optique
3. → Le jeu continue normalement

## Dépannage

### Problème : Le jeu ne détecte pas les plots
**Solution :**
- Vérifier le câblage des résistances 220Ω
- Vérifier que HVCC est bien alimenté en 3.3V
- Tester les LEDs optocoupleurs (doivent s'allumer au contact)

### Problème : GPIO toujours à LOW
**Solution :**
- Vérifier que les optocoupleurs ne sont pas montés à l'envers
- Vérifier les connexions HVCC et HGND
- Tester la continuité des sorties optocoupleurs

### Problème : Le jeu démarre tout seul
**Solution :**
- Vérifier que les plots ne touchent pas la structure ou entre eux
- Vérifier qu'il n'y a pas de court-circuit sur les LEDs optocoupleurs

## Alternative: Module VMA452

Si vous utilisez le module Velleman VMA452:
- **P1 broches 1-6** : Entrées (plots et structure) avec résistances intégrées
- **P2 broche 5** : HVCC → 3.3V ESP32
- **P2 broches 1-4** : Sorties → GPIO 43, 17, 18 (+ 1 libre)
- **P2 broche 6** : HGND → GND ESP32

Le VMA452 inclut déjà les résistances de limitation, simpliant encore le câblage!

## Pour revenir à l'ancienne version

Si cette solution ne fonctionne pas, vous pouvez revenir à la version stable:

```bash
git checkout main
```

La branche `feature/detection-plots-robuste` peut être supprimée si nécessaire.
