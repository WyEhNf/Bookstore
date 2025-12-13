#include "Financesys.hpp"
#include <iostream>
#include <algorithm>

Finance_Sys Finance_Record;

Finance_Sys::Finance_Sys() {
    FinanceStore.Init("Finance_File");
    if (std::filesystem::exists("Finance_File_tot")) {
        FinanceList.initialise("Finance_File_tot");
        FinanceList.get_info(cnt, 1);
    } else
        FinanceList.initialise("Finance_File_tot"), cnt = 0;
}

Finance_Sys::~Finance_Sys() { FinanceList.write_info(cnt, 1); }

void Finance_Sys::update(Finance cur) {
    if (!cnt) {
        ++cnt;
        FinanceStore.ins(cnt, cur);
    } else {
        std::vector<Finance> res = FinanceStore.qry(cnt);
        cnt++;
        res[0] = res[0] + cur;
        FinanceStore.ins(cnt, res[0]);
    }
}

void Finance_Sys::print(Read &a) {
    User cur = Stack.getTop();
    if (cur.Privilege < 7) {
        throw 0;
    }
    Finance res = Finance();
    if (a.length() == 0) {
        if (cnt)
            res = *(FinanceStore.qry(cnt).begin());
        std::cout << res << '\n';
    } else {
        string Count_st = a.get_string();
        if (!isInteger(Count_st)) throw 0;
        int Count = string_to_int(Count_st);
        if (a.length()) throw 0;
        if (Count > cnt) throw 0;
        if (Count < 0) throw 0;
        if (Count == 0) {
            std::cout << '\n';
            return;
        }
        if (Count == cnt) {
            std::cout << *(FinanceStore.qry(cnt).begin()) << '\n';
            return;
        }
        Finance R = *(FinanceStore.qry(cnt).begin());
        Finance L = *(FinanceStore.qry(cnt - Count).begin());
        std::cout << (R - L) << '\n';
    }
}