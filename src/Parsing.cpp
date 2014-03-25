/*
 * Parsing.cpp
 *
 *  Created on: 2014-3-23
 *      Author: jyhong
 */

#include "Parsing.h"
#include <iostream>
//#include <vector>

#define NO_MARCH_DEBUGE

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

int Parsing::error(vector<int> follow_sync, int (Parsing::*func)(vector<int>))
/* 忽略当前的记号直到同步符号的出现
 * 递归下降分析的错误恢复
 * 忽略若干输入符号，直至同步集合follow_sync中的符号的出现
 * 为止，然后分析过程结束。
 *
 * 即：这个错误恢复方法直接将这个产生式废掉了。对应的实际情况
 * 是+号后面缺少了一个id,则寻找id的Follow,找到则结束id，报
 * 错：缺少id，弹出产生式
 */
{
    cout<<"错误恢复，";
    // 判断当前符号是否是同步符号，是则结束，否则跳过，并重新进行语法分析
    for (int i=follow_sync.size()-1;i>=0;i--)
    {
        if (lookahead==follow_sync[i])
        {
//            cout<<"cur="<<cur;
            cout<<"弹栈"<<endl;//在调用函数中输出弹出的产生式。
            return 0;//成功处理错误
        }
    }
    cout<<"忽略未知字符：";
    (stat[cur].id[0]==0)?(cout<<stat[cur].num):(cout<<stat[cur].id);
    cout<<endl;
    lookahead = next();
    return (this->*func)(follow_sync);
}

//int Parsing::ignore(int first_token)
///* 忽略当前的记号直到First(E)
// * 跳过当前的输入符
// * 栈顶不变，继续栈顶符的分析。
// *
// * 比如，在没有id的情况下出现了一个+号，则忽略+，直到出现产生式
// * 的First。
// */
//{
//    while(lookahead!=first_token && lookahead!=0)
//    {
//        cout<<"忽略未知字符：";
//        (stat[cur].id[0]==0)?(cout<<stat[cur].num):(cout<<stat[cur].id);
//        cout<<endl;
//        lookahead = next();
//    }
//    return 0;
//}

int Parsing::S()
{
    vector<int> sync;
    sync.push_back(0);//'#
//    sync.push_back(28);//')'
    int ret = E(sync);
    if (stat[cur].token==28)//')'
    {
        cout<<"发现不匹配的右括号"<<endl;
    }
    cout<<"分析出的表达式："<<endl;
    for (int i=0;i<sucCur.size();i++)
    {
        (stat[sucCur[i]].id[0]==0)?
                (cout<<stat[sucCur[i]].num):
                (cout<<stat[sucCur[i]].id);
    }
    cout<<endl;
    return ret;
}

int Parsing::E(vector<int> sync)
//E->TE',Follow(E) = #
{
	if (lookahead==27||
			lookahead==10)
	{ // 27->'(',10->id; First(E)={'(',id}
		if (!silent)
			cout<<"E  -> TE'"<<endl;
//		if (T()!=0||E1()!=0)return -1;
		sync.push_back(13);//'+'
		T(sync);
		sync.pop_back();
		E1(sync);
	}
	else
	{
	    return error(sync, &Parsing::E);
	}
//	else if(lookahead==28||lookahead==0)//')' || '#'
//	{
//	    int ret = error(sync);
//	    return ret;
//	}
//	else if(lookahead==28)
//	{//')'
//	    cout<<"unexpected ')'"<<endl;
//	    do{
//	        match(28);
//	    }while(lookahead==28);
//	    E();
//	}
//	else // error
//	{
//
////		cout<<"require '('(27) or id(10) get:token("<<lookahead<<")"<<endl;
////		return error(sync);
////		return -1;
//	}
	return 0;
}

int Parsing::E1(vector<int> sync)
// E' -> +TE | epsilon
{
	if (lookahead==13)//13->'+'
	// first（E‘）匹配
	{
		if (!silent)
			cout<<"E' -> +TE"<<endl;
//		if (match(13)!=0||T()!=0||E1()!=0)return -1;
		match(13);
		sync.push_back(13);
		T(sync);
		sync.pop_back();
		E1(sync);
	}
	else if (lookahead==28||lookahead==0)//||lookahead==13)
	    // ')' || '#'
	{
		if (!silent)
			cout<<"E' -> epsilon"<<endl;
	}
	else
	{
	    return error(sync, &Parsing::E1);
	}
//	else
//	{
//		cout<<"unexpected token:"<<lookahead<<" @ E'"<<endl;
//		if (lookahead==10)
//		{
//			// 跳过id
//			cout<<"ignore token:"<<lookahead<<"\n  输入串跳过记号id，用户多输入了一以上的id"<<endl;
//			do {
//				cout<<"  忽略id"<<endl;
//				match(lookahead);
//			}while(lookahead==10);
//			return 0;
//		}
//		return -1;
//	}
	return 0;
}

