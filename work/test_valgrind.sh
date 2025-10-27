#!/bin/bash

# Script de test Valgrind pour cub3d
# Utilise les suppressions pour masquer les erreurs connues de MLX/X11

echo "🔍 Test Valgrind - Détection des fuites mémoire..."
echo "================================================="

if [ $# -eq 0 ]; then
    echo "Usage: $0 <map_file>"
    echo "Exemple: $0 map/valid/10.cub"
    exit 1
fi

MAP_FILE="$1"

if [ ! -f "$MAP_FILE" ]; then
    echo "❌ Erreur: Le fichier $MAP_FILE n'existe pas"
    exit 1
fi

echo "🗺️  Carte testée: $MAP_FILE"
echo "📊 Exécution de Valgrind..."
echo ""

# Exécution avec suppressions
valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --suppressions=.valgrind_suppressions \
    --error-exitcode=1 \
    ./cub3d "$MAP_FILE"

VALGRIND_EXIT_CODE=$?

echo ""
echo "================================================="

if [ $VALGRIND_EXIT_CODE -eq 0 ]; then
    echo "✅ Test Valgrind RÉUSSI - Aucune fuite de mémoire détectée!"
else
    echo "❌ Test Valgrind ÉCHOUÉ - Des fuites de mémoire ont été détectées"
fi

exit $VALGRIND_EXIT_CODE
