#include<iostream>
#include<windows.h>
using namespace std;
void menu()
{
    cout<<endl;
    cout<<"\t\t* * * * * * * * * * * * * * * * * * * * * * * * * *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t*                   功能菜单                      *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t*                                                 *"<<endl;
    cout<<"\t\t* 1、从文件导入图书信息";
    cout<<"\t\t2、打印B树的结构  *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t* 3、打印所有图书信息";
    cout<<"\t\t4、采编入库       *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t* 5、清除库存";
    cout<<"\t\t\t6、图书借阅       *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t* 7、图书归还";
    cout<<"\t\t\t8、作者相关作品   *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t* 9、查看书的状态";
    cout<<"\t\t10、图书预约      *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;

    cout<<"\t\t* 11、退出";
    cout<<"\t\t\t                  *"<<endl;
    cout<<"\t\t*                                                 *"<<endl;
    cout<<"\t\t* * * * * * * * * * * * * * * * * * * * * * * * * *"<<endl;
}

//函数功能：显示欢迎界面
void Welcome()
{
	cout<<endl<<endl;
	cout<<"                   *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout<<"                    *          欢迎使用图书管理系统         *"<<endl;
	cout<<"                   *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl<<endl;
	cout<<"                             ----------------------"<<endl;
	cout<<"                             *      温馨提示      *"<<endl;
	cout<<"--------------------------------------------------------------------------------"<<endl;
	cout<<"        本系统使用B树实现图书管理系统功能，具体功能如下："<<endl;
	cout<<"            ①图书的入库与出库"<<endl;
	cout<<"            ②图书的状态查询"<<endl;
	cout<<"            ③图书的借阅与归还"<<endl;
	cout<<"            ④图书的预约"<<endl;
	cout<<"        提示："<<endl;
	cout<<"           在使用图书的相关功能前，请先从文件导入图书信息或手动插入图书信息"<<endl<<endl;
	cout<<"--------------------------------------------------------------------------------"<<endl;
	cout<<endl<<endl;

	system("pause");
	system("cls");
}
