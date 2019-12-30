#include<iostream>
#include<stdio.h>
#include"Btree.h"
using namespace std;
int evmonth[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
int mdays[13]={0,0,31,60,91,121,152,182,213,244,274,305,335};
//�ڽ����q����kӦ�����λ�û����ڵ�λ��
int Btree::SearchInNode(BtreeNode *q,int k)
{
    int i;
    for(i=0;i<q->keynum&&q->key[i+1]<=k;i++);
    return i;
}

//�����ؼ��뺯��
Result Btree::Search(int k)
{
    Result res;//��������ṹ��
    BtreeNode *p=root,*q=NULL;//����B�����ڵ�
    int i=1,flag=0;
    while(p!=NULL&&flag==0)
    {
        /*int i=0;
        p->key[(p->keynum)+1]=maxValue;
        while(k>p->key[i+1])
            ++i;*/
        i=SearchInNode(p,k);
        if(i>0&&p->key[i]==k)    //�ҵ�Ҫ�����Ĺؼ���
            flag=1;
        else
        {
            q=p;
            p=p->ptr[i];
        }
        /*
        {
            res.r=p;
            res.i=i+1;//k���������Ϊ[key_i,key_(i+1)]
            res.tag=1;
            return res;
        }
        q=p;
        p=p->ptr[i];//�½�һ��Ѱ��*/
    }
    if(flag==1)
    {
        res.r=p;
        res.i=i;//k���������Ϊ[key_i,key_(i+1)]
        res.tag=1;
        return res;
    }
    else
    {
        //����ʧ�ܣ����عؼ���Ӧ�ò���Ľ��
        res.r=q;
        res.i=i;
        res.tag=0;
        return res;
    }
}

//���ѽ�㺯��,�����q���з���
void Btree::SplitNode(BtreeNode* &q,BtreeNode* &ap)//����ΪҪ���ѽ��ָ������úͲ������½�������
{
    int n=q->keynum,i;//����Ҫ���ѽ��Ĺؼ������
    ap=new BtreeNode;

    ap->keynum=2;//�����½��Ĺؼ������

    ap->key[1]=q->key[3];//�����½��Ĺؼ���
    ap->key[2]=q->key[4];

    ap->ptr[0]=q->ptr[2];  //�����½����ӽڵ�ָ��
    ap->ptr[1]=q->ptr[3];
    ap->ptr[2]=q->ptr[4];

    ap->recptr[1]=q->recptr[3];//�����½���������ָ��
    ap->recptr[2]=q->recptr[4];

    ap->parent=q->parent;//�޸��½��ĸ�ָ��
    for(i=0;i<=ap->keynum;i++)
    {
        if(ap->ptr[i]!=NULL)
            ap->ptr[i]->parent=ap;
    }

    q->keynum=1;//�޸ľɽ��Ĺؼ������
}

//�����µĸ���㺯��
void Btree::CreateRootNode(BtreeNode *q,BtreeNode *ap,int k,BookNode *recptr)
{
    root=new BtreeNode;
    root->keynum=1;
    root->parent=NULL;
    root->key[1]=k;
    root->recptr[1]=recptr;
    root->ptr[0]=q;
    root->ptr[1]=ap;

    //�޸����ӽ��ĸ��ڵ�
    if(q!=NULL)
        q->parent=root;
    if(ap!=NULL)
        ap->parent=root;
}
//����ؼ��뺯��
void Btree::insertKey(BtreeNode *q,int i,BtreeNode *ap,int k,BookNode *recptr)
{
    int n=q->keynum;
    //���ؼ��롢�ӽڵ�ָ�롢������ָ�����
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
    //�޸��ӽ��ĸ��ڵ�
    if(ap!=NULL)
        ap->parent=q;
}

//����ؼ��벢���µ�������
bool Btree::Insert(BtreeNode *q,int i,int k,BookNode *recptr)
{
    BtreeNode *ap=NULL;
    int flag=0;//�жϳ��������������㶼�ѵ������
    if(root==NULL)  //B��Ϊ��
        CreateRootNode(NULL,NULL,k,recptr);
    else    //B����Ϊ��
    {
        while(!flag)
        {
            insertKey(q,i,ap,k,recptr);
            if(q->keynum<M)
                flag=1; //�������
            else
            {
                recptr=q->recptr[2];
                k=q->key[2];
                SplitNode(q,ap);
                if(q->parent==NULL)
                    flag=2;//�������������Ҫ�����¸�
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

//ɾ��q���ĵ�i���ؼ��룬��q��ΪҶ�ӽ��ʱ��Ѱ��q�ĺ�̽��д���,����q��i��������Ҫɾ���Ĺؼ��ֵ�λ��
void Btree::FindSub(BtreeNode* &q,int &i)
{
    BtreeNode *p=q;
    q=q->ptr[i];
    while(q->ptr[0]!=NULL)
    {
        q=q->ptr[0];
        //cout<<'*'<<endl;
    }

    //���������ؼ�����������ֵ
    p->key[i]=q->key[1];
    p->recptr[i]=q->recptr[1];
/*
    BookNode *pt=p->recptr[i];
    p->recptr[i]=q->recptr[1];
    q->recptr[1]=pt;*/
    i=1;
}
//ɾ����Ӧ�Ĺؼ��뼰���Ӧ������
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
//���ֵܽ���ؼ���,�����ֵܽ������Ĺؼ��룬�����ֵܽ�����С�Ĺؼ���
bool Btree::borrowKey(BtreeNode *q)
{
    int i;
    BtreeNode *pp=q->parent,*pr=NULL,*pl=NULL;
    for(i=0;pp->ptr[i]!=q;++i);
    if(i>=0&&i+1<=pp->keynum&&pp->ptr[i+1]->keynum>1)//���ֵܽ��ɽ���С�ؼ���
    {
        pr=pp->ptr[i+1];//�������ֵܽ��ָ��
        q->ptr[1]=pr->ptr[0];//ͬʱ�ƶ�����

        q->key[1]=pp->key[i+1];//�踸�ڵ�ĵ�i+1���ؼ���
        q->recptr[1]=pp->recptr[i+1];//��ֵ��������

        pp->key[i+1]=pr->key[1];//�����ֵܽ��ĵ�һ�ؼ���ȥ��������ĵ�i+1���ؼ���
        pp->recptr[i+1]=pr->recptr[i+1];//��������Ÿı�
        //�ƶ����ֵܽ��Ĺؼ��룬�������ӽ��ָ��
        for(i=1;i<=pr->keynum;++i)
        {
            pr->key[i]=pr->key[i+1];
            pr->recptr[i]=pr->recptr[i+1];
            pr->ptr[i-1]=pr->ptr[i];
        }
        pr->keynum--;
        q->keynum++;
    }
    else if(i>0&&pp->ptr[i-1]->keynum>1)//�����ֵܽ�ؼ���
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
//�ϲ��ֵܽ��
void Btree::Merge(BtreeNode* &q)
{
    int i,j;
    BtreeNode *p=q->parent,*b=NULL;
    for(i=0;q!=p->ptr[i];i++);//����q�ڸ�����е�λ��
    if(i==0)//ֻ�������ֵܺϲ�
    {
        b=p->ptr[i+1];//ָ�����ֵ�
        for(j=b->keynum;j>=0;j--)//�����ֵܵĹؼ��֡����ݡ�ָ�����
        {
            b->key[j+1]=b->key[j];
            b->recptr[j+1]=b->recptr[j];
            b->ptr[j+1]=b->ptr[j];
        }
        //�ϲ�����
        b->ptr[0]=q->ptr[0];
        b->key[1]=p->key[1];
        b->recptr[1]=p->recptr[1];
    }
    else if(i>0)//�����ֵܽ��кϲ�
    {
        //�ϲ�����
        b=p->ptr[i-1];
        b->key[b->keynum+1]=p->key[i];
        b->recptr[b->keynum+1]=p->recptr[i];
        b->ptr[b->keynum+1]=q->ptr[0];
    }
    if(i==0||i==1||i==2)//�������p�ؼ���ǰ��һλ
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
    //��b�����ӽ��ĸ����ָ��b
    for(i=0;i<=b->keynum;i++)
    {
        if(b->ptr[i]!=NULL)
            b->ptr[i]->parent=b;
    }
}
//ɾ���ؼ��벢��������
bool Btree::Remove(int k)
{
    int x=k;
    BtreeNode *q,*b=NULL;
    int flag=0,i=1;
    Result res=Search(k);
    if(res.tag==0)//û���ҵ�Ҫɾ���Ĺؼ���
        return false;
    else
    {
        q=res.r;
        i=res.i;
        if(q->ptr[0]!=NULL)//Ҫɾ���Ĺؼ��벻��Ҷ���
        {

            FindSub(q,i);//Ѱ������Ҫɾ���Ĺؼ���

        }
        DeleteKey(q,i);
        if(q->keynum>=1||q->parent==NULL)//q�Ľ�����㶨���qΪ�����
        {
            flag=1;//��־�����ɾ��
            if(q->keynum==0)//ɾΪ����
                root=NULL;
        }
        while(flag!=1)
        {
            if(borrowKey(q))//����ͨ�����ֵܽ��Ĺؼ������ɾ��
                flag=1;
            else//��Ҫ���кϲ�
            {
                Merge(q);//�ϲ�
                q=q->parent;
                if(q==root&&root->keynum==0)//�ϲ����¸����Ϊ0
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
//ʹ�ð��뷨��ӡB��
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
// �ݹ��԰������ʽ��ʾB��T,ÿ���������Ϊx����ʼ������Ϊ8
{
	if (T==NULL)
        return;
	int	i;

	cout<<endl;
	for (i=0;i<=x;i++) putchar(' ');			// ����x
	for (i=1;i<=T->keynum;i++)
	{
		//printf("%d,", T->key[i]);
		cout<<T->key[i]<<' ';
	}
	for (i=0;i<=T->keynum;i++)				// �ݹ���ʾ�������ؼ���
		ShowBTree(T->ptr[i],x+7);
}

//������ӡ�������������Ϣ
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
	for (i = 0; i <= p->keynum; i++)				// �ݹ���ʾ�������ؼ���
		PrintAllBook(p->ptr[i]);
}

//��ӡ�������ͼ��
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
	for (i = 0; i <= p->keynum; i++)				// �ݹ���ʾ�������ؼ���
		PrintAuthor(p->ptr[i],author);
}

//��ӡһ�����״̬��Ϣ
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
            printf("|************************ͼ�������Ϣ*************************|\n");
            printf("|  ���  |       ����         |      ����     | �ִ� | �ܿ�� |\n");
            printf("|  %-4d  |%-20s|%-15s|",p->recptr[i]->booknum,p->recptr[i]->bookname,p->recptr[i]->author);
            printf(" %-5d|   %-4d |\n",p->recptr[i]->current_stock,p->recptr[i]->total_stock);
            printf("|--------|--------------------|---------------|------|--------|\n");
            //cout<<p->recptr[i]->booknum<<' '<<p->recptr[i]->bookname<<' '<<p->recptr[i]->author<<' '<<p->recptr[i]->current_stock<<' '<<p->recptr[i]->total_stock<<endl;
            BorrowerNode *b=p->recptr[i]->bpHead;
            //������絽�ڵĽ����߱��
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
                cout<<"���絽��ʱ��Ϊ��"<<bt->tBegin.year<<"��"<<bt->tBegin.month+(bt->tBegin.day+bt->span)/evmonth[bt->tBegin.month]<<"��"<<(bt->tBegin.day+bt->span)%evmonth[bt->tBegin.month]<<"��";
                cout<<"\t������߱��Ϊ��"<<bt->ID<<endl;
            }
            return;
        }

	}
	for (i = 0; i <= p->keynum; i++)				// �ݹ���ʾ�������ؼ���
		PrintOneBook(p->ptr[i],x);
}
