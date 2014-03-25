/*
 * LexScan.cpp
 *
 *  Created on: 2014-3-16
 *      Author: Juyuan Hong
 */


#include <iostream>
#include "LexScan.h"
#include <stdio.h>
#include <stack>
using namespace std;

#define isa(c)   (((c)>='a'&&(c)<='z')||((c)>='A'&&(c)<='Z'))
#define isnum(c) ((c)>='0'&&(c)<='9')
#define isntrs(c) ((c)=='\n'||(c)=='\t'||(c)=='\r'||(c)==' ')

LexScan::LexScan(int silent) {
	this->silent = silent;
	str=NULL;
	strlen=0;
	token=0;
	idMaxLen=64;
	num=0;
	keywordnum=6;
	opnum=16;
	cur=0;
	for (int i=0;i<idMaxLen;i++)
	{
		id[i] = 0;
	}
	cout<<"读取词法记号表:mapping"<<endl;
	cout<<"mode=";
	(silent)?(cout<<"silent"):(cout<<"unsilent");
	cout<<endl;
	FILE *fp=fopen("mapping","r");
	if (!fp)
	{
		cout<< "error:cannot open file 'mapping'"<<endl;
	}
	for (int i=0;i<keywordnum;i++)
	{
		fscanf(fp,"%s",keyword[i]);
		fscanf(fp,"%d",&keywordtoken[i]);
		if (!silent)
			cout << " ["<<keywordtoken[i]<<"]: "<<keyword[i]<<endl;
	}
	for (int j=0;j<opnum;j++)
	{
		fscanf(fp,"%s",operat[j]);
		fscanf(fp,"%d",&operattoken[j]);
		if (!silent)
			cout << " ["<<operattoken[j]<<"]:"<<operat[j]<<endl;
	}
	fclose(fp);
	cout<<"成功读取词法记号"<<endl;
}

int LexScan::SetCode(const char *str1, int len)
// 保存句子
{
	cout << "you set string:\n\t" << str1 << endl;
	str = str1;
	strlen=len;
	return 0;
}

int LexScan::Lexical() {
	// get token,id,num
	if (cur==strlen)//check if the cur is end
	{
		cout<<"[0]error:cur==eof @LexScan::Lexical"<<endl;
		return -1;
	}

	while (isntrs(str[cur]))
		cur++;
	if (str[cur]=='#')// end
	{
		token=0;
		id[0]='#';
		id[2]=0;
		cur++;
		return 0;
	}
	if (getword(&str[cur])){//is keyword or word
		checkKeyword();// if is keyword, change the token, if not, no change
		return 0;
	}else if (isnum(str[cur])){
		if (getnum(&str[cur])){//is number
			return 0;
		}
	}
	// is operater, or#
	if (getOperater(&str[cur]))
		return 0;
	cout<<"error:cannot find char:"<<str[cur]<<" @ Lexical()"<<endl;
	token = -1;
	return -1;
}

int LexScan::getword(const char *str) {
	// return status, save word to id, save token, update cur
	int i=0;
	if (isa(str[i])) // check the first char
	{
		id[i]=str[i];
		i++;
	}else // the first char is not alpha, return 0
	{
		return 0;
	}
	while(str[i]!=0)
	{
		if(isa(str[i])||isnum(str[i]))
			id[i]=str[i];
		else { // find epsilon
			id[i] = 0;//'\0'
			break;
		}
		i++;

		if (i>=idMaxLen)
		// check for flow out
		{
			cout<<"flow out of id @LexScan::getword"<<endl;
			token=-1;
			return 0;// return success, but token is -1, means error
		}
	}
	cur+=i;
	token=10;
	return 1;
}

int LexScan::getnum(const char *str) {
	//return status, save num to num, save token, update cur
	int i=0;
	char id[128]={0};
	if (isnum(str[i])) // check the first char
	{
		id[i]=str[i];
		i++;
	}else // the first char is not alpha, return 0
	{
		return 0;
	}
	while(str[i]!=0)
	{
		if(isnum(str[i]))
			id[i]=str[i];
		else { // find epsilon
			id[i] = 0;//'\0'
			break;
		}
		i++;

		if (i>=idMaxLen)
		// check for flow out
		{
			cout<<"flow out of id @LexScan::getword"<<endl;
			token=-1;
			return 0;// return success, but token is -1, means error
		}
	}
	num=0;
	int base=1;
	for (int j=i-1;j>=0;j--)
	{
		num+=(id[j]-'0')*base;
		base*=10;
	}
	cur+=i;
	token=11;
	return 1;
}

int LexScan::checkKeyword() {
	// check if the id is a keyword
	int j=0;
	for (int i=0;i<keywordnum;i++)
	{
		j=0;
		while (keyword[i][j]!=0&&id[j]!=0)
		{
			if (keyword[i][j]!=id[j])
				break;
			j++;
		}
		if (keyword[i][j]==id[j])//both 0 or equal
		{
			token=keywordtoken[i];
		}
	}
	return 0;
}

int LexScan::getOperater(const char *str) {
	char c = str[0];
	// 如果两个操作符开头相同，则继续看第二个
	for (int i=0;i<opnum;i++)
	{
		if (c==operat[i][0])
		{
			id[0]=str[0];
			if (!isa(str[1])&&!isnum(str[1])&&!isntrs(str[1])&&operat[i][1]!=0)
			{
			// is operater
				if (str[1]==operat[i][1])
				{
					id[1]=str[1];
					id[2]=0;
					token=operattoken[i];
					cur+=2;
					return 1;
				}
			} else {
				id[1]=0;
				token=operattoken[i];
				cur+=1;
				return 1;
			}
		}
	}
	token=0;
	return 0;
}

int LexScan::getCur()
{
	return cur;
}

LexScan::~LexScan() {
	cout<<"exit LexScan, Byebye!"<<endl;
}
