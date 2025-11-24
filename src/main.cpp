#include "Manager.hpp"
#include "User.hpp"
#include "UserStack.hpp"
#include "read.hpp"
#include "Financesys.hpp"
#include <string>
#include <iostream>

std::vector<std::pair<chars,string> > LOG;
int main()
{
    int batch=0;
    while(!std::cin.eof())
    {
        ++batch;
        try{
            std::string st;
            std::cin>>st;
            string stcop=st;
            Read a(st);
            if(!a.length()) continue;
            string op=a.get_string();
            if(op=="quit"||op=="exit") return 0;
            else if(op=="su")
            {
                Stack.Login(a);
            }else if(op=="logout")
            {
                if(a.length()) throw 0;
                Stack.Logout();
            }else if(op=="passwd"){
                Stack.Password(a);
            }else if(op=="useradd")
            {
                Stack.Create(a);
            }else if(op=="delete")
            {
                Stack.Delete(a);
            }else if(op=="show")
            {
                Read cop=a;
                if(cop.length()&&cop.get_string()=="finance")
                {
                    string tmp=a.get_string();
                    Finance_Record.print(a);
                }else BookManager.show(a);
            }else if(op=="buy")
            {
                BookManager.buy(a);
            }else if(op=="select")
            {
                BookManager.select(a);
            }else if(op=="modify")
            {
                BookManager.modify(a);
            }else if(op=="import")
            {
                BookManager.import(a);
            }else if(op=="log")
            {
                std::cout<<"LOG:"<<'\n';
                for(auto x: LOG) std::cout<<x.first<<": "<<x.second<<'\n';
            }
            User cur=Stack.getTop();
            LOG.push_back(std::make_pair(cur.UserID,stcop));
        }catch(int)
        {
            std::cout<<"Request Failed\n";
        }
    }
    return 0;
}