# 🔄 SYSTÈME ANTI-IDLE POUR L'ALIEN

## 🎯 Objectif
Empêcher l'alien de rester bloqué à ne rien faire et forcer son retour en mode patrouille automatiquement.

## �� Problème Résolu
L'alien pouvait rester coincé dans certains états (SEARCH, ATTACK, etc.) sans action, créant une situation d'inactivité totale.

## 🔧 Solution Implémentée

### 1. **Nouveau Timer d'Inactivité**
Ajout de 2 nouveaux champs dans `t_alien` (includes/cub3d.h) :

```c
double   idle_timer;        /* compteur d'inactivité totale */
int      idle_state_check;  /* dernier état vérifié pour idle */
```

### 2. **Détection d'Inactivité**
Dans `alien_update()` (src/Alien/alien.c), ajout d'une logique qui vérifie si l'alien fait quelque chose :

**Conditions considérées comme "actives"** :
- ✅ `is_attacking` (en train d'attaquer)
- ✅ `is_preparing` (préparation d'attaque)
- ✅ `is_stalking` (traque du joueur)
- ✅ `has_attacked` (vient d'attaquer)
- ✅ `player_visible` (joueur visible)
- ✅ `player_audible` (joueur audible)
- ✅ `state == ALIEN_CHASE` (en poursuite)

**Si aucune condition n'est remplie** :
```c
a->idle_timer += dt;  // Incrémente le compteur d'inactivité

if (a->idle_timer > 3.0)  // Après 3 secondes d'inactivité
{
    a->state = ALIEN_PATROL;          // Force retour en patrouille
    a->state_timer = 0.0;
    a->idle_timer = 0.0;
    alien_reset_path(a);
    alien_choose_patrol_target(cub, a);
    a->is_crouching = 0;
    a->crouch_timer = 0.0;
}
```

### 3. **Timeout SEARCH**
Ajout d'une sécurité pour l'état SEARCH qui pourrait durer trop longtemps :

```c
/* Si en SEARCH depuis trop longtemps sans trouver le joueur */
if (a->state_timer <= -5.0)
{
    a->state = ALIEN_PATROL;
    a->state_timer = 0.0;
    alien_reset_path(a);
    alien_choose_patrol_target(cub, a);
}
```

## ⏱️ Durées et Seuils

| Paramètre | Valeur | Description |
|-----------|--------|-------------|
| **Seuil idle** | 3.0s | Temps d'inactivité avant retour forcé |
| **Timeout SEARCH** | 5.0s | Temps max en SEARCH avant abandon |
| **Reset flags** | Immédiat | Réinitialise crouch/timers lors du retour |

## 🎮 Comportement En Jeu

### Scénario 1: Alien Bloqué en SEARCH
```
1. Alien perd le joueur → passe en SEARCH
2. Cherche pendant 3 secondes sans action
3. idle_timer atteint 3.0s
4. [ALIEN ANTI-IDLE] Forcing return to PATROL
5. Alien reprend sa patrouille normale
```

### Scénario 2: Alien Bloqué Après Attaque
```
1. Alien attaque le joueur
2. has_attacked = 1, puis timer expire
3. Aucune détection de joueur pendant 3s
4. idle_timer > 3.0s
5. Retour forcé en PATROL
```

### Scénario 3: SEARCH Trop Long
```
1. Alien en mode SEARCH
2. state_timer décrémente jusqu'à -5.0s
3. [ALIEN SEARCH TIMEOUT] Returning to PATROL
4. Retour forcé en PATROL
```

## 📊 Messages de Debug

Le système affiche des messages dans la console :

```bash
[ALIEN ANTI-IDLE] Forcing return to PATROL (was in state 3, idle 3.1s)
[ALIEN SEARCH TIMEOUT] Returning to PATROL after 5.2fs
```

## �� Fichiers Modifiés

### `includes/cub3d.h`
```diff
+ double   idle_timer;        /* compteur d'inactivité totale */
+ int      idle_state_check;  /* dernier état vérifié pour idle */
```

### `src/Alien/alien.c`
```diff
Ligne 507-508: Initialisation
+ a->idle_timer = 0.0;
+ a->idle_state_check = ALIEN_PATROL;

Ligne 82-108: Logique anti-idle
+ /* SYSTÈME ANTI-IDLE */
+ int is_doing_something = (...);
+ if (!is_doing_something && a->state != ALIEN_PATROL) {
+     a->idle_timer += dt;
+     if (a->idle_timer > 3.0) {
+         // Force retour PATROL
+     }
+ }

Ligne 320-327: Timeout SEARCH
+ if (a->state_timer <= -5.0) {
+     // Force retour PATROL
+ }
```

## ✅ Avantages

1. **Robustesse** : L'alien ne reste jamais complètement inactif
2. **Gameplay fluide** : Retour automatique en patrouille
3. **Débug facile** : Messages console clairs
4. **Configurable** : Seuils ajustables (3.0s idle, 5.0s search)
5. **Non-intrusif** : N'affecte pas le comportement normal

## 🎯 Résultat

✅ L'alien reprend automatiquement sa patrouille s'il reste inactif
✅ Évite les situations de blocage permanent
✅ Maintient un gameplay dynamique et réactif
✅ Compatible avec tous les états existants (CHASE, SEARCH, ATTACK, etc.)

## 🧪 Test Recommandé

```bash
./cub3d map/valid/1.cub

# Scénarios à tester :
1. Fuir l'alien après qu'il vous voie → Vérifier retour en PATROL après 3s
2. Attendre dans une zone éloignée → L'alien devrait patrouiller
3. Observer les messages console pour confirmer les timeouts
```

**🎉 Système Anti-Idle Opérationnel !**
