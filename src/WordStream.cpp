/*
 * WordStream.cpp
 *
 *  Created on: 2014年4月1日
 *      Author: jyhong
 */

//#include "wordscan.h"
#include "WordStream.h"
#include <iostream>

WordStream::WordStream(Word *stream, int size) {
	// TODO Auto-generated constructor stub
	this->stream = stream;
	this->size = size;
	this->cur = 0;
}

WordStream::~WordStream() {
	// TODO Auto-generated destructor stub
}

int WordStream::getWord(Word &w)
/* return cur */
{
	if (cur==size)
	{
		std::cout<<"已经到达记号流末尾，返回的将是最后一个记号，这与上次返回的记号重复!"<<std::endl;
	}
	w = this->stream[cur];
	cur++;
	return cur;
}

int WordStream::getCur()
{
	return this->cur;
}

int WordStream::getSize()
{
	return this->size;
}

