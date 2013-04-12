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
	// TODO Auto-generated constructor stub
	fname = filename;
	//	strcpy(filename, fname);
}

TextLogger::TextLogger()
{
	// TODO Auto-generated constructor stub
	fname = "log.txt";
	//	strcpy(filename, fname);
}
TextLogger::~TextLogger()
{
	// TODO Auto-generated destructor stub
}

void TextLogger::LogContent(string content)
{

	string str5 = content;
	ofstream outfile(fname, ios::app);
	outfile << str5;
	outfile.close();

}

} /* namespace sisyphus */
