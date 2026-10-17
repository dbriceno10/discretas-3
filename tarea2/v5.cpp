#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const string ACCEPTED = "Aceptada";
const string REJECTED = "Rechazada";

// estructura para definir a donde moverse y que escribir
struct Action {
  int nextState;
  char symbolToWrite;
  char direction;
};

// estructura completa de la tupla para impresion
struct Transition {
  int currentState;
  char symbolRead;
  int nextState;
  char symbolToWrite;
  char direction;
};

// funcion para contar la cantidad de 1s seguidos
int countOnes(const string &str, int &index) {
  int count = 0;
  while (index < str.length() && str[index] == '1') {
    count++;
    index++;
  }
  return count;
}

// decodificador de simbolos de cinta a caracteres ascii
char decodeSymbol(int count) {
  // 1 es el simbolo blanco
  if (count == 1)
    return 'B';
  // 2 es 'a', 3 es 'b', etc
  return static_cast<char>('a' + (count - 2));
}

// decodificador de direccion de movimiento
char decodeDirection(int count) {
  if (count == 1)
    return 'R'; // derecha
  if (count == 2)
    return 'L'; // izquierda
  return '-';   // sin movimiento
}

// utileria para simular conjuntos numericos (sets)
void addUniqueInt(vector<int> &collection, int value) {
  for (int v : collection) {
    if (v == value)
      return;
  }
  collection.push_back(value);
}

// utileria para conjuntos de caracteres
void addUniqueChar(vector<char> &collection, char value) {
  // el simbolo blanco B no va en el alfabeto I
  if (value == 'B')
    return;
  for (char v : collection) {
    if (v == value)
      return;
  }
  collection.push_back(value);
}

// simulador de la maquina de turing
string simulateMachine(string word,
                       const map<pair<int, char>, Action> &transitions,
                       const vector<int> &statesWithOutput) {
  // vectores para simular cinta infinita
  vector<char> rightTape;
  vector<char> leftTape;

  // inicializacion de la cinta derecha
  if (word.empty()) {
    word = "B";
  }
  for (char c : word) {
    rightTape.push_back(c);
  }

  int currentState = 1; // state inicial s1
  int headPosition = 0; // pos inicial del cabezal
  int steps = 0;
  const int MAX_STEPS = 100000; // prevencion de bucles infinitos

  while (steps < MAX_STEPS) {
    // verificacion de estado final (sin transiciones de salida)
    bool isFinal = true;
    for (int qOut : statesWithOutput) {
      if (currentState == qOut) {
        isFinal = false;
        break;
      }
    }
    if (isFinal) {
      return ACCEPTED;
    }

    // lectura de simbolo en cinta
    char currentSymbol = 'B';
    if (headPosition >= 0) {
      if (headPosition < rightTape.size()) {
        currentSymbol = rightTape[headPosition];
      }
    } else {
      // normalizacion de indice para cinta izquierda
      int leftIndex = -headPosition - 1;
      if (leftIndex < leftTape.size()) {
        currentSymbol = leftTape[leftIndex];
      }
    }

    // transicion desde matriz dispersa (map)
    auto key = make_pair(currentState, currentSymbol);
    if (transitions.find(key) == transitions.end()) {
      // validacion estado de error (trancado)
      return REJECTED;
    }

    Action action = transitions.at(key);

    // escritura en cinta
    if (headPosition >= 0) {
      if (headPosition >= rightTape.size()) {
        rightTape.resize(headPosition + 1, 'B');
      }
      rightTape[headPosition] = action.symbolToWrite;
    } else {
      int leftIndex = -headPosition - 1;
      if (leftIndex >= leftTape.size()) {
        leftTape.resize(leftIndex + 1, 'B');
      }
      leftTape[leftIndex] = action.symbolToWrite;
    }

    // actualizacion de estado y movimiento
    currentState = action.nextState;
    if (action.direction == 'R') {
      headPosition++;
    } else if (action.direction == 'L') {
      headPosition--;
    }

    steps++;
  }

  // supero el limite de pasos
  return REJECTED;
}

