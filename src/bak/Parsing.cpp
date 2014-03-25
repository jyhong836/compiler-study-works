/*
 * Parsing.cpp
 *
 *  Created on: 2014-3-23
 *      Author: jyhong
 */

#include "Parsing.h"
#include <iostream>
#include <stack>
using namespace std;

Parsing::Parsing(int silent)
{
	lookahead=NULL;
	statlen = 0;
	this->silent=silent;
	cur = 0;
}

Parsing::~Parsing()
{
  // TODO Auto-generated destructor stub
}

int Parsing::E()
//E->TE'
{
	if (lookahead==27||
			lookahead==10)
	{ // 27->'(',10->id; First(E)={'(',id}
		if (!silent)
			cout<<"E  -> TE'"<<endl;
		if (T()!=0||E1()!=0)return -1;
	}
	else if(lookahead==28)
	{//')'
	    cout<<"unexpected ')'"<<endl;
	    do{
	        match(28);
	    }while(lookahead==28);
	    E();
	}
	else
	{
		cout<<"require '('(27) or id(10) get:token("<<lookahead<<")"<<endl;
		return -1;
	}
	return 0;
}

int Parsing::E1()
// E' -> +TE | epsilon
{
	if (lookahead==13)//13->'+'
	// first（E‘）匹配
	{
		if (!silent)
			cout<<"E' -> +TE"<<endl;
		if (match(13)!=0||T()!=0||E1()!=0)return -1;
	}
	else if (lookahead==28||lookahead==0||lookahead==13)
	{
		if (!silent)
			cout<<"E' -> epsilon"<<endl;
	}
	else
	{
		cout<<"unexpected token:"<<lookahead<<" @ E'"<<endl;
		if (lookahead==10)
		{
			// 跳过id
			cout<<"ignore token:"<<lookahead<<"\n  输入串跳过记号id，用户多输入了一以上的id"<<endl;
			do {
				cout<<"  忽略id"<<endl;
				match(lookahead);
			}while(lookahead==10);
			return 0;
		}
		return -1;
	}
	return 0;
}

int Parsing::T()
// T->FT'
{
	if (lookahead==27||
			lookahead==10)
	{ // 27->'(',10->id; First(T)={'(',id})
		if (!silent)
			cout<<"T  -> FT'"<<endl;
		if (F()!=0||T1()!=0)return -1;
	}
	else
	{
		cout<<"unexpected token:"<<lookahead<<" @ T->FT'"<<endl;
		if (lookahead==13)
		// 又遇到一个'+'
		{
			cout<<" Warning:用户少输入了一个id,'+'需要一个id"<<endl;
			do {
				cout<<" 忽略+,";
				match(13);
			}while(lookahead==13);
			cout<<" 尝试重新匹配"<<endl;
			if (T()!=0)
			{
				cout<<"匹配失败，弹栈，弹出非终结符T";//重新匹配
				return -1;
			}
			return 0;
		}
		else if(lookahead==28)//右括号
		{
			cout<<" Warning:不匹配的右括号"<<endl;
			do{
				match(28);
				cout<<" 忽略')',";
			}while(lookahead==28);
			cout<<" 尝试继续匹配"<<endl;
			if (T()!=0){
				cout<<" 匹配失败，弹出T";
				return -1;
			}
			return 0;
		}
		return -1;
	}
	return 0;
}

int Parsing::F()
// F->(E) | id
{
	if (lookahead==27)
	{ // 27->'(',10->id; First(T)={'(',id})
		if (!silent)
			cout<<"F  -> (E)"<<endl;
		if (match(27)!=0||E()!=0)
			return -1;
		if (match(28)!=0)//不匹配')'
		{
			cout<<" 弹栈，弹出终结符 ) ，括号不匹配"<<endl;
			lookahead = next();
		}
	}
	else if(lookahead==10)
	{
		if (!silent)
			cout<<"F  -> id"<<endl;
		match(10);
	}
	else
	{
		cout<<"unexpected token:"<<lookahead<<" @ F->(E) | id"<<endl;
		if (lookahead==15)
		{
			cout<<" Warning:用户少输入了一个id,'*'需要一个id"<<endl;
			do{
				cout<<" 忽略*,";
				match(15);
			}while(lookahead==15);
			cout<<"尝试重新匹配"<<endl;
			if (F()!=0)
			{
				cout<<"匹配失败，弹栈，弹出非终结符F"<<endl;
				return -1;
			}
			return 0;
		}
		return -1;
	}
	return 0;
}

int Parsing::T1()
// T'->*FT' | epsilon
{
	if (lookahead==15) {
		// 15->'*'
		if (!silent)
			cout<<"T' -> *FT"<<endl;
		if (match(15)!=0||F()!=0||T1()!=0)return -1;
	}
	else if (lookahead==28||lookahead==0||lookahead==13)
		// ')' | '#' | '+'
	{
		if (!silent)
			cout<<"T' -> epsilon"<<endl;
	}
	else
	{
		cout<<"unexpected token:"<<lookahead<<" @ T'"<<endl;
		if (lookahead==10)
		{
			// 跳过id
			cout<<"ignore token:"<<lookahead<<"\n 输入串跳过记号id，用户多输入了一个以上的id"<<endl;
			do {
				cout<<" 忽略id,";
				match(lookahead);
			}while(lookahead==10);
			cout<<" 尝试继续匹配"<<endl;
			if (T1()!=0)
			{
				cout<<"匹配失败，弹出T'"<<endl;
				return -1;
			}
			return 0;
		}
		return -1;
	}
	return 0;
}

int Parsing::match(int token)
{
	if (lookahead == token) {
//		cout<<"matched:"<<lookahead<<endl;
		lookahead = next();
	}
	else
	{
		cout<<"unmatched:lookahead="<<lookahead<<" not match token="<<token<<endl;
		return -1;
	}
	return 0;
}

int Parsing::setStatement(Word *stat1, int len)
{
	if (stat1==NULL)
	{
		cout<<"error:stat1 is NULL @ setStatment"<<endl;
		return -1;
	}
	stat = stat1;
	statlen = len;
	return 0;
}

/**
 * 获取下一个记号，若结束，则返回0
 */
int Parsing::next()
{
	if (cur<statlen) {
		if (stat[cur].token!=ENDTOKEN)
		{
			return stat[++cur].token;
		}
		else
		{
			cout<<"statement end"<<endl;
			return 0;
		}
	}
	else
	{
		cout<< "cur>=statlen"<<endl;
	}
	return -1;
}

int Parsing::lookforward()
{
	lookahead = this->stat[cur].token;
	return lookahead;
}

int Parsing::isEnd()
/*
 * 如果光标已经到达句子末尾，则返回1
 * 否则返回当前光标位置
 */
{
	if (lookahead==0)
		return 0;
	return cur;
}

int Parsing::run()
// 非递归预测分析方法
{
    stack<char> w;//用e代表E',用t代表T'
    return 0;
}

