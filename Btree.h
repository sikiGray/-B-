#include<iostream>
#include<string.h>
using namespace std;
const int M=4;  //四阶B树
const int max_name_length=50;//最大的书名、姓名长度
const int maxValue=100;//关键码即书号的最大值

class Date//日期结构体
{
public:
    int year;
    int month;
    int day;
};

class BorrowerNode//借阅者信息结点
{
public:
    int ID;//借阅者的借阅证号
    Date tBegin;//借书日期
    //Date tEnd;//还书日期
    int span;//借阅天数
    BorrowerNode *next;//指向下一个借阅者结点
};

class BookNode  //图书信息结点
{
public:
    int booknum;//书号
    char bookname[max_name_length];//书名
    char author[max_name_length];//作者名
    int current_stock;//书的现存量
    int total_stock;//书的总存量
    BorrowerNode *bpHead;//借阅者链表指针头指针
    BorrowerNode *preOder;//预约者链表指针头指针
    //BookNode(){root=NULL;}
};

class BtreeNode    //B树结点结构体
{
public:
    int keynum;  //关键码的个数
    BtreeNode *parent;  //父节点指针
    int key[M+1];   //使用书号作为关键码，
    BtreeNode *ptr[M+1];    //子树结点指针数组，ptr[m]在插入溢出时使用
    BookNode *recptr[M+1];  //关键码对应数据域指针

};

class Result    //搜索结果信息结构
{
public:
    BtreeNode *r;//查询关键码所在结点的地址指针
    int i;  //结点关键码序号
    int tag;//用来标志搜索是否成功，tag=0搜索失败，tag=1搜索成功
};

class Btree     //B树类定义
{
public:
    Btree(){root=NULL;cnt=0;}//构造函数
    Result Search(int k);//搜索关键码函数
    int SearchInNode(BtreeNode *q,int k);//在结点内q搜索k应插入的位置或所在的位置

    //插入关键码相关函数
    bool Insert(BtreeNode *q,int i,int k,BookNode *recptr);//插入关键码并重新调整函数
    void SplitNode(BtreeNode* &q,BtreeNode* &ap);//分裂结点函数
    void CreateRootNode(BtreeNode *q,BtreeNode *ap,int k,BookNode *recptr);//创建新的根结点函数
    void insertKey(BtreeNode *q,int i,BtreeNode *ap,int k,BookNode *recptr);//插入关键码函数

    //删除关键码相关函数
    bool Remove(int k);//删除关键码函数
    void FindSub(BtreeNode* &q,int &i);//删除q结点的第i个关键码，当q不为叶子结点时，寻找q的后继进行代替
    void DeleteKey(BtreeNode *q,int i);//删除对应的关键码及其对应的数据
    bool borrowKey(BtreeNode *q);//向兄弟结点借关键码
    void Merge(BtreeNode* &q);//合并结点

    //遍历B树相关函数
    void PrintAllBook(){PrintAllBook(root);}//打印所有所有书的信息
    void ShowBTree(short x=8){ShowBTree(root,x);}//凹入法打印B树
    void PrintAuthor(const char* author){PrintAuthor(root,author);}//打印作者相关图书
    void PrintOneBook(int x){PrintOneBook(root,x);}//打印一本书的状态信息
public:
    getKeyNum(){return cnt;}
private:
    BtreeNode *root;//B树的根节点
    int cnt;//关键码数量
    void ShowBTree(BtreeNode *T,int x);//使用凹入法打印B树
    void PrintAllBook(BtreeNode *p);//打印所有所有书的信息
    void PrintAuthor(BtreeNode *p,const char* author);//打印作者相关图书
    void PrintOneBook(BtreeNode *p,int x);//打印一本书的状态信息
};
