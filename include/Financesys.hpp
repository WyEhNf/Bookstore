#ifndef FINANCESYS_HPP
#define FINANCESYS_HPP

#include "FileStore.hpp"
#include "Finance.hpp"
#include "MemoryRiver.hpp"
#include "Read.hpp"
#include "UserStack.hpp"
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

using std::string;

extern UserStack Stack;

class Finance_Sys {
private:
    MemoryRiver<Finance, 1> FinanceList;
    FileStore<int, Finance> FinanceStore;
    int cnt;
    friend class Manager;

public:
    Finance_Sys();
    ~Finance_Sys();
    void update(Finance cur);
    void print(Read &a);
};

extern Finance_Sys Finance_Record;

#endif