/*
 * LexScan.h
 *
 *  Created on: 2014-3-16
 *      Author: Junyuan Hong
 */

#ifndef LEXSCAN_H_
#define LEXSCAN_H_

class LexScan {
public:
	LexScan(int);
	virtual ~LexScan();

public:
	int SetCode(const char *str, int len);
	int getCur();
	int Lexical();
	int token;
	char id[64];
	int num;
	int idMaxLen;
	int keywordnum;
	int opnum;
	int silent;

private:
	const char *str;
	char keyword[6][8];// keywords
	int keywordtoken[6];
	char operat[16][6];// operators
	int operattoken[16];
	int cur;
	int strlen;
	/* for all below: if success, add cur */
	int getword(const char *str);//return status, save word to id, save token, update cur
	int getnum(const char *str);//return status, save num to num, save token, update cur
	int checkKeyword();//check if the id is a keyword.return status, save keyword to id, save token, update cur
	int getOperater(const char *str);//return status, save op to id, save token, update cur
};

#endif /* LEXSCAN_H_ */
