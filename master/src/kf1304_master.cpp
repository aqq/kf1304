//============================================================================
// Name        : kf1304_master.cpp
// Author      : andrew
// Version     :
// Copyright   : newplayer
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "master_test.h"
#include "GlobalHelper.h"
#include "Cpp2mysql.h"
using namespace std;
using namespace poseidon;

#define DEBUG
int main() {

	//(new GlobalHelper())->time_test2();//pass测试计算耗时函数
	//(new GlobalHelper())->string_contain_test();//pass 测试 字符串包含函数
	//(new GlobalHelper())->split_line_test(); //pass 测试 字符串包含函数
	//	(new GlobalHelper())->init_command_map_test();//pass
	//(new GlobalHelper())->get_time_str_test();
//	(new GlobalHelper())->replace_test();
//	(new GlobalHelper())->log2_test();
//------------------------------
//cpp2mysql
	//(new Cpp2mysql())->update_worker_tb_test();
	//(new Cpp2mysql())->update_worker_site_tb_test();

//	c->connectTest();

	//--------------------------------
	//A dynamic assign urls
	//1.read conf and test
	// config_test();
	//2. add new slave into slave_status
	// update_slave_status_test();
	//3.get_min_bad_of_sites
	// get_min_bad_of_sites_test();
	//4.continue to read
	//read_sitefile_lines_test();
	//read_site_test(); //pass
	//--------------------------------
//build pages
	// build_csdn_url();
	//hand_request_test(); //pass
	//
	//need_sleep_test();

//	hand_slave_request_test();

	master_test2slave(); //final test

	return 0;
}
