/*
 * Parsing.h
 *
 *  Created on: 2014-3-23
 *      Author: jyhong
 *      使用递归下降算法
 */

#ifndef PARSING_H_
#define PARSING_H_

#include "wordscan.h"

class Parsing
{
private:
	int lookahead;
	Word *stat;
	int statlen;
	int cur;
	int next(); // 移动光标

public:
	int silent;
	Parsing(int);
	int setStatement(Word *stat1, int len);
	int E();
	int T();
	int F();
	int E1();
	int T1();
	int match(int token);
	int lookforward(); // update lookahead
	int isEnd();

	int run(); // 非递归预测分析

  virtual
	~Parsing();
};

#endif /* PARSING_H_ */
