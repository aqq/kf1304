/*
 * TextLogger.h
 *
 *  Created on: 2012-5-13
 *      Author: root
 */

#ifndef TEXTLOGGER_H_
#define TEXTLOGGER_H_
#include <iostream>

using namespace std;

namespace poseidon
{

class TextLogger
{
private:
	const char * fname;
	TextLogger();
public:
	TextLogger(const char* filename);
	virtual ~TextLogger();
	//void Log(const char* content);
	void LogContent(string content);

};

}/* namespace sisyphus */
#endif /* TEXTLOGGER_H_ */
