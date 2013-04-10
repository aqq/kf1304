/*
 * StringHelper.cpp
 *
 *  Created on: 2013-1-17
 *      Author: root
 */

#include "StringHelper.h"
#include <string.h>
#include <stdio.h>

#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

#include <sstream>

#include <iterator>

using namespace std;
namespace poseidon {

StringHelper::StringHelper() {

}

StringHelper::~StringHelper() {

}
void StringHelper::CharCopy(char * des, char* src, int size) {
	for (int i = 0; i < size; i++) {
		*(des + i) = *(src + i);
	}

}

//---------------------------------------------------------------------------
// Description:将一行字符串在换行处截断。
// Input: <输入参数及其描述>
// Output: <输出参数及其描述>
// Return: <返回值及其描述>
// Exception: <异常情况描述>
//----------------------------------------------------------------------------
void StringHelper::LineOpt(char *thefileName) {
	int i = 0;
	while (1) {
		if (thefileName[i] != '\n')
			i++;
		else
			break;
	}
	thefileName[i] = 0;
}
//---------------------------------------------------------------------------
// Description:将一行字符串在换行处截断。
// Input: <输入参数及其描述>
// Output: <输出参数及其描述>
// Return: <返回值及其描述>
// Exception: <异常情况描述>
//----------------------------------------------------------------------------
void StringHelper::LineSplit(string s1, string* name, int* t1, int *t2) {

	int f1 = s1.find_first_of(':');
	int f2 = s1.find_first_of(',');
	int f3 = s1.find_first_of('.');

	string c1 = s1.substr(0, f1);
	string c2 = s1.substr(f1 + 1, f2 - f1 - 1);
	string c3 = s1.substr(f2 + 1, f3 - f2 - 1);
	*name = c1;
	*t1 = atoi(c2.c_str());
	*t2 = atoi(c3.c_str());
	cout << *name << endl;
	cout << *t1 << endl;
	cout << *t2 << endl;

	//list<string>::iterator i1;
	// list<string>::iterator i;//iterator
	/*for (i = list->begin(); i != list->end(); ++i)
	 cout << *i << " "<<endl;
	 */
	//cout<<<<endl;
	//cout<<*list[1]<<endl;
	//cout<<*list[2]<<endl;
}
void StringHelper::split(string in, vector<string> &vec) //以" "为分隔符
		{
	istringstream ss(in);
	string t;
	while (ss >> t) {
		vec.push_back(t);
	}

}
string StringHelper::readLine(string filename, int num) {
	FILE *fp;
	char *filep;
	char thefileName[1024];
	char *PCAPINDEX = NULL;

	if ((fp = fopen(filename.c_str(), "r")) == NULL) {
		printf("open file %s error!!\n", PCAPINDEX);
		return "";
	}

	while (num--) {
		filep = fgets(thefileName, 1024, fp); //每次调用文件指针fp会自动后移一行
		if (!filep) //文件读取结束则跳出循环
			break;
	}
	if (num > 0) {
		cout << filename << "行数不足" << num << endl;
	}
	string s1 = thefileName;
	int f1 = s1.find_first_of('\n');
	string s2 = s1.substr(0, f1);
	fclose(fp);
	return s2;
}
}

/* namespace sisyphus */