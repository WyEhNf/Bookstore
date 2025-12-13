#ifndef USERSTACK_HPP
#define USERSTACK_HPP

#include "Chars.hpp"
#include "FileStore.hpp"
#include "Read.hpp"
#include "User.hpp"
#include "UserStore.hpp"
#include <assert.h>
#include <iostream>
#include <vector>
#include <string>

using std::string;

class UserStack {
private:
    std::vector<chars> Stack;
    std::vector<int> curBook;
    UserStore buc;
    friend class Manager;
    friend class Finance_sys;
    friend class UserStore;

public:
    UserStack();

    bool checkChar(char c) {
        unsigned char uc = static_cast<unsigned char>(c);
        return uc >= 32 && uc <= 126;
    }

    void checkString(Read &a);
    void checkEmpty(Read &a);

    User getTop();
    int getBook();

    bool checkString(string ID);
    bool checkName(string Name);
    bool checkPrivilege(int pri);

    void Login(Read &a);
    void Logout();
    void Register(Read &a);
    void Password(Read &a);
    void Create(Read &a);
    void Delete(Read &a);
};

extern UserStack Stack;

#endif