//============================================================================
// Name        : kf1304_slave.cpp
// Author      : andrew
// Version     :
// Copyright   : newplayer
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "slaver_test.h"
#include "GlobalHelper.h"

using namespace std;
using namespace poseidon;
#define DEBUG
int main() {
	//

	//(new GlobalHelper())->time_test2();//pass
	//(new GlobalHelper())->string_contain_test();//pass
	//(new Gread_buflobalHelper())->split_line_test();//pass
	//(new GlobalHelper())->split_test();//pass
	//(new GlobalHelper())->replace_test(); //pass
	//(new GlobalHelper())->convert_url_to_http_req_test(); //pass

	//slaver_test_grab(2); //pass
	//	slaver_test_grab_3(); //pass
	str2task_test(); //  pass

	//slaver_test_request(); //pass
	//slaver_test_prepare_req_cmd();
 //slaver_test();

	return 0;
}
