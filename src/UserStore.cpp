#include "UserStore.hpp"
#include <iostream>

UserStore::UserStore() {
    UserList.Init("UserList");
    User su("root", "sjtu", "root", 7);
    string id = "root";
    std::vector<User> res = UserList.qry((chars)(id));
    if (res.size() == 0)
        UserList.ins(id, su);
}

bool UserStore::inStore(string ID) {
    std::vector<User> res = UserList.qry((chars)(ID));
    if (res.size() == 0) return false;
    return true;
}

bool UserStore::get_user(string ID, User &cur) {
    std::vector<User> res = UserList.qry((chars)(ID));
    if (res.size() == 0) return false;
    cur = res[0];
    return true;
}

void UserStore::user_create(string ID, string passwd, string username, int pri) {
    User cur(ID, passwd, username, pri);
    if (inStore(ID)) throw 0;
    UserList.ins(ID, cur);
}

void UserStore::user_delete(string ID) {
    if (!inStore(ID)) throw 0;
    std::vector<User> res = UserList.qry((chars)(ID));
    UserList.del((chars)(ID), res[0]);
}

void UserStore::change_password(string ID, string OldPassword, string CurPassword) {
    if (!inStore(ID)) throw 0;
    std::vector<User> res = UserList.qry((chars)(ID));
    if (res[0].password != OldPassword) throw 0;
    UserList.del(ID, res[0]);
    res[0].password = CurPassword;
    UserList.ins(ID, res[0]);
}

void UserStore::change_password(string ID, string CurPassword) {
    if (!inStore(ID)) throw 0;
    std::vector<User> res = UserList.qry((chars)(ID));
    UserList.del(ID, res[0]);
    res[0].password = CurPassword;
    UserList.ins(ID, res[0]);
}