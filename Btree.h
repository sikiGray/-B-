#include<iostream>
#include<string.h>
using namespace std;
const int M=4;  //�Ľ�B��
const int max_name_length=50;//������������������
const int maxValue=100;//�ؼ��뼴��ŵ����ֵ

class Date//���ڽṹ��
{
public:
    int year;
    int month;
    int day;
};

class BorrowerNode//��������Ϣ���
{
public:
    int ID;//�����ߵĽ���֤��
    Date tBegin;//��������
    //Date tEnd;//��������
    int span;//��������
    BorrowerNode *next;//ָ����һ�������߽��
};

class BookNode  //ͼ����Ϣ���
{
public:
    int booknum;//���
    char bookname[max_name_length];//����
    char author[max_name_length];//������
    int current_stock;//����ִ���
    int total_stock;//����ܴ���
    BorrowerNode *bpHead;//����������ָ��ͷָ��
    BorrowerNode *preOder;//ԤԼ������ָ��ͷָ��
    //BookNode(){root=NULL;}
};

class BtreeNode    //B�����ṹ��
{
public:
    int keynum;  //�ؼ���ĸ���
    BtreeNode *parent;  //���ڵ�ָ��
    int key[M+1];   //ʹ�������Ϊ�ؼ��룬
    BtreeNode *ptr[M+1];    //�������ָ�����飬ptr[m]�ڲ������ʱʹ��
    BookNode *recptr[M+1];  //�ؼ����Ӧ������ָ��

};

class Result    //���������Ϣ�ṹ
{
public:
    BtreeNode *r;//��ѯ�ؼ������ڽ��ĵ�ַָ��
    int i;  //���ؼ������
    int tag;//������־�����Ƿ�ɹ���tag=0����ʧ�ܣ�tag=1�����ɹ�
};

class Btree     //B���ඨ��
{
public:
    Btree(){root=NULL;cnt=0;}//���캯��
    Result Search(int k);//�����ؼ��뺯��
    int SearchInNode(BtreeNode *q,int k);//�ڽ����q����kӦ�����λ�û����ڵ�λ��

    //����ؼ�����غ���
    bool Insert(BtreeNode *q,int i,int k,BookNode *recptr);//����ؼ��벢���µ�������
    void SplitNode(BtreeNode* &q,BtreeNode* &ap);//���ѽ�㺯��
    void CreateRootNode(BtreeNode *q,BtreeNode *ap,int k,BookNode *recptr);//�����µĸ���㺯��
    void insertKey(BtreeNode *q,int i,BtreeNode *ap,int k,BookNode *recptr);//����ؼ��뺯��

    //ɾ���ؼ�����غ���
    bool Remove(int k);//ɾ���ؼ��뺯��
    void FindSub(BtreeNode* &q,int &i);//ɾ��q���ĵ�i���ؼ��룬��q��ΪҶ�ӽ��ʱ��Ѱ��q�ĺ�̽��д���
    void DeleteKey(BtreeNode *q,int i);//ɾ����Ӧ�Ĺؼ��뼰���Ӧ������
    bool borrowKey(BtreeNode *q);//���ֵܽ���ؼ���
    void Merge(BtreeNode* &q);//�ϲ����

    //����B����غ���
    void PrintAllBook(){PrintAllBook(root);}//��ӡ�������������Ϣ
    void ShowBTree(short x=8){ShowBTree(root,x);}//���뷨��ӡB��
    void PrintAuthor(const char* author){PrintAuthor(root,author);}//��ӡ�������ͼ��
    void PrintOneBook(int x){PrintOneBook(root,x);}//��ӡһ�����״̬��Ϣ
public:
    getKeyNum(){return cnt;}
private:
    BtreeNode *root;//B���ĸ��ڵ�
    int cnt;//�ؼ�������
    void ShowBTree(BtreeNode *T,int x);//ʹ�ð��뷨��ӡB��
    void PrintAllBook(BtreeNode *p);//��ӡ�������������Ϣ
    void PrintAuthor(BtreeNode *p,const char* author);//��ӡ�������ͼ��
    void PrintOneBook(BtreeNode *p,int x);//��ӡһ�����״̬��Ϣ
};
