# Améliorations Avancées du Système d'Armes v2.0

## ✅ Nouvelles fonctionnalités implémentées

### 🎯 **1. Marteau décalé vers la droite**

**Changement** : Le marteau est maintenant positionné comme s'il était tenu dans la main droite

**Détails** :
- **Offset** : +200 pixels vers la droite (contre 100 avant)
- Donne l'impression que le joueur tient l'arme dans sa main droite
- Plus immersif et naturel
- Libère l'espace central pour une meilleure visibilité

**Code** : `weapon_draw()` dans `src/player/weapon.c`
```c
else /* Hammer */
{
    offset_x = 200; /* Décalage PLUS à droite pour effet main droite */
}
```

---

### 🏃 **2. Effet de balancement pendant la course**

**Feature** : Les armes basculent de gauche à droite quand le joueur se déplace

**Comportement** :
- **Déclenchement** : Dès que le joueur bouge (WASD)
- **Amplitude** : ±15 pixels de balancement
- **Vitesse normale** : 8 Hz (marche)
- **Vitesse rapide** : 12 Hz (course avec Shift)
- **Retour au centre** : Lissage à 85% quand le joueur s'arrête

**Formule** : Oscillation sinusoïdale fluide
```c
p->weapon_bob_offset = sin(p->weapon_bob_timer) * 15.0;
```

**Effet** :
- Immersion accrue
- Feedback visuel du mouvement
- Similaire aux FPS modernes
- S'applique aux deux armes (marteau + MP)

---

### 🎯 **3. Système de visée précis pour le MP**

**Amélioration** : Détection de hit beaucoup plus réaliste et exigeante

**Avant** :
- Cone de 20°
- Portée 400px
- Détection basique (dot product)

**Maintenant** :
- **Cone ultra-précis** : 10° (2x plus étroit)
- **Portée augmentée** : 500 pixels
- **Double vérification** :
  1. Angle avec la direction du joueur (cos > 0.985)
  2. Distance perpendiculaire au rayon < 50px
- **Résultat** : Le joueur doit vraiment viser l'alien

**Code** : `weapon_mp_hits_alien()` dans `src/player/weapon.c`
```c
double cos_threshold = cos(10.0 * M_PI / 180.0); /* 10° */
double perp_dist = fabs(dx * to_alien_y - dy * to_alien_x) * dist;
if (perp_dist < 50.0) /* Alien doit être dans un rayon de 50px */
```

**Impact gameplay** :
- ✅ Plus skill-based
- ✅ Récompense la précision
- ✅ Pas de "easy mode" - il faut vraiment aligner le tir
- ✅ Plus satisfaisant quand on touche

---

### 🩸 **4. Effet de sang animé**

**Feature** : Sprite animé de sang qui s'affiche sur l'alien quand il est touché

**Animation** :
- **6 frames** : blood_alien1.xpm à blood_alien6.xpm
- **Durée** : 0.48 secondes (0.08s par frame)
- **Affichage** : Par-dessus le sprite de l'alien
- **Fond** : Transparent (géré automatiquement)

**Timeline** :
```
Frame 1 (0.00s) → Impact initial
Frame 2 (0.08s) → Expansion
Frame 3 (0.16s) → Maximum
Frame 4 (0.24s) → Début fade
Frame 5 (0.32s) → Fade out
Frame 6 (0.40s) → Presque invisible
[0.48s]         → Fin de l'effet
```

**Déclenchement** :
- Automatique quand le MP touche l'alien
- Synchronisé avec le stun (2 secondes)
- Un seul effet à la fois (pas de spam)

**Code** : `alien_apply_stun()` dans `src/Alien/alien3.c`
```c
a->show_blood = 1;
a->blood_timer = 0.0;
a->blood_frame = 0;
```

**Rendu** : `draw_alien_sprite()` dans `src/Alien/alien1.c`
```c
if (a->show_blood && a->blood_frame >= 0 && a->blood_frame < 6)
{
    draw_alien_sprite_texture_with(cub, blood_tex, ...);
}
```

---

## 📊 Structures de données modifiées

### `t_player` (includes/cub3d.h)
```c
/* Nouveaux champs */
double  weapon_bob_timer;       /* Timer pour oscillation */
double  weapon_bob_offset;      /* Offset actuel (-15 à +15) */
```

### `t_alien` (includes/cub3d.h)
```c
/* Nouveaux champs */
int      show_blood;         /* 1 si effet de sang actif */
double   blood_timer;        /* Timer animation sang */
int      blood_frame;        /* Frame actuelle (0-5) */
```

### `t_cub` (includes/cub3d.h)
```c
/* Nouvelles textures */
t_tur blood_alien[6];   /* blood_alien1.xpm à blood_alien6.xpm */
```

---

## 🎮 Gameplay Impact

### Avant vs Après

