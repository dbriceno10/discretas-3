#include <iostream>
#include <string>

using namespace std;

const string ACCEPTED = "Aceptada";
const string REJECTED = "Rechazada";

// Función para contar la cantidad de unos (1) consecutivos a partir de una
// posición
int countOnes(const string &str, int &index) {
  int count = 0;
  while (index < str.length() && str[index] == '1') {
    count++;
    index++;
  }
  return count;
}

// Decodifica el símbolo unario a carácter
char decodeSymbol(int count) {
  if (count == 1)
    return 'B';
  return static_cast<char>('a' + (count - 2));
}

// Decodifica la dirección unaria a carácter
char decodeDirection(int count) {
  if (count == 1)
    return 'R';
  if (count == 2)
    return 'L';
  return '-';
}

// Busca una transición en la cadena original de M (parte_M)
// Retorna true si encuentra una regla para (estado_actual, simbolo_leido)
bool findTransition(const string &parte_M, int estado_actual,
                    char simbolo_leido, int &estado_sig, char &simbolo_escrito,
                    char &direccion) {

  int i = 0;
  while (i < parte_M.length()) {
    // Ignorar los separadores iniciales o múltiples ceros
    if (parte_M[i] == '0') {
      i++;
      continue;
    }

    // Leer la 5-tupla actual
    int q = countOnes(parte_M, i);
    if (i < parte_M.length() && parte_M[i] == '0')
      i++; // saltar cero

    char x = decodeSymbol(countOnes(parte_M, i));
    if (i < parte_M.length() && parte_M[i] == '0')
      i++; // saltar cero

    int p = countOnes(parte_M, i);
    if (i < parte_M.length() && parte_M[i] == '0')
      i++; // saltar cero

    char y = decodeSymbol(countOnes(parte_M, i));
    if (i < parte_M.length() && parte_M[i] == '0')
      i++; // saltar cero

    char d = decodeDirection(countOnes(parte_M, i));

    // Si coincide con el estado y símbolo que buscamos
    if (q == estado_actual && x == simbolo_leido) {
      estado_sig = p;
      simbolo_escrito = y;
      direccion = d;
      return true;
    }

    // Si no coincide, avanzar hasta el próximo separador de tupla (00)
    while (i < parte_M.length() && parte_M[i] != '0') {
      i++;
    }
  }
  return false;
}

// Revisa si un estado es final (si no aparece como primer elemento en ninguna
// tupla)
bool isFinalState(const string &parte_M, int estado) {
  int i = 0;
  while (i < parte_M.length()) {
    if (parte_M[i] == '0') {
      i++;
      continue;
    }

    int q = countOnes(parte_M, i);
    if (q == estado) {
      return false; // Tiene transiciones de salida, no es final
    }

    // Saltar el resto de la tupla hasta el siguiente '00'
    while (i < parte_M.length() && parte_M[i] != '0') {
      i++;
    }
    // Saltar posibles múltiples ceros
    while (i < parte_M.length() && parte_M[i] == '0') {
      i++;
    }
  }
  return true; // No se encontró como origen, es final
}

// Simula la MT sobre una cadena usando string en lugar de deque
string simulateTuringMachine(const string &parte_M, string w) {
  int estado_actual = 1; // S0 = s1
  int cabezal = 0;
  int pasos = 0;
  const int MAX_PASOS = 100000; // Prevención de bucles infinitos

  if (w.empty()) {
    w = "B";
  }

  while (pasos < MAX_PASOS) {
    // Verificar aceptación
    if (isFinalState(parte_M, estado_actual)) {
      return ACCEPTED;
    }

    char simbolo_actual = w[cabezal];

    int estado_sig;
    char simbolo_escrito;
    char direccion;

    // Buscar transición
    bool encontrada = findTransition(parte_M, estado_actual, simbolo_actual,
                                     estado_sig, simbolo_escrito, direccion);

    if (!encontrada) {
      return REJECTED;
    }

    // Aplicar transición
    w[cabezal] = simbolo_escrito;
    estado_actual = estado_sig;

    // Mover cabezal
    if (direccion == 'R') {
      cabezal++;
      if (cabezal >= w.length()) {
        w += "B"; // Expandir a la derecha
      }
    } else if (direccion == 'L') {
      if (cabezal == 0) {
        w = "B" + w; // Expandir a la izquierda
                     // El cabezal se queda en 0 apuntando a la nueva B
      } else {
        cabezal--;
      }
    }

    pasos++;
  }

  return REJECTED;
}

