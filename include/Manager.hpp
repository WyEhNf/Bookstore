#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Book.hpp"
#include "FileStore.hpp"
#include "Financesys.hpp"
#include "Read.hpp"
#include "User.hpp"
#include "UserStack.hpp"
#include <algorithm>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <filesystem>

extern Finance_Sys Finance_Record; 
extern UserStack Stack; 

using std::string;

class Manager {
private:
    MemoryRiver<int, 1> List;
    int cnt = 0;
    FileStore<int, Book> BookStore;
    FileStore<chars, int> Keyword_Store, ISBN_Store, BookName_Store, Author_Store;

public:
    Manager();
    ~Manager();

    bool checkChar(char c) {
        unsigned char uc = static_cast<unsigned char>(c);
        return uc >= 32 && uc <= 126;
    }

    void checkString(Read &a) {if (!a.length()) throw 0;}
    void checkEmpty(Read &a) {if (a.length()) throw 0;}

    bool check_ISBN(string &ISBN);
    bool check_Name(string &Name);
    bool check_Keyword(string &Keyword);
    bool checkDiv(string &Keyword);

    static bool ISBN_cmp(const Book &A, const Book &B);

    bool find_ID(int id, Book &obj);
    bool find_ISBN(string &ISBN, Book &obj);
    bool find_BookName(string &BookName, std::vector<Book> &obj);
    bool find_Keyword(string &Keyword, std::vector<Book> &obj);
    bool find_Author(string &Author, std::vector<Book> &obj);

    void print(std::vector<Book> res);

    string getOpt(string msg);
    string getWord(string msg);

    void show(Read &a);
    void select(Read &a);
    void buy(Read &a);
    void check_modify(Read &a);
    void modify(Read &a);
    void import(Read &a);
};

extern Manager BookManager; 

#endif