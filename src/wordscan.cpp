//============================================================================
// Name        : wordscan.cpp
// Author      : Junyuan Hong
// Version     : 1.0
// Copyright   : Free
// Description : Word Analysis in C++, Ansi-style
// Coding	   : UTF-8
// GitHub	   : https://github.com/jyhong836/
// 功能		   : 词法分析，语法分析（递归下降方法）
// compiler    : gcc-4.4
// os          : ubuntu10.04
//============================================================================

#include <iostream>
#include "LexScan.h"
#include "Parsing.h"
#include "wordscan.h"
#include "LR1Parse.h"
#include <stdio.h>
#include <string.h>

using namespace std;

int wordscan(const char *);
int lexfunc(LexScan *ls, int silent, const char *str, Word *w);
int parsingfunc(Parsing *pars, int tkcur, Word *w, const char *str);

int len(const char *str) {
	int l=0;
	const char *p=str;
	while(*p!=0)
	{
		p++;
		l++;
	}
	return l;
}

int main() {
#ifndef TEST
    cout << "Welcome to wordscan!\n  coding:utf-8\n" << endl; // prints Welcome!
    cout << "+,-,*,/算术表达式语法分析器"<<endl;

    const char *str0="((x  +  y+(x*y)+x/z/(x-y*x)#";
    char str[128] = {0};
    cout<<"示例语句："<<str<<endl;
    while(1)
    {
        cout<<"==========================================="<<endl;
        cout<<"请输入要分析的句子,以#号结束,只输入#则使用默认的句子"<<endl;
        cout<<"==========================================="<<endl;
        scanf("%s", str);
        int l = len(str);
        if(!l||str[0]=='#'||str[l-1]!='#')
            wordscan(str0);
        else
            wordscan(str);
    }
#else
    return LR1Parse_test();
#endif
}

int wordscan(const char *str)
//*******************分析的开始***********************
{
	int silent = 1;
	LexScan *ls=new LexScan(silent);
	const int wordBuffNum = 128;
	Word w[wordBuffNum]; // 用来保存词法分析器返回的词法记号,词法单元

	for (int i=0;i<wordBuffNum;i++)
	{
		w[i].num = 0;
		for (int j=0;j<64;j++)
		{
			w[i].id[j] = 0;
		}
		w[i].token = 0;
	}
	//const char *str="x:=5;  if (x>0)  then  x:=2*x+1/3;  else  x:=2/x;  #";

	// 词法分析器，返回的词法记号和单元保存在Word *w中
	int tkcur = lexfunc(ls, silent, str, w);
	if (!tkcur)
	    return -1;

    Parsing *pars = new Parsing(0);

    //语法分析
    LR1Parse lr(0);
    WordStream *ws = new WordStream(w, tkcur);// tkcur 就是w的长度

    //************************实验3：LR(1)分析器************************
    return lr.parse(ws); // ws 是词法记号流，保存了从词法分析器中分析出来的词法记号
//    return parsingfunc(pars, tkcur, w, str);//parsing返回状态值0或非0，以后可能用来返回记号给词法分析器
}

int lexfunc(LexScan *ls, int silent, const char *str, Word *w)
//************************实验1词法分析************************
{

	ls->SetCode(str, len(str));

	// 语法分析
	int tkcur = 0;

	cout<<"开始词法分析"<<endl<<"(词法记号，词法单元)"<<endl;
	do{
		ls->Lexical(); //将词法单元对应的记号保存到token中，属性值保存到num或者id中
		w[tkcur].strCur = ls->getCur();
		switch(ls->token) {
			case 11:
				printf ("(11,%d)", ls->num);
				w[tkcur].token = 11;
				w[tkcur].num = ls->num;
				tkcur++;
				break;
			case -1:
				cout<<"ERROR: stoped in"<<endl<<str<<endl;
				tkcur = ls->getCur();
				for (int i=0;i<tkcur;i++)
					putchar(' ');
				putchar('|');
				return 0;
			default:
				w[tkcur].token = ls->token;
				strcpy(w[tkcur].id, ls->id);
				printf("(%d,%s)", ls->token, w[tkcur].id);// ls->id);
				tkcur++;
		}
		putchar('\n');
	}while (ls->token!=0);
	putchar('\n');
	return tkcur;
}



int parsingfunc(Parsing *pars, int tkcur, Word *w, const char *str)
//--------------------实验3语法分析-------------------
{
    // 语法分析，将词法分析的结果通过Word *w传递给语法分析器parsing
    cout<<"算术语法分析（要求以#结束）开始..."<<endl;
    cout<<"mode=unsilent"<<endl;
    cout<<"允许的语法：id和+,*,()组成的算术表达式"<<endl;

    pars->setStatement(w, tkcur); // 将词法记号流保存到parsing中
    pars->lookforward(); // 初始化lookahead

    pars->S(); // 开始语法分析
    tkcur = pars->isEnd();
    if (tkcur==0)
    {
        cout<<"分析成功，这是一个+,*算术表达式"<<endl;
        return 0;
    }
    else
    {
        cout<<"\nERROR:语法分析提前结束,结束位置"<<endl<<" ";
        cout<<str<<"的第"<<(tkcur+1)<<"个词法单元：（"<<w[tkcur].token<<",";
        (w[tkcur].id[0]==0)?(cout<<w[tkcur].num):(cout<<w[tkcur].id);
        cout<<") "<<endl;
        int cur = w[tkcur].strCur;
        for (int i=0;i<cur;i++)
            putchar(' ');
        putchar('|');
        cout<<endl;
        return -1;
    }
    cout<<endl;

    return 0;
}