int Parsing::T(vector<int> sync)
// T->FT'
{
	if (lookahead==27||
			lookahead==10)
	{ // 27->'(',10->id; First(T)={'(',id})
		if (!silent)
			cout<<"T  -> FT'"<<endl;
//		if (F()!=0||T1()!=0)return -1;
		sync.push_back(15);//'*'
		F(sync);
		sync.pop_back();
		T1(sync);
	}
	else
	{
	    return error(sync, &Parsing::T);
	}
//	else
//	{
//		cout<<"unexpected token:"<<lookahead<<" @ T->FT'"<<endl;
//		if (lookahead==13)
//		// 又遇到一个'+'
//		{
//			cout<<" Warning:用户少输入了一个id,'+'需要一个id"<<endl;
//			do {
//				cout<<" 忽略+,";
//				match(13);
//			}while(lookahead==13);
//			cout<<" 尝试重新匹配"<<endl;
//			if (T()!=0)
//			{
//				cout<<"匹配失败，弹栈，弹出非终结符T";//重新匹配
//				return -1;
//			}
//			return 0;
//		}
//		else if(lookahead==28)//右括号
//		{
//			cout<<" Warning:不匹配的右括号"<<endl;
//			do{
//				match(28);
//				cout<<" 忽略')',";
//			}while(lookahead==28);
//			cout<<" 尝试继续匹配"<<endl;
//			if (T()!=0){
//				cout<<" 匹配失败，弹出T";
//				return -1;
//			}
//			return 0;
//		}
//		return -1;
//	}
	return 0;
}

int Parsing::F(vector<int> sync)
// F->(E) | id
{
	if (lookahead==27)
	{ // 27->'(',10->id; First(T)={'(',id})
		if (!silent)
			cout<<"F  -> (E)"<<endl;
		match(27);
		sync.push_back(28);//')'
		E(sync);
		sync.pop_back();
		if (match(28)!=0){
		    cout<<"  括号不匹配"<<endl;
		    return -1;
		}
//		if (match(27)!=0||E()!=0)return -1;
//		if (match(28)!=0)//不匹配')'
//		{
//			cout<<" 弹栈，弹出终结符 ) ，括号不匹配"<<endl;
//			lookahead = next();
//		}
	}
	else if(lookahead==10)
	{
		if (!silent)
			cout<<"F  -> id"<<endl;
		match(10);
	}
	else
	{
	    return error(sync, &Parsing::F);
	}
//	else
//	{
//		cout<<"unexpected token:"<<lookahead<<" @ F->(E) | id"<<endl;
//		if (lookahead==15)
//		{
//			cout<<" Warning:用户少输入了一个id,'*'需要一个id"<<endl;
//			do{
//				cout<<" 忽略*,";
//				match(15);
//			}while(lookahead==15);
//			cout<<"尝试重新匹配"<<endl;
//			if (F()!=0)
//			{
//				cout<<"匹配失败，弹栈，弹出非终结符F"<<endl;
//				return -1;
//			}
//			return 0;
//		}
//		return -1;
//	}
	return 0;
}

int Parsing::T1(vector<int> sync)
// T'->*FT' | epsilon
{
	if (lookahead==15) {
		// 15->'*'
		if (!silent)
			cout<<"T' -> *FT"<<endl;
//		if (match(15)!=0||F()!=0||T1()!=0)return -1;
		match(15);
		sync.push_back(15);
		F(sync);
		sync.pop_back();
        T1(sync);

	}
	else if (lookahead==28||lookahead==0||lookahead==13)
		// ')' | '#' | '+'
	{
		if (!silent)
			cout<<"T' -> epsilon"<<endl;
	}
	else
	{
	    return error(sync,&Parsing::T1);
	}
//	else
//	{
//		cout<<"unexpected token:"<<lookahead<<" @ T'"<<endl;
//		if (lookahead==10)
//		{
//			// 跳过id
//			cout<<"ignore token:"<<lookahead<<"\n 输入串跳过记号id，用户多输入了一个以上的id"<<endl;
//			do {
//				cout<<" 忽略id,";
//				match(lookahead);
//			}while(lookahead==10);
//			cout<<" 尝试继续匹配"<<endl;
//			if (T1()!=0)
//			{
//				cout<<"匹配失败，弹出T'"<<endl;
//				return -1;
//			}
//			return 0;
//		}
//		return -1;
//	}
	return 0;
}

int Parsing::match(int token)
{
	if (lookahead == token) {
#ifndef NO_MARCH_DEBUGE
		cout<<"matched:"<<lookahead<<endl;
#endif
		this->sucCur.push_back(cur);
		lookahead = next();
	}
	else
	{
//		cout<<"ERROR:unmatched token,lookahead="<<
//		        lookahead<<" not match token="<<token<<endl;
	    cout<<"ERROR:期望得到记号为"<<token<<"的符号，却得到：("
	            <<lookahead<<",";
//	    cout<<"忽略未知字符：";
	    (stat[cur].id[0]==0)?(cout<<stat[cur].num):(cout<<stat[cur].id);
	    cout<<")"<<endl;
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
//    stack<char> w;//用e代表E',用t代表T'
    return 0;
}

