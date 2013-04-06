/*
 * master.h
 *
 *  Created on: 2013-4-1
 *      Author: root
 */

#ifndef MASTER_H_
#define MASTER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

#include "map"
#include "vector"
#include "GlobalHelper.h"
#define DEBUG
using namespace std;

namespace poseidon {
struct site {
	string site_name;
	int good;
	int bad;
	int process;
	int total;
};
struct slave_status {
	string slave_id;
	int status;
	vector<site> site_status;
};

struct command {
	int commd_id;
	int slave_id;
	int app_version;
	int time;
	int task_id;
	vector<string> http_request_headers;
};
class master {

private:
	int master_port; //80
	string master_ip; //"192.168.75.128";
	//command 1 salve 1
	string command_m2s_1; //"2\r\n1\r\n\f";
	GlobalHelper *gh;
	bool isworktime;
	int current_version;
	string sleep_time;
	ifstream *site_2;
public:
	master() {
		master_port = 9000;
		master_ip = "192.168.75.128";
		command_m2s_1 = "2\r\n1\r\n\f";
		gh = new GlobalHelper();
		isworktime = false;
		current_version = 2;
		//	ifstream s1("./urls/site_2");
		//site_2 = &s1;

		sleep_time = "5";
	}

	bool is_worktime() {
		isworktime = !isworktime;
		return isworktime;
	}
	int service() {
		//1 init variable
		int socketfd;
		int new_fd;
		struct sockaddr_in client_addr;
		struct sockaddr_in local_addr;
		char request_buff[gh->TASK_BUF_SIZE];

		//2 create socket
		if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
			perror("socket fd create fail...");
			return -1;
		}

		//3 prepare listen access point
		gh->init_address(&local_addr, PF_INET, this->master_port, INADDR_ANY);

		//4 bind and listen
		if (-1
				== bind(socketfd, (struct sockaddr*) &local_addr,
						sizeof(struct sockaddr))) {
			perror("socket fd connet fail...");
			return -1;
		}
		if (listen(socketfd, 128) == -1) {
			perror("socket fd listen fail...");
			return -1;
		}
		cout << "waiting for your connection!" << endl;

		//5  service is receive cmd and response cmd
		socklen_t size;
		while (1) {
			size = sizeof(client_addr);
			//5.1 accept
			if ((new_fd = accept(socketfd, (struct sockaddr *) (&client_addr),
					&size)) == -1) {
				perror("socket new_fd accept fail...");
				return -1;
			}
			cout << "conntection from:" << inet_ntoa(client_addr.sin_addr)
					<< endl;

			//5.2  accept the full message
			int read_count;
			while (1) {
				read_count = read(new_fd, request_buff, sizeof request_buff);
				if (read_count == 0) {
					perror("socket fd read fail...");
					break;
				}
				if (gh->tail_with_feature(request_buff, read_count, "\f")) {
					break;
					//TODO:: extract a function :is revice OK?
				}
			}
			request_buff[read_count] = '\0';
#ifdef DEBUG
			cout << "request is :" << request_buff << endl;
#endif

			//5.3 hand request
			string request_str = request_buff;
			string respose_content = hand_request(request_str);
#ifdef DEBUG
			cout << "response:" << respose_content << endl;
#endif

			//5.4 write to request
			int write_result = write(new_fd, respose_content.c_str(),
					strlen(respose_content.c_str()));
			if (write_result < 0) {
				perror("socket fd write fail...");
			}

		} //end step 5
		return 0;
	}
	//input cmd and return string
	string init_command(map<string, string> &cmd_map) {
		map<string, string>::iterator it2;
		string cmd_str;
		for (it2 = cmd_map.begin(); it2 != cmd_map.end(); ++it2) {
			//std::cout << it2->first << " => " << it2->second << '\n';
			cmd_str.append(it2->first + ":" + it2->second + "\r\n");
		}
		cmd_str.append("\f");
		return cmd_str;
	}
	//
	string hand_request(string request_str) {
		string respose_content;
		//==============================================
		//1  request string struct command
		//==============================================
		map<string, string> req_cmd_map;
		gh->command_str_to_map(request_str, &req_cmd_map);
		struct command req_cmd;
		req_cmd.commd_id = atoi(req_cmd_map["commd_id"].c_str());
		req_cmd.app_version = atoi(req_cmd_map["application_version"].c_str());
		req_cmd.slave_id = atoi(req_cmd_map["slave_id"].c_str());
#ifdef DEBUG
		cout << "request_command:" << req_cmd.commd_id << endl;
#endif

		//==============================================
		//2 switch command type
		//==============================================
		map<string, string> response_cmd_map;
		switch (req_cmd.commd_id) {
		case 1: //request from slave
			//2.1.1 version
			if (req_cmd.app_version < this->current_version) {
				response_cmd_map["commd_id"] = "3";
				response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
				response_cmd_map["version"] = gh->num2str(
						this->current_version);
				response_cmd_map["url"] = "192.168.1.1/slave_v2.gz";
			} //2.1.2 is work time
			else if (is_worktime()) {
				response_cmd_map["commd_id"] = "4";
				response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
//task_id
				response_cmd_map["urls"] = assign_request(req_cmd.slave_id);

			} else { //2.1.2 sleep time
				response_cmd_map["commd_id"] = "2";
				response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
				response_cmd_map["time"] = this->sleep_time;
			}
			respose_content = init_command(response_cmd_map);
			break;
		case 6: //feedback from rep
			//:TODO::feedback from rep logic
			respose_content = init_command(response_cmd_map);
			break;
		default:
			respose_content = "-_-!,bad request!\f";
			break;
		}
		return respose_content;
	}
	string assign_request(int slave_id) {
		//TODO::assign_task1
		vector<string> vec;
		string request_url;
//lock in
		read_site("./urls/site_2", 3, &vec);
//lock out
		for (vector<string>::iterator it = vec.begin(); it < vec.end(); it++) {
			request_url.append(*it);
			request_url.append("#");
		}
		return request_url;
	}

	bool read_site(string filename, int urlnum, vector<string> *vec) {
		FILE *fp;
		char *filep;
		string url;

		if ((fp = fopen(filename.c_str(), "r")) == NULL) {
			printf("open file %s error!!\n", filename.c_str());
			return 0;
		}

		char read_buff[1024];
		while (urlnum--) {
			filep = fgets(read_buff, 1024, fp);
			string s1 = read_buff;
			int f1 = s1.find_first_of('\n');
			string url = s1.substr(0, f1);
			/*
			 *site_2 >> url;
			 if ((*site_2).bad()) { //if we all lines is read!
			 return false;
			 }*/
			vec->push_back(url);
		}
		return 1;
	}
	virtual ~master();
};

}
/* namespace poseidon */
#endif /* MASTER_H_ */
