/*
 * LR1Parse.h
 *
 *  Created on: 2014年4月1日
 *      Author: Junyuan Hong
 */

#ifndef LR1PARSE_H_
#define LR1PARSE_H_

#include "wordscan.h"
#include <map>
#include <string>
#include <stack>

using namespace std;

struct cmdPair
{
	char cmd;
	int cmdNum;
};
#define ALPHA_SIZE 31
typedef struct _alpha
{
	char name;
	int token;
	struct cmdPair cmd[ALPHA_SIZE];
	int size;
}alpha;


//typedef (int (*ERROR_FUNC)(void)) error_func;

class LR1Parse
{
private:
//	Word *wordstream;
	struct WordStream *ws;     // 词法记号流
	map<int, alpha> alpha_map; // 分析表

//	int status;//当前状态
	stack<int> status;         // 状态栈
	Word curw;				   // 当前的词法单元

//	int errorNum;//error#
	int error(int);
	int error0();
	int error1();
	int error2();
	int error3();
	int error4();
	int error5();

//	int reduceNum;//reduce#
	int reduce(int);
	int reduce1();
	int reduce2();
	int reduce3();
	int reduce4();
	int reduce5();
	int reduce6();
	int reduce7();
	int reduce8();
	int reduce9();

public:
    LR1Parse(FILE *);
    int parse(WordStream*);
    virtual
    ~LR1Parse();
};

int LR1Parse_test();

#endif /* LR1PARSE_H_ */
