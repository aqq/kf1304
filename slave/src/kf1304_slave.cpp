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
	//(new slaver())->request_task_setsocketop_expectin();
//	(new slaver())->request_task_timeo_test3();
	//GlobalHelper
//	(new GlobalHelper())->call_updata_shell_test();
	//(new GlobalHelper())->read_line_from_test();
	//slave to rep

//	(new GlobalHelper())->time_test2(); //pass

//	(new GlobalHelper())->command_str_to_map_test();

//	(new GlobalHelper())->trave_dir_into_vec_test(); //pass	// read file in menu
// tar_pages_and_get_fnames_vec_test();//pass
// page_read_binary_test(); //
// message to hand
	//remoteStorePage_test(); //pass  send and revice msg
//store_page_test();
// hand msg
//-----------------------------
//	(new GlobalHelper())->log2_test();
	//(new GlobalHelper())->page_read_binary_test(); //pass

	//(new GlobalHelper())->time_test2();//pass
	//(new GlobalHelper())->string_contain_test();//pass
	//(new GlobalHelper())->split_line_test(); //pass
	//(new GlobalHelper())->split_test();//pass
	//(new GlobalHelper())->replace_test(); //pass
	//(new GlobalHelper())->convert_url_to_http_req_test(); //pass
	//(new GlobalHelper())->log_test(); //pass

	//(new GlobalHelper())->test_strlen_size_len(); //pass
	//	(new GlobalHelper())->get_time_str_test(); //pass
	//(new GlobalHelper())->is_html_end_test(); //pass
	//(new GlobalHelper())->delete_self_test(); //pass

	//(new GlobalHelper())->convert_url_to_http_req_test();
//	(new GlobalHelper())->get_status_of_head_test();

	//grab
	//-------------------------------
// 	slaver_test_grab(); //pass
	//slaver_test_grab_3(); //pass
	//prepare_urls_test(); //  pass
	//str2task_test(); //  pass
//	slaver_test_prepare_req_cmd();
	//slaver_test_request(); //pass
	//get_ip_by_name_test(); //test ip
	//lookup_ip_test(); //pass
//	grabpage_work_test(); //pass
//	hand_response_test(); //pass
//	str2task_test();
	//split_response_status();
	slaver_work_test(); //final test

	//slaver_test_grab_for_login();

//----------sys call
	//(new GlobalHelper())->call_updata_shell_test();
	//(new GlobalHelper())->auto_restart_test();

	return 0;
}
