#ifndef READ_HPP
#define READ_HPP

#include <iostream>
#include <math.h>
#include <string>

class Read {
private:
  std::string a;
  friend class Manager;
  friend int main();

public:
  void check() {
    while (!a.empty() && a[0] == ' ')
      a.erase(0, 1);
    int pos = a.length();
    while (pos && a[pos - 1] == ' ')
      pos--, a.erase(pos, 1);
  }

  Read() {}
  Read(std::string a) : a(a) { check(); }

  int length() {
    check();
    return a.length();
  }

  std::string get_string() {
    check();
    if (!a.length())
      throw 0;
    std::string re = "";
    while (!a.empty() && a[0] != ' ') {
      re += a[0];
      a.erase(0, 1);
    }
    return re;
  }
};

int string_to_int(std::string &st) {
  long long res = 0;
  bool sgn = 0;
  int len = st.length();
  for (int i = 0; i < len; i++) {
    if (!isdigit (st[i])) {
      if (st[i] == '-' && !i) {
        sgn = 1;
        continue;
      }
      throw 0;
    }
    res = res * 10 + st[i] - '0';
    if (res > 2147483647) throw 0;
  }
  if (res == 0 && len != 1) throw 0;
  if (res != 0 && st[0] == '0') throw 0;
  return sgn ? -res : res;
};

bool isInteger(std::string &st) {
  for (auto c : st)
    if (c == '.')
      return false;
  return true;
}

double string_to_double(std::string &st) {
  double res = 0;
  bool sgn = 0, turn = 0;
  int len = st.length();
  int intSize = 0, dotSize = 0;
  double di = 1;
  for (int i = 0; i < len; i++) {
    if (!isdigit (st[i])) {
      if (st[i] == '-' && !i) {
        sgn = 1;
        continue;
      }
      if (st[i] == '.') {
        if (turn) throw 0;
        turn = 1;
        if (i != 1 & res == 0) throw 0;
        continue;
      }
      throw 0;
    }
    if (!turn) {
      intSize++;
      res = res * 10 + st[i] - '0';
    } else {
      dotSize++;
      di = di * 0.1;
      res = res + di * (st[i] - '0');
    }
    if (res > 2147483647) throw 0;
  }
  if (!intSize) throw 0; 
  if (turn && !dotSize) throw 0; 
  if (dotSize > 2) throw 0;
  if (std::floor(res) != 0 && st[0] == '0') throw 0;
  return sgn ? -res : res;
}

#endif
