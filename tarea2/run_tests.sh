#!/usr/bin/env bash
# Script para compilar y ejecutar los 10 casos de prueba con un solo comando.
# Uso:
#   ./run_tests.sh               (compila v1.cpp en ./v1 y corre las pruebas)
#   ./run_tests.sh ./otro_prog   (corre las pruebas contra un ejecutable específico)

set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

EJECUTABLE="${1:-./v1}"

# Si no se pasó un ejecutable existente y el objetivo por defecto no existe o v1.cpp es más reciente, compilar:
if [ "$EJECUTABLE" = "./v1" ]; then
    echo -e "\033[94m[INFO]\033[0m Compilando v1.cpp -> ./v1 ..."
    g++ -O2 v1.cpp -o v1
fi

chmod +x "$EJECUTABLE" || true
python3 test_suite.py "$EJECUTABLE"
