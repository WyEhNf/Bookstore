#include<bits/stdc++.h>
using namespace std;
string a[5000],b[5000];
int A,B;
int main()
{
    freopen("compare.txt","r",stdin);
    ios::sync_with_stdio(false);
    cin.tie(0);
    for(int i=1;i<=4416;++i)
    {
        getline(cin,a[i]);
    }
    for(int i=1;i<=4417;++i)
    {
        getline(cin,b[i]);
    }
    for(int i=1;i<=4416;++i)
    {
        if(a[i]!=b[i])
        {
            cout<<"WA on Line "<<i<<'\n';
            cerr<<a[i].size()<<' '<<b[i].size()<<'\n';
            return 0;
        }
        
    }
    return 0;
}