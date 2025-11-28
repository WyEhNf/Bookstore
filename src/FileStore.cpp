#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
const int SIZE = 300;

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

template <class T, int info_len = 2> class MemoryRiver {
private:
  fstream file;
  string file_name;
  int sizeofT = sizeof(T);

public:
  MemoryRiver() = default;

  MemoryRiver(const string &file_name) : file_name(file_name) {}

  ~MemoryRiver() { file.close(); }

  void initialise(string FN = "") {
    if (FN != "")
      file_name = FN;
    if (std::filesystem::exists(file_name)) {
      file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
      file.seekp(info_len * sizeof(int), std::ios_base::beg);
      file.close();
      file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
      return;
    }
    file.open(file_name, std::ios::out | std::ios::binary);
    int tmp = 0;
    for (int i = 0; i < info_len; ++i)
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
  }

  // 读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n) {
    if (n > info_len)
      return;
    file.seekg((n - 1) * sizeof(int), std::ios_base::beg);
    file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
  }

  void write_info(int tmp, int n) {
    if (n > info_len)
      return;
    file.seekp((n - 1) * sizeof(int), std::ios_base::beg);
    file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
  }

  // 在文件合适位置写入类对象t，并返回写入的位置索引index
  // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  // 位置索引index可以取为对象写入的起始位置
  int write(T &t) {
    file.seekp(0, std::ios::end);
    int index = file.tellp();
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    return index;
  }

  // 用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) {
    file.seekp(index, std::ios_base::beg);
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
  }

  // 读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index) {
    file.seekg(index, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(&t), sizeof(T));
  }

  // 删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
  void Delete(int index) { /* your code here */ }
};

struct chars {
public:
  char a[80];

  chars() { a[0] = 0; }

  chars(std::string &st) {
    int len = st.length();        
    if (len > 70) throw 0;
    for (int i = 0; i < len; i++) a[i] = st[i];
    a[len] = 0;
  }

  char &operator[](int id) { return a[id]; }

  bool operator==(const chars other) const {
    if (strlen(a) != strlen(other.a))
      return 0;
    for (int i = 0; i < strlen(a); i++)
      if (a[i] != other.a[i]) return 0;
      else if (!a[i])return 1;
    return 1;
  }
  bool operator!=(const chars other) const { return !(*this == other); }
  bool operator<(const chars other) const {
    if (*this == other)
      return 0;
    int k = std::min(strlen(a), strlen(other.a)) + 1;
    for (int i = 0; i < k; i++)
      if (a[i] != other.a[i])
        return a[i] < other.a[i];
    return 0;
  }
  bool operator<=(const chars other) const {
    return (*this == other) || (*this < other);
  }
  bool operator>(const chars other) const { return !(*this <= other); }
  bool operator>=(const chars other) const { return !(*this < other); }
};

chars Chars(std::string st) {
  int len = st.length();
  chars res;
  for (int i = 0; i < len; i++) res[i] = st[i];
  for (int i = len; i < 61; i++) res[i] = 0;
  return res;
}

std::string String(chars st) {
  int len = strlen(st.a);
  std::string res;
  res.resize(len);
  for (int i = 0; i < len; i++) res[i] = st.a[i];
  return res;
}

std::ostream &operator<<(std::ostream &out, const chars &now) {
  out << now.a;
  return out;
}

