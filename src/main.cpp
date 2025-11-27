#include "../include/Financesys.hpp"
#include "../include/Manager.hpp"
#include "../include/Read.hpp"
#include "../include/User.hpp"
#include "../include/UserStack.hpp"
#include <assert.h>
#include <cstdio>
#include <iostream>
#include <string>
inline void checkStr(Read x) {
  Read cop = x;
  while (cop.length() != 0) {
    string cur = cop.get_string();
    for (auto c : cur)
      if (!isprint(c)) {
        throw 0;
      }
  }
}
int main() {
  while (!std::cin.eof()) {
    try {
      std::string st;
      getline(std::cin, st);
      string stcop = st;
      Read a(st);
      checkStr(a);
      if (!a.length())
        continue;
      string op = a.get_string();
      if (op == "quit" || op == "exit")
        return 0;
      else if (op == "su") {
        Stack.Login(a);
      } else if (op == "logout") {
        if (a.length())
          throw 0;
        Stack.Logout();
      } else if (op == "passwd") {
        Stack.Password(a);
      } else if (op == "register") {
        Stack.Register(a);
      } else if (op == "useradd") {
        Stack.Create(a);
      } else if (op == "delete") {
        Stack.Delete(a);
      } else if (op == "show") {
        Read cop = a;
        if (cop.length() && cop.get_string() == "finance") {
          string tmp = a.get_string();
          Finance_Record.print(a);
        } else
          BookManager.show(a);
      } else if (op == "buy") {
        BookManager.buy(a);
      } else if (op == "select") {
        BookManager.select(a);
      } else if (op == "modify") {
        BookManager.modify(a);
      } else if (op == "import") {
        BookManager.import(a);
      } else
        throw 0;
    } catch (int) {
      std::cout << "Invalid\n";
    }
  }
  return 0;
}
