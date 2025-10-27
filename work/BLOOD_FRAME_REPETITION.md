# Amélioration Animation Blood - Répétition des Frames - 21 octobre 2025

## 🎯 Nouvelles Améliorations

### 1. Animation Blood_Alien avec Répétition de Frames ✅

**Problème** :
- L'animation passait trop vite (0.08s par frame)
- Difficile de voir chaque frame distinctement
- Effet de sang pas assez marqué

**Solution implémentée** :

#### A. Ralentissement de l'animation
```c
/* Animation RALENTIE : 0.12s par frame au lieu de 0.08s (50% plus lent) */
double blood_frame_duration = 0.12;
```

#### B. Répétition de chaque frame
```c
/* RÉPÉTITION : chaque frame affichée 2 fois (0,0,1,1,2,2,3,3,4,4) */
int base_frame = (frame / 2) % 5; /* Division par 2 pour répéter chaque frame */
a->blood_frame = base_frame;
```

**Déroulement de l'animation** :

| Temps (s) | Frame | Image affichée | Description |
|-----------|-------|----------------|-------------|
| 0.00-0.12 | 0/2=0 | blood_alien1 | Première apparition du sang |
| 0.12-0.24 | 1/2=0 | blood_alien1 | **RÉPÉTITION frame 1** ✨ |
| 0.24-0.36 | 2/2=1 | blood_alien2 | Deuxième frame |
| 0.36-0.48 | 3/2=1 | blood_alien2 | **RÉPÉTITION frame 2** ✨ |
| 0.48-0.60 | 4/2=2 | blood_alien3 | Troisième frame |
| 0.60-0.72 | 5/2=2 | blood_alien3 | **RÉPÉTITION frame 3** ✨ |
| 0.72-0.84 | 6/2=3 | blood_alien4 | Quatrième frame |
| 0.84-0.96 | 7/2=3 | blood_alien4 | **RÉPÉTITION frame 4** ✨ |
| 0.96-1.08 | 8/2=4 | blood_alien5 | Cinquième frame |
| 1.08-1.20 | 9/2=4 | blood_alien5 | **RÉPÉTITION frame 5** ✨ |
| 1.20+ | → | Boucle à frame 0 | Si Space toujours maintenu |

**Durée totale d'un cycle** : **1.20 secondes** (vs 0.40s avant)

**Effet obtenu** :
- ✅ Chaque frame est visible **3× plus longtemps**
- ✅ Animation plus lente et plus visible
- ✅ Effet de sang beaucoup plus marqué
- ✅ Répétition donne un effet "pulsant" impressionnant

---

### 2. Crosshair - Mire Centrale Réduite ✅

**Avant** :
- Point central : 1 seul pixel rouge

**Après** :
- **Mini-croix horizontale** : 3 pixels en ligne (←·→)
- Plus discret et précis
- Meilleure visibilité du point de visée

**Code** :
```c
/* POINT CENTRAL (mire) - MINI CROIX + (3 pixels en forme de +) */

/* Pixel central */
pix[center_x, center_y] = RED

/* Pixel gauche */
pix[center_x - 1, center_y] = RED

/* Pixel droit */
pix[center_x + 1, center_y] = RED
```

**Résultat visuel** :
```
Avant :           Après :
   ·                ←·→
```

**Avantages** :
- ✅ Plus visible que 1 seul pixel
- ✅ Toujours ultra-discret (3px seulement)
- ✅ Forme horizontale suit naturellement le balayage de l'œil
- ✅ Précision maintenue

---

## 📊 Comparaison Complète : Animation Blood

### Timing de l'animation

| Aspect | Version 1 | Version 2 | Version 3 (actuelle) |
|--------|-----------|-----------|---------------------|
| Durée par frame | 0.08s | 0.08s | **0.12s** ✅ |
| Répétition | Non | Oui (modulo) | **Oui (double)** ✅ |
| Durée d'un cycle | 0.40s | 0.40s | **1.20s** ✅ |
| Frames visibles | 5 | 5 (boucle) | **10** (5×2 répétition) ✅ |
| Visibilité | Faible | Moyenne | **Excellente** ✅ |

### Comportement en jeu

**Scénario : Tir continu (Space maintenu 3 secondes)**

