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
};
struct slave_status {
	string slave_id;
	int status;
	//key=site_name,value=site_status
	map<string, site> site_status;
	string last_task_site;
	//int last_task_site;

};

struct command {
	int commd_id;
	int slave_id;
	int app_version;
	int time;
	//int task_id;

	int last_task_status;

	vector<string> urls; //master to slave
	vector<string> task_id;
};
class master {

private:
	int master_port; //80
//	string master_ip; //"192.168.75.128";
	//command 1 salve 1

	GlobalHelper *gh;
	bool isworktime;
	int current_version;
	string sleep_time;
	//ifstream *site_2;
	//FILE *fp;
	int read_url_num;
	//map<string,FILE> url_map;//key=site;value file

	//assign_url_number
public:
	//key=site,value=FILE POINT
	map<string, FILE> url_map;

	//key=slave_id,value=slave_status
	map<string, slave_status> slave_map;

	int assign_url_number;
	master() {
		master_port = 9000;

		gh = new GlobalHelper();
		isworktime = false;
		current_version = 2;
		//	ifstream s1("./urls/site_2");
		//site_2 = &s1;

		sleep_time = "2";
		//string filename = "./urls/site_2";
		//	if ((fp = fopen(filename.c_str(), "r")) == NULL) {
		//		printf("open file %s error!!\n", filename.c_str());
		//		exit(0);
//		}

		read_url_num = 3;

		this->config_master();
		this->config_website();
	}