template <typename indextype, typename valuetype> class FileStore {
  class data {
  private:
    indextype index;
    valuetype value;
    friend class block;
    friend class FileStore;

  public:
    data() {
      index = indextype();
      value = valuetype();
    }
    data(indextype ind, valuetype val) : index(ind), value(val) {}
    bool operator==(const data &A) const {
      return index == A.index && value == A.value;
    }
    bool operator!=(const data &A) const {
      return index != A.index || value != A.value;
    }
    bool operator<(const data &A) const {
      return (index == A.index) ? value < A.value : index < A.index;
    }
    bool operator<=(const data &A) const {
      return (*this == A) ? 1
            : ((index == A.index) ? value < A.value : index < A.index);
    }
    bool operator>(const data &A) const {
      return (index == A.index) ? value > A.value : index > A.index;
    }
    bool operator>=(const data &A) const { return !(*this < A); }
  };

  class node {
  private:
    data mx;
    int size, id, nxt;
    friend class FileStore;

  public:
    node(int size = 0, int id = 0, int nxt = -1)
        : size(size), id(id), nxt(nxt) {
      mx = data();
    }
  };
  class block {
  private:
    data a[SIZE + 10];
    friend class FileStore;

  public:
    block() {};
    data &operator[](int pos) { return a[pos]; }
  };

private:
  int head;
  MemoryRiver<block, 0> array;
  MemoryRiver<node, 1> head_list;

public:
  FileStore() {
     head_list.initialise("_head");
      if (!std::filesystem::exists("_body")) head = -1;
          else head_list.get_info(head, 1);
      array.initialise("_body");
  }
  ~FileStore() { head_list.write_info(head, 1); }
  void ins(const indextype index, const valuetype &value) {
    data cur = data(index, value);
    node point;
    block Store;
    int pos = head;
    if (head == -1) {
      point.id = array.write(Store);
      head = head_list.write(point);
      pos = head;
    } else {

      while (head != -1) {
        head_list.read(point, pos);
        if (point.size == 0 || point.nxt == -1 || point.mx >= cur) {
          break;
        }
        pos = point.nxt;
      }
      array.read(Store, point.id);
    }

    int len = point.size, p = -1;
    if (len == 0)
      Store[++point.size] = cur;
    else {
      for (int i = 1; i <= len; ++i) {
        if (Store[i] > cur) {
          p = i;
          break;
        }
      }
      if (p == -1)
        Store[len + 1] = cur, ++point.size;
      else {
        for (int i = len; i >= p; --i)
          Store[i + 1] = Store[i];
        Store[p] = cur;
        ++point.size;
      }
    }
    if (cur > point.mx)
      point.mx = cur;
    if (point.size > SIZE) {
      node new_point;
      block new_Store;
      new_point.mx = Store[point.size];
      int cut_pos = point.size - (point.size >> 1);
      for (int i = cut_pos + 1; i <= point.size; ++i)
        new_Store[i - cut_pos] = Store[i],
                      new_point.mx = std::max(new_point.mx, Store[i]),
                      Store[i] = data();
      point.mx = Store[1];
      for (int i = 1; i <= cut_pos; ++i)
        point.mx = std::max(Store[i], point.mx);
      new_point.nxt = point.nxt;
      new_point.size = point.size - cut_pos;
      new_point.id = array.write(new_Store);
      point.nxt = head_list.write(new_point);
      point.size = cut_pos;
      array.update(Store, point.id);
      head_list.update(point, pos);
    } else {
      array.update(Store, point.id);
      head_list.update(point, pos);
    }
    return;
  }

  void del(const indextype index, const valuetype &value) {
    if (head == -1)
      return;
    data cur = data(index, value);
    node point;
    int pos, pre;
    block Store;

    pos = pre = head;
    while (head != -1) {
      head_list.read(point, pos);
      if (point.nxt == -1 || point.size == 0 || point.mx >= cur)
        break;
      pre = pos;
      pos = point.nxt;
    }
    if (pos == -1)
      return;

    int p = -1, len = point.size;
    array.read(Store, point.id);
    for (int i = 1; i <= len; ++i)
      if (Store[i] == cur) {
        p = i;
        break;
      }
    if (p == -1)
      return;
    for (int i = p; i < len; ++i)
      Store[i] = Store[i + 1];
    Store[len] = data();
    point.size--;

    if (point.size == 0) {
      if (head == pos) {
        head = point.nxt;
        return;
      }
      node pre_point;
      head_list.read(pre_point, pre);
      pre_point.nxt = point.nxt;
      head_list.update(pre_point, pre);
    } else {
      point.mx = Store[1];
      for (int i = 2; i <= point.size; ++i)
        point.mx = std::max(point.mx, Store[i]);
      array.update(Store, point.id);
      head_list.update(point, pos);
    }
    return;
  }
  std::vector<valuetype> qry(const indextype index) {
    std::vector<valuetype> res;
    if (head == -1)
      return res;
    valuetype zero;
    data cur = data(index, zero);
    node point;
    block Store;
    int pos = head;
    while (head != -1) {
      head_list.read(point, pos);
      if (point.nxt == -1 || point.size == 0 || point.mx.index == cur.index ||
          point.mx > cur)
        break;
      pos = point.nxt;
    }
    array.read(Store, point.id);
    int p = -1, len = point.size;
    for (int i = 1; i <= len; ++i) {
      if (Store[i].index == cur.index) {
        p = i;
        break;
      }
    }
    if (p == -1)
      return res;
    while (Store[p].index == cur.index) {
      res.push_back(Store[p].value);
      ++p;
      if (p > point.size) {
        pos = point.nxt;
        if (pos == -1)
          break;
        head_list.read(point, pos);
        array.read(Store, point.id);
        p = 1;
      }
    }
    return res;
  }
};
int main()
{

    int n; std::cin>>n;
    FileStore <string,int> F;
    string index;
    for(int i=1;i<=n;++i)
    {
        string opt; std::cin>>opt;
        int value;
        if(opt=="insert")
        {
            std::cin>>index>>value;
            F.ins(index,value);
        }else if(opt=="delete")
        {
            std::cin>>index>>value;
            F.del(index,value); 
        }else if(opt=="find")
        {
            std::cin>>index;
            std::vector<int> res=F.qry(index);
            if(res.size()==0) std::cout<<"null"<<std::endl;
            else
            {
                for(auto x: res)
                {
                    std::cout<<x<<' ';
                }
                std::cout<<std::endl;
            }
        }
    }
    return 0;
}