// Recopilar caracteres únicos para el conjunto I
void addSymbolToI(string &I_str, char c) {
  if (c != 'B') {
    bool exists = false;
    for (char x : I_str) {
      if (x == c)
        exists = true;
    }
    if (!exists)
      I_str += c;
  }
}

// Recopilar estados únicos para el conjunto S
void addStateToS(string &S_str, int s) {
  // Implementación simple para estados de 1 dígito para imprimir.
  // Para estados mayores se requiere arreglo, pero para el formato simple con
  // strings:
  char state_char = '0' + s;
  bool exists = false;
  for (char x : S_str) {
    if (x == state_char)
      exists = true;
  }
  if (!exists)
    S_str += state_char;
}

void procesarCaso(int numero_caso, const string &linea) {
  size_t pos_separador = linea.find("000");
  if (pos_separador == string::npos)
    return;

  // Quitar 0s de los extremos como pide la codificación
  int m_start = 0;
  while (m_start < pos_separador && linea[m_start] == '0')
    m_start++;
  string parte_M = linea.substr(m_start, pos_separador - m_start);

  int w_start = pos_separador + 3;
  while (w_start < linea.length() && linea[w_start] == '0')
    w_start++;
  string parte_w = linea.substr(w_start);
  while (!parte_w.empty() && parte_w.back() == '0')
    parte_w.pop_back();

  cout << "Caso " << numero_caso << ":\n";
  cout << "T = (S, I, f, S0)\n";

  // Primera pasada manual a la parte_M para recolectar e imprimir S, I y f
  string S_set = "1"; // s1 siempre está
  string I_set = "";
  string f_print = "f = {\n";

  int i = 0;
  bool first_tupla = true;
  while (i < parte_M.length()) {
    if (parte_M[i] == '0') {
      i++;
      continue;
    }

    int q = countOnes(parte_M, i);
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    char x = decodeSymbol(countOnes(parte_M, i));
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    int p = countOnes(parte_M, i);
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    char y = decodeSymbol(countOnes(parte_M, i));
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    char d = decodeDirection(countOnes(parte_M, i));

    addStateToS(S_set, q);
    addStateToS(S_set, p);
    addSymbolToI(I_set, x);
    addSymbolToI(I_set, y);

    if (!first_tupla)
      f_print += ",\n";
    f_print += "(s" + to_string(q) + ", " + x + ", s" + to_string(p) + ", " +
               y + ", " + d + ")";
    first_tupla = false;
  }
  f_print += "\n}\n";

  // Imprimir S
  cout << "S = {";
  for (int j = 0; j < S_set.length(); j++) {
    if (j > 0)
      cout << ", ";
    cout << "s" << S_set[j];
  }
  cout << "}\n";

  // Imprimir I (Ordenación simple)
  cout << "I = {";
  // Bubble sort simple para I_set
  for (int j = 0; j < I_set.length(); j++) {
    for (int k = j + 1; k < I_set.length(); k++) {
      if (I_set[k] < I_set[j]) {
        swap(I_set[j], I_set[k]);
      }
    }
  }
  for (int j = 0; j < I_set.length(); j++) {
    if (j > 0)
      cout << ", ";
    cout << I_set[j];
  }
  cout << "}\n";

  cout << "S0 = s1\n";
  cout << f_print;

  // Analizar parte_w para obtener las cadenas a evaluar separadas por "00"
  string palabra_actual = "";
  int w_idx = 0;
  while (w_idx < parte_w.length()) {
    if (parte_w[w_idx] == '0') {
      w_idx++;
      if (w_idx < parte_w.length() && parte_w[w_idx] == '0') {
        // Separador de palabras (00)
        cout << simulateTuringMachine(parte_M, palabra_actual) << "\n";
        palabra_actual = "";
        w_idx++; // saltar segundo 0
      }
    } else {
      char symbol = decodeSymbol(countOnes(parte_w, w_idx));
      palabra_actual += symbol;
    }
  }
  // Procesar la última palabra
  cout << simulateTuringMachine(parte_M, palabra_actual) << "\n";
}

int main() {
  int N;
  if (!(cin >> N))
    return 0;

  string temp;
  getline(cin, temp); // Consumir salto de linea residual

  for (int i = 1; i <= N; ++i) {
    string str;
    getline(cin, str);

    // Evitar líneas vacías perdidas
    while (str.empty()) {
      getline(cin, str);
    }

    procesarCaso(i, str);
  }

  return 0;
}