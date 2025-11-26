#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include "FileStore.hpp"
#include "Financesys.hpp"
#include "Book.hpp"
#include "Read.hpp"
#include "UserStack.hpp"
#include "User.hpp"
#include <map>

class Manager
{
    private:
        MemoryRiver<int,1> List; int cnt=0;
        FileStore<int,Book> BookStore;
        FileStore<chars,int> Keyword_Store,ISBN_Store,BookName_Store,Author_Store;
    public:
        Manager()
        {
            List.initialise("Manager_List");
            if (std::filesystem::exists("Manager_List")) List.get_info(cnt, 1);
            else cnt=0; 
            BookStore.Init("BookStore");
            ISBN_Store.Init("ISBN_Storage"); Keyword_Store.Init("Keyword_Storage");
            BookName_Store.Init("BookName_Storage"); Author_Store.Init("Author_Storage");
        }
        ~Manager()
        {
            List.write_info(cnt,1);
        }

        bool checkChar(char c)
        {
            if(isprint(c)) return true;
            else assert(0);
            unsigned char uc = static_cast<unsigned char>(c);
            return uc >= 32 && uc <= 126;
        }

        void checkString(Read &a) {if(!a.length()) throw 0;}
        void checkEmpty(Read &a) {if(a.length()) throw 0;}

        bool check_ISBN(string &ISBN)
        {
            if(ISBN.size()>20||ISBN.size()==0) throw 0;
            for(auto c: ISBN) if(!checkChar(c)) throw 0;
            return true;
        }

        bool check_Name(string &Name)
        {
            int len=Name.size();
            if(Name.size()>62||Name.size()<3) throw 0;
            if(Name[0]!='"'||Name[len-1]!='"') throw 0;
            for(int i=1;i<len-1;++i)
                 if(!checkChar(Name[i])||Name[i]=='"') throw 0;
            return true;
        }
        
        bool check_Keyword(string &Keyword)
        {
            std::map<string,bool> key; key.clear();
            if(!check_Name(Keyword)) throw 0;
            string cur=""; int len=Keyword.size();
            for(int i=1;i<len-1;++i)
            {
                if(Keyword[i]=='|') 
                {
                    if(cur.size()==0) throw 0;
                    if(key.find(cur)!=key.end()) throw 0;
                    key[cur]=1;
                    cur="";
                }
                else cur+=Keyword[i];
            }
            if(cur.size()==0) throw 0;
            if(key.find(cur)!=key.end()) throw 0;
            return true;
        }

        bool checkDiv(string &Keyword)
        {
            int len=Keyword.size();
            for(int i=1;i<len-1;++i) if(Keyword[i]=='|') throw 0;
            return true;
        }

        static bool ISBN_cmp(const Book &A,const Book &B) 
        {
            return A.ISBN<B.ISBN;
        }

        bool find_ID(int id,Book &obj)
        {
            std::vector<Book> res=BookStore.qry(id);
            if(res.size()==0) throw 0;
            obj=res[0]; return true;
        }

        bool find_ISBN(string &ISBN,Book &obj)
        {
            std::vector<int> res=ISBN_Store.qry((chars)(ISBN));
            if(res.size()==0) return false;
            find_ID(res[0],obj); return true;
        }

        bool find_BookName(string &BookName,std::vector<Book> &obj)
        {
            std::vector<int> res=BookName_Store.qry((chars)(BookName));
            if(res.size()==0) return false;
            obj.resize((int)(res.size()));
            for(int i=0;i<obj.size();++i) find_ID(res[i],obj[i]);
            sort(obj.begin(),obj.end(),ISBN_cmp);
            return true;
        }

        bool find_Keyword(string &Keyword,std::vector<Book> &obj)
        {
            std::vector<int> res=Keyword_Store.qry((chars)(Keyword));
            if(res.size()==0) return false;
            obj.resize((int)(res.size()));
            for(int i=0;i<obj.size();++i) find_ID(res[i],obj[i]);
            sort(obj.begin(),obj.end(),ISBN_cmp);return true;
        }

        bool find_Author(string &Author,std::vector<Book> &obj)
        {
            std::vector<int> res=Author_Store.qry((chars)(Author));
            if(res.size()==0) return false;
            // std::cerr<<res[0]<<'\n';
            obj.resize((int)(res.size()));
            for(int i=0;i<res.size();++i)
            {
                // std::cerr<<res[i]<<' ';
                find_ID(res[i],obj[i]);
            } 
            // std::cerr<<'\n';
            sort(obj.begin(),obj.end(),ISBN_cmp);
            // for(auto x: obj) std::cerr<<x<<'\n'; 
            return true;
        }
        
