# Système de Combat - Documentation

## 📋 Vue d'ensemble

Le système de combat a été intégré au jeu avec les fonctionnalités suivantes :

### ⚔️ Mécaniques implémentées

#### 1. **Système de vie du joueur (3 cœurs)**
- **Points de vie initiaux** : 3 cœurs
- **Hitbox joueur** : Rayon de 12 pixels (collision circulaire)
- **Invincibilité temporaire** : 1.5 secondes après chaque coup
- **État invincible** : Le joueur ne peut pas être blessé pendant ce temps

#### 2. **Système d'attaque de l'alien**
- **Hitbox alien** : Rayon de 24 pixels
- **Détection de collision** : Distance entre centres < (rayon alien + rayon joueur)
- **Dégâts** : 1 cœur par attaque
- **Feedback visuel** : Messages de debug dans stderr

#### 3. **Cooldown après attaque (6 secondes)**
- Après avoir touché le joueur, l'alien :
  1. **Se retire immédiatement** vers une position éloignée (4.5 blocs dans la direction opposée)
  2. **Ignore complètement le joueur** pendant 6 secondes
  3. **Se déplace vers sa position de retraite** à vitesse de patrouille
  4. **Retourne en mode PATROL** après le cooldown

#### 4. **Réduction du timer de traque**
- **Avant** : 5 secondes de mémorisation en mode STALKING
- **Maintenant** : 3.3 secondes de mémorisation
- Plus dynamique et réactif

---

## 📊 Structures de données

### Ajouts à `t_player` (cub3d.h)
```c
int     health;                 /* points de vie (3 cœurs max) */
int     max_health;             /* vie maximale (3) */
double  hitbox_radius;          /* rayon de la hitbox circulaire (12.0) */
double  invincibility_timer;    /* temps d'invincibilité restant */
int     is_invincible;          /* flag d'invincibilité */
```

### Ajouts à `t_alien` (cub3d.h)
```c
int      has_attacked;      /* flag d'attaque récente */
double   attack_cooldown;   /* timer de cooldown (6.0s) */
double   retreat_x;         /* position X de retraite */
double   retreat_y;         /* position Y de retraite */
double   hitbox_radius;     /* rayon de la hitbox (24.0) */
```

---

## 🎮 Comportement de l'alien

### Priorités des états (ordre décroissant)
1. **COOLDOWN** (après attaque) - 6 secondes
   - Ignore tout le reste
   - Retraite vers position calculée
   - Retour en PATROL après expiration
   
2. **CHASE** (joueur visible dans FOV et ligne de vue)
   - Poursuite directe à vitesse élevée (180 px/sec)
   - Détection de collision pour attaque
   
3. **STALKING** (joueur à 2-3 blocs, caché derrière mur)
   - Vitesse réduite (25% de la patrouille)
   - Mémorisation 3.3 secondes
   - Contournement intelligent des obstacles
   
4. **PREPARING** (joueur < 3 blocs, hors FOV, ligne de vue)
   - Rotation lente (30°/sec)
   - Bond après 2 secondes si conditions maintenues
   
5. **SEARCH** (vient de perdre la trace)
   - Retourne vers dernière position connue
   
6. **PATROL** (mode par défaut)
   - Patrouille avec détection de rebonds intelligents

---

## 🔧 Fichiers modifiés

### Headers
- `includes/cub3d.h` : Ajout des champs de combat (player + alien)

### Sources
- `src/player/player_init.c` : Initialisation des stats de combat du joueur
- `src/player/player_move.c` : Gestion du timer d'invincibilité
- `src/Alien/alien.c` : 
  - Initialisation des champs d'attaque
  - Logique de collision/dégâts
  - Système de cooldown/retraite
  - Réduction timer STALKING (5s → 3.3s)

---

## 🚀 Prochaines étapes suggérées

