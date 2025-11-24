#ifndef FILESTORE_HPP
#define FILESTORE_HPP


#include<iostream>
#include<vector> 
#include<cstring> 
#include <fstream>
#include <filesystem>
#include "MemoryRiver.hpp"
#include "Chars.hpp"
const int SIZE=300;

template <typename indextype,typename valuetype>
class FileStore
{
    class data
    {
        private:
            indextype index;
            valuetype value;
            friend class block;
            friend class FileStore;

        public:
            data() { index=indextype(); value=valuetype();}
            data(indextype ind, valuetype val) : index(ind),value(val) {}
            bool operator == (const data &A) const
            {
                return index==A.index && value==A.value;
            }
            bool operator != (const data &A) const
            {
                return index!=A.index||value!=A.value;
            }
            bool operator < (const data &A) const
            {
                return (index==A.index) ? value<A.value : index<A.index;
            }
            bool operator <= (const data &A) const
            {
                return (*this==A) ? 1 :  ((index==A.index) ? value<A.value : index<A.index);
            }
            bool operator > (const data &A) const
            {
                return (index==A.index) ? value>A.value : index>A.index;
            }
            bool operator >= (const data &A) const
            {
                return !(*this<A) ;
            }
    };
    
    class node
    {
        private: 
            data mx; 
            int size,id,nxt;
            friend class FileStore; 
        public:
            node(int size=0,int id=0,int nxt=-1):size(size),id(id),nxt(nxt){
                mx=data();
            }
    };
    class block
    {
        private:
            data a[SIZE+10];
            friend class FileStore;
        public:
            block(){};
            data& operator[] (int pos)
            {
                return a[pos];
            }
    };
    
    private:
        int head;
        MemoryRiver <block,0> array;
        MemoryRiver <node,1> head_list;

    public:

        FileStore() {
           head_list.initialise("_head");
            if (!std::filesystem::exists("_body")) head = -1;
                else head_list.get_info(head, 1);
            array.initialise("_body");
        }
        void Init(string StoreName="")
        {
            head_list.initialise(StoreName+"_head");
            if (!std::filesystem::exists(StoreName+"_body")) head = -1;
                else head_list.get_info(head, 1);
            array.initialise(Storename"_body");
        }

        ~FileStore() {
            head_list.write_info(head, 1);
        }
        void ins(const indextype index,const valuetype &value)
        {
            data cur=data(index,value);
            node point; block Store;
            int pos=head;
            if(head==-1)
            {
                point.id=array.write(Store);
                head=head_list.write(point);
                pos=head;
                // std::cerr<<"?? head"<<head<<'\n';
            }
            else{

                while(head!=-1)
                {
                    head_list.read(point,pos);
                    if(point.size==0||point.nxt==-1||point.mx>=cur) {break;}
                    pos=point.nxt;
                }
                array.read(Store,point.id);
            }

            // std::cerr<<"HERE\n";

            int len=point.size,p=-1;
            // std::cerr<<"Length of the block: "<<len<<"  head: "<<head<<" pos "<<pos<<'\n';
            if(len==0) Store[++point.size]=cur;
            else{
                for(int i=1;i<=len;++i)
                {
                    if(Store[i]>cur) {p=i; break;}
                }
                if(p==-1) Store[len+1]=cur,++point.size;
                else
                {
                    for(int i=len;i>=p;--i) Store[i+1]=Store[i];
                    Store[p]=cur;
                    ++point.size;
                }
            }
            // std::cerr<<"PRINT:\n";
            // for(int i=1;i<=point.size;++i)
            // {
            //     std::cerr<<Store[i].index<<' '<<Store[i].value<<'\n';
            // }
            // std::cerr<<"point: "<<point.id<<' '<<point.size<<'\n';
            if(cur>point.mx) point.mx=cur;
            
            // std::cerr<<"HERE\n";

            if(point.size>SIZE)
            {
                node new_point; block new_Store;
                new_point.mx=Store[point.size];
                int cut_pos=point.size-(point.size>>1);
                // std::cerr<<"CUT:"<<cut_pos<<'\n';
                for(int i=cut_pos+1;i<=point.size;++i) new_Store[i-cut_pos]=Store[i],new_point.mx=std::max(new_point.mx,Store[i]),Store[i]=data();
                point.mx=Store[1];
                for(int i=1;i<=cut_pos;++i) point.mx=std::max(Store[i],point.mx);
                new_point.nxt=point.nxt; new_point.size=point.size-cut_pos;
                new_point.id=array.write(new_Store);
                point.nxt=head_list.write(new_point);
                point.size=cut_pos;
                array.update(Store,point.id);
                head_list.update(point,pos);
            }else{
                array.update(Store,point.id);
                head_list.update(point,pos);
            }
            return;
        }

