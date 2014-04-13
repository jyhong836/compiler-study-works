/*
 * WordStream.h
 *
 *  Created on: 2014年4月1日
 *      Author: jyhong
 */

#ifndef WORDSTREAM_H_
#define WORDSTREAM_H_

#include "wordscan.h"

class WordStream {
private:
	Word *stream;
	int size;
	int cur;
public:
	WordStream(Word *stream, int size);
	int getWord(Word &);
	int getCur();
	int getSize();
	virtual ~WordStream();
};

//struct WordStream
//{// 词法记号流

//};

#endif /* WORDSTREAM_H_ */
