/*
 * TextLogger.cpp
 *
 *  Created on: 2012-5-13
 *      Author: root
 */

#include "TextLogger.h"
#include <iostream>
#include "fstream"
using namespace std;
namespace poseidon
{

TextLogger::TextLogger(const char* filename)
{

	fname = filename;
	//	strcpy(filename, fname);
}

TextLogger::TextLogger()
{

	fname = "log.txt";
	//	strcpy(filename, fname);
}
TextLogger::~TextLogger()
{

}

void TextLogger::LogContent(string content)
{

	string str5 = content;
	ofstream outfile(fname, ios::app);
	outfile << str5;
	outfile.close();

}

} /* namespace sisyphus */