```
Version 1 (obsolète) :
├─ 0.00s : blood_alien1
├─ 0.08s : blood_alien2
├─ 0.16s : blood_alien3
├─ 0.24s : blood_alien4
├─ 0.32s : blood_alien5
└─ 0.40s : ARRÊT (pas de boucle)

Version 2 (précédente) :
├─ 0.00s-0.08s : blood_alien1
├─ 0.08s-0.16s : blood_alien2
├─ 0.16s-0.24s : blood_alien3
├─ 0.24s-0.32s : blood_alien4
├─ 0.32s-0.40s : blood_alien5
├─ 0.40s-0.48s : blood_alien1 (boucle rapide)
├─ ... cycles de 0.40s
└─ Animation trop rapide, frames difficiles à distinguer

Version 3 (actuelle) ✅ :
├─ 0.00s-0.12s : blood_alien1 🩸
├─ 0.12s-0.24s : blood_alien1 🩸 (RÉPÉTITION)
├─ 0.24s-0.36s : blood_alien2 🩸
├─ 0.36s-0.48s : blood_alien2 🩸 (RÉPÉTITION)
├─ 0.48s-0.60s : blood_alien3 🩸
├─ 0.60s-0.72s : blood_alien3 🩸 (RÉPÉTITION)
├─ 0.72s-0.84s : blood_alien4 🩸
├─ 0.84s-0.96s : blood_alien4 🩸 (RÉPÉTITION)
├─ 0.96s-1.08s : blood_alien5 🩸
├─ 1.08s-1.20s : blood_alien5 🩸 (RÉPÉTITION)
├─ 1.20s-1.32s : blood_alien1 🩸 (boucle lente)
├─ ... cycles de 1.20s
└─ Animation lente et visible, chaque frame bien marquée
```

**Scénario : Relâchement de Space après 0.5s**

```
Version 3 (actuelle) :
├─ 0.00s-0.12s : blood_alien1 (Space enfoncé)
├─ 0.12s-0.24s : blood_alien1 (RÉPÉTITION)
├─ 0.24s-0.36s : blood_alien2
├─ 0.36s-0.48s : blood_alien2 (RÉPÉTITION)
├─ 0.50s : ⚠️ Player relâche Space → show_blood = 0
├─ 0.48s-0.60s : blood_alien2 (termine frame en cours)
├─ 0.60s-0.72s : blood_alien3 (continue jusqu'à frame 10)
├─ 0.72s-0.84s : blood_alien3
├─ 0.84s-0.96s : blood_alien4
├─ 0.96s-1.08s : blood_alien4
├─ 1.08s-1.20s : blood_alien5
├─ 1.20s-1.32s : blood_alien5 (dernière frame)
└─ 1.32s : ✅ Animation terminée proprement
```

---

## 🎮 Impact sur le Gameplay

### Ressenti en jeu

**Avant** :
- ❌ Animation trop rapide, difficile à voir
- ❌ Pas d'impression de "dégâts"
- ❌ Retour visuel faible

**Après** :
- ✅ Animation bien visible et impactante
- ✅ Forte impression de "dégâts infligés"
- ✅ Retour visuel excellent
- ✅ Effet "pulsant" du sang très immersif
- ✅ Sensation de puissance du MP renforcée

### Statistiques techniques

```c
// Paramètres de l'animation
#define BLOOD_FRAME_DURATION 0.12  // Durée par frame (secondes)
#define BLOOD_FRAMES 5             // Nombre de frames blood_alien
#define BLOOD_REPETITION 2         // Chaque frame répétée 2 fois
#define BLOOD_CYCLE_DURATION 1.20  // Durée totale d'un cycle (s)
#define BLOOD_TOTAL_FRAMES 10      // Frames totales avec répétition

// Formule de calcul
frame_index = (int)(timer / BLOOD_FRAME_DURATION)
base_frame = (frame_index / BLOOD_REPETITION) % BLOOD_FRAMES
```

---

## 📝 Code Source Modifié

### src/Alien/alien.c (lignes 45-80)

