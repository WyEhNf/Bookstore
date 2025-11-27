#include "../include/Manager.hpp"
#include "../include/User.hpp"
#include "../include/UserStack.hpp"
#include "../include/Read.hpp"
#include "../include/Financesys.hpp"
#include <string>
#include <assert.h>
#include <iostream>
#include <cstdio>
std::vector<std::pair<chars,string> > LOG;
inline void checkStr(Read x)
{
    Read cop=x;
    while(cop.length()!=0)
    {
        string cur=cop.get_string();
        for(auto c: cur) if(!isprint(c)) {throw 0;}
    }
}
int main()
{
    // std::freopen("../read.in","r",stdin);
    // std::freopen("../report.txt","w",stdout);
    // std::freopen("write.out","w",stdout);
    int batch=0;
    // std::cerr<<"BookStore Launched\n";
    while(!std::cin.eof())
    {
        ++batch;
        // std::cout<<batch<<' ';
        try{
            std::string st;
            getline(std::cin,st);
            for(auto c: st) if(c=='\n') assert(0);
            string stcop=st;
            Read a(st);
            checkStr(a);
            if(!a.length()) continue;
            string op=a.get_string();
            // std::cerr<<op<<'\n';
            if(op=="quit"||op=="exit") return 0;
            else if(op=="su")
            {
                Stack.Login(a);
                // std::cerr<<"Done.\n";
            }else if(op=="logout")
            {
                if(a.length()) throw 0;
                Stack.Logout();
                // std::cerr<<"Done.\n";
            }else if(op=="passwd"){
                Stack.Password(a);
                // std::cerr<<"Done.\n";
            }else if(op=="register")
            {
                Stack.Register(a);
                // std::cerr<<"Done.\n";
            }
            else if(op=="useradd")
            {
                Stack.Create(a);
                // std::cerr<<"Done.\n";
            }else if(op=="delete")
            {
                Stack.Delete(a);
                // std::cerr<<"Done.\n";
            }else if(op=="show")
            {
                Read cop=a;
                if(cop.length()&&cop.get_string()=="finance")
                {
                    string tmp=a.get_string();
                    Finance_Record.print(a);
                }else BookManager.show(a);
                // std::cerr<<"Done.\n";
            }else if(op=="buy")
            {
                BookManager.buy(a);
                // std::cerr<<"Done.\n";
            }else if(op=="select")
            {
                BookManager.select(a);
                // std::cerr<<"Done.\n";
            }else if(op=="modify")
            {
                BookManager.modify(a);
                // std::cerr<<"Done.\n";
            }else if(op=="import")
            {
                BookManager.import(a);
                // std::cerr<<"Done.\n";
            }else if(op=="log")
            {
                std::cout<<"LOG:"<<'\n';
                for(auto x: LOG) std::cout<<x.first<<": "<<x.second<<'\n';
                // std::cerr<<"Done.\n";
            }else 
            {
                throw 0;
            }
            User cur=Stack.getTop();
            LOG.push_back(std::make_pair(cur.UserID,stcop));
        }catch(int)
        {
            std::cout<<"Invalid\n";
        }
    }
    return 0;

}









