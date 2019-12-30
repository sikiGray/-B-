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
    Result res;//查询结果信息
    int booknum,curstock,totstock,bookcnt;
    char bookname[max_name_length],author[max_name_length];

    int ID,year,month,day,span;
    //int month[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
    int monthday[13]={0,0,31,60,91,121,152,182,213,244,274,305,335};

    BookNode *book;//书的记录指针
    BtreeNode *pB;//B树结点指针
    BorrowerNode *pR1,*pR2;//借阅者结点指针
    ifstream fin;

    Welcome();

    while(1)
    {
        menu();
        int choice;
        cout<<endl;
        cout<<"请输入功能选项：";
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
                cout<<"                导入成功！"<<endl<<endl;
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
                printf("|************************图书基本信息*************************|\n");
                printf("|  书号  |       书名         |      作者     | 现存 | 总库存 |\n");
                T.PrintAllBook();
                printf("|--------|--------------------|---------------|------|--------|\n");
                cout<<endl;
                cout<<"     书库中书的种类有"<<T.getKeyNum()<<"种"<<endl<<endl;

                system("pause");
                system("cls");
                break;
            case 4:     //采编入库
                cout<<endl;
                cout<<"请输入要入库的书的编号：";
                cin>>booknum;
                cout<<"请输入要入库的书的书名：";
                cin>>bookname;
                cout<<"请输入要入库的书的作者：";
                cin>>author;
                cout<<"请输入要入库的书的数量：";
                cin>>bookcnt;

                book=new BookNode;
                book->booknum=booknum;
                strcpy(book->bookname,bookname);
                strcpy(book->author,author);
                book->current_stock=bookcnt;
                book->total_stock=bookcnt;
                book->bpHead=NULL;

                res=T.Search(booknum);

                if(res.tag==0)  //库存中没有该书
                    T.Insert(res.r,res.i,booknum,book);
                else    //库存中已有该书，增加库存量
                {
                    book=res.r->recptr[res.i];
                    book->current_stock+=bookcnt;
                    book->total_stock+=bookcnt;
                }
                cout<<endl;
                cout<<"                入库成功！"<<endl;
                cout<<endl;

                system("pause");
                system("cls");
                break;
            case 5:     //清除库存
                cout<<"请输入要清除书的编号：";
                cin>>booknum;
                if(T.Remove(booknum))
                    cout<<"                删除成功!"<<endl;
                else
                    cout<<"                删除失败!"<<endl;

                system("pause");
                system("cls");
                break;
            case 6:
                cout<<"请输入你要借阅的图书的编号：";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                没有这本书！"<<endl;
                else
                {
                    book=res.r->recptr[res.i];
                    if(book->current_stock>0)//可以进行借阅
                    {
                        cout<<"请输入你的借阅ID号：";
                        cin>>ID;
                        cout<<"请输入借书的日期(年 月 日)：";
                        cin>>year>>month>>day;
                        cout<<"请输入你的借阅天数：";
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

                        cout<<"                借阅成功！"<<endl;
                    }
                    else
                        cout<<"                此图书已被借完！"<<endl;
                }

                system("pause");
                system("cls");
                break;
            case 7:
                cout<<"请输入要归还的图书的编号：";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                没有这本书！"<<endl;
                else
                {
                    cout<<"请输入你的借阅ID号：";
                    cin>>ID;
                    cout<<"请输入你的还书日期(年 月 日)：";
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
                        cout<<"                没有找到你的借书记录！"<<endl;
                    else
                    {
                        if(monthday[month]+day - monthday[pR1->tBegin.month]-pR1->tBegin.day > pR1->span)
                            cout<<"                借阅超时，请交罚款！"<<endl;
                        else
                        {

                            cout<<"                归还成功！"<<endl;
                        }


                        if(book->preOder==NULL)//不存在预约者
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
                cout<<"请输入要查看的作者：";
                cin>>author;
                printf("\n");
                printf("|************************图书基本信息*************************|\n");
                printf("|  书号  |       书名         |      作者     | 现存 | 总库存 |\n");
                T.PrintAuthor(author);
                printf("|--------|--------------------|---------------|------|--------|\n");

                system("pause");
                system("cls");
                break;
            case 9:
                cout<<"请输入要查询图书的编号：";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                没有这本书！"<<endl;
                else
                    T.PrintOneBook(booknum);

                system("pause");
                system("cls");
                break;
            case 10:
                cout<<"请输入想要预约的图书编号：";
                cin>>booknum;
                res=T.Search(booknum);
                if(res.tag==0)
                    cout<<"                没有这本书！"<<endl;
                else
                {
                    book=res.r->recptr[res.i];
                    if(book->current_stock>0)//可以进行借阅
                    {
                        cout<<"                无需进行预约，请选择借书功能！"<<endl;
                        break;
                    }
                    else
                    {
                        cout<<"请输入你的借阅ID号：";
                        cin>>ID;
                        //cout<<"请输入借书的日期(年 月 日)：";
                        //cin>>year>>month>>day;
                        cout<<"请输入你的借阅天数：";
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
                        cout<<"                预约成功！"<<endl;
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
