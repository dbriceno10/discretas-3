#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// ---------------------------------------------------------
// ESTRUCTURAS BÁSICAS
// ---------------------------------------------------------
struct Accion {
  int estado_sig;
  char simbolo_escrito;
  char direccion;
};

struct Transicion {
  int estado_act;
  char simbolo_leido;
  int estado_sig;
  char simbolo_escrito;
  char direccion;
};

// ---------------------------------------------------------
// FUNCIONES DE DECODIFICACIÓN (Notación Unaria)
// ---------------------------------------------------------
int contarUnos(const string &str, int &index) {
  int count = 0;
  while (index < str.length() && str[index] == '1') {
    count++;
    index++;
  }
  return count;
}

char decodificarSimbolo(int count) {
  if (count == 1)
    return 'B';
  return static_cast<char>('a' + (count - 2));
}

char decodificarDireccion(int count) {
  if (count == 1)
    return 'R';
  if (count == 2)
    return 'L';
  return '-';
}

// ---------------------------------------------------------
// FUNCIONES AUXILIARES PARA CONJUNTOS (Reemplazo de <set>)
// ---------------------------------------------------------
void agregarUnicoInt(vector<int> &conjunto, int valor) {
  for (int v : conjunto) {
    if (v == valor)
      return;
  }
  conjunto.push_back(valor);
}

void agregarUnicoChar(vector<char> &conjunto, char valor) {
  if (valor == 'B')
    return; // El blanco no entra en el alfabeto I
  for (char v : conjunto) {
    if (v == valor)
      return;
  }
  conjunto.push_back(valor);
}

// ---------------------------------------------------------
// MOTOR DE SIMULACIÓN (Cinta con dos vectores)
// ---------------------------------------------------------
string simularMaquina(string w,
                      const map<pair<int, char>, Accion> &funcion_transicion,
                      const vector<int> &estados_con_salida) {
  // Implementación de la cinta infinita usando dos vectores
  vector<char> cinta_der;
  vector<char> cinta_izq;

  // Inicializamos la cinta derecha con la palabra de entrada
  if (w.empty())
    w = "B";
  for (char c : w)
    cinta_der.push_back(c);

  int estado_actual = 1; // S0 = s1
  int cabezal = 0;
  int pasos = 0;
  const int MAX_PASOS = 100000;

  while (pasos < MAX_PASOS) {
    // 1. Verificación de Estado Final
    bool es_final = true;
    for (int q_salida : estados_con_salida) {
      if (estado_actual == q_salida) {
        es_final = false;
        break;
      }
    }
    if (es_final)
      return "Aceptada";

    // 2. Leer símbolo actual en la cinta bidireccional
    char simbolo_actual = 'B';
    if (cabezal >= 0) {
      if (cabezal < cinta_der.size())
        simbolo_actual = cinta_der[cabezal];
    } else {
      int idx_izq = -cabezal - 1;
      if (idx_izq < cinta_izq.size())
        simbolo_actual = cinta_izq[idx_izq];
    }

    // 3. Buscar la regla en nuestra "matriz dispersa" (map)
    auto clave = make_pair(estado_actual, simbolo_actual);
    if (funcion_transicion.find(clave) == funcion_transicion.end()) {
      return "Rechazada"; // Se trancó, no hay regla
    }

    Accion accion = funcion_transicion.at(clave);

    // 4. Escribir el nuevo símbolo
    if (cabezal >= 0) {
      if (cabezal >= cinta_der.size())
        cinta_der.resize(cabezal + 1, 'B');
      cinta_der[cabezal] = accion.simbolo_escrito;
    } else {
      int idx_izq = -cabezal - 1;
      if (idx_izq >= cinta_izq.size())
        cinta_izq.resize(idx_izq + 1, 'B');
      cinta_izq[idx_izq] = accion.simbolo_escrito;
    }

    // 5. Actualizar estado y mover cabezal
    estado_actual = accion.estado_sig;
    if (accion.direccion == 'R')
      cabezal++;
    else if (accion.direccion == 'L')
      cabezal--;

    pasos++;
  }
  return "Rechazada";
}