### Interface utilisateur
- [x] Afficher les cœurs à l'écran (barres de vie avec 4 états)
- [x] Animation de flash lors d'une attaque (3 niveaux progressifs)
- [x] Effet visuel lors d'une attaque (lil/mid/big_dega)
- [x] Barre de santé avec progression de couleurs (vert→jaune→orange→rouge)

### Écran Game Over
- [x] Détection de la mort (health <= 0)
- [x] Affichage d'un écran "Game Over"
- [x] Option de relancer automatiquement (10 secondes)
- [x] Restart complet avec téléportation au spawn

### Équilibrage
- [ ] Ajuster les hitbox selon les tests
- [ ] Modifier le cooldown (actuellement 6s)
- [ ] Ajuster l'invincibilité (actuellement 1.5s)
- [ ] Tester avec plusieurs aliens

### Améliorations possibles
- [ ] Système de récupération de vie (items/power-ups)
- [ ] Arme de défense pour le joueur
- [ ] Différents types d'aliens avec stats variées
- [ ] Système de score/points

---

## 🐛 Tests à effectuer

1. **Test de collision**
   - Vérifier que l'alien touche le joueur à la bonne distance
   - Confirmer que le cooldown se déclenche

2. **Test d'invincibilité**
   - Vérifier les 1.5 secondes d'immunité
   - Tester plusieurs attaques rapides

3. **Test de retraite**
   - Observer le comportement de fuite de l'alien
   - Vérifier qu'il ignore le joueur pendant 6 secondes

4. **Test de mort**
   - Perdre les 3 cœurs
   - Vérifier le message dans stderr

5. **Test de traque**
   - Vérifier le nouveau timer de 3.3s en mode STALKING

---

## 📝 Notes techniques

### Delta Time (dt)
Le système utilise `dt` (delta time) pour la cohérence temporelle :
- Cooldown alien : `attack_cooldown -= dt`
- Invincibilité : basée sur approximation 60 FPS (`-= 1.0/60.0`)
- Timers de préparation/traque : incrément par `dt`

### Calcul de collision
```c
double distance = sqrt(dx*dx + dy*dy);
double collision_threshold = alien_hitbox + player_hitbox;
if (distance < collision_threshold) { /* COLLISION */ }
```

### Position de retraite
```c
double retreat_angle = atan2(-dy, -dx);  // Opposé au joueur
retreat_x = alien_x + cos(angle) * (BLOCK * 4.5);
retreat_y = alien_y + sin(angle) * (BLOCK * 4.5);
```

---

**Statut** : ✅ Compilé et prêt pour les tests
**Version** : 2.0.0 - HUD Interface Complete
**Date** : 7 octobre 2025

---

## 🎨 MISE À JOUR MAJEURE : Interface HUD complète

### Nouvelles fonctionnalités visuelles (v2.0.0)

#### ✅ Barre de vie dynamique
- 4 états visuels avec progression de couleurs
- Position : Haut gauche (overlay permanent)
- Transparence : Fond vert automatiquement retiré

#### ✅ Système de flash de dégâts
- 3 niveaux progressifs : lil → mid → big
- Durée : 0.3 secondes par flash
- Augmentation de l'intensité selon dégâts cumulés

#### ✅ Écran Game Over
- Asset dédié centré à l'écran
- Compteur de restart : 10 secondes
- Restart automatique avec téléportation au spawn

#### ✅ Assets HUD intégrés
```
textures/HUD/
├── Over_full_hp.xpm    ✅ (4 HP - barre verte)
├── Over_less3_hp.xpm   ✅ (3 HP - barre jaune)
├── Over_less2_hp.xpm   ✅ (2 HP - barre orange)
├── Over_less1_hp.xpm   ✅ (1 HP - barre rouge)
├── lil_dega.xpm        ✅ (1er hit - petit flash)
├── mid_dega.xpm        ✅ (2ème hit - moyen flash)
├── Big_dega.xpm        ✅ (3ème hit - grand flash)
└── game_over.xpm       ✅ (écran de mort)
```

### Fichier dédié
Voir **HUD_IMPLEMENTATION.md** pour documentation technique complète.
