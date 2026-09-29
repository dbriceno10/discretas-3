#include <iostream>
#include <string>

using namespace std;

string validarCStyle(const string &s) {
  if (s.empty())
    return "RECHAZADA";
  string min = "abcdefghijklmnopqrstuvwxyz";
  string dig = "0123456789";

  int estado = 0;
  for (char c : s) {
    if (estado == 0) {
      if (min.find(c) != string::npos)
        estado = 1;
      else
        return "RECHAZADA";
    } else if (estado == 1 || estado == 2) {
      if (min.find(c) != string::npos || dig.find(c) != string::npos)
        estado = 1;
      else if (c == '_')
        estado = 2;
      else
        return "RECHAZADA";
    }
  }
  return estado == 1 ? "ACEPTADA" : "RECHAZADA";
}

string validarPython(const string &s) {
  if (s.empty())
    return "RECHAZADA";
  string letras = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string dig = "0123456789";

  int estado = 0;
  for (char c : s) {
    if (estado == 0) {
      if (letras.find(c) != string::npos || c == '_')
        estado = 1;
      else
        return "RECHAZADA";
    } else if (estado == 1) {
      if (letras.find(c) != string::npos || dig.find(c) != string::npos ||
          c == '_')
        estado = 1;
      else
        return "RECHAZADA";
    }
  }
  return estado == 1 ? "ACEPTADA" : "RECHAZADA";
}

string validarCOBOL(const string &s) {
  if (s.empty())
    return "RECHAZADA";
  string may = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string dig = "0123456789";

  int estado = 0;
  for (char c : s) {
    if (estado == 0) {
      if (may.find(c) != string::npos)
        estado = 1;
      else
        return "RECHAZADA";
    } else if (estado == 1 || estado == 2) {
      if (may.find(c) != string::npos || dig.find(c) != string::npos)
        estado = 1;
      else if (c == '-')
        estado = 2;
      else
        return "RECHAZADA";
    }
  }
  return estado == 1 ? "ACEPTADA" : "RECHAZADA";
}

int main() {
  int k;
  if (!(cin >> k))
    return 0;
  string temp;
  getline(cin, temp);

  for (int i = 0; i < k; ++i) {
    string s;
    getline(cin, s);
    cout << "Cadena: " << s << "\n";
    cout << "C-Style: " << validarCStyle(s) << "\n";
    cout << "Python: " << validarPython(s) << "\n";
    cout << "COBOL: " << validarCOBOL(s) << "\n";
    if (i < k - 1)
      cout << "\n";
  }
  return 0;
}