/*
 * Parsing.h
 *
 *  Created on: 2014-3-23
 *      Author: jyhong
 *      使用递归下降算法
 */

#ifndef PARSING_H_
#define PARSING_H_

#include <vector>
#include "wordscan.h"

using namespace std;

class Parsing
{
private:
	int lookahead;
	Word *stat;
	int statlen;
	int cur;
	vector<int> sucCur;
	int next(); // 移动光标
	int error(vector<int> follow_sync, int (Parsing::*func)(vector<int>));
//	int ignore(int);

public:
	int silent;
	Parsing(int);
	int setStatement(Word *stat1, int len);
	int S();
	int E(vector<int> sync);
	int T(vector<int> sync);
	int F(vector<int> sync);
	int E1(vector<int> sync);
	int T1(vector<int> sync);
	int match(int token);
	int lookforward(); // update lookahead
	int isEnd();

	int run(); // 非递归预测分析

  virtual
	~Parsing();
};

#endif /* PARSING_H_ */
