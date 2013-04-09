/*
 * master_test.cpp
 *
 *  Created on: 2013-4-1
 *      Author: root
 */
#ifndef MASTER_TEST_H
#define MASTER_TEST_H
#include "master.h"
#include <assert.h>

namespace poseidon {
void master_test2slave() {
	master * mt = new master();
	mt->service();

}
void hand_request_test() {
	master * mt = new master();
//req1 application_version:1 update
	string command_request_task = "commd_id:1\r\n"
			"slave_id:1\r\n"
			"application_version:1\r\n"
			"\f";
	string s2;
//	  s2 = mt->hand_request(command_request_task);
//	cout << "respose_content:\n" << s2 << endl;

//req2   get grab job
	command_request_task = "commd_id:1\r\n"
			"slave_id:1\r\n"
			"last_task_status:1\r\n"
			"application_version:2\r\n"
			"\f";
	s2 = mt->hand_request(command_request_task);
	cout << "respose_content2:" << s2 << endl;


	//req3 get grab job again
	command_request_task = "commd_id:1\r\n"
			"slave_id:1\r\n"
			"last_task_status:1\r\n"
			"application_version:2\r\n"
			"\f";
	s2 = mt->hand_request(command_request_task);
	cout << "respose_content3:" << s2 << endl;

}

void read_site_test() {
	master * mt = new master();
	vector<string> vec;

	struct command req_cmd;
	req_cmd.slave_id = 1;

	mt->update_slave_status(req_cmd);

	mt->read_urls(1, &vec);
	for (vector<string>::iterator it = vec.begin(); it < vec.end(); it++) {
		cout << (*it) << endl;
	}

}

void config_test() {
	//int assign_num;
	master * mt = new master();
	//mt->config();
	assert(mt->assign_url_number==2);
	cout << mt->assign_url_number << endl;

}
//

void update_slave_status_test() {
	master * mt = new master();
	struct command req_cmd;
	req_cmd.slave_id = 1;

	mt->update_slave_status(req_cmd);
	assert(mt->slave_map["1"].slave_id=="1");
	for (map<string, slave_status>::iterator it = mt->slave_map.begin();
			it != mt->slave_map.end(); it++) {
		//cout << "key:" << (*it).first << endl;
		//	cout << " v:" << (*it).second.slave_id << endl;
		//	cout << "alibb=?=" << (*it).second.site_status["alibb"].site_name
		//			<< endl;
	}
//
	mt->slave_map["1"].last_task_site = "alibb";
	req_cmd.last_task_status = 1;
	mt->update_slave_status(req_cmd);
	int good1 = mt->slave_map["1"].site_status["alibb"].good;
	//cout << "good1:" << good1 << endl;
	assert(good1==1);
	assert(good1==1);
	for (map<string, slave_status>::iterator it = mt->slave_map.begin();
			it != mt->slave_map.end(); it++) {
		cout << "key:" << (*it).first << " v:" << (*it).second.slave_id << endl;
	}
	return;
	//mt->update_slave_status(req_cmd);

}

void get_min_bad_of_sites_test() {
	master * mt = new master();
	struct command req_cmd;
	req_cmd.slave_id = 1;
	mt->update_slave_status(req_cmd);
	//mt->slave_map["1"].slave_id="1";
	mt->slave_map["1"].site_status["alibb"].bad = 4;
	mt->slave_map["1"].site_status["hc360"].bad = 2;
	mt->slave_map["1"].site_status["114ch"].bad = 3;
	string s1 = mt->get_min_bad_of_sites(1);
	cout << "get_min_bad_of_sites: " << s1 << endl;
	assert(s1=="hc360");
}
//
} /* namespace poseidon */
#endif
