#ifndef BOOK_HPP
#define BOOK_HPP
#include<iostream>
#include<cstring>
#include<string.h>
#include<iomanip>
#include "Chars.hpp"

class Book
{
    private: 
        int ID;
        chars ISBN,Keyword,BookName,Author;
        int Quantity;
        double Price;
        friend std::ostream &operator  << (std:: ostream &, const Book &);
        friend class Manager;
        friend bool cmp(Book,Book);
    public:
        Book(){ ID=Quantity=0; Price=0;}
        Book(int id) {ID=id; Quantity=0; Price=0;}
        Book(std::string &s)
        {
            ISBN=chars(s);
            ID=Quantity=0; Price=0;
        }
        Book(int id,std::string &s)
        {
            ISBN=chars(s);
            ID=id; Quantity=0; Price=0;
        }
        Book(int id,std::string &s,int num,double pri)
        {
            ID=id; Quantity=num; ISBN=chars(s);
            Price=pri;
        }
        bool operator == (const Book &A) const
        {
            return ID==A.ID;
        }
        bool operator < (const Book &A) const
        {
            return ID<A.ID;
        }
        bool operator <= (const Book &A) const
        {
            return ID<=A.ID;
        }
        bool operator > (const Book &A) const
        {
            return !((*this) <= A);
        }
        bool operator >= (const Book &A) const
        {
            return !((*this)>A);
        }
};
std::ostream &operator << (std:: ostream &out,const Book & obj)
{
    out << obj.ISBN <<'\t'<<obj.BookName<<'\t'<<obj.Author<<'\t'<<obj.Keyword<<'\t'<<std::fixed<<std::setprecision(2)<<obj.Price<<std::setprecision(0)<<'\t'<<obj.Quantity<<'\n';
    return out;
}


#endif
