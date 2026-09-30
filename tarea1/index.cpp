#include <iostream>
#include <string>

using namespace std;

string const ACCEPTED = "ACEPTADA";
string const REJECTED = "RECHAZADA";
char const UNDER_SCORE = '_';
char const DASH = '-';
bool validateUpperCase(char c) { return c >= 'A' && c <= 'Z'; }
bool validateLowerCase(char c) { return c >= 'a' && c <= 'z'; }
bool validateDigit(char c) { return c >= '0' && c <= '9'; }

// AFD C-Style
string validateCStyle(const string &str) {
  if (str.empty()) {
    return REJECTED;
  }
  // maquina de 3 estados qo inicial, q1 final y q2
  int state = 0; // state inicial q0

  for (char c : str) {
    if (state == 0) {
      // transision inicial con minuscula
      if (validateLowerCase(c)) {
        state = 1;
      } else {
        state = -1; // q_err
      }
    } else if (state == 1) {
      // transision minusculas y digitos
      if (validateLowerCase(c) || validateDigit(c)) {
        state = 1;
      }
      // transision guion bajo
      else if (c == UNDER_SCORE) {
        state = 2; // q2: leído guión bajo (no puede terminar aquí)
      } else {
        state = -1;
      }
    } else if (state == 2) {
      // transision miniusculas y digitos
      if (validateLowerCase(c) || validateDigit(c)) {
        state = 1;
      }
      // transicion guion bajo
      else if (c == UNDER_SCORE) {
        state = 2;
      } else {
        state = -1;
      }
    }

    // validacion estado de error
    if (state == -1) {
      break;
    }
  }
  // Si la cadena termina en el state 1, es aceptada
  return (state == 1) ? ACCEPTED : REJECTED;
}

// AFD Python
string validatePython(const string &str) {
  if (str.empty()) {
    return REJECTED;
  }
  // maquina de 3 estados qo inicial, q1 final y q2
  int state = 0; // state inicial q0

  for (char c : str) {
    if (state == 0) {
      // transicion inicial con mayuscula, minuscula o guion bajo
      if (validateLowerCase(c) || validateUpperCase(c) || c == UNDER_SCORE) {
        state = 1;
      } else {
        state = -1;
      }
    } else if (state == 1) {
      // transicion mayusculas, minusculas, digitos y guion bajo
      if (validateLowerCase(c) || validateUpperCase(c) || validateDigit(c) ||
          c == UNDER_SCORE) {
        state = 1;
      } else {
        state = -1;
      }
    }

    // validacion estado de error
    if (state == -1) {
      break;
    }
  }

  return (state == 1) ? ACCEPTED : REJECTED;
}

// AFD COBOL
string validateCOBOL(const string &str) {
  if (str.empty()) {
    return REJECTED;
  }
  // maquina de 2 estados qo inicial y q1 final
  int state = 0; // state inicial q0

  for (char c : str) {
    if (state == 0) {
      // transicion inicial con mayuscula
      if (validateUpperCase(c)) {
        state = 1;
      } else {
        state = -1;
      }
    } else if (state == 1) {
      // transicion mayusculas y digitos
      if (validateUpperCase(c) || validateDigit(c)) {
        state = 1;
      }
      // transicion guion
      else if (c == DASH) {
        state = 2;
      } else {
        state = -1;
      }
    } else if (state == 2) {
      // transicion mayusculas y digitos
      if (validateUpperCase(c) || validateDigit(c)) {
        state = 1;
      }
      // transicion guion
      else if (c == DASH) {
        state = 2;
      } else {
        state = -1;
      }
    }

    // validacion estado de error
    if (state == -1) {
      break;
    }
  }

  return (state == 1) ? ACCEPTED : REJECTED;
}

int main() {
  int nroK;
  if (!(cin >> nroK)) {
    return 0;
  }

  // Consumir el salto de línea residual después de leer K
  string temp;
  getline(cin, temp);

  for (int i = 0; i < nroK; ++i) {
    string str;
    // Se utiliza getline para capturar toda la línea, garantizando que un
    // espacio intermedio provoque rechazo directo al no pertenecer al
    // alfabeto.
    getline(cin, str);

    cout << "Cadena: " << str << "\n";
    cout << "C-Style: " << validateCStyle(str) << "\n";
    cout << "Python: " << validatePython(str) << "\n";
    cout << "COBOL: " << validateCOBOL(str) << "\n";

    // Imprime una línea en blanco entre resultados si no es la última
    // evaluación
    if (i < nroK - 1) {
      cout << "\n";
    }
  }

  return 0;
}