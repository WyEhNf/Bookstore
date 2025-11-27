#ifndef USERSTACK_HPP
#define USERSTACK_HPP

#include <assert.h>
#include <iostream>
#include <vector>
#include "User.hpp"
#include "UserStore.hpp"
#include "Chars.hpp"
#include "FileStore.hpp"
#include "Read.hpp"


class UserStack
{
    private:
        std::vector<chars> Stack;
        std::vector<int> curBook;
        UserStore buc;
        friend class Manager;
        friend class Finance_sys;
        friend class UserStore;
    public:
        UserStack()
        {
            Stack.clear(); curBook.clear();
        }

        bool checkChar(char c)
        {
            if(isprint(c)) return true;
            else return false;
            unsigned char uc = static_cast<unsigned char>(c);
            return uc >= 32 && uc <= 126;
        }

        void checkString(Read &a) {if(!a.length()) throw 0;}
        void checkEmpty(Read &a) {if(a.length()) throw 0;}

        User getTop()
        {
            if(Stack.empty()) return User();
            else {
                int len=Stack.size();
                std::vector<User> res=buc.UserList.qry(Stack[len-1]);
                return res[0];
            }
        }
        int getBook()
        {
            if(Stack.empty()||curBook.empty()) return -1;
            else return curBook[curBook.size()-1]; 
        }

        bool checkString(string ID)
        {
            if(ID.size()>30||ID.size()==0) throw 0;
            for(auto c: ID)
            {
                if(!isdigit(c)&&!isalpha(c)&&c!='_') throw 0;
            }
            return true;
        }
        
        bool checkName(string Name)
        {
            if(Name.size()>30||Name.size()==0) throw 0;
            for(auto c: Name)
            {
                if(!checkChar(c)) throw 0;
            }
            return true;
        }

        bool checkPrivilege(int pri)
        {
            if(pri!=0&&pri!=1&&pri!=3&&pri!=7) throw 0;
            return true;
        }

        void Login(Read &a)
        {
            checkString(a);
            string ID=a.get_string();
            // std::cerr<<"ID "<<ID<<'\n';
            if(a.length()!=0)
            {
                string passwd=a.get_string();
                // std::cerr<<passwd<<'\n';
                if(a.length()) throw 0;
                User cur;
                // std::cerr<<"!!\n";
                if(!buc.get_user(ID,cur)) throw 0;
                // std::cout<<"GET";
                if((Chars)(passwd)!=cur.password) throw 0;
                Stack.push_back(ID);
            }else{
                User cur;
                if(!buc.get_user(ID,cur)) throw 0;
                if(getTop().Privilege<=cur.Privilege) throw 0;
                Stack.push_back(ID);
            }
            curBook.push_back(-1);
        }
        void Logout()
        {
            if(Stack.empty()) throw 0;
            User cur=getTop();
            buc.UserList.del(cur.UserID,cur);
            curBook.pop_back(); Stack.pop_back();
            buc.UserList.ins(cur.UserID,cur);
        }
        void Register(Read &a)
        {
            checkString(a);
            string ID=a.get_string();
            checkString(a);
            string passwd=a.get_string();
            checkString(a);
            string username=a.get_string();
            checkEmpty(a);
            if(buc.inStore(ID)) throw 0;
            buc.user_create(ID,passwd,username,1);
        }
        void Password(Read &a)
        {
            checkString(a);
            string ID=a.get_string();
            User target;
            if(!buc.get_user(ID,target)) throw 0;
            checkString(a);
            string pass1=a.get_string();
            if(!a.length())
            {
                User cur=getTop(); 
                if(cur.Privilege<=target.Privilege&&cur.Privilege!=7) throw 0;
                buc.change_password(ID,pass1);
            }else{
                string pass2=a.get_string();
                checkEmpty(a);
                if(target.password!=pass1) throw 0;
                buc.change_password(ID,pass1,pass2);
            }
        }
        void Create(Read &a)
        {
            User op=getTop();
            checkString(a);
            string ID=a.get_string();
            checkString(a);
            User cur;
            if(buc.get_user(ID,cur)) throw 0;
            string passwd=a.get_string();
            checkString(a);
            string pri_st=a.get_string();
            if(pri_st.size()>1||pri_st.size()==0) throw 0;
            checkString(a);
            int pri=string_to_int(pri_st);
            if(!checkPrivilege(pri)) throw 0;
            if(!isInteger(pri_st)) throw 0; 
            if(pri>=op.Privilege) throw 0;
            string username=a.get_string();
            checkEmpty(a);
            buc.user_create(ID,passwd,username,pri);
        }
        void Delete(Read &a)
        {
            checkString(a);
            User cur=getTop();
            if(cur.Privilege!=7) throw 0;
            string ID=a.get_string();
            checkEmpty(a);
            User target;
            if(!buc.get_user(ID,target)) throw 0;
            for(auto x: Stack)
            {
                if(x==ID) throw 0;
            }
            buc.user_delete(ID);
        }
}Stack;


#endif