        void print (std::vector<Book> res)
        {
            // std::cout<<res.size()<<'\n';
            // std::cerr<<"PRINT "<<res.size()<<'\n';
            if(res.size()==0) std::cout<<'\n';
            for(auto vec: res) std::cout<<vec;
            // std::cout<<'\n';
            // std::cerr<<"DONE\n";
        }

        string getOpt(string msg)
        {
            string st="";
            for(auto c: msg)
            {
                if(c=='=') return st;
                else st+=c;
            }
            throw 0;
        }

        string getWord(string msg)
        {
            string st="";
            int pos=-1;
            for(int i=0;i<msg.size();++i)
            {
                if(msg[i]=='=') {pos=i; break;}
            }
            if(pos==-1) throw 0;
            for(int i=pos+1;i<msg.size();++i) st+=msg[i];
            return st;
        }

        void show(Read &a)
        {
            User cur=Stack.getTop();
            if(cur.Privilege<1) throw 0;
            if(a.length()==0)
            {
                std::vector<Book> res(cnt); 
                for(int i=1;i<=cnt;++i) find_ID(i,res[i-1]);
                sort(res.begin(),res.end(),ISBN_cmp);
                print(res);
                return;
            }
            
            string msg=a.get_string();
            checkEmpty(a);
            string opt=getOpt(msg),word=getWord(msg);
            if(opt.size()==0||word.size()==0) throw 0;
            // std::cerr<<"Show msg obtained:"<<opt<<' '<<word<<'\n';
            if(opt=="-ISBN")
            {
                Book res;
                if(!check_ISBN(word)) throw 0;
                if(find_ISBN(word,res))
                {
                    std::cout<<res;                    
                }else std::cout<<'\n';
            }else if(opt=="-name")
            {
                std::vector<Book> res;
                if(!check_Name(word)) throw 0;
                word.erase(0,1),word.erase(word.size()-1,1);
                if(find_BookName(word,res))
                {
                    print(res);
                }else std::cout<<'\n';
            }
            else if(opt=="-keyword")
            {
                std::vector<Book> res;
                if(!check_Keyword(word)) throw 0;
                if(!checkDiv(word)) throw 0;
                word.erase(0,1),word.erase(word.size()-1,1);
                if(find_Keyword(word,res))
                {
                    print(res);
                }else std::cout<<'\n';
            }else if(opt=="-author")
            {
                std::vector<Book> res;
                if(!check_Name(word)) throw 0;
                word.erase(0,1),word.erase(word.size()-1,1);
                // std::cerr<<word<<'\n';
                if(find_Author(word,res))
                {
                    // std::cerr<<"Found "<<res.size()<<'\n';
                    print(res);
                }else std::cout<<'\n';
            }else throw 0;
        }   

        void select(Read &a)
        {
            User cur=Stack.getTop();
            if(cur.Privilege<3) throw 0;
            checkString(a);
            string msg=a.get_string();
            if(!check_ISBN(msg)) throw 0;
            checkEmpty(a);
            Stack.buc.UserList.del(cur.UserID,cur);
            std::vector<int> res=ISBN_Store.qry(msg);
            // std::cerr<<"select msg obtained\n";
            if(res.size()==0)
            {
                int ID=++cnt;
                Book newBook=Book(cnt);
                newBook.ISBN=msg;
                BookStore.ins(ID,newBook);
                ISBN_Store.ins(msg,ID);
                Stack.curBook.pop_back();
                Stack.curBook.push_back(cnt);
            }else{
                Stack.curBook.pop_back();
                Stack.curBook.push_back(res[0]);
            }
            Stack.buc.UserList.ins(cur.UserID,cur);
        }

    void buy(Read &a)
    {
        // std::cerr<<"Create a trade\n";
        User cur=Stack.getTop();
        if(cur.Privilege<1) throw 0;
        checkString(a);
        string ISBN=a.get_string();
        checkString(a);
        if(!check_ISBN(ISBN)) throw 0;
        Book target;
        if(!find_ISBN(ISBN,target)) throw 0;
        string quat_st=a.get_string();
        checkEmpty(a);
        // std::cerr<<"Trade msg obtained:"<<ISBN<<' '<<quat_st<<' '<<quat_st.size()<<'\n';
        if(!isInteger(quat_st)) throw 0;
        int quantity=string_to_int(quat_st);
        // std::cerr<<"Integer get:"<<quantity<<'\n';
        // std::cerr<<"Storage: "<<target.Quantity<<'\n';
        if(quantity<=0||quantity>target.Quantity) throw 0;

        BookStore.del(target.ID,target);
        target.Quantity-=quantity;
        std::cout<<std::fixed<<std::setprecision(2)<<1.0*quantity*target.Price<<'\t'<<std::setprecision(0)<<'\n';
        BookStore.ins(target.ID,target);

        Finance rec(Finance_Record.cnt+1,1.0*quantity*target.Price,0);
        Finance_Record.update(rec);
    }
    
