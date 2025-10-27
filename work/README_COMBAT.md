# 🎮 Système de Combat et HUD - Guide Rapide

## 🚀 Lancement du jeu

```bash
make re
./cub3d map/valid/10.cub
```

---

## 🎯 Mécaniques de combat

### Santé du joueur
- **Démarrage** : 3 cœurs (barre verte complète en haut à gauche)
- **Invincibilité** : 1.5 secondes après chaque coup
- **Mort** : 0 HP → Écran Game Over → Restart auto 10s

### Comportement de l'alien
1. **Détection** : L'alien vous voit dans son champ de vision (60°, 8 blocs)
2. **Poursuite** : Il vous chase à vitesse élevée (180 px/s)
3. **Attaque** : Si collision avec sa hitbox (24px) → -1 HP
4. **Retraite** : Après attaque, il fuit 4.5 blocs et ignore le joueur pendant 6 secondes
5. **Retour** : Après cooldown, reprend sa patrouille

### États avancés de l'IA
- **PREPARING** : Rotation lente quand vous êtes proche mais hors champ de vision (2 sec)
- **STALKING** : Traque lente autour des murs quand vous êtes caché (3.3 sec max)
- **CHASE** : Poursuite directe quand visible
- **COOLDOWN** : Retraite après attaque (6 sec)

---

## 🎨 Interface HUD

### Barre de vie (permanent, haut gauche)
```
4 HP : 🟩 Barre verte complète
3 HP : 🟨 Barre jaune
2 HP : 🟧 Barre orange
1 HP : 🟥 Barre rouge
0 HP : ☠️  Game Over
```

### Flash de dégâts (0.3 sec par hit)
```
1er coup : 🔴 Petit flash rouge (lil_dega)
2ème coup : 🔴🔴 Moyen flash rouge (mid_dega)
3ème coup : 🔴🔴🔴 Grand flash rouge (Big_dega)
4ème coup : ☠️ Mort
```

### Game Over
```
┌────────────────────────────────┐
│                                │
│        GAME OVER               │
│                                │
│   Restarting in 10...          │
│                                │
└────────────────────────────────┘
```

---

## ⌨️ Contrôles

### Mouvement
- **W/Z** : Avancer
- **S** : Reculer
- **A/Q** : Strafe gauche
- **D** : Strafe droite
- **←/→** : Rotation caméra
- **Shift** : Courir (FOV augmenté)
- **Ctrl** : S'accroupir (vitesse réduite)

### Autres
- **M** : Toggle minimap
- **T** : Demi-tour rapide animé
- **ESC** : Quitter

---

## 🔧 Détails techniques

### Hitboxes
- **Joueur** : 12 pixels (rayon)
- **Alien** : 24 pixels (rayon)
- **Collision** : Distance < (12 + 24) = 36 pixels

### Timers (à 60 FPS)
```
Flash de dégâts  : 0.3 sec  (~18 frames)
Invincibilité    : 1.5 sec  (~90 frames)
Cooldown alien   : 6.0 sec  (~360 frames)
Traque (STALKING): 3.3 sec  (~198 frames)
Game Over        : 10.0 sec (~600 frames)
```

### Système de restart
Au bout de 10 secondes après la mort :
1. Health = 3 HP
2. Téléportation au spawn initial
3. Reset de la direction
4. Reset de tous les flags (is_dead, invincibility, damage_level)
5. Reprise du jeu

---

## 📁 Fichiers de documentation

- **COMBAT_SYSTEM.md** : Vue d'ensemble du système de combat
- **HUD_IMPLEMENTATION.md** : Documentation technique détaillée du HUD
- **README_COMBAT.md** : Ce fichier (guide rapide)

---

## 🐛 Debugging

### Messages de debug (stderr)
```
[ALIEN] HIT! Player health: 2/3, damage_level=2
[ALIEN] ATTACK! Retreating to (X, Y) for 6 seconds
[ALIEN] COOLDOWN: 4.50s remaining, ignoring player
[GAME] PLAYER DIED! Game Over in 10 seconds...
[GAME] Restarting game...
[PLAYER] Invincibility ended
```

### Vérifications visuelles
- ✅ Barre de vie visible en haut gauche
- ✅ Flash rouge lors d'un hit
- ✅ Progression des couleurs (vert→jaune→orange→rouge)
- ✅ Écran game over centré
- ✅ Aucun fond vert visible (transparence active)

---

## 🎯 Tips de survie

1. **Utilisez la minimap** (M) pour repérer l'alien
2. **Profitez du cooldown** de 6 secondes après chaque attaque
3. **Cachez-vous derrière les murs** pour éviter la détection
4. **Gardez vos distances** (hitbox alien = 24px)
5. **1.5 secondes d'invincibilité** : Fuyez après chaque coup !

---

## 🏆 Objectif

**Survivre le plus longtemps possible face à l'alien !**

Le système ne compte pas encore les points, mais vous pouvez :
- Chronométrer votre temps de survie
- Compter le nombre d'attaques évitées
- Défier vos amis sur le même setup

---

**Version** : 2.0.0 - HUD Complete  
**Date** : 7 octobre 2025  
**Statut** : ✅ Prêt à jouer
