//============================================================================
// Name        : kf1304_master.cpp
// Author      : andrew
// Version     :
// Copyright   : newplayer
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "slaver_test.h"
#include "master_test.h"
#include "GlobalHelper.h"

using namespace std;
using namespace poseidon;
#define DEBUG
int main() {
	//(new GlobalHelper())->time_test2();//pass测试计算耗时函数
	//(new GlobalHelper())->string_contain_test();//pass 测试 字符串包含函数
	//(new GlobalHelper())->split_line_test(); //pass 测试 字符串包含函数

	//slaver_test_getHttpHeader();//pass测试替换HTTP头
	//slaver_test_grab(2); //pass
	//	slaver_test_grab_3(); //pass
	master_test2slave(); //test pass

	return 0;
}