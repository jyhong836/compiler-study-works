/*
 * LR1Parse.cpp
 *
 *  Created on: 2014年4月1日
 *      Author: jyhong
 */

#include "LR1Parse.h"
#include <stdio.h>
#include <iostream>


//#define _SHOW_ERROR_NUM
//#define _SHOW_READ_PARSEFILE_MSG
//#define _SHOW_REDUCE_MSG


LR1Parse::LR1Parse(FILE *parsefile)
{
	ws = NULL;

	string key;
	char cname, ct;
	string str;
	int i;
    if (!parsefile)
    	cout << "parsefile is empty,try default file"<<endl;
    parsefile = fopen("analysis_table", "r");
    if (!parsefile)
    {
    	cout<<"cannot open analysis_table, exit"<<endl;
    	return;
    }
    int statNum;
    fscanf(parsefile, "status Number:%d", &statNum);
    cout<<"statNum="<<statNum<<endl;
    fgetc(parsefile); // 去掉回车符
    while (!feof(parsefile))
    //读取终结符
    {
    	alpha al;
    	al.size = ALPHA_SIZE;
    	i = 0;

    	cname = fgetc(parsefile);
    	al.name = cname;
    	fscanf(parsefile, "%d", &(al.token));
    	ct = fgetc(parsefile);

    	while(ct!='\n')
    	{
    		if (ct=='a'||ct=='e'||ct=='r'||ct=='s')
    		{
    			al.cmd[i].cmd = ct;
    			fscanf(parsefile, "%d", &(al.cmd[i].cmdNum));
    			i++;
    		}

    		ct = fgetc(parsefile);
    	}
    	this->alpha_map.insert(make_pair(al.token, al));
    	map<int, alpha>::iterator it = alpha_map.find(al.token);
#ifdef _SHOW_READ_PARSEFILE_MSG
    	if (it != alpha_map.end())
    	{
    		cout << "insert success in " << it->first <<endl;
    	}
#endif
    	if (cname=='#')// # 为最后一个终结符
    		break;
    }
    while (!feof(parsefile))
    //读取非终结符
    {
    	alpha al;
    	i = 0;
    	cname = fgetc(parsefile);
    	al.name = cname;
    	fscanf(parsefile, "%d", &(al.token));
    	for (int j=0;j<statNum;j++)
    	{
    		al.cmd[i].cmd = 0;
    		fscanf(parsefile, "%d", &(al.cmd[i].cmdNum));
    		i++;
    	};
    	this->alpha_map.insert(make_pair(al.token, al));
    	map<int, alpha>::iterator it = alpha_map.find(al.token);
#ifdef _SHOW_READ_PARSEFILE_MSG
    	if (it != alpha_map.end())
    	{
    		cout << "insert success in " << it->first <<endl;
    	}
#endif
    	if (cname=='F')// F 为最后一个非终结符
    		break;
    }
    return ;
}

LR1Parse::~LR1Parse()
{
}

int LR1Parse::parse(WordStream* ws)
{
	cout<<"========parse=========="<<endl;
	this->ws = ws;
	while(!status.empty())
	{
		cout<<"清空分析栈"<<endl;
		status.pop();
	}
	status.push(0);
	int s = 0; // s保存栈顶记号
	map<int, alpha>::iterator it;
	this->ws->getWord(curw);
	char cmd;
	while (1)
	{
		s = status.top();
		it = this->alpha_map.find(curw.token);
		cmd = it->second.cmd[s].cmd;
		if (cmd=='s')
		{
			cout<<"shift:["<<s<<","<<curw.id<<"]"<<it->second.cmd[s].cmdNum<<endl;
			status.push(it->second.cmd[s].cmdNum);
			this->ws->getWord(curw);
		}else if(cmd=='r')
		{
#ifdef _SHOW_REDUCE_MSG
			cout<<"reduce:"<<it->second.cmd[s].cmdNum<<endl;
#endif
			int retToken = this->reduce(it->second.cmd[s].cmdNum); // 返回的是非终结符
			// reduce 完成退栈和输出产生式的任务
			if(!retToken)
				cout<<"归约发生未知错误，继续执行"<<endl;
			else
			{
				s = status.top();
				it = this->alpha_map.find(retToken);
				status.push(it->second.cmd[s].cmdNum);
#ifdef _SHOW_REDUCE_MSG
				cout<<"归约后的栈顶状态:"<<"["<<s<<","<<retToken<<"]="<<it->second.cmd[s].cmdNum<<endl;
#endif
			}
		}else if(cmd=='a')
		{
			cout<<"分析成功"<<endl;
			break;
		}else
		{
#ifdef _SHOW_ERROR_NUM
			cout<<"ERROR:["<<status.top()<<","<<curw.token<<"]="<<cmd<<it->second.cmd[s].cmdNum<<endl;
#endif
			if(this->error(it->second.cmd[s].cmdNum))
			{
				cout<<"错误恢复失败，退出语法分析"<<endl;
				return -1;
			}
		}
	}
	return 0;
}