// ---------------------------------------------------------
// PARSER Y PROCESAMIENTO PRINCIPAL
// ---------------------------------------------------------
void procesarCaso(int numero_caso, const string &linea) {
  size_t pos_separador = linea.find("000");
  if (pos_separador == string::npos)
    return;

  // Extraer parte de la Máquina y parte de las Palabras
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

  // Estructuras de almacenamiento
  vector<int> S = {1}; // El estado s1 siempre existe
  vector<char> I;
  vector<int> estados_con_salida;
  vector<Transicion> orden_transiciones;
  map<pair<int, char>, Accion> f;

  // Parsear Transiciones
  int i = 0;
  while (i < parte_M.length()) {
    if (parte_M[i] == '0') {
      i++;
      continue;
    }

    int q = contarUnos(parte_M, i);
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    char x = decodificarSimbolo(contarUnos(parte_M, i));
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    int p = contarUnos(parte_M, i);
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    char y = decodificarSimbolo(contarUnos(parte_M, i));
    if (i < parte_M.length() && parte_M[i] == '0')
      i++;
    char d = decodificarDireccion(contarUnos(parte_M, i));

    agregarUnicoInt(S, q);
    agregarUnicoInt(S, p);
    agregarUnicoChar(I, x);
    agregarUnicoChar(I, y);
    agregarUnicoInt(estados_con_salida, q);

    f[make_pair(q, x)] = {p, y, d};
    orden_transiciones.push_back({q, x, p, y, d});
  }

  // Ordenar los conjuntos (Bubble sort simple para impresión limpia)
  for (int a = 0; a < S.size(); a++) {
    for (int b = a + 1; b < S.size(); b++) {
      if (S[b] < S[a])
        swap(S[a], S[b]);
    }
  }
  for (int a = 0; a < I.size(); a++) {
    for (int b = a + 1; b < I.size(); b++) {
      if (I[b] < I[a])
        swap(I[a], I[b]);
    }
  }

  // -----------------------------------------------------
  // IMPRESIÓN DEL FORMATO DE SALIDA
  // -----------------------------------------------------
  cout << "Caso " << numero_caso << ":\n";
  cout << "T = (S, I, f, S0)\n";

  cout << "S = {";
  for (int j = 0; j < S.size(); j++) {
    if (j > 0)
      cout << ", ";
    cout << "s" << S[j];
  }
  cout << "}\n";

  cout << "I = {";
  for (int j = 0; j < I.size(); j++) {
    if (j > 0)
      cout << ", ";
    cout << I[j];
  }
  cout << "}\n";

  cout << "S0 = s1\n";
  cout << "f = {\n";
  for (int j = 0; j < orden_transiciones.size(); j++) {
    const auto &t = orden_transiciones[j];
    cout << "(s" << t.estado_act << ", " << t.simbolo_leido << ", s"
         << t.estado_sig << ", " << t.simbolo_escrito << ", " << t.direccion
         << ")";
    if (j + 1 < orden_transiciones.size())
      cout << ",";
    cout << "\n";
  }
  cout << "}\n";

  // -----------------------------------------------------
  // SIMULACIÓN DE CADENAS
  // -----------------------------------------------------
  string palabra_actual = "";
  int w_idx = 0;
  while (w_idx < parte_w.length()) {
    if (parte_w[w_idx] == '0') {
      w_idx++;
      if (w_idx < parte_w.length() && parte_w[w_idx] == '0') {
        cout << simularMaquina(palabra_actual, f, estados_con_salida) << "\n";
        palabra_actual = "";
        w_idx++;
      }
    } else {
      palabra_actual += decodificarSimbolo(contarUnos(parte_w, w_idx));
    }
  }
  cout << simularMaquina(palabra_actual, f, estados_con_salida) << "\n";
}

int main() {
  int N;
  if (!(cin >> N))
    return 0;
  string temp;
  getline(cin, temp);

  for (int i = 1; i <= N; ++i) {
    string str;
    while (getline(cin, str) && str.empty())
      ;
    procesarCaso(i, str);
  }
  return 0;
}