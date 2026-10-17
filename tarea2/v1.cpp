#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Representa la salida (imagen) de la transición: (s_act, leido) = (s_sig,
// escrito, dir)
struct Accion {
  int estado_sig;
  char simbolo_escrito;
  char direccion; // 'R', 'L' o '-'
};

// Representa la 5-tupla completa para imprimir la descripción formal f
struct Transicion {
  int estado_act;
  char simbolo_leido;
  int estado_sig;
  char simbolo_escrito;
  char direccion;
};

// Divide una cadena usando un delimitador específico ("00" o "0")
vector<string> dividir(const string &cadena, const string &delimitador) {
  vector<string> partes;
  if (cadena.empty())
    return partes;

  size_t inicio = 0;
  size_t fin = cadena.find(delimitador);
  while (fin != string::npos) {
    if (fin > inicio) {
      partes.push_back(cadena.substr(inicio, fin - inicio));
    }
    inicio = fin + delimitador.length();
    fin = cadena.find(delimitador, inicio);
  }
  if (inicio < cadena.length()) {
    partes.push_back(cadena.substr(inicio));
  }
  return partes;
}

// Elimina los ceros ('0') sobrantes en los extremos de un bloque
string limpiarCerosExtremos(const string &s) {
  size_t primero = s.find_first_not_of('0');
  if (primero == string::npos)
    return "";
  size_t ultimo = s.find_last_not_of('0');
  return s.substr(primero, ultimo - primero + 1);
}

// Decodifica un símbolo de cinta a partir de su longitud unaria (cantidad de
// '1's)
char decodificarSimbolo(int cantidad_unos) {
  if (cantidad_unos == 1)
    return 'B'; // 1 -> Blanco (B)
  return static_cast<char>('a' +
                           (cantidad_unos - 2)); // 11 -> 'a', 111 -> 'b', etc.
}

// Decodifica la directriz de desplazamiento a partir de su longitud unaria
char decodificarDireccion(int cantidad_unos) {
  if (cantidad_unos == 1)
    return 'R'; // 1   -> Derecha (->)
  if (cantidad_unos == 2)
    return 'L'; // 11  -> Izquierda (<-)
  return '-';   // 111 -> Sin movimiento (-)
}

// Simula el funcionamiento de la Máquina de Turing sobre una cadena de entrada
// w
bool simularMaquina(const string &w,
                    const map<pair<int, char>, Accion> &funcion_transicion,
                    const set<int> &estados_con_salida) {
  // Cinta 2 de la MT Universal: cinta de trabajo inicializada con w
  deque<char> cinta;
  for (char c : w) {
    cinta.push_back(c);
  }
  if (cinta.empty()) {
    cinta.push_back('B');
  }

  // Cinta 3 de la MT Universal: registro del estado actual (inicia en s1)
  int estado_actual = 1;
  int cabezal = 0;

  // Límite de seguridad para evitar bucles infinitos en máquinas que no se
  // detienen
  const int MAX_PASOS = 1000000;
  int pasos = 0;

  while (pasos < MAX_PASOS) {
    // Condición de parada por aceptación:
    // Un estado es final si no es el primer estado de ninguna 5-tupla
    if (estados_con_salida.find(estado_actual) == estados_con_salida.end()) {
      return true; // Aceptada
    }

    char simbolo_actual = cinta[cabezal];
    auto clave = make_pair(estado_actual, simbolo_actual);

    // Buscamos si existe transición definida para (estado_actual,
    // simbolo_actual)
    auto it = funcion_transicion.find(clave);
    if (it == funcion_transicion.end()) {
      // La máquina se detiene en un estado que NO es final
      return false; // Rechazada
    }

    // Aplicamos la transición: (s_act, simbolo_actual) = (estado_sig,
    // simbolo_escrito, direccion)
    Accion accion = it->second;
    cinta[cabezal] = accion.simbolo_escrito;
    estado_actual = accion.estado_sig;

    // Movimiento del cabezal sobre la cinta infinita en ambas direcciones
    if (accion.direccion == 'R') {
      cabezal++;
      if (cabezal >= static_cast<int>(cinta.size())) {
        cinta.push_back('B');
      }
    } else if (accion.direccion == 'L') {
      if (cabezal == 0) {
        cinta.push_front('B');
        // El cabezal permanece en el índice 0 apuntando al nuevo blanco a la
        // izquierda
      } else {
        cabezal--;
      }
    }
    // Si es '-', el cabezal no se mueve

    pasos++;
  }

  return false; // Si supera el límite de pasos sin detenerse, no acepta
}

