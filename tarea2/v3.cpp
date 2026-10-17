#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Estructura que maneja la cinta infinita usando solo vectores estándar
struct CintaTuring {
  vector<char> derecha;
  vector<char> izquierda;

  // Inicializar la cinta con la cadena de entrada w
  void inicializar(const string &w) {
    derecha.clear();
    izquierda.clear();
    for (char c : w) {
      derecha.push_back(c);
    }
    if (derecha.empty()) {
      derecha.push_back('B'); // B es el símbolo en blanco
    }
  }

  // Leer el símbolo en la posición actual del cabezal
  char leer(int cabezal) {
    if (cabezal >= 0) {
      return (cabezal < derecha.size()) ? derecha[cabezal] : 'B';
    } else {
      int idx = -cabezal - 1; // Convierte -1 en 0, -2 en 1, etc.
      return (idx < izquierda.size()) ? izquierda[idx] : 'B';
    }
  }

  // Escribir un símbolo en la posición actual del cabezal
  void escribir(int cabezal, char simbolo) {
    if (cabezal >= 0) {
      // Si el cabezal superó el tamaño actual, expandimos el vector con blancos
      // 'B'
      if (cabezal >= derecha.size()) {
        derecha.resize(cabezal + 1, 'B');
      }
      derecha[cabezal] = simbolo;
    } else {
      int idx = -cabezal - 1;
      // Si el cabezal fue muy a la izquierda, expandimos el vector izquierdo
      if (idx >= izquierda.size()) {
        izquierda.resize(idx + 1, 'B');
      }
      izquierda[idx] = simbolo;
    }
  }
};

int main() {
  CintaTuring cinta;
  cinta.inicializar("aab");

  int cabezal = 0;

  // Ejemplo de un paso computacional donde la máquina escribe y se mueve a la
  // izquierda
  cout << "Leido inicialmente: " << cinta.leer(cabezal) << "\n";

  cinta.escribir(cabezal, 'x'); // Escribe 'x'
  cabezal--;                    // Se mueve a la izquierda (dirección L)

  cout << "Leido despues de mover a la izquierda (indice -1): "
       << cinta.leer(cabezal) << "\n";

  cinta.escribir(cabezal, 'y'); // Expande automáticamente el vector izquierdo
  cout << "Escrito en la izquierda: " << cinta.leer(cabezal) << "\n";

  return 0;
}