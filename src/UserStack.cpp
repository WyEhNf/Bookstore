#include "UserStack.hpp"
#include <iostream>
#include <cctype>
#include <algorithm>

// 假设这些函数在其他地方定义
// int string_to_int(const std::string& s);
// bool isInteger(const std::string& s);


UserStack Stack;

UserStack::UserStack() {
    Stack.clear();
    curBook.clear();
}

void UserStack::checkString(Read &a) {if (!a.length()) throw 0;}
void UserStack::checkEmpty(Read &a) {if (a.length()) throw 0;}

User UserStack::getTop() {
    if (Stack.empty()) return User();
    else {
        int len = Stack.size();
        std::vector<User> res = buc.UserList.qry(Stack[len - 1]);
        return res[0];
    }
}

int UserStack::getBook() {
    if (Stack.empty() || curBook.empty())
        return -1;
    else return curBook[curBook.size() - 1];
}

bool UserStack::checkString(string ID) {
    if (ID.size() > 30 || ID.size() == 0) throw 0;
    for (auto c : ID) {
        if (!std::isdigit(c) && !std::isalpha(c) && c != '_')
            throw 0;
    }
    return true;
}

bool UserStack::checkName(string Name) {
    if (Name.size() > 30 || Name.size() == 0) throw 0;
    for (auto c : Name) {
        if (!checkChar(c))
            throw 0;
    }
    return true;
}

bool UserStack::checkPrivilege(int pri) {
    if (pri != 0 && pri != 1 && pri != 3 && pri != 7) throw 0;
    return true;
}

void UserStack::Login(Read &a) {
    checkString(a);
    string ID = a.get_string();
    checkString(ID);
    if (a.length() != 0) {
        string passwd = a.get_string();
        checkString(passwd);
        if (a.length()) throw 0;
        User cur;
        if (!buc.get_user(ID, cur)) throw 0;
        if ((Chars)(passwd) != cur.password) throw 0;
        Stack.push_back(ID);
    } else {
        User cur;
        if (!buc.get_user(ID, cur)) throw 0;
        if (getTop().Privilege <= cur.Privilege) throw 0;
        Stack.push_back(ID);
    }
    curBook.push_back(-1);
}

void UserStack::Logout() {
    User op = getTop();
    if (op.Privilege < 1) throw 0;
    if (Stack.empty()) throw 0;
    User cur = getTop();
    buc.UserList.del(cur.UserID, cur);
    curBook.pop_back();
    Stack.pop_back();
    buc.UserList.ins(cur.UserID, cur);
}

void UserStack::Register(Read &a) {
    checkString(a);
    string ID = a.get_string();
    checkString(ID); checkString(a);
    string passwd = a.get_string();
    checkString(passwd); checkString(a);
    string username = a.get_string();
    checkName(username); checkEmpty(a);
    if (buc.inStore(ID)) throw 0;
    buc.user_create(ID, passwd, username, 1);
}

void UserStack::Password(Read &a) {
    User op = getTop();
    if (op.Privilege < 1) throw 0;
    checkString(a);
    string ID = a.get_string();
    checkString(ID);
    User target;
    if (!buc.get_user(ID, target)) throw 0;
    checkString(a);
    string pass1 = a.get_string();
    checkString(pass1);
    if (!a.length()) {
        User cur = getTop();
        if (cur.Privilege <= target.Privilege && cur.Privilege != 7)
            throw 0;
        buc.change_password(ID, pass1);
    } else {
        string pass2 = a.get_string();
        checkString(pass2);
        checkEmpty(a);
        if (target.password != pass1)
            throw 0;
        buc.change_password(ID, pass1, pass2);
    }
}

void UserStack::Create(Read &a) {
    User op = getTop();
    if (op.Privilege < 3) throw 0;
    checkString(a);
    string ID = a.get_string();
    checkString(ID); checkString(a);
    User cur;
    if (buc.get_user(ID, cur)) throw 0;
    string passwd = a.get_string();
    checkString(passwd); checkString(a);
    string pri_st = a.get_string();
    if (pri_st.size() > 1 || pri_st.size() == 0) throw 0;
    checkString(a);
    int pri = string_to_int(pri_st);
    if (!checkPrivilege(pri)) throw 0;
    if (!isInteger(pri_st)) throw 0;
    if (pri >= op.Privilege) throw 0;
    string username = a.get_string();
    checkName(username); checkEmpty(a);
    buc.user_create(ID, passwd, username, pri);
}

void UserStack::Delete(Read &a) {
    checkString(a);
    User cur = getTop();
    if (cur.Privilege != 7) throw 0;
    string ID = a.get_string();
    checkString(ID); checkEmpty(a);
    User target;
    if (!buc.get_user(ID, target)) throw 0;
    for (auto x : Stack) {
        if (x == ID)
            throw 0;
    }
    buc.user_delete(ID);
}