# Améliorations du Système d'Armes

## ✅ Modifications implémentées

### 1. **Positionnement des armes** 🎯

#### Marteau
- **Décalage à droite** : +100 pixels
- Position : Bas de l'écran, décalé vers la droite
- Permet une meilleure visibilité centrale

#### MP (Mitraillette)
- **Agrandissement** : Scale x1.8 (180% de la taille originale)
- **Léger décalage** : +30 pixels à droite
- Position : Bas de l'écran, centré avec léger offset
- Image agrandie via scaling en temps réel dans weapon_draw()

### 2. **Ordre de rendu** 🎨

**Nouvelle hiérarchie** (de l'arrière vers l'avant) :
1. Sol & Ciel
2. Murs (raycasting)
3. Sprite alien
4. Minimap
5. **Arme (dessinée en premier)** ← Modifié
6. **HUD (santé, dégâts, game over)** ← Par-dessus l'arme

**Résultat** : La barre de PV apparaît maintenant au-dessus des sprites d'armes

### 3. **Système de Tir MP - Stun FPS** 🔫

#### Nouvelles fonctionnalités

**Detection de hit** :
- `weapon_mp_hits_alien()` : Détecte si le tir touche l'alien
- Utilise un raycasting simplifié (dot product)
- Cone de tir : ~20° (ajustable)
- Portée max : 400 pixels

**Système de Stun** :
- Nouvel état alien : `is_stunned`
- Durée du stun : **2.0 secondes**
- L'alien est **complètement immobilisé** pendant le stun
- Toutes les vélocités sont mises à 0
- Les attaques en cours sont annulées
- Le joueur peut s'enfuir pendant ce temps

**Mécaniques** :
- Quand le joueur tire avec le MP (touche Espace avec arme 1)
- Le système vérifie si l'alien est dans le cone de tir
- Si touché → Stun de 2 secondes appliqué
- Message de debug affiché dans la console
- Pas de stack de stun (impossible de retriger pendant un stun actif)

### 4. **Modifications techniques**

#### Fichiers modifiés :

**includes/cub3d.h** :
- Ajout `is_stunned` et `stun_timer` dans `t_alien`
- Ajout prototypes `alien_apply_stun()` et `alien_check_hit_by_mp()`
- Ajout prototype `weapon_mp_hits_alien()`

**src/player/weapon.c** :
- Modification `weapon_draw()` : ajout scaling et offsets
- Ajout fonction `weapon_mp_hits_alien()` : détection de hit géométrique
- Variables locales `scale`, `offset_x` pour chaque arme

**src/Alien/alien3.c** :
- Ajout `alien_apply_stun()` : applique l'immobilisation
- Ajout `alien_check_hit_by_mp()` : wrapper de vérification

**src/Alien/alien.c** :
- Modification `alien_update()` : gestion du stun en début de fonction
- Return early si stunned (pas de mouvement, pas d'IA)
- Initialisation `is_stunned = 0` et `stun_timer = 0.0` dans `alien_init()`

**src/game/game_keys.c** :
- Modification du handler Space pour le MP
- Appel `alien_check_hit_by_mp()` lors du tir
- Affichage message de confirmation

**src/game/render_frame.c** :
- Réorganisation : `weapon_draw()` avant le HUD
- HUD dessiné en dernier pour être au-dessus

## 🎮 Utilisation

### Contrôles
- **E** : Changer d'arme (Marteau ↔ MP)
- **Espace** : Attaquer/Tirer
  - Marteau : Animation 6 frames
  - MP : Tir avec effet de stun sur l'alien

### Stratégie de combat
1. **Face à face avec l'alien** :
   - Passer au MP (touche E)
   - Viser l'alien (il doit être face à vous)
   - Tirer (Espace)
   - Si hit → Alien stunné 2 secondes
   - **Courir pour s'échapper** pendant le stun !

2. **Utilisation tactique** :
   - Créer de la distance quand l'alien charge
   - Tirer pour le ralentir
   - Se repositionner stratégiquement
   - Le stun permet de retrouver le contrôle de la situation

## 🔧 Paramètres ajustables

Dans `weapon_mp_hits_alien()` :
```c
double cos_threshold = cos(20.0 * M_PI / 180.0); /* Cone de tir */
double max_range = 400.0; /* Portée max */
```

Dans `alien_check_hit_by_mp()` :
```c
alien_apply_stun(cub, 2.0); /* Durée du stun */
```

Dans `weapon_draw()` :
```c
scale = 1.8;     /* Agrandissement MP */
offset_x = 30;   /* Décalage MP */
offset_x = 100;  /* Décalage Marteau */
```

## 📊 Performance

- Pas d'impact significatif sur les FPS
- Le scaling est fait en temps réel mais optimisé
- La détection de hit est légère (calcul géométrique simple)
- Le stun n'ajoute qu'une vérification boolean au début de `alien_update()`

## 🐛 Debug

Messages console disponibles :
- `[WEAPON] Alien stunned for X.Xs!` - Stun appliqué
- `[WEAPON] Alien stun ended, resuming normal behavior` - Fin du stun
- `[WEAPON] MP shot hit alien! Stunned for 2 seconds.` - Confirmation de hit
- `[WEAPON] MP hit detected! Alien stunned.` - Debug interne

---

**Date** : 20 octobre 2025
**Version** : Cub3D avec système d'armes FPS complet