void procesarCaso(int numero_caso, const string &linea) {
  // 1. Buscar la secuencia "000" que separa la máquina M de las cadenas w
  size_t pos_separador = linea.find("000");
  if (pos_separador == string::npos)
    return;

  string parte_M = limpiarCerosExtremos(linea.substr(0, pos_separador));
  string parte_w = limpiarCerosExtremos(linea.substr(pos_separador + 3));

  // Estructuras de la Máquina de Turing T = (S, I, f, S0)
  set<int> S;
  set<char> I;
  set<int> estados_con_salida;
  vector<Transicion> lista_transiciones;
  map<pair<int, char>, Accion> funcion_transicion;

  S.insert(1); // El estado inicial s1 siempre forma parte de la máquina

  // 2. Decodificar las transiciones de M (separadas por "00")
  vector<string> bloques_transicion = dividir(parte_M, "00");
  for (const string &bloque : bloques_transicion) {
    vector<string> elementos = dividir(bloque, "0");
    if (elementos.size() == 5) {
      int q = elementos[0].length();
      char x = decodificarSimbolo(elementos[1].length());
      int p = elementos[2].length();
      char y = decodificarSimbolo(elementos[3].length());
      char D = decodificarDireccion(elementos[4].length());

      S.insert(q);
      S.insert(p);
      estados_con_salida.insert(q);

      // El alfabeto de entrada I excluye el símbolo blanco 'B'
      if (x != 'B')
        I.insert(x);
      if (y != 'B')
        I.insert(y);

      lista_transiciones.push_back({q, x, p, y, D});
      funcion_transicion[{q, x}] = {p, y, D};
    }
  }

  // 3. Decodificar la lista de cadenas en w (separadas por "00")
  vector<string> cadenas_evaluar;
  vector<string> bloques_cadenas = dividir(parte_w, "00");
  for (const string &bloque_cad : bloques_cadenas) {
    vector<string> simbolos_cod = dividir(bloque_cad, "0");
    string palabra = "";
    for (const string &sim : simbolos_cod) {
      char c = decodificarSimbolo(sim.length());
      palabra += c;
      if (c != 'B')
        I.insert(c);
    }
    cadenas_evaluar.push_back(palabra);
  }

  // 4. Imprimir la descripción formal según el formato requerido
  cout << "Caso " << numero_caso << ":\n";
  cout << "T = (S, I, f, S0)\n";

  // Conjunto de estados S
  cout << "S = {";
  for (auto it = S.begin(); it != S.end(); ++it) {
    if (it != S.begin())
      cout << ", ";
    cout << "s" << *it;
  }
  cout << "}\n";

  // Alfabeto de entrada I
  cout << "I = {";
  for (auto it = I.begin(); it != I.end(); ++it) {
    if (it != I.begin())
      cout << ", ";
    cout << *it;
  }
  cout << "}\n";

  // Estado inicial S0
  cout << "S0 = s1\n";

  // Función de transición f
  cout << "f = {\n";
  for (size_t i = 0; i < lista_transiciones.size(); ++i) {
    const auto &t = lista_transiciones[i];
    cout << "(s" << t.estado_act << ", " << t.simbolo_leido << ", "
         << "s" << t.estado_sig << ", " << t.simbolo_escrito << ", "
         << t.direccion << ")";
    if (i + 1 < lista_transiciones.size()) {
      cout << ",";
    }
    cout << "\n";
  }
  cout << "}\n";

  // 5. Simular cada cadena y mostrar si es Aceptada o Rechazada
  for (const string &w : cadenas_evaluar) {
    if (simularMaquina(w, funcion_transicion, estados_con_salida)) {
      cout << "Aceptada\n";
    } else {
      cout << "Rechazada\n";
    }
  }
}

int main() {
  // Optimización de lectura/escritura por consola estándar
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int N;
  if (!(cin >> N))
    return 0;

  string linea;
  for (int i = 1; i <= N; ++i) {
    // Leer la siguiente línea no vacía
    while (cin >> linea && linea.empty())
      ;
    procesarCaso(i, linea);
  }

  return 0;
}