#ifndef USER_HPP
#define USER_HPP

#include "Chars.hpp"
#include <string>

class User {
private:
  int Privilege;
  chars UserID, password, UserName;
  friend class UserStore;
  friend class UserStack;
  friend class Manager;
  friend class Finance_Sys;
  friend int main();

public:
  User() { Privilege = 0; }
  User(std::string userID, std::string passwd, std::string username, int pri) {
    UserID = (chars)(userID);
    password = (chars)(passwd);
    UserName = (chars)(username);
    Privilege = pri;
  }
  bool operator==(const User A) const { return UserID == A.UserID; }
  bool operator!=(const User A) const { return UserID != A.UserID; }
  bool operator<(const User A) const { return UserID < A.UserID; }
  bool operator<=(const User A) const { return UserID <= A.UserID; }
  bool operator>(const User A) const { return !(*this <= A); }
  bool operator>=(const User A) const { return !(*this < A); }
};

#endif