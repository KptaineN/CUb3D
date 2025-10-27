# Textures de Sang Requises

## ⚠️ Fichiers manquants

Le système d'effet de sang est implémenté mais nécessite les fichiers suivants :

```
textures/alien/blood_alien1.xpm
textures/alien/blood_alien2.xpm
textures/alien/blood_alien3.xpm
textures/alien/blood_alien4.xpm
textures/alien/blood_alien5.xpm
textures/alien/blood_alien6.xpm
```

## 📋 Spécifications

### Format
- **Type** : XPM (X PixMap)
- **Fond** : Transparent (sera traité automatiquement par le système alien)
- **Frames** : 6 images pour créer une animation fluide
- **Durée** : 0.48 secondes au total (0.08s par frame)

### Contenu recommandé
- **Frame 1** : Petit impact initial (éclaboussure légère)
- **Frame 2** : Expansion de l'éclaboussure
- **Frame 3** : Maximum de l'effet (le plus visible)
- **Frame 4** : Début de disparition
- **Frame 5** : Fade out progressif
- **Frame 6** : Presque invisible

### Style artistique
- Couleur rouge sang (variations possibles : rouge foncé, rouge vif)
- Style cohérent avec les textures alien existantes
- Peut inclure des gouttes/particules pour plus de réalisme
- Doit être visible mais pas trop dominant

## 🔧 Solution temporaire

En attendant les vraies textures, vous pouvez :

1. **Copier une texture existante** (temporaire) :
```bash
cd textures/alien
for i in {1..6}; do
    cp alien_front.xpm blood_alien$i.xpm
done
```

2. **Créer des textures de test** avec un outil comme GIMP :
   - Créer une image 64x64 ou 128x128
   - Fond transparent
   - Dessiner une éclaboussure rouge
   - Exporter en XPM
   - Répéter avec variations pour les 6 frames

3. **Désactiver temporairement** (si problèmes de chargement) :
   - Commenter les lignes de chargement dans `src/textures/textures.c`
   - L'effet de sang ne s'affichera pas mais le jeu fonctionnera

## 🎮 Fonctionnement actuel

Une fois les textures en place :
- Quand le MP touche l'alien → Effet de sang automatique
- Animation de 6 frames sur 0.48 secondes
- S'affiche par-dessus le sprite de l'alien
- Disparaît automatiquement après l'animation

## 📝 Notes

Le système est entièrement implémenté et attend uniquement les assets graphiques.
Le code gère automatiquement :
- Le chargement des 6 textures
- L'animation temporisée
- L'affichage par-dessus l'alien
- La transparence du fond

---

**Date** : 20 octobre 2025
