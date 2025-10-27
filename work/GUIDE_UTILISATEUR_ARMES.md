# 🎮 Guide Complet des Améliorations d'Armes

## 🆕 Nouveautés Implémentées

### 1. 🔨 **Marteau positionné main droite**
- Décalé de **200 pixels vers la droite**
- Donne l'impression de tenir l'arme dans la main droite
- Plus naturel et immersif

### 2. 🏃 **Effet de course réaliste**
- Les armes **balancent de gauche à droite** quand vous bougez
- **±15 pixels** d'amplitude
- Plus rapide quand vous courez (Shift)
- Retour fluide au centre quand vous vous arrêtez

### 3. 🎯 **Visée ultra-précise du MP**
- **Cone de 10°** (très étroit, il faut vraiment viser !)
- **Portée de 500 pixels**
- **Double vérification** :
  - Angle avec votre direction
  - Distance perpendiculaire < 50px
- **Plus difficile** mais **plus satisfaisant** !

### 4. 🩸 **Effet de sang quand vous touchez l'alien**
- Animation de **6 frames** sur 0.48 secondes
- S'affiche automatiquement quand le MP touche l'alien
- Transparent, s'affiche par-dessus le sprite alien
- **⚠️ Nécessite les textures** `blood_alien1.xpm` à `blood_alien6.xpm`

---

## 🎮 Comment ça marche

