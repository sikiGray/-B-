#include<iostream>
#include<windows.h>
#include<string.h>
#include"Btree.h"
#include<fstream>
using namespace std;
void menu();
void Welcome();
int main()
{
    Btree T;
    Result res;//��ѯ�����Ϣ
    int booknum,curstock,totstock,bookcnt;
    char bookname[max_name_length],author[max_name_length];

    int ID,year,month,day,span;
    //int month[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
    int monthday[13]={0,0,31,60,91,121,152,182,213,244,274,305,335};

    BookNode *book;//��ļ�¼ָ��
    BtreeNode *pB;//B�����ָ��
    BorrowerNode *pR1,*pR2;//�����߽��ָ��
    ifstream fin;

    Welcome();

    while(1)
    {
        menu();
        int choice;
        cout<<endl;
        cout<<"�����빦��ѡ�";
        cin>>choice;
        switch(choice)
        {
            case 1:
                fin.open("book.txt",ios::in);
                while(fin>>booknum>>bookname>>author>>curstock>>totstock)
                {
                    //cout<<booknum<<' '<<bookname<<' '<<author<<' '<<curstock<<' '<<totstock<<endl;
                    book=new BookNode;
                    book->booknum=booknum;
                    strcpy(book->bookname,bookname);
                    strcpy(book->author,author);
                    book->current_stock=curstock;
                    book->total_stock=totstock;
                    book->bpHead=NULL;
                    //cout<<book->booknum<<' '<<book->bookname<<' '<<book->author<<' '<<book->current_stock<<' '<<book->total_stock<<endl;
                    res=T.Search(book->booknum);
                    T.Insert(res.r,res.i,book->booknum,book);
                }
                fin.close();
                cout<<endl;
                cout<<"                ����ɹ���"<<endl<<endl;
                system("pause");
                system("cls");
                break;
            case 2:
                T.ShowBTree();
                cout<<endl;

                system("pause");
                system("cls");
                break;
            case 3:
                //cout<<endl;
                printf("\n");
                printf("|************************ͼ�������Ϣ*************************|\n");
                printf("|  ���  |       ����         |      ����     | �ִ� | �ܿ�� |\n");
                T.PrintAllBook();
                printf("|--------|--------------------|---------------|------|--------|\n");
                cout<<endl;
                cout<<"     ��������������"<<T.getKeyNum()<<"��"<<endl<<endl;

                system("pause");
                system("cls");
                break;
            case 4:     //�ɱ����
                cout<<endl;
                cout<<"������Ҫ������ı�ţ�";
                cin>>booknum;
                cout<<"������Ҫ�������������";
                cin>>bookname;
                cout<<"������Ҫ����������ߣ�";
                cin>>author;
                cout<<"������Ҫ�������������";
                cin>>bookcnt;

                book=new BookNode;
                book->booknum=booknum;
                strcpy(book->bookname,bookname);
                strcpy(book->author,author);
                book->current_stock=bookcnt;
                book->total_stock=bookcnt;
                book->bpHead=NULL;

                res=T.Search(booknum);

                if(res.tag==0)  //�����û�и���
                    T.Insert(res.r,res.i,booknum,book);
                else    //��������и��飬���ӿ����
                {
                    book=res.r->recptr[res.i];
                    book->current_stock+=bookcnt;
                    book->total_stock+=bookcnt;
                }
                cout<<endl;
                cout<<"                ���ɹ���"<<endl;
                cout<<endl;

                system("pause");
                system("cls");
                break;
            case 5:     //������
                cout<<"������Ҫ�����ı�ţ�";
                cin>>booknum;
                if(T.Remove(booknum))
                    cout<<"                ɾ���ɹ�!"<<endl;
                else
                    cout<<"                ɾ��ʧ��!"<<endl;

                system("pause");
                system("cls");
                break;
            case 6:
                cout<<"��������Ҫ���ĵ�ͼ��ı�ţ�";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                û���Ȿ�飡"<<endl;
                else
                {
                    book=res.r->recptr[res.i];
                    if(book->current_stock>0)//���Խ��н���
                    {
                        cout<<"��������Ľ���ID�ţ�";
                        cin>>ID;
                        cout<<"��������������(�� �� ��)��";
                        cin>>year>>month>>day;
                        cout<<"��������Ľ���������";
                        cin>>span;
                        if(book->bpHead==NULL)
                        {
                            book->bpHead=new BorrowerNode;
                            book->bpHead->ID=ID;
                            book->bpHead->tBegin.year=year;
                            book->bpHead->tBegin.month=month;
                            book->bpHead->tBegin.day=day;
                            book->bpHead->span=span;
                            book->bpHead->next=NULL;
                        }
                        else
                        {
                            pR1=book->bpHead;
                            pR2=pR1;
                            while(pR1!=NULL)
                            {
                                //cout<<'*'<<endl;
                                pR2=pR1;
                                pR1=pR1->next;
                            }

                            pR2->next=new BorrowerNode;
                            pR2->next->ID=ID;
                            pR2->tBegin.year=year;
                            pR2->tBegin.month=month;
                            pR2->tBegin.day=day;
                            pR2->span=span;

                            pR2->next->next=NULL;
                        }
                        book->current_stock--;

                        cout<<"                ���ĳɹ���"<<endl;
                    }
                    else
                        cout<<"                ��ͼ���ѱ����꣡"<<endl;
                }

                system("pause");
                system("cls");
                break;
            case 7:
                cout<<"������Ҫ�黹��ͼ��ı�ţ�";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                û���Ȿ�飡"<<endl;
                else
                {
                    cout<<"��������Ľ���ID�ţ�";
                    cin>>ID;
                    cout<<"��������Ļ�������(�� �� ��)��";
                    cin>>year>>month>>day;

                    book=res.r->recptr[res.i];
                    pR1=book->bpHead;
                    while(pR1!=NULL)
                    {
                        //cout<<'*'<<endl;
                        pR2=pR1;
                        if(pR1->ID==ID)
                            break;
                        pR1=pR1->next;
                    }
                    if(pR1==NULL)
                        cout<<"                û���ҵ���Ľ����¼��"<<endl;
                    else
                    {
                        if(monthday[month]+day - monthday[pR1->tBegin.month]-pR1->tBegin.day > pR1->span)
                            cout<<"                ���ĳ�ʱ���뽻���"<<endl;
                        else
                        {

                            cout<<"                �黹�ɹ���"<<endl;
                        }


                        if(book->preOder==NULL)//������ԤԼ��
                        {
                            pR2->next=pR1->next;
                            delete pR1;
                            book->current_stock++;
                        }
                        else
                        {
                            pR1->ID=book->preOder->ID;
                            pR1->tBegin.year=year;
                            pR1->tBegin.month=month;
                            pR1->tBegin.day=day;
                            pR1->span=book->preOder->span;

                            book->preOder=book->preOder->next;
                        }
                    }
                }
                system("pause");
                system("cls");
                break;

            case 8:
                cout<<"������Ҫ�鿴�����ߣ�";
                cin>>author;
                printf("\n");
                printf("|************************ͼ�������Ϣ*************************|\n");
                printf("|  ���  |       ����         |      ����     | �ִ� | �ܿ�� |\n");
                T.PrintAuthor(author);
                printf("|--------|--------------------|---------------|------|--------|\n");

                system("pause");
                system("cls");
                break;
            case 9:
                cout<<"������Ҫ��ѯͼ��ı�ţ�";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                û���Ȿ�飡"<<endl;
                else
                    T.PrintOneBook(booknum);

                system("pause");
                system("cls");
                break;
            case 10:
                cout<<"��������ҪԤԼ��ͼ���ţ�";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                û���Ȿ�飡"<<endl;
                else
                {
                    book=res.r->recptr[res.i];
                    if(book->current_stock>0)//���Խ��н���
                    {
                        cout<<"                �������ԤԼ����ѡ����鹦�ܣ�"<<endl;
                        break;
                    }
                    else
                    {
                        cout<<"��������Ľ���ID�ţ�";
                        cin>>ID;
                        //cout<<"��������������(�� �� ��)��";
                        //cin>>year>>month>>day;
                        cout<<"��������Ľ���������";
                        cin>>span;
                        if(book->preOder==NULL)
                        {
                            book->preOder=new BorrowerNode;
                            book->preOder->ID=ID;

                            book->preOder->span=span;
                            book->preOder->next=NULL;
                        }
                        else
                        {
                            pR1=book->preOder;
                            pR2=pR1;
                            while(pR1!=NULL)
                            {
                                //cout<<'*'<<endl;
                                pR2=pR1;
                                pR1=pR1->next;
                            }

                            pR2->next=new BorrowerNode;
                            pR2->next->ID=ID;

                            pR2->span=span;

                            pR2->next->next=NULL;
                        }
                        cout<<"                ԤԼ�ɹ���"<<endl;
                    }
                }

                system("pause");
                system("cls");
                break;
            default:
                return 0;
        }
    }
}
