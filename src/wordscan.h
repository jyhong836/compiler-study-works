/*
 * wordscan.h
 *
 *  Created on: 2014-3-24
 *      Author: jyhong
 */

#ifndef WORDSCAN_H_
#define WORDSCAN_H_


struct Word{
	int token; // 词法记号
	char id[64]; //id
	int num; // 数值
	int strCur; // 在句子字符串中的位置
};
#define ENDTOKEN 0
// 结束字符

#endif /* WORDSCAN_H_ */