### Marteau
- **Position** : À droite de l'écran (comme si vous le teniez)
- **Balancement** : Oscille pendant que vous bougez
- **Attaque** : Espace (6 frames d'animation)

### MP
- **Position** : Centre-droit, agrandi x1.8
- **Balancement** : Oscille pendant que vous bougez
- **Visée** : Très précise, il faut vraiment aligner le tir
- **Tir** : Espace
  - Si vous touchez → Alien stunné 2 secondes + effet de sang
  - Message console : `[WEAPON] MP hit detected! Alien stunned.`

---

## 🎯 Comment bien viser avec le MP

### Conseils
1. **Regardez l'alien en face** (pas de côté)
2. **Centrez-le dans votre écran**
3. **Attendez qu'il soit proche** (portée 500px = ~12 blocs)
4. **Tirez** (Espace)

### Indicateurs de hit
- ✅ Console : `[WEAPON] MP hit detected!`
- ✅ Console : `[WEAPON] Alien stunned for 2.0s!`
- ✅ Console : `Blood effect activated` (si textures présentes)
- ✅ L'alien s'arrête net pendant 2 secondes
- ✅ Effet de sang visible (si textures chargées)

### Pourquoi j'ai raté ?
- ❌ **Alien trop loin** (>500px)
- ❌ **Angle trop large** (>10°)
- ❌ **Alien pas centré** (>50px du rayon)
- ❌ **Alien déjà stunné** (pas de stack)

---

## 🩸 Textures de sang (optionnelles)

### Status
Le système est **implémenté et fonctionnel**, mais les textures manquent.

### Ce qui se passe sans les textures
- ⚠️ Message au démarrage : `[WARNING] Blood textures not found`
- ✅ Le jeu fonctionne normalement
- ✅ Le stun fonctionne
- ❌ Pas d'effet visuel de sang

### Comment ajouter les textures

**Option 1 : Placeholder temporaire**
```bash
cd textures/alien
for i in {1..6}; do
    cp alien_front.xpm blood_alien$i.xpm
done
```

**Option 2 : Créer les vraies textures**
1. Créer 6 images XPM (64x64 ou 128x128)
2. Fond transparent
3. Éclaboussure de sang rouge
4. Variation pour chaque frame (expansion puis fade)
5. Placer dans `textures/alien/`

### Frames recommandées
- **Frame 1** : Petit impact (début)
- **Frame 2** : Expansion
- **Frame 3** : Maximum (le plus visible)
- **Frame 4** : Début fade
- **Frame 5** : Fade moyen
- **Frame 6** : Presque invisible

---

## 🎮 Contrôles

| Touche | Action |
|--------|--------|
| **E** | Changer d'arme (Marteau ↔ MP) |
| **Espace** | Attaquer/Tirer |
| **WASD** | Bouger (déclenche le balancement d'arme) |
| **Shift** | Courir (balancement plus rapide) |
| **M** | Minimap |

---

## 🏆 Stratégie de combat

### Face à un alien qui charge
1. **Équiper le MP** (touche E)
2. **Reculer** (S) tout en le gardant face à vous
3. **Viser précisément** (centrer l'alien)
4. **Tirer** (Espace) quand il est dans la portée
5. **Si hit** → Alien stunné 2 secondes → **Fuyez !**
6. **Recommencer** après le stun

### Distance optimale
- **Trop près** : Alien vous attaque avant que vous puissiez tirer
- **Optimal** : 200-400 pixels (environ 5-10 blocs)
- **Trop loin** : Hors portée (>500px)

### Skill ceiling
- ✅ Visée précise récompensée
- ✅ Timing important
- ✅ Positionnement tactique
- ✅ Pas de spam, chaque tir compte

---

## 🔧 Paramètres modifiables

Si vous voulez ajuster la difficulté :

### Rendre la visée plus facile
```c
// src/player/weapon.c - ligne ~120
double cos_threshold = cos(15.0 * M_PI / 180.0); /* 15° au lieu de 10° */
double max_perp_dist = 80.0;                      /* 80px au lieu de 50px */
```

### Augmenter la portée
```c
// src/player/weapon.c - ligne ~115
double max_range = 700.0;  /* 700px au lieu de 500px */
```

### Changer le balancement
```c
// src/player/weapon.c - ligne ~35
double bob_speed = p->is_running ? 10.0 : 6.0;  /* Moins rapide */
p->weapon_bob_offset = sin(...) * 10.0;         /* Moins d'amplitude */
```

---

## 📊 Comparaison Avant/Après

| Feature | Avant | Maintenant |
|---------|-------|------------|
| **Position marteau** | Centre | Droite (+200px) |
| **Balancement** | ❌ Aucun | ✅ ±15px oscillation |
| **Visée MP** | Facile (20°) | Précise (10°) |
| **Feedback hit** | Stun uniquement | Stun + sang animé |
| **Immersion** | Basique | FPS moderne |
| **Skill required** | Low | Medium-High |

---

## 🐛 Troubleshooting

### L'arme ne balance pas
- Vérifiez que vous bougez avec WASD
- Le balancement est subtil (±15px)

### Le MP ne touche jamais
- **Normal !** La visée est très précise
- Vérifiez que l'alien est bien face à vous
- Essayez de vous rapprocher (<400px)

### Pas d'effet de sang
- Les textures `blood_alien*.xpm` sont absentes
- Ajoutez-les ou utilisez des placeholders
- Le jeu fonctionne quand même

### Le marteau est trop à droite
- C'est voulu ! Effet "main droite"
- Modifiable dans `weapon_draw()` : `offset_x = 200;`

---

## ✅ Checklist

- [x] Marteau décalé droite
- [x] Effet de balancement pendant course
- [x] Visée MP ultra-précise (10°)
- [x] Système de stun amélioré
- [x] Effet de sang (code prêt, textures optionnelles)
- [x] Compilation réussie
- [x] Aucun crash
- [x] Gameplay skill-based

---

## 🎯 Prochaines étapes (optionnel)

Si vous voulez aller plus loin :

1. **Créer les textures de sang** pour l'effet visuel complet
2. **Ajouter un son** quand le MP touche l'alien
3. **Crosshair** au centre de l'écran pour aider la visée
4. **Munitions limitées** pour le MP (système de rechargement)
5. **Dégâts** : plusieurs hits pour tuer l'alien

---

**Compilation** : ✅ Réussie  
**Taille** : 386 Ko  
**Version** : v2.0 Advanced  
**Date** : 20 octobre 2025

🎮 **Le jeu est prêt à jouer !** 🎮
