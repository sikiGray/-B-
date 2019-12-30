#include<iostream>
#include<stdio.h>
#include"Btree.h"
using namespace std;
int evmonth[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
int mdays[13]={0,0,31,60,91,121,152,182,213,244,274,305,335};
//在结点内q搜索k应插入的位置或所在的位置
int Btree::SearchInNode(BtreeNode *q,int k)
{
    int i;
    for(i=0;i<q->keynum&&q->key[i+1]<=k;i++);
    return i;
}

//搜索关键码函数
Result Btree::Search(int k)
{
    Result res;//搜索结果结构体
    BtreeNode *p=root,*q=NULL;//保存B树根节点
    int i=1,flag=0;
    while(p!=NULL&&flag==0)
    {
        /*int i=0;
        p->key[(p->keynum)+1]=maxValue;
        while(k>p->key[i+1])
            ++i;*/
        i=SearchInNode(p,k);
        if(i>0&&p->key[i]==k)    //找到要搜索的关键码
            flag=1;
        else
        {
            q=p;
            p=p->ptr[i];
        }
        /*
        {
            res.r=p;
            res.i=i+1;//k落入的区间为[key_i,key_(i+1)]
            res.tag=1;
            return res;
        }
        q=p;
        p=p->ptr[i];//下降一层寻找*/
    }
    if(flag==1)
    {
        res.r=p;
        res.i=i;//k落入的区间为[key_i,key_(i+1)]
        res.tag=1;
        return res;
    }
    else
    {
        //搜索失败，返回关键码应该插入的结点
        res.r=q;
        res.i=i;
        res.tag=0;
        return res;
    }
}

//分裂结点函数,将结点q进行分裂
void Btree::SplitNode(BtreeNode* &q,BtreeNode* &ap)//参数为要分裂结点指针的引用和产生的新结点的引用
{
    int n=q->keynum,i;//保存要分裂结点的关键码个数
    ap=new BtreeNode;

    ap->keynum=2;//设置新结点的关键码个数

    ap->key[1]=q->key[3];//设置新结点的关键码
    ap->key[2]=q->key[4];

    ap->ptr[0]=q->ptr[2];  //设置新结点的子节点指针
    ap->ptr[1]=q->ptr[3];
    ap->ptr[2]=q->ptr[4];

    ap->recptr[1]=q->recptr[3];//设置新结点的数据域指针
    ap->recptr[2]=q->recptr[4];

    ap->parent=q->parent;//修改新结点的父指针
    for(i=0;i<=ap->keynum;i++)
    {
        if(ap->ptr[i]!=NULL)
            ap->ptr[i]->parent=ap;
    }

    q->keynum=1;//修改旧结点的关键码个数
}

//创建新的根结点函数
void Btree::CreateRootNode(BtreeNode *q,BtreeNode *ap,int k,BookNode *recptr)
{
    root=new BtreeNode;
    root->keynum=1;
    root->parent=NULL;
    root->key[1]=k;
    root->recptr[1]=recptr;
    root->ptr[0]=q;
    root->ptr[1]=ap;

    //修改子子结点的父节点
    if(q!=NULL)
        q->parent=root;
    if(ap!=NULL)
        ap->parent=root;
}
//插入关键码函数
void Btree::insertKey(BtreeNode *q,int i,BtreeNode *ap,int k,BookNode *recptr)
{
    int n=q->keynum;
    //将关键码、子节点指针、数据域指针后移
    for(int j=n;j>i;j--)
    {
        q->key[j+1]=q->key[j];
        q->ptr[j+1]=q->ptr[j];
        q->recptr[j+1]=q->recptr[j];
    }
    q->key[i+1]=k;
    q->ptr[i+1]=ap;
    q->recptr[i+1]=recptr;

    q->keynum++;
    //修改子结点的父节点
    if(ap!=NULL)
        ap->parent=q;
}

//插入关键码并重新调整函数
bool Btree::Insert(BtreeNode *q,int i,int k,BookNode *recptr)
{
    BtreeNode *ap=NULL;
    int flag=0;//判断除根以外的其他结点都已调整完成
    if(root==NULL)  //B树为空
        CreateRootNode(NULL,NULL,k,recptr);
    else    //B树不为空
    {
        while(!flag)
        {
            insertKey(q,i,ap,k,recptr);
            if(q->keynum<M)
                flag=1; //调整完成
            else
            {
                recptr=q->recptr[2];
                k=q->key[2];
                SplitNode(q,ap);
                if(q->parent==NULL)
                    flag=2;//根结点已满，需要建立新根
                else
                {
                    q=q->parent;
                    i=SearchInNode(q,k);
                }
            }
        }
        if(flag==2)
            CreateRootNode(q,ap,k,recptr);
    }
    ++cnt;
    return true;
}

//删除q结点的第i个关键码，当q不为叶子结点时，寻找q的后继进行代替,并用q和i返回真正要删除的关键字的位置
void Btree::FindSub(BtreeNode* &q,int &i)
{
    BtreeNode *p=q;
    q=q->ptr[i];
    while(q->ptr[0]!=NULL)
    {
        q=q->ptr[0];
        //cout<<'*'<<endl;
    }

    //交换两个关键码和数据域的值
    p->key[i]=q->key[1];
    p->recptr[i]=q->recptr[1];
/*
    BookNode *pt=p->recptr[i];
    p->recptr[i]=q->recptr[1];
    q->recptr[1]=pt;*/
    i=1;
}
//删除对应的关键码及其对应的数据
void Btree::DeleteKey(BtreeNode *q,int i)
{
    //delete q->recptr[i];
    for(;i<q->keynum;i++)
    {
        q->key[i]=q->key[i+1];
        q->recptr[i]=q->recptr[i+1];
    }
    q->keynum--;
}
//向兄弟结点借关键码,从左兄弟结点借最大的关键码，从右兄弟结点借最小的关键码
bool Btree::borrowKey(BtreeNode *q)
{
    int i;
    BtreeNode *pp=q->parent,*pr=NULL,*pl=NULL;
    for(i=0;pp->ptr[i]!=q;++i);
    if(i>=0&&i+1<=pp->keynum&&pp->ptr[i+1]->keynum>1)//右兄弟结点可借最小关键码
    {
        pr=pp->ptr[i+1];//保存右兄弟结点指针
        q->ptr[1]=pr->ptr[0];//同时移动子树

        q->key[1]=pp->key[i+1];//借父节点的第i+1个关键码
        q->recptr[1]=pp->recptr[i+1];//赋值数据域结点

        pp->key[i+1]=pr->key[1];//用右兄弟结点的第一关键码去替代父结点的第i+1个关键码
        pp->recptr[i+1]=pr->recptr[i+1];//数据域跟着改变
        //移动右兄弟结点的关键码，数据域，子结点指针
        for(i=1;i<=pr->keynum;++i)
        {
            pr->key[i]=pr->key[i+1];
            pr->recptr[i]=pr->recptr[i+1];
            pr->ptr[i-1]=pr->ptr[i];
        }
        pr->keynum--;
        q->keynum++;
    }
    else if(i>0&&pp->ptr[i-1]->keynum>1)//向左兄弟借关键码
    {
        pl=pp->ptr[i-1];
        q->ptr[1]=q->ptr[0];
        q->ptr[0]=pl->ptr[pl->keynum];

        q->key[1]=pp->key[i];
        q->recptr[1]=pp->recptr[i];

        pp->key[i]=pl->key[pl->keynum];
        pp->recptr[i]=pl->recptr[pl->keynum];

        pl->keynum--;
        q->keynum++;
    }
    else
        return false;
}
//合并兄弟结点
void Btree::Merge(BtreeNode* &q)
{
    int i,j;
    BtreeNode *p=q->parent,*b=NULL;
    for(i=0;q!=p->ptr[i];i++);//查找q在父结点中的位置
    if(i==0)//只能与右兄弟合并
    {
        b=p->ptr[i+1];//指向右兄弟
        for(j=b->keynum;j>=0;j--)//将右兄弟的关键字、数据、指针后移
        {
            b->key[j+1]=b->key[j];
            b->recptr[j+1]=b->recptr[j];
            b->ptr[j+1]=b->ptr[j];
        }
        //合并过程
        b->ptr[0]=q->ptr[0];
        b->key[1]=p->key[1];
        b->recptr[1]=p->recptr[1];
    }
    else if(i>0)//与左兄弟进行合并
    {
        //合并过程
        b=p->ptr[i-1];
        b->key[b->keynum+1]=p->key[i];
        b->recptr[b->keynum+1]=p->recptr[i];
        b->ptr[b->keynum+1]=q->ptr[0];
    }
    if(i==0||i==1||i==2)//将父结点p关键字前移一位
    {
        for(;i<p->keynum;i++)
        {
            p->key[i]=p->key[i+1];
            p->ptr[i]=p->ptr[i+1];
            p->recptr[i]=p->recptr[i+1];
        }
    }
    p->keynum--;
    b->keynum++;
    delete q;
    q=b;
    //将b结点的子结点的父结点指向b
    for(i=0;i<=b->keynum;i++)
    {
        if(b->ptr[i]!=NULL)
            b->ptr[i]->parent=b;
    }
}
//删除关键码并调整函数
bool Btree::Remove(int k)
{
    int x=k;
    BtreeNode *q,*b=NULL;
    int flag=0,i=1;
    Result res=Search(k);
    if(res.tag==0)//没有找到要删除的关键码
        return false;
    else
    {
        q=res.r;
        i=res.i;
        if(q->ptr[0]!=NULL)//要删除的关键码不在叶结点
        {

            FindSub(q,i);//寻找真正要删除的关键码

        }
        DeleteKey(q,i);
        if(q->keynum>=1||q->parent==NULL)//q的结点满足定义或q为根结点
        {
            flag=1;//标志以完成删除
            if(q->keynum==0)//删为空树
                root=NULL;
        }
        while(flag!=1)
        {
            if(borrowKey(q))//可以通过借兄弟结点的关键码完成删除
                flag=1;
            else//需要进行合并
            {
                Merge(q);//合并
                q=q->parent;
                if(q==root&&root->keynum==0)//合并导致根结点为0
                {
                    root=root->ptr[0];
                    root->parent=NULL;
                    delete q;
                    flag=1;
                }
                else if(q->keynum>=1)
                    flag=1;
            }
        }//cout<<'*'<<endl;
    }
    --cnt;
    return true;
}
/*
//使用凹入法打印B树
void Btree::PrintBtree(BtreeNode *p,string space)
{
    if(p==NULL)
        return;
    //cout<<endl;
    space+="    ";
    for(int i=p->keynum;i>0;i--)
        cout<<space<<p->key[i]<<endl;
    for(int i=p->keynum;i>=0;i--)
    {
        PrintBtree(p->ptr[i],space);
    }
}*/
void Btree::ShowBTree(BtreeNode *T, int x)
// 递归以凹入表形式显示B树T,每层的缩进量为x，初始缩进量为8
{
	if (T==NULL)
        return;
	int	i;

	cout<<endl;
	for (i=0;i<=x;i++) putchar(' ');			// 缩进x
	for (i=1;i<=T->keynum;i++)
	{
		//printf("%d,", T->key[i]);
		cout<<T->key[i]<<' ';
	}
	for (i=0;i<=T->keynum;i++)				// 递归显示子树结点关键字
		ShowBTree(T->ptr[i],x+7);
}

//遍历打印所有所有书的信息
void Btree::PrintAllBook(BtreeNode *p)
{
    if (p==NULL)
        return;
	int	i;
	for (i = 1; i <= p->keynum; i++)
	{

        printf("|  %-4d  |%-20s|%-15s|",p->recptr[i]->booknum,p->recptr[i]->bookname,p->recptr[i]->author);
        printf(" %-5d|   %-4d |\n",p->recptr[i]->current_stock,p->recptr[i]->total_stock);

		//cout<<p->recptr[i]->booknum<<' '<<p->recptr[i]->bookname<<' '<<p->recptr[i]->author<<' '<<p->recptr[i]->current_stock<<' '<<p->recptr[i]->total_stock<<endl;
	}
	for (i = 0; i <= p->keynum; i++)				// 递归显示子树结点关键字
		PrintAllBook(p->ptr[i]);
}

//打印作者相关图书
void Btree::PrintAuthor(BtreeNode *p,const char* author)
{
    if (p==NULL)
        return;
	int	i;
	for (i = 1; i <= p->keynum; i++)
	{
	    if(!strcmp(author,p->recptr[i]->author))
        {
            printf("|  %-4d  |%-20s|%-15s|",p->recptr[i]->booknum,p->recptr[i]->bookname,p->recptr[i]->author);
            printf(" %-5d|   %-4d |\n",p->recptr[i]->current_stock,p->recptr[i]->total_stock);
            //cout<<p->recptr[i]->booknum<<' '<<p->recptr[i]->bookname<<' '<<p->recptr[i]->author<<' '<<p->recptr[i]->current_stock<<' '<<p->recptr[i]->total_stock<<endl;
        }

	}
	for (i = 0; i <= p->keynum; i++)				// 递归显示子树结点关键字
		PrintAuthor(p->ptr[i],author);
}

//打印一本书的状态信息
void Btree::PrintOneBook(BtreeNode *p,int x)
{
        if (p==NULL)
        return;
	int	i;
	for (i = 1; i <= p->keynum; i++)
	{
	    if(p->key[i]==x)
        {
            cout<<endl;
            printf("|************************图书基本信息*************************|\n");
            printf("|  书号  |       书名         |      作者     | 现存 | 总库存 |\n");
            printf("|  %-4d  |%-20s|%-15s|",p->recptr[i]->booknum,p->recptr[i]->bookname,p->recptr[i]->author);
            printf(" %-5d|   %-4d |\n",p->recptr[i]->current_stock,p->recptr[i]->total_stock);
            printf("|--------|--------------------|---------------|------|--------|\n");
            //cout<<p->recptr[i]->booknum<<' '<<p->recptr[i]->bookname<<' '<<p->recptr[i]->author<<' '<<p->recptr[i]->current_stock<<' '<<p->recptr[i]->total_stock<<endl;
            BorrowerNode *b=p->recptr[i]->bpHead;
            //输出最早到期的借阅者编号
            if(b!=NULL)
            {
                int sum=mdays[b->tBegin.month]+b->tBegin.day+b->span;
                BorrowerNode *bt=b;
                b=b->next;
                while(b!=NULL)
                {
                    if(sum>mdays[b->tBegin.month]+b->tBegin.day+b->span)
                    {
                        sum=mdays[b->tBegin.month]+b->tBegin.day+b->span;
                        bt=b;
                    }
                    b=b->next;
                }
                cout<<"最早到期时间为："<<bt->tBegin.year<<"年"<<bt->tBegin.month+(bt->tBegin.day+bt->span)/evmonth[bt->tBegin.month]<<"月"<<(bt->tBegin.day+bt->span)%evmonth[bt->tBegin.month]<<"日";
                cout<<"\t其借阅者编号为："<<bt->ID<<endl;
            }
            return;
        }

	}
	for (i = 0; i <= p->keynum; i++)				// 递归显示子树结点关键字
		PrintOneBook(p->ptr[i],x);
}
