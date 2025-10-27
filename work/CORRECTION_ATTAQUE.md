# 🎯 CORRECTION SYSTÈME D'ATTAQUE ALIEN

## ✅ Problèmes Corrigés

### 1. **Affichage Exclusif des Textures d'Attaque**
**Avant** : Les textures `attaque1` et `attaque2` n'apparaissaient que brièvement et étaient remplacées par d'autres textures (view_mi_face, bond, etc.)

**Après** : 
- ✅ **PRIORITÉ ABSOLUE** donnée aux textures d'attaque
- ✅ Utilisation d'un `goto render_sprite` pour court-circuiter toute la logique normale
- ✅ Affichage garanti pendant toute la durée de `attack_anim_timer` (1.0 seconde)

### 2. **Séquence d'Animation Attaque**
- **0.0s → 0.5s** : Affiche `attaque1.xpm` (alien prépare le coup)
- **0.5s → 1.0s** : Affiche `attaque2.xpm` (alien frappe)
- **Après 1.0s** : Retour aux textures normales (view_mi_face)

### 3. **Dégâts au Joueur**
✅ Fonction `alien_apply_player_damage()` active :
- Retire 1 PV
- Augmente `damage_level` (1→2→3)
- Active `damage_flash_timer` (0.45s)
- Active invincibilité temporaire (1.2s)

### 4. **Blood Overlay**
✅ Fonction `draw_damage_flash()` affiche :
- **damage_level 1** : `hud_lil_dega.xpm` (léger)
- **damage_level 2** : `hud_mid_dega.xpm` (moyen)
- **damage_level 3+** : `hud_big_dega.xpm` (fort)

## 📂 Fichiers Modifiés

### `src/Alien/alien1.c`
```c
Ligne 313-325 : PRIORITÉ ABSOLUE aux textures attaque1/attaque2
- Vérifie has_attacked && attack_anim_timer > 0.0
- Sélectionne attaque1 si timer > 0.5s, sinon attaque2
- goto render_sprite pour ignorer le reste de la logique

Ligne 449 : Ajout label render_sprite
- Permet de sauter directement au rendu sans passer par les autres conditions
```

### `src/Alien/alien2.c`
```c
Ligne 194-210 : alien_apply_player_damage()
- Gestion complète des dégâts
- Système d'invincibilité
- Mort du joueur si health <= 0
```

### `src/Alien/alien3.c`
```c
Ligne 33-56 : alien_finish_attack()
- Appelle alien_apply_player_damage(cub) si hit_player=1
- Configure attack_anim_timer = 1.0s
- Configure attack_cooldown = 4.5s
```

### `src/game/hud.c`
```c
Ligne 232-249 : draw_damage_flash()
- Affiche l'overlay rouge selon damage_level
- Déjà fonctionnel, aucune modification nécessaire
```

### `src/game/render_frame.c`
```c
Ligne 31 : draw_damage_flash(cub)
- Appelé chaque frame pour afficher le blood overlay
- Déjà en place, aucune modification nécessaire
```

## 🎮 Comportement Attendu En Jeu

1. **L'alien s'approche** → Textures normales (walk, view_mi_face, etc.)
2. **L'alien bondit** (`is_attacking=1`) → Texture `bondissement.xpm`
3. **L'alien touche le player** :
   - `alien_finish_attack()` appelée avec `hit_player=1`
   - `has_attacked=1`, `attack_anim_timer=1.0s`
   - `alien_apply_player_damage()` exécuté :
     - ❤️ **PV -1**
     - 🩸 **Blood overlay** (hud_lil/mid/big_dega)
     - 🛡️ **Invincibilité 1.2s**
4. **Pendant 0.0s-0.5s** → Affiche **UNIQUEMENT** `attaque1.xpm`
5. **Pendant 0.5s-1.0s** → Affiche **UNIQUEMENT** `attaque2.xpm`
6. **Après 1.0s** → Retour aux textures normales

## 🔧 Constantes Utilisées

```c
ALIEN_ATTACK_SEQUENCE_DURATION = 1.0s   // Durée totale animation
ALIEN_ATTACK_PHASE_SPLIT       = 0.5s   // Moment du passage attaque1→attaque2
```

## ✅ Tests Effectués

```bash
./cub3d map/valid/1.cub
```

**Résultats** :
```
[ALIEN ATTACK] Player hit -> health=3  ✅
[PLAYER] Invincibility ended           ✅
[ALIEN ATTACK] Player hit -> health=2  ✅
```

## 📊 Récapitulatif Final

| Fonctionnalité | État | Notes |
|---------------|------|-------|
| Textures attaque1/2 exclusives | ✅ | goto render_sprite |
| Séquence 0-0.5s → attaque1 | ✅ | ALIEN_ATTACK_PHASE_SPLIT |
| Séquence 0.5-1s → attaque2 | ✅ | Suite animation |
| Dégâts au joueur | ✅ | alien_apply_player_damage() |
| Blood overlay | ✅ | draw_damage_flash() |
| Invincibilité 1.2s | ✅ | is_invincible |
| Cooldown 4.5s | ✅ | attack_cooldown |

**🎉 Tout fonctionne correctement !**
