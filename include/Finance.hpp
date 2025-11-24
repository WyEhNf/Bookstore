#ifndef FINANCE_HPP
#define FINANCE_HPP

#include<iostream>
#include<iomanip>

class Finance
{
    private:
        int id;
        double income,outcome;
        friend class Finance_Sys;
        friend std::ostream &operator <<(std::ostream &out, const Finance &now);
    public:
        Finance(){ id=0; income=outcome=0;}
        Finance(int ID){ id=ID; income=outcome=0;}
        Finance(int ID,double inCome,double outCome){id=ID; income=inCome;outcome=outCome;}
        bool operator == (const Finance &A) const
        {
            return id==A.id;
        }
        bool operator != (const Finance &A) const
        {
            return id==A.id;
        }
        bool operator < (const Finance &A) const
        {
            return id<A.id;
        }
        bool operator <= (const Finance &A) const
        {
            return id<=A.id;
        }
        bool operator > (const Finance &A) const
        {
            return !(*this<=A);
        }
        bool operator >= (const Finance &A) const
        {
            return !(*this<A);
        }
        Finance operator + (const Finance &A) const{
            Finance res=*this;
            res.income+=A.income,res.outcome+=A.outcome;
            return res;
        }
        Finance operator - (const Finance &A) const{
            Finance res=*this;
            res.income-=A.income,res.outcome-=A.outcome;
            return res;
        }
};

std::ostream &operator <<(std::ostream &out, const Finance &now) {
    out << std::fixed << std::setprecision(2) << "+ " << now.income << " - " << now.outcome << std::fixed << std::setprecision(0);
    return out;
}
#endif