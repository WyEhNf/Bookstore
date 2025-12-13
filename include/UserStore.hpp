#ifndef USERSTORE_HPP
#define USERSTORE_HPP

#include "Chars.hpp"
#include "FileStore.hpp"
#include "User.hpp"
#include <vector>
#include <string>

using std::string;

class UserStore {
private:
    FileStore<chars, User> UserList;
    friend class UserStack;
    friend class Finance;
    friend class Manager;

public:
    UserStore();

    bool inStore(string ID);

    bool get_user(string ID, User &cur);

    void user_create(string ID, string passwd, string username, int pri);

    void user_delete(string ID);

    void change_password(string ID, string OldPassword, string CurPassword);
    
    void change_password(string ID, string CurPassword);
};

#endif