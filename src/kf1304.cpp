//============================================================================
// Name        : kf1304.cpp
// Author      : test1
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include "slaver_test.h"
#include "GlobalHelper.h"

using namespace std;
using namespace poseidon;
#define DEBUG
int main()
{
	//

	//(new GlobalHelper())->time_test2();//pass
//	(new GlobalHelper())->string_contain_test();

//slaver_test_getHttpHeader();//pass
	//slaver_test_grab(2); //pass
	slaver_test_grab_3(); //pass

	return 0;
}
