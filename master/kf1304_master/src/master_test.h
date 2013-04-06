/*
 * master_test.cpp
 *
 *  Created on: 2013-4-1
 *      Author: root
 */
#ifndef MASTER_TEST_H
#define MASTER_TEST_H
#include "master.h"

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
	string s2 = mt->hand_request(command_request_task);
	cout << "respose_content:" << s2 << endl;
	//req1 application_version:2  get grab job
	command_request_task = "commd_id:1\r\n"
			"slave_id:1\r\n"
			"application_version:2\r\n"
			"\f";
	s2 = mt->hand_request(command_request_task);
	cout << "respose_content:" << s2 << endl;
	//req1 application_version:3  get sleep
	command_request_task = "commd_id:1\r\n"
			"slave_id:1\r\n"
			"application_version:2\r\n"
			"\f";
	s2 = mt->hand_request(command_request_task);
	cout << "respose_content:" << s2 << endl;

}

void read_site_test() {
	master * mt = new master();
	vector<string> vec;
	mt->read_site("./urls/site_2", 3, &vec);
	for (vector<string>::iterator it = vec.begin(); it < vec.end(); it++) {
		cout << (*it) << endl;
	}
}
} /* namespace poseidon */
#endif
