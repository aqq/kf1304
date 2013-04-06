/*
 * slaver.h
 *
 *  Created on: 2013-4-1
 *      Author: root
 */

#ifndef SLAVER_H_
#define SLAVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>

#include "GlobalHelper.h"

#define REQUEST 1
#define SLEEP 2
#define UPDATE 3
#define GRABPAGE 4
using namespace std;

namespace poseidon {
typedef struct task {
	int sleep_time;

	int task_id;
	int slave_id;
	int cmd_id;

	string store_ip;
	int store_port;

	string request_ip;
	int request_port;

	string url_header;
	vector<string> *url_body;

	map<string, string> response_cmd_map;

	int version;

};
class slaver {
private:
	int master_port;
	string master_ip;
//command 1 salve 1
	string cmd_req_model;

	GlobalHelper *gh;
protected:

public:
	int slave_id;
	int app_version;
	string cmd_req_2send;
	slaver() {
		slave_id = 1;
		app_version = 1;
		master_port = 9000;
		this->master_ip = "192.168.75.128";
		cmd_req_model = "commd_id:1\r\n"
				"slave_id:#\r\n"
				"application_version:@\r\n"
				"\f";
		gh = new GlobalHelper();
	}
	virtual ~slaver();
	string requestTask(task *mytask);
	bool grabpage_work(task mytask);
	bool grabpage(task mytask, int index);
	bool localStorePage();
	bool remoteStorePage();

	void work() {
		task mytask; //commad
		string task_str;
		int i = 2;
		while (i--) {
			memset(&mytask, 0, sizeof(mytask));
			task_str = requestTask(&mytask);
			str2task(task_str, &mytask);
#ifdef DEBUG
			cout <<"hand_response cmd_id:"<< mytask.cmd_id <<endl;
#endif
			hand_response(mytask);
		}
		//slaver_worker

	}
	void hand_response(task& mytask) {

		switch (mytask.cmd_id) {
		case SLEEP:
			cout << "sleep_time" << mytask.sleep_time << endl;
			sleep(mytask.sleep_time); //sleep 1 second;
			break;
		case UPDATE:
			//update app begin
			//TODO::ADD update app begin
			//update app end
			cout << "updata version" << mytask.version << endl;
			this->app_version = mytask.version;
			break;
		case GRABPAGE:
			cout << "GRABPAGE  " << mytask.url_body->size() << endl;
			grabpage_work(mytask);
			break;
		}
	}
	void prepare_urls(task mytask) {
		string urls = mytask.response_cmd_map["urls"];
		vector<string> str_vec;
		gh->split(urls, "#", str_vec);

#ifdef DEBUG
		cout<<"mytask.url_body:"<<str_vec.size()<<endl;
#endif
		vector<string>::iterator it;
		string req;
		for (it = str_vec.begin(); it != str_vec.end(); it++) {
			mytask.url_body->push_back(gh->convert_url_to_http_req(*it));
		}
	}
	//
	string& replace_all(string& str, const string& old_value,
			const string& new_value);
	string getHttpHeader(string url_header, vector<string> url_body, int id);
	//help

	void init_address(struct sockaddr_in *dest_addr, int sin_family, int port,
			string ip) {
		bzero(dest_addr, sizeof(dest_addr));
		dest_addr->sin_family = sin_family;
		dest_addr->sin_port = htons(port);
		dest_addr->sin_addr.s_addr = inet_addr(ip.c_str());
		bzero(&(dest_addr->sin_zero), 8);
	}
	//
	void str2task(string response_command, task *mytask) {
		map<string, string> response_cmd_map;
		gh->command_str_to_map(response_command, &response_cmd_map);
		int cmd_id = atoi(response_cmd_map["commd_id"].c_str());
		mytask->cmd_id = cmd_id;
		mytask->response_cmd_map = response_cmd_map;
		mytask->sleep_time = atoi(mytask->response_cmd_map["time"].c_str());
		mytask->slave_id = atoi(mytask->response_cmd_map["slave_id"].c_str());
		mytask->version = atoi(mytask->response_cmd_map["version"].c_str());
		prepare_urls(*mytask);
		//cout << "cmd_id:" << (*mytask).cmd_id << endl;
		//cout << "slave_id:" << (*mytask).response_cmd_map["slave_id"] << endl;
		//	cout << "version" << (*mytask).response_cmd_map["version"] << endl;

	}
	void prepare_req_cmd() {
		string cmd_req_str = this->cmd_req_model;

		cout << "*app_version:" << app_version << endl;
		cmd_req_str = gh->replace(cmd_req_str, gh->num2str(this->slave_id),
				"#");
		cmd_req_str = gh->replace(cmd_req_str, gh->num2str(this->app_version),
				"@");
		this->cmd_req_2send = cmd_req_str;
	}
	//

//

};

} /* namespace poseidon */
#endif /* SLAVER_H_ */
