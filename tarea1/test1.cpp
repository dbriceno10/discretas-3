#include <iostream>
#include <string>

using namespace std;

// Función que simula el AFD para C-Style
string validarCStyle(const string& s) {
  if (s.empty()) return "RECHAZADA";

  int estado = 0;  // Estado inicial q0

  for (char c : s) {
    if (estado == 0) {
      // Debe iniciar con minúscula (a-z)
      if (c >= 'a' && c <= 'z')
        estado = 1;
      else
        estado = -1;  // q_err
    } else if (estado == 1) {
      // Acepta letras minúsculas, dígitos y guión bajo
      if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        estado = 1;
      else if (c == '_')
        estado = 2;  // q2: leído guión bajo (no puede terminar aquí)
      else
        estado = -1;
    } else if (estado == 2) {
      // Después de un guión bajo, acepta minúsculas, dígitos u otros guiones
      if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        estado = 1;
      else if (c == '_')
        estado = 2;
      else
        estado = -1;
    }

    // Optimización: si caemos en estado de error, interrumpimos el análisis
    if (estado == -1) break;
  }

  // Si la cadena termina en el estado 1, es aceptada
  return (estado == 1) ? "ACEPTADA" : "RECHAZADA";
}

// Función que simula el AFD para Python
string validarPython(const string& s) {
  if (s.empty()) return "RECHAZADA";

  int estado = 0;  // Estado inicial q0

  for (char c : s) {
    if (estado == 0) {
      // Inicia con letra mayúscula/minúscula o guión bajo
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        estado = 1;
      else
        estado = -1;
    } else if (estado == 1) {
      // Acepta letras, dígitos y guiones bajos (puede terminar en cualquiera)
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == '_')
        estado = 1;
      else
        estado = -1;
    }

    if (estado == -1) break;
  }

  return (estado == 1) ? "ACEPTADA" : "RECHAZADA";
}

// Función que simula el AFD para COBOL
string validarCOBOL(const string& s) {
  if (s.empty()) return "RECHAZADA";

  int estado = 0;  // Estado inicial q0

  for (char c : s) {
    if (estado == 0) {
      // Debe iniciar con mayúscula (A-Z)
      if (c >= 'A' && c <= 'Z')
        estado = 1;
      else
        estado = -1;
    } else if (estado == 1) {
      // Acepta mayúsculas, dígitos o guión normal
      if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        estado = 1;
      else if (c == '-')
        estado = 2;  // q2: leído guión (no puede terminar aquí)
      else
        estado = -1;
    } else if (estado == 2) {
      // Después de un guión, acepta mayúsculas, dígitos u otros guiones
      if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        estado = 1;
      else if (c == '-')
        estado = 2;
      else
        estado = -1;
    }

    if (estado == -1) break;
  }

  return (estado == 1) ? "ACEPTADA" : "RECHAZADA";
}

int main() {
  int k;
  if (!(cin >> k)) return 0;

  // Consumir el salto de línea residual después de leer K
  string temp;
  getline(cin, temp);

  for (int i = 0; i < k; ++i) {
    string s;
    // Se utiliza getline para capturar toda la línea, garantizando que un
    // espacio intermedio provoque rechazo directo al no pertenecer al alfabeto.
    getline(cin, s);

    cout << "Cadena: " << s << "\n";
    cout << "C-Style: " << validarCStyle(s) << "\n";
    cout << "Python: " << validarPython(s) << "\n";
    cout << "COBOL: " << validarCOBOL(s) << "\n";

    // Imprime una línea en blanco entre resultados si no es la última
    // evaluación
    if (i < k - 1) {
      cout << "\n";
    }
  }

  return 0;
}