	bool is_worktime() {
		isworktime = !isworktime;
		return 1;
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
				== bind(socketfd, (struct sockaddr*) (((((&local_addr))))),
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
			if ((new_fd = accept(socketfd,
					(struct sockaddr*) ((((((&client_addr)))))), &size))
					== -1) {
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
			cout << "request is :" << request_buff << endl;
			//5.3 hand request
			string request_str = request_buff;
			string respose_content = hand_request(request_str);
			cout << "response:" << respose_content << endl;
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
	string init_command(map<string, string>& cmd_map) {
		map<string, string>::iterator it2;
		string cmd_str;
		for (it2 = cmd_map.begin(); it2 != cmd_map.end(); ++it2) {
			//std::cout << it2->first << " => " << it2->second << '\n';
			cmd_str.append(it2->first + ":" + it2->second + "\r\n");
		}
		cmd_str.append("\f");
		return cmd_str;
	}

	void init_sleep_cmd(map<string, string>& response_cmd_map,
			struct command req_cmd) {
		//2.1.2 sleep time
		response_cmd_map["commd_id"] = "2";
		response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
		response_cmd_map["time"] = this->sleep_time;
	}

	void init_updata_cmd(map<string, string>& response_cmd_map,
			struct command& req_cmd) {
		response_cmd_map["commd_id"] = "3";
		response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
		response_cmd_map["version"] = gh->num2str(this->current_version);
		response_cmd_map["url"] = "192.168.1.1/slave_v2.gz";
	}

	void init_assign_cmd(struct command& req_cmd,
			map<string, string>& response_cmd_map) {
		string urls;
		if (assign_request(req_cmd.slave_id, urls)) {
			response_cmd_map["urls"] = urls;
			response_cmd_map["commd_id"] = "4";
			response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
			response_cmd_map["task_id"] = gh->get_time_str();
		} else {
			init_sleep_cmd(response_cmd_map, req_cmd);
		}
	}

	//2 update slave_status
	void update_slave_status(struct command& req_cmd) {
		//2.1 weather slave_id in map
		slave_status s_status;
		string tmp_slave_id = gh->num2str(req_cmd.slave_id);
		if (slave_map.find(tmp_slave_id) != slave_map.end()) {
			s_status = slave_map[tmp_slave_id];
			s_status.status = 1;
			if (req_cmd.last_task_status) {
				cout << "last_task_site:" << s_status.last_task_site << endl;
				s_status.site_status[s_status.last_task_site].good++;
			} else {
				s_status.site_status[s_status.last_task_site].bad++;
			}
			slave_map[tmp_slave_id] = s_status;
		} else {
			//add_slave
			slave_status s_status1; //= (slave_status) malloc(t1);
			s_status1.slave_id = gh->num2str(req_cmd.slave_id);
			for (map<string, FILE>::iterator it = this->url_map.begin();
					it != this->url_map.end(); it++) {
				site s1;
				s1.bad = 0;
				s1.good = 0;
				s1.site_name = (*it).first;
				s_status1.site_status[s1.site_name] = s1;
				//	cout << "site_name:" << (*it).first << endl;
			}
			//s_status1.site_status
			slave_map[s_status1.slave_id] = s_status1;
		}

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
		req_cmd.last_task_status = atoi(
				req_cmd_map["last_task_status"].c_str());
		cout << "request_command:" << req_cmd.commd_id << endl;
		//==============================================
		//2 update slave_status
		//==============================================
		update_slave_status(req_cmd);
		//==============================================
		//3 switch command type
		//==============================================
		map<string, string> response_cmd_map;
		switch (req_cmd.commd_id) {
		case 1: //request from slave
			//2.1.1 version
			if (req_cmd.app_version < this->current_version) {
				init_updata_cmd(response_cmd_map, req_cmd);
			} //2.1.2 is work timeelse
			else if (is_worktime()) {
				//	bool is_exist_work = 0;
				init_assign_cmd(req_cmd, response_cmd_map);
				// show slaves status
				show_slave_status();
			} else {
				//2.1.2 sleep time
				init_sleep_cmd(response_cmd_map, req_cmd);
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

	bool assign_request(int slave_id, string& request_url) {
		//TODO::assign_task1
		vector<string> vec_urls;
		//		string request_url;
		//lock in
		if (!read_urls(slave_id, &vec_urls)) {
			return 0;
		}
		//lock out
		for (vector<string>::iterator it = vec_urls.begin();
				it < vec_urls.end(); it++) {
			request_url.append(*it);
			request_url.append("#");
		}
		return 1;
	}
	//================================================
	//get the min bad of all site
	string get_min_bad_of_sites(int slave_id) {

		struct slave_status s_status;
		s_status = this->slave_map[gh->num2str(slave_id)];

		map<string, site>::iterator it = s_status.site_status.begin();
		int badnum = (*it).second.bad;
		string site_name = (*it).second.site_name;
		//int
		for (; it != s_status.site_status.end(); it++) {
			if (badnum > ((*it).second.bad)) {
				badnum = (*it).second.bad;
				site_name = (*it).second.site_name;
			}
			//cout << "key:" << (*it).first << " v:" << (*it).second.slave_id << endl;
		}

		//site_name = (*min_fail_site).second.site_name;
		return site_name;
	}

	bool read_urls(int slave_id, vector<string>* vec_urls) {
		//
		//FILE* fp_now;
		//void assign_fp(){}
		string site_name = get_min_bad_of_sites(slave_id);
		cout << "site_name:" << site_name << endl;
		//================================================
		//get the min bad of all site
		//================================================
		FILE fp_now1 = this->url_map[site_name];
		FILE *fp_now = &fp_now1;
		char *filep;
		string url;

		char read_buff[1024];
		bool isok = true;
		int read_line = this->read_url_num;
		while (read_line--) {
			filep = fgets(read_buff, 1024, fp_now);
			if (filep == NULL) {
				isok = false;
				break;
			}
			string s1 = read_buff;
			int f1 = s1.find_first_of('\n');
			string url = s1.substr(0, f1);
			vec_urls->push_back(url);
		}
		//FILE fp_now1 = (
//		this->url_map[site_name] = fp_now1;
		return isok;
	}
	//
	bool config_master() {
		map<string, string> config_map1;

		gh->read_config(gh->MASTER_CONF, config_map1);
		this->assign_url_number = atoi(
				(config_map1["assign_url_number"]).c_str());

		//
		return 1;
	}
	bool config_website() {
		map<string, string> config_map2;
		gh->read_config(gh->WEBSITE_CONF, config_map2);

		//foreach
		for (map<string, string>::iterator it2 = config_map2.begin();
				it2 != config_map2.end(); ++it2) {
			//	std::cout << it2->first << " => " << it2->second << '\n';
			string site_name = it2->first;
			long num = atol(it2->second.c_str());
//init var
			FILE *fp_config;

			string fsite = "./urls/" + site_name;
			string url_config;
			//open file
			if ((fp_config = fopen(fsite.c_str(), "r")) == NULL) {
				printf("open file %s error!!\n", fsite.c_str());
				return false;
			}
			//read  num line
			char *filep;
			char read_buff[1024];
			while (num--) {
				filep = fgets(read_buff, 1024, fp_config);
				if (filep == NULL) {
					return false;
				}
			}
			//store fp into map
			url_map[site_name] = *fp_config;

		}

		return 1;
	}
	void show_slave_status() {

	}
	virtual ~master() {
		//fclose(fp);
	}

};
}
/* namespace poseidon */
#endif /* MASTER_H_ */
