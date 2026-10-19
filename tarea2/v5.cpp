#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const string ACCEPTED = "Aceptada";
const string REJECTED = "Rechazada";

struct Action {
  int nextState;
  char symbolToWrite;
  char direction;
};

struct Transition {
  int currentState;
  char symbolRead;
  int nextState;
  char symbolToWrite;
  char direction;
};

// cuenta los 1s seguidos a partir de index
int countOnes(const string &str, int &index) {
  int count = 0;
  while (index < str.length() && str[index] == '1') {
    count++;
    index++;
  }
  return count;
}

// 1 -> 'B', 2 -> 'a', 3 -> 'b', etc
char decodeSymbol(int count) {
  if (count == 1)
    return 'B';
  return (char)('a' + (count - 2));
}

// 1 -> R, 2 -> L, 3 -> -
char decodeDirection(int count) {
  if (count == 1)
    return 'R';
  if (count == 2)
    return 'L';
  return '-';
}

// funcion para evitar repetidos
void addUnique(vector<int> &v, int val) {
  for (int x : v) {
    if (x == val)
      return;
  }
  v.push_back(val);
}

void addUniqueChar(vector<char> &v, char val) {
  if (val == 'B') // blanco no entra en el alfabeto I
    return;
  for (char x : v) {
    if (x == val)
      return;
  }
  v.push_back(val);
}

string simulateMachine(string word,
                       const map<pair<int, char>, Action> &transitions,
                       const vector<int> &statesWithOutput) {
  vector<char> rightTape;
  vector<char> leftTape;

  if (word.empty()) {
    word = "B";
  }
  for (char c : word) {
    rightTape.push_back(c);
  }

  int currentState = 1;
  int headPosition = 0;
  int steps = 0;
  const int MAX_STEPS = 100000; // prevencion de bucles infinitos

  while (steps < MAX_STEPS) {
    // si no tiene transicion de salida, es estado final
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

    char currentSymbol = 'B';
    if (headPosition >= 0) {
      if (headPosition < rightTape.size()) {
        currentSymbol = rightTape[headPosition];
      }
    } else {
      // indices negativos para la cinta izquierda
      int leftIndex = -headPosition - 1;
      if (leftIndex < leftTape.size()) {
        currentSymbol = leftTape[leftIndex];
      }
    }

    auto key = make_pair(currentState, currentSymbol);
    if (transitions.find(key) == transitions.end()) {
      return REJECTED;
    }

    Action action = transitions.at(key);

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

    currentState = action.nextState;
    if (action.direction == 'R') {
      headPosition++;
    } else if (action.direction == 'L') {
      headPosition--;
    }

    steps++;
  }

  return REJECTED;
}

void processCase(int caseNumber, const string &line) {
  // 000 separa la maquina de las palabras
  size_t separatorPos = line.find("000");
  if (separatorPos == string::npos)
    return;

  int mStart = 0;
  while (mStart < separatorPos && line[mStart] == '0') {
    mStart++;
  }
  string machinePart = line.substr(mStart, separatorPos - mStart);

  int wStart = separatorPos + 3;
  while (wStart < line.length() && line[wStart] == '0') {
    wStart++;
  }
  string wordPart = line.substr(wStart);
  while (!wordPart.empty() && wordPart.back() == '0') {
    wordPart.pop_back();
  }

  vector<int> states = {1}; // s1 siempre presente
  vector<char> alphabet;
  vector<int> statesWithOutput;
  vector<Transition> transitionList;
  map<pair<int, char>, Action> transitions;

  int i = 0;
  while (i < machinePart.length()) {
    if (machinePart[i] == '0') {
      i++;
      continue;
    }

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

    addUnique(states, q);
    addUnique(states, p);
    addUniqueChar(alphabet, x);
    addUniqueChar(alphabet, y);
    addUnique(statesWithOutput, q);

    transitions[make_pair(q, x)] = {p, y, d};
    transitionList.push_back({q, x, p, y, d});
  }

  // ordenar estados y alfabeto
  for (int a = 0; a < states.size(); a++) {
    for (int b = a + 1; b < states.size(); b++) {
      if (states[a] > states[b])
        swap(states[a], states[b]);
    }
  }
  for (int a = 0; a < alphabet.size(); a++) {
    for (int b = a + 1; b < alphabet.size(); b++) {
      if (alphabet[a] > alphabet[b])
        swap(alphabet[a], alphabet[b]);
    }
  }

  cout << "Caso " << caseNumber << ":\n";
  cout << "T = (S, I, f, S0)\n";

  cout << "S = {";
  for (int j = 0; j < states.size(); j++) {
    if (j > 0)
      cout << ", ";
    cout << "s" << states[j];
  }
  cout << "}\n";

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

  // evaluar cada palabra
  string currentWord = "";
  int wIndex = 0;
  while (wIndex < wordPart.length()) {
    if (wordPart[wIndex] == '0') {
      wIndex++;
      // doble cero separa palabras
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
  cout << simulateMachine(currentWord, transitions, statesWithOutput) << "\n";
}

int main() {
  int cases;
  if (!(cin >> cases))
    return 0;

  string temp;
  getline(cin, temp);

  for (int i = 1; i <= cases; ++i) {
    string line;
    while (getline(cin, line) && line.empty())
      ;
    processCase(i, line);
  }

  return 0;
}