// funcion principal para cada linea
void processCase(int caseNumber, const string &line) {
  // validacion separador principal 000
  size_t separatorPos = line.find("000");
  if (separatorPos == string::npos)
    return;

  // obtencion de codigo de la maquina
  int mStart = 0;
  while (mStart < separatorPos && line[mStart] == '0') {
    mStart++;
  }
  string machinePart = line.substr(mStart, separatorPos - mStart);

  // obtencion de las cadenas a evaluar
  int wStart = separatorPos + 3;
  while (wStart < line.length() && line[wStart] == '0') {
    wStart++;
  }
  string wordPart = line.substr(wStart);
  while (!wordPart.empty() && wordPart.back() == '0') {
    wordPart.pop_back();
  }

  // colecciones de la MT
  vector<int> states = {1}; // state inicial s1 siempre presente
  vector<char> alphabet;
  vector<int> statesWithOutput;
  vector<Transition> transitionList;
  map<pair<int, char>, Action> transitions;

  // decodificacion de transiciones
  int i = 0;
  while (i < machinePart.length()) {
    // salto de ceros
    if (machinePart[i] == '0') {
      i++;
      continue;
    }

    // lectura de cada componente de la 5-tupla
    int q = countOnes(machinePart, i);
    if (i < machinePart.length() && machinePart[i] == '0')
      i++;

    char x = decodeSymbol(countOnes(machinePart, i));
    if (i < machinePart.length() && machinePart[i] == '0')
      i++;

    int p = countOnes(machinePart, i);
    if (i < machinePart.length() && machinePart[i] == '0')
      i++;

    char y = decodeSymbol(countOnes(machinePart, i));
    if (i < machinePart.length() && machinePart[i] == '0')
      i++;

    char d = decodeDirection(countOnes(machinePart, i));

    // almacenamiento en conjuntos manuales
    addUniqueInt(states, q);
    addUniqueInt(states, p);
    addUniqueChar(alphabet, x);
    addUniqueChar(alphabet, y);
    addUniqueInt(statesWithOutput, q);

    // poblado de la tabla de transicion (map) y lista para impresion
    transitions[make_pair(q, x)] = {p, y, d};
    transitionList.push_back({q, x, p, y, d});
  }

  // ordenamiento basico estilo burbuja
  for (int a = 0; a < states.size(); a++) {
    for (int b = a + 1; b < states.size(); b++) {
      if (states[b] < states[a])
        swap(states[a], states[b]);
    }
  }
  for (int a = 0; a < alphabet.size(); a++) {
    for (int b = a + 1; b < alphabet.size(); b++) {
      if (alphabet[b] < alphabet[a])
        swap(alphabet[a], alphabet[b]);
    }
  }

  // impresion del caso formateado
  cout << "Caso " << caseNumber << ":\n";
  cout << "T = (S, I, f, S0)\n";

  // impresion de states S
  cout << "S = {";
  for (int j = 0; j < states.size(); j++) {
    if (j > 0)
      cout << ", ";
    cout << "s" << states[j];
  }
  cout << "}\n";

  // impresion de alphabet I
  cout << "I = {";
  for (int j = 0; j < alphabet.size(); j++) {
    if (j > 0)
      cout << ", ";
    cout << alphabet[j];
  }
  cout << "}\n";

  cout << "S0 = s1\n";
  cout << "f = {\n";
  for (int j = 0; j < transitionList.size(); j++) {
    const auto &t = transitionList[j];
    cout << "(s" << t.currentState << ", " << t.symbolRead << ", s"
         << t.nextState << ", " << t.symbolToWrite << ", " << t.direction
         << ")";
    if (j + 1 < transitionList.size())
      cout << ",";
    cout << "\n";
  }
  cout << "}\n";

  // validacion de cada palabra a evaluar
  string currentWord = "";
  int wIndex = 0;
  while (wIndex < wordPart.length()) {
    if (wordPart[wIndex] == '0') {
      wIndex++;
      // doble cero separa las palabras
      if (wIndex < wordPart.length() && wordPart[wIndex] == '0') {
        cout << simulateMachine(currentWord, transitions, statesWithOutput)
             << "\n";
        currentWord = "";
        wIndex++;
      }
    } else {
      currentWord += decodeSymbol(countOnes(wordPart, wIndex));
    }
  }
  // simula la ultima palabra almacenada
  cout << simulateMachine(currentWord, transitions, statesWithOutput) << "\n";
}

int main() {
  int cases;
  if (!(cin >> cases))
    return 0;

  // Consumir el salto de linea residual despues de leer casos
  string temp;
  getline(cin, temp);

  for (int i = 1; i <= cases; ++i) {
    string line;
    // se obtiene la cadena completa evitando vacias
    while (getline(cin, line) && line.empty())
      ;
    processCase(i, line);
  }

  return 0;
}