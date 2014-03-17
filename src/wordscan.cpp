//============================================================================
// Name        : wordscan.cpp
// Author      : Junyuan Hong
// Version     : 1.0
// Copyright   : copy for free
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "LexScan.h"
#include <stdio.h>
#include <string.h>
using namespace std;

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
	cout << "Welcome!" << endl; // prints Welcome!
	LexScan *ls=new LexScan();

	//init
	const char *str="x:=5;  if (x>0)  then  x:=2*x+1/3;  else  x:=2/x;  #";
	ls->SetCode(str, len(str));

	do{
		ls->Lexical(); //将词法单元对应的记号保存到token中，属性值保存到num或者id中
		switch(ls->token) {
			case 11: printf ("(11,%d)", ls->num); break;
			case -1: printf("error!\n");return -1;
			default: printf("(%d,%s)", ls->token, ls->id);
		}
	}while (ls->token!=0);
	putchar('\n');

	return 0;
}