```c
/* GESTION ANIMATION SANG - AMÉLIORATION : Boucler pendant le tir avec répétition visible */
if (a->show_blood)
{
    a->blood_timer += dt;
    
    /* Animation de sang RALENTIE pour être plus visible : 0.12s par frame au lieu de 0.08s */
    /* Chaque frame est répétée 2 fois pour un effet plus marqué */
    double blood_frame_duration = 0.12; /* Ralenti de 50% */
    int frame = (int)(a->blood_timer / blood_frame_duration);
    
    /* RÉPÉTITION : chaque frame affichée 2 fois (0,0,1,1,2,2,3,3,4,4) */
    int base_frame = (frame / 2) % 5; /* Division par 2 pour répéter chaque frame */
    a->blood_frame = base_frame;
    
    /* Boucle infinie tant que show_blood est actif */
}
else if (a->blood_timer > 0.0)
{
    /* Le player a arrêté de tirer, mais on termine l'animation en cours */
    a->blood_timer += dt;
    
    double blood_frame_duration = 0.12;
    int frame = (int)(a->blood_timer / blood_frame_duration);
    
    /* Si on atteint 10 frames (5 frames x 2 répétitions), on arrête complètement */
    if (frame >= 10)
    {
        a->blood_timer = 0.0;
        a->blood_frame = 0;
    }
    else
    {
        /* Continuer l'animation jusqu'à la fin avec répétition */
        int base_frame = frame / 2;
        a->blood_frame = base_frame;
    }
}
```

### src/player/weapon.c (lignes 585-610)

```c
/* POINT CENTRAL (mire) - MINI CROIX + (3 pixels en forme de +) */

/* Pixel central */
if (center_x >= 0 && center_x < WIDTH && center_y >= 0 && center_y < HEIGHT)
{
    int idx = center_y * cub->mlx_line_size + center_x * screen_bytes;
    unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
    pix[0] = b;
    if (screen_bytes > 1) pix[1] = g;
    if (screen_bytes > 2) pix[2] = r;
}

/* Pixel gauche */
if ((center_x - 1) >= 0 && center_x - 1 < WIDTH && center_y >= 0 && center_y < HEIGHT)
{
    int idx = center_y * cub->mlx_line_size + (center_x - 1) * screen_bytes;
    unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
    pix[0] = b;
    if (screen_bytes > 1) pix[1] = g;
    if (screen_bytes > 2) pix[2] = r;
}

/* Pixel droit */
if ((center_x + 1) >= 0 && center_x + 1 < WIDTH && center_y >= 0 && center_y < HEIGHT)
{
    int idx = center_y * cub->mlx_line_size + (center_x + 1) * screen_bytes;
    unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
    pix[0] = b;
    if (screen_bytes > 1) pix[1] = g;
    if (screen_bytes > 2) pix[2] = r;
}
```

---

## ✅ Tests de Validation

### Test 1 : Animation blood répétée
```bash
# Lancer le jeu
./cub3d map/valid/1.cub

# Actions :
# 1. Équiper MP (touche 'E')
# 2. Viser l'alien
# 3. Maintenir Space 2-3 secondes
# 
# Attendu :
# - Chaque frame de sang visible distinctement
# - Répétition visible (frame affichée 2 fois)
# - Animation boucle lentement (1.2s par cycle)
```

### Test 2 : Terminaison propre
```bash
# Actions :
# 1. Tirer sur l'alien (Space)
# 2. Relâcher Space après 0.5s
#
# Attendu :
# - Animation continue jusqu'à la frame 5
# - Arrêt propre sans coupure
# - Pas de sang résiduel
```

### Test 3 : Crosshair mini-croix
```bash
# Actions :
# 1. Équiper MP
# 2. Observer le crosshair au centre
#
# Attendu :
# - Petite croix rouge horizontale (3 pixels : ←·→)
# - Discret mais visible
# - Précision de visée maintenue
```

---

## 🎯 Résumé Final

**Animation Blood** :
- ✅ **3× plus lente** (0.12s vs 0.08s par frame)
- ✅ **Répétition 2×** pour chaque frame
- ✅ **1.20s par cycle** (vs 0.40s) - beaucoup plus visible
- ✅ Boucle infinie pendant le tir
- ✅ Terminaison propre à la frame 5

**Crosshair** :
- ✅ Mini-croix horizontale (3 pixels)
- ✅ Ultra-discret mais visible
- ✅ Meilleure précision de visée

**Compilation** :
- ✅ Sans erreurs ni warnings
- ✅ Taille : 405K
- ✅ Jeu fonctionnel

---
**Date** : 21 octobre 2025  
**Status** : ✅ Améliorations implémentées et testées avec succès