        void del(const indextype index, const valuetype &value)
        {
            if(head==-1) return;
            data cur=data(index,value);
            node point; int pos,pre;
            block Store;

            pos=pre=head;
            while(head!=-1)
            {
                head_list.read(point,pos);
                if(point.nxt==-1||point.size==0||point.mx>=cur) break;
                pre=pos; pos=point.nxt;
            }
            if(pos==-1) return;

            int p=-1,len=point.size;
            array.read(Store,point.id);
            for(int i=1;i<=len;++i) if(Store[i]==cur) {p=i; break;}
            if(p==-1) return;
            for(int i=p;i<len;++i) Store[i]=Store[i+1];
            Store[len]=data();
            point.size--;
            
            if(point.size==0)
            {
                if(head==pos) {head=point.nxt; return;}
                node pre_point;
                head_list.read(pre_point,pre);
                pre_point.nxt=point.nxt;
                head_list.update(pre_point,pre);
            }else{
                point.mx=Store[1];
                for(int i=2;i<=point.size;++i) point.mx=std::max(point.mx,Store[i]);
                array.update(Store,point.id);
                head_list.update(point,pos);
            }
            return;
        }
        std::vector<valuetype> qry(const indextype index)
        {
            std::vector<valuetype> res;
            // std::cerr<<"QRY: "<<index<<'\n';
            if(head==-1) return res;
            // std::cerr<<"GO\n";
            valuetype zero;
            data cur=data(index,zero);
            node point; 
            block Store;
            int pos=head;
            // std::cerr<<"HEAD"<<head <<" CUR:"<<cur.index<<' '<<cur.value<<'\n';
            while(head!=-1)
            {
                head_list.read(point,pos);
                // std::cerr<<point.mx.index<<' '<<point.mx.value<<'\n';
                if(point.nxt==-1||point.size==0||point.mx.index==cur.index||point.mx>cur) break;
                pos=point.nxt;
            }
            // std::cerr<<"BLK:"<<pos<<' '<<point.size<<'\n';
            array.read(Store,point.id);
            int p=-1,len=point.size;
            for(int i=1;i<=len;++i) 
            {
                // std::cerr<<Store[i].index<<'\n';
                if(Store[i].index==cur.index){p=i; break;}
            }
            if(p==-1) return res;
            // std::cerr<<"GO2\n";
            while(Store[p].index==cur.index)
            {
                res.push_back(Store[p].value); ++p;
                if(p>point.size)
                {
                    pos=point.nxt;
                    if(pos==-1) break;
                    head_list.read(point,pos);
                    array.read(Store,point.id);
                    p=1;
                }
            }
            return res;
        }

};
// int main()
// {

//     int n; std::cin>>n;
//     // std::cout<<"?"<<n<<'\n';
//     FileStore <string,int> F;
//     // F.Init();
//     string index;
//     for(int i=1;i<=n;++i)
//     {
//         string opt; std::cin>>opt;
//         int value;
//         if(opt=="insert")
//         {
//             std::cin>>index>>value;
//             F.ins(index,value);
//         }else if(opt=="delete")
//         {
//             std::cin>>index>>value;
//             F.del(index,value);
//             // std::cerr<<i<<'\n';
//         }else if(opt=="find")
//         {
//             std::cin>>index;
//             std::vector<int> res=F.qry(index);
//             if(res.size()==0) std::cout<<"null"<<std::endl;
//             else
//             {
//                 for(auto x: res)
//                 {
//                     std::cout<<x<<' ';
//                 }
//                 std::cout<<std::endl;
//             }
//             // std::cerr<<i<<'\n';
//         }
//     }
//     return 0;
// }
#endif