int LR1Parse::error(int errorNum)
// 错误恢复
{
	switch(errorNum)
	{
	case 1:
		return error1();
	case 2:
		return error2();
	case 3:
		return error3();
	case 4:
		return error4();
	case 5:
		return error5();
	case 0:
	default:
		return error0();
	}
	return 0;
}
int LR1Parse::error0()
{
	cout<<"unknown error:强制结束"<<endl;
	return -1;
}
int LR1Parse::error1()
//因为已经默认输入#的时候使用默认的表达式，所以这个错误事实上不会发生。
{
	cout<<"error1: 空的表达式"<<endl;
	return 0;
}
int LR1Parse::error2()
{
	ws->getWord(curw);
//	if (curw.token==ENDTOKEN) {
//		cout<<"error2: 提前结束，缺少右操作符或右括号"<<endl;
//		return -1;//已经结束，不再进行错误回复
//	}
	cout<<"error2: 缺少操作数，忽略操作符，将操作符"<<curw.id<<"从输入串中跳过"<<endl;
	return 0;
}
int LR1Parse::error3()
// 期望得到id或表达式，但是得到了右括号
{
	if (status.top()==4||status.top()==13)
	{
		cout<<"\t括号内为空，将左括号弹出栈，";
		status.pop();
		cout<<"\t跳过右括号"<<endl;
		ws->getWord(curw);
	}else
	{
		cout<<"error3: 括号不匹配，缺少左括号，跳过"<<endl;
		ws->getWord(curw);
	}
	return 0;
}
int LR1Parse::error4()
// 似乎不会发生这种错误
{
	cout<<"error4: 与期望的操作符不匹配，未知的问题"<<endl;
	return 0;
}
int LR1Parse::error5()
// 期望得到右括号或操作符，但得到是#？？？？
{
	cout<<"error5: 缺少操作符或#或缺少右括号"<<curw.id<<endl;
	cout<<"\t弹出左括号"<<endl;
	status.pop();
	return 0;
}

int LR1Parse::reduce(int reduceNum)
// 完成退栈和输出产生式的任务
{
	switch(reduceNum)
	{
		case 2:
		return reduce2();
		case 3:
		return reduce3();
		case 4:
		return reduce4();
		case 5:
		return reduce5();
		case 6:
		return reduce6();
		case 7:
		return reduce7();
		case 8:
		return reduce8();
		case 9:
		return reduce9();
		default:
			cout<<"找不到产生式："<<reduceNum<<endl;
	}
	return 0;
}

int LR1Parse::reduce2()
{
	for (int i = 0; i < 3; ++i) {
		this->status.pop();
	}
	cout<<"E->E+T"<<endl;
	return -1;
}

int LR1Parse::reduce3()
{
	for (int i = 0; i < 3; ++i) {
		this->status.pop();
	}
	cout<<"E->E-T"<<endl;
	return -1;
}

int LR1Parse::reduce4()
{
	this->status.pop();
	cout<<"E->T"<<endl;
	return -1;
}

int LR1Parse::reduce5()
{
	for (int i = 0; i < 3; ++i) {
		this->status.pop();
	}
	cout<<"T->T*F"<<endl;
	return -2;
}

int LR1Parse::reduce6()
{
	for (int i = 0; i < 3; ++i) {
		this->status.pop();
	}
	cout<<"T->T/F"<<endl;
	return -2;
}

int LR1Parse::reduce7()
{
	this->status.pop();
	cout<<"T->F"<<endl;
	return -2;
}

int LR1Parse::reduce8()
{
	for (int i = 0; i < 3; ++i) {
		this->status.pop();
	}
	cout<<"F->(E)"<<endl;
	return -3;
}

int LR1Parse::reduce9()
{
	this->status.pop();
	cout<<"F->id"<<endl;
	return -3;
}
