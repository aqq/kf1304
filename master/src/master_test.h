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
	//
	//foreach
	for (map<string, site_info>::iterator it2 = mt->url_map.begin();
			it2 != mt->url_map.end(); ++it2) {
		std::cout << it2->first << " : " << it2->second.finished_length << '\n';
	}

	vector<string> vec;

	struct req_command req_cmd;
	int i = 14;

	req_cmd.slave_id = 1;
	req_cmd.last_task_status = 2;
	mt->update_slave_status_in_memory(req_cmd);

	mt->read_urls_and_record(1, &vec);
	for (vector<string>::iterator it = vec.begin(); it < vec.end(); it++) {
		cout << "url:" << (*it) << endl;
	}
	while (i--) {
		cout << "read again 1:" << endl;
		req_cmd.last_task_status = 1;
		mt->update_slave_status_in_memory(req_cmd);
		vec.clear();
		mt->read_urls_and_record(1, &vec);
		for (vector<string>::iterator it = vec.begin(); it < vec.end(); it++) {
			cout << "url:" << (*it) << endl;
		}
	}
	return;
	req_cmd.last_task_status = 0;
	mt->update_slave_status_in_memory(req_cmd);
	cout << "read again 2:" << endl;
	vec.clear();
	mt->read_urls_and_record(1, &vec);
	for (vector<string>::iterator it = vec.begin(); it < vec.end(); it++) {
		cout << "url:" << (*it) << endl;
	}
}

void config_test() {
	//int assign_num;
	master * mt = new master();
	//mt->config();
	assert(mt->assign_url_number==2);
	cout << mt->store_port << endl;

}
//

void update_slave_status_test() {
	master * mt = new master();
	struct req_command req_cmd;
	req_cmd.slave_id = 1;

	mt->update_slave_status_in_memory(req_cmd);
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
	mt->update_slave_status_in_memory(req_cmd);
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
	struct req_command req_cmd;
	req_cmd.slave_id = 1;
	mt->update_slave_status_in_memory(req_cmd);
	//mt->slave_map["1"].slave_id="1";
//	mt->slave_map["1"].site_status["alibb"].bad = 4;
	mt->slave_map["1"].site_status["hc360"].bad = 2;
	mt->slave_map["1"].site_status["114ch"].bad = 3;
	string s1;
	mt->get_min_bad_of_sites(1, s1);
	cout << "get_min_bad_of_sites: " << s1 << endl;
	assert(s1=="hc360");
}
//

void read_sitefile_lines_test() {
	vector<string> vec_urls;
	master * mt = new master();
	string s1 = "114ch_badurls";
	long i1 = 0;
	mt->read_sitefile_lines(i1, s1, &vec_urls);
	cout << vec_urls[0] << endl;
	cout << vec_urls[1] << endl;
}
//
void build_csdn_url() {
	ofstream ofs("./urls/csdn");
	int i = 1;
	while (i < 10000) {
		ofs << "http://bbs.csdn.net/topics/" << i << endl;
		i++;
	}
	ofs.close();
}
//
void need_sleep_test() {
	struct req_command req_cmd;
	master * mt = new master();
	cout << mt->need_sleep(req_cmd) << endl;
}
//
void bad_url_read_test() {
	struct req_command req_cmd;
	master * mt = new master();
	cout << mt->need_sleep(req_cmd) << endl;
}
//
void hand_slave_request_test() {
	master * mt = new master();
	struct req_command req_cmd;
	map<string, string> response_cmd_map;
	//1 request
	req_cmd.commd_id = 1;
	req_cmd.slave_id = 1;

	mt->hand_slave_request(req_cmd, response_cmd_map);
	assert(response_cmd_map["commd_id"]=="7");
// time out return store

//2 store success
	req_cmd.commd_id = 1;
	req_cmd.last_cmd_id = 7;
	req_cmd.last_task_status = 1;
	map<string, string> response_cmd_map2;
	mt->hand_slave_request(req_cmd, response_cmd_map2);
	assert(response_cmd_map2["commd_id"]!="2");

	for (map<string, string>::iterator it2 = response_cmd_map2.begin();
			it2 != response_cmd_map2.end(); ++it2) {
		std::cout << it2->first << " => " << it2->second << '\n';
	}
}

//
} /* namespace poseidon */
#endif