    void check_modify(Read &a)
    {
        User cur=Stack.getTop();
        // std::cout<<"USER:"<<cur.UserID<<' '<<cur.Privilege<<' ';
        // std::cout<<"BOOKSTACK"<<Stack.curBook.size()<<' ';
        if(cur.Privilege<3) throw 0;
        if(Stack.getBook()==-1) throw 0;
        // std::cout<<"HaveBook Permitted";
        std::map<string,bool> op;
        op.clear();

        // std::cerr<<"modify_checking started.\n";
        // std::cerr<<a.length()<<'\n';
        checkString(a);
        while(a.length()!=0)
        {
            string msg=a.get_string();
            // checkEmpty(a);
            string opt=getOpt(msg),word=getWord(msg);

            // std::cerr<<"msg: "<<opt<<' '<<word<<'\n';
            if(opt.size()==0||word.size()==0) throw 0;
            if(op.find(opt)!=op.end()) throw 0;
            op[opt]=1;            

            if(opt=="-ISBN")
            {
                Book trial;
                if(!check_ISBN(word)) throw 0;
                if(find_ISBN(word,trial)) throw 0;

            }else if(opt=="-name")
            {
                if(!check_Name(word)) throw 0;
            }
            else if(opt=="-author")
            {
                if(!check_Name(word)) throw 0;
            }else if(opt=="-keyword")
            {
                if(!check_Keyword(word)) throw 0;
                
            }else if(opt=="-price")
            {
                if(word.size()>13) throw 0;
                double price=string_to_double(word);
                if(price<0) throw 0;
            }else throw 0;
        }
    }

    void modify(Read &a)
    {
        Read cop=a;
        check_modify(cop);

        // std::cerr<<"modify_check passed.\n";

        User cur=Stack.getTop();
        Book selected;
        find_ID(Stack.getBook(),selected);
        BookStore.del(selected.ID,selected);
        ISBN_Store.del(selected.ISBN,selected.ID);
        Author_Store.del(selected.Author,selected.ID);
        BookName_Store.del(selected.BookName,selected.ID);
        while(a.length())
        {
            string msg=a.get_string();
            // checkEmpty(a);
            string opt=getOpt(msg),word=getWord(msg);
            if(opt!="-ISBN"&&opt!="-price") word.erase(0,1),word.erase(word.size()-1,1);
            if(opt=="-ISBN")
            {
                selected.ISBN=(Chars)(word);
            }else if(opt=="-name")
            {
                selected.BookName=(Chars)(word);
            }
            else if(opt=="-author")
            {
                // std::cerr<<"Modify to"<<word<<'\n';
                selected.Author=(Chars)(word);
            }else if(opt=="-keyword")
            {
                selected.Keyword;
                string cur="";
                for(auto c: String(selected.Keyword))
                {
                    if(c=='|')
                    {
                        Keyword_Store.del(cur,selected.ID);
                        cur="";
                    }else cur+=c;
                }
                Keyword_Store.del(cur,selected.ID);
                cur="";
                selected.Keyword=(Chars)(word);
                for(auto c: String(selected.Keyword))
                {
                    if(c=='|')
                    {
                        Keyword_Store.ins(cur,selected.ID);
                        cur="";
                    }else cur+=c;
                }
                Keyword_Store.ins(cur,selected.ID);

            }else if(opt=="-price")
            {
                double price=string_to_double(word);
                selected.Price=price;
            }
        }
        BookStore.ins(selected.ID,selected);
        ISBN_Store.ins(selected.ISBN,selected.ID);
        Author_Store.ins(selected.Author,selected.ID);
        BookName_Store.ins(selected.BookName,selected.ID);
    }
    void import(Read &a)
    {
        User cur=Stack.getTop();
        if(cur.Privilege<3) throw 0;
        checkString(a);
        string quant_st=a.get_string();
        checkString(a);
        string totCost_st=a.get_string();
        checkEmpty(a);
        if(!isInteger(quant_st)) throw 0;
        int quantity=string_to_int(quant_st);
        if(quantity<=0) throw 0;
        if(totCost_st.size()>13) throw 0;
        double totCost=string_to_double(totCost_st);
        if(totCost<=0) throw 0; 
        if(Stack.getBook()==-1) throw 0;
        Book selected;
        find_ID(Stack.getBook(),selected);
        BookStore.del(selected.ID,selected);
        selected.Quantity+=quantity;
        BookStore.ins(selected.ID,selected);
        
        Finance rec(Finance_Record.cnt+1,0,totCost);
        Finance_Record.update(rec);
    }
}BookManager;


#endif