| Aspect | Avant | Après |
|--------|-------|-------|
| **Visée MP** | Large (20°, facile) | Précise (10°, skill-based) |
| **Feedback hit** | Stun uniquement | Stun + effet de sang |
| **Immersion course** | Statique | Balancement réaliste |
| **Position marteau** | Centre | Main droite (décalé) |
| **Portée MP** | 400px | 500px |
| **Difficulté** | Easy mode | Réaliste/exigeante |

---

## 🔧 Fichiers modifiés

### Code principal
1. **includes/cub3d.h**
   - Ajout `weapon_bob_timer`, `weapon_bob_offset` dans `t_player`
   - Ajout `show_blood`, `blood_timer`, `blood_frame` dans `t_alien`
   - Ajout `blood_alien[6]` dans `t_cub`

2. **src/player/weapon.c**
   - `weapon_init()` : Init bobbing
   - `weapon_update()` : Calcul balancement pendant mouvement
   - `weapon_mp_hits_alien()` : Détection précise avec double check
   - `weapon_draw()` : Application bobbing + offset marteau augmenté

3. **src/Alien/alien3.c**
   - `alien_apply_stun()` : Active l'effet de sang

4. **src/Alien/alien.c**
   - `alien_update()` : Anime les frames de sang
   - `alien_init()` : Init champs sang à 0

5. **src/Alien/alien1.c**
   - `draw_alien_sprite()` : Affiche sang par-dessus l'alien

6. **src/textures/textures.c**
   - Chargement optionnel des 6 textures de sang
   - Fallback graceful si textures absentes

---

## ⚠️ Textures de sang requises

**Status** : Non implémentées (code prêt, assets manquants)

**Fichiers nécessaires** :
```
textures/alien/blood_alien1.xpm
textures/alien/blood_alien2.xpm
textures/alien/blood_alien3.xpm
textures/alien/blood_alien4.xpm
textures/alien/blood_alien5.xpm
textures/alien/blood_alien6.xpm
```

**Solution temporaire** :
```bash
# Créer des placeholders (le jeu ne crashera pas)
cd textures/alien
for i in {1..6}; do
    cp alien_front.xpm blood_alien$i.xpm
done
```

Le jeu affichera un warning mais continuera de fonctionner :
```
[WARNING] Blood textures not found, blood effects disabled
```

Voir `BLOOD_TEXTURES_NEEDED.md` pour plus de détails.

---

## 🎯 Comment tester

### Effet de balancement
1. Lancer le jeu
2. Appuyer sur WASD pour bouger
3. **Observer** : L'arme oscille de gauche à droite
4. Appuyer sur Shift : L'oscillation s'accélère
5. S'arrêter : L'arme retourne doucement au centre

### Marteau décalé
1. Équiper le marteau (touche E si nécessaire)
2. **Observer** : Le marteau est nettement à droite
3. Effet "main droite" visible

### Visée précise MP
1. Équiper le MP (touche E)
2. Chercher l'alien
3. **Essayer de tirer sans bien viser** : Ne touche pas
4. **Viser précisément l'alien** : Touche !
5. Console affiche : `[WEAPON] MP hit detected! Alien stunned.`

### Effet de sang (si textures présentes)
1. Tirer sur l'alien avec le MP
2. **Observer** : Animation de sang sur l'alien (0.48s)
3. Disparaît automatiquement

---

## 📈 Paramètres ajustables

### Balancement d'arme
```c
// src/player/weapon.c - weapon_update()
double bob_speed = p->is_running ? 12.0 : 8.0;  /* Vitesse oscillation */
p->weapon_bob_offset = sin(...) * 15.0;         /* Amplitude */
```

### Visée MP
```c
// src/player/weapon.c - weapon_mp_hits_alien()
double max_range = 500.0;                       /* Portée */
double cos_threshold = cos(10.0 * M_PI / 180); /* Angle */
double max_perp_dist = 50.0;                    /* Précision */
```

### Effet de sang
```c
// src/Alien/alien.c - alien_update()
double blood_frame_duration = 0.08;  /* Vitesse animation */
if (a->blood_frame >= 6)             /* Nombre de frames */
```

### Position marteau
```c
// src/player/weapon.c - weapon_draw()
offset_x = 200;  /* Décalage droite (100-300 recommandé) */
```

---

## 🏆 Résumé des améliorations

✅ **Marteau** : Décalé +200px (effet main droite)  
✅ **Balancement** : ±15px oscillation pendant course  
✅ **Visée MP** : Cone 10° + check perpendiculaire (ultra-précis)  
✅ **Effet sang** : Animation 6 frames sur hit (optionnel)  
✅ **Immersion** : Gameplay plus réaliste et skill-based  

**Le jeu est maintenant compilé et jouable** ! 🎮

---

**Date** : 20 octobre 2025  
**Version** : Cub3D Advanced Weapon System v2.0
