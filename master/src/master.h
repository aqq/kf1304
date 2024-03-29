/*
 * master.h
 *
 *  Created on: 2013-4-1
 *      Author: andrew
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
#include "Cpp2mysql.h"

using namespace std;

namespace poseidon {
const string s_moudle = "moudle";
const string s_socket = "socket";
const string s_normal = "normal";
const string s_sql = "sql";
const string s_work = "task";
const int S2M_REQUEST = 1;
const int M2S_SLEEP = 2;
const int M2S_UPDATE = 3;
const int M2S_URL = 4;
const int S2R_STORE = 5;
const int R2S_FEEDBACK = 6;
const int M2S_STORE = 7;

struct site_in_slave {
	string site_name;
	int good;
	int bad;
};

struct site_info {

	long int total_length;
	long int finished_length;
	bool isfinised;
};

struct slave_status {
	string slave_id;
	int status;
	//key=site_name,value=site_status
	map<string, site_in_slave> site_status;
	string last_task_site;
	//int last_task_site;

};

struct req_command {
	int commd_id;
	int slave_id;
	int app_version;
	int time;
	//int task_id;

	int last_task_status;
	int last_cmd_id;
	float available_disk_space;

	vector<string> slave_ip;
	vector<string> rep_id;
	vector<string> worker_type;
};

class master {

private:
	int master_port;

	GlobalHelper *gh;
	Cpp2mysql *mysql;
	bool isworktime;
	int current_version;
	string sleep_time;

	vector<string> update_shell;
	int slave_work_time_begin;
	int slave_work_time_end;
public:
	vector<string> store_ip;
	int store_port;
	//key=site,value=FILE POINT
	map<string, site_info> url_map;

	//key=slave_id,value=slave_status
	map<string, slave_status> slave_map;

	int receive_time_out;
	int send_time_out;
	int connect_time_out;

	int assign_url_number;

	master() {

		gh = new GlobalHelper();

		isworktime = false;
		//is_store_time = false;
		current_version = 2;
		sleep_time = "2";

		this->config_master();
		this->config_website();
		gh->log2("Begin init mysql", "sql");
		mysql = new Cpp2mysql();
		gh->log2("Finish init mysql", "sql");
	}

	bool need_sleep(struct req_command req_cmd) {
		//%H 24小时制的小时
		int time_h = atoi(gh->get_string_time("%H").c_str());

		if (time_h >= this->slave_work_time_begin
				&& time_h <= this->slave_work_time_end) {
			return 0;
		}

		if (req_cmd.last_cmd_id != M2S_STORE || req_cmd.last_task_status != 1) {
			return 0;
		}
		this->sleep_time = "3600";
		return 1;
	}
	bool store;
	bool need_store(struct req_command req_cmd) {
		store = !store; //测试
		return store;

		int time_h = atoi(gh->get_string_time("%H").c_str());

		if (time_h >= this->slave_work_time_begin
				&& time_h <= this->slave_work_time_end) {
			return 0;
		}
		if (req_cmd.last_cmd_id == M2S_STORE && req_cmd.last_task_status == 1) {
			return 0;
		}
		//		return req_cmd.available_disk_space < 0.5;
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
		this->set_socket(socketfd);
		//3 prepare listen access point
		gh->init_address(&local_addr, PF_INET, this->master_port, INADDR_ANY);

		//4 bind and listen
		if (-1
				== bind(socketfd, (struct sockaddr*) (((((((&local_addr))))))),
						sizeof(struct sockaddr))) {
			perror("socket fd connet fail...");
			return -1;
		}
		if (listen(socketfd, 128) == -1) {
			perror("socket fd listen fail...");
			return -1;
		}
		cout << "master waiting for your connection!" << endl;
		//5  service is receive cmd and response cmd
		socklen_t size;
		while (1) {
			size = sizeof(client_addr);
			//5.1 accept
			if ((new_fd = accept(socketfd,
					(struct sockaddr*) (((&client_addr))), &size)) == -1) {
			//	perror("socket new_fd accept fail...");
				continue;
			}
			cout << "conntection from:" << inet_ntoa(client_addr.sin_addr)
					<< endl;
			//5.2  accept the full message
			int read_count;
			while (1) {
				read_count = read(new_fd, request_buff, sizeof request_buff);
				if (read_count == -1) {
					perror("socket fd read fail...");
					break;
				}
				if (read_count == 0) {
					break;
				}
				if (gh->tail_with_feature(request_buff, read_count, "\f")) {
					break;
				}
			} //end while

			request_buff[read_count] = '\0';
			gh->log2("request :", request_buff, s_socket);
			//5.3 hand request
			string request_str = request_buff;
			string respose_content = hand_request(request_str); ///
			gh->log2("response :", respose_content, s_socket);
			//5.4 write to request
			if (-1
					== write(new_fd, respose_content.c_str(),
							strlen(respose_content.c_str()))) {
				perror("socket fd write fail...");
			}
		} //end step 5

		return 0;
	}

	//input cmd and return string
	string cmd_map_to_str(map<string, string>& cmd_map) {
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
			struct req_command req_cmd) {
		//2.1.2 sleep time
		response_cmd_map["commd_id"] = "2";
		response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
		response_cmd_map["time"] = this->sleep_time;
	}

	void init_updata_cmd(map<string, string>& response_cmd_map,
			struct req_command& req_cmd) {
		response_cmd_map["commd_id"] = "3";
		response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
		response_cmd_map["version"] = gh->num2str(this->current_version);
		response_cmd_map["update_shell"] = this->update_shell[0];
	}

	void init_assign_cmd(struct req_command& req_cmd,
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

	void init_store_cmd(struct req_command& req_cmd,
			map<string, string>& response_cmd_map) {
		response_cmd_map["commd_id"] = "7";
		response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
		response_cmd_map["store_ip"] = this->store_ip[0];
		response_cmd_map["store_port"] = gh->num2str(this->store_port);
	}

	void init_feedback_cmd(map<string, string>& response_cmd_map,
			struct req_command req_cmd) {
		//	updata_rep_status(req_cmd);
		//	response_cmd_map["commd_id"] = "2";
		//response_cmd_map["slave_id"] = gh->num2str(req_cmd.slave_id);
		//	response_cmd_map["time"] = this->sleep_time;
	}

	void updata_rep_status(struct req_command& req_cmd) {
		//log
		string log_str = "rep available_disk_space :";
		log_str += gh->float2str(req_cmd.available_disk_space);
		gh->log2(log_str, "rep_disk_space");

		worker_tb w;
		w.slave_id = atoi(req_cmd.rep_id[0].c_str());
		w.last_request_ip = req_cmd.slave_ip[0];
		w.available_disk_space = req_cmd.available_disk_space;
		w.last_request_time = gh->get_time_str("%Y-%m-%d %H:%M:%S");
		w.worker_type = req_cmd.worker_type[0];
		mysql->update_worker_tb(w);
	}

	void update_slave_status_to_db(struct req_command& req_cmd) {
		//update_worker_tb
		worker_tb w;
		w.slave_id = req_cmd.slave_id;
		w.last_request_ip = req_cmd.slave_ip[0];
		w.available_disk_space = req_cmd.available_disk_space;
		w.last_request_time = gh->get_time_str("%Y-%m-%d %H:%M:%S"); //"bbs_csdn";
		mysql->update_worker_tb(w);
	}

	void update_slave_site_status_to_db(struct req_command& req_cmd) {
		//update_worker_site_tb
		if (req_cmd.commd_id != S2M_REQUEST) {
			return;
		}
		if (req_cmd.last_task_status == 2) {
			return;
		}
		string tmp_slave_id = gh->num2str(req_cmd.slave_id);
		if (slave_map.find(tmp_slave_id) == slave_map.end()) {
			return;
		}
		//exist
		worker_site_tb ws;
		ws.slave_id = req_cmd.slave_id;
		slave_status s_status = slave_map[tmp_slave_id];
		ws.site_id = s_status.last_task_site;
		ws.site_task_sucess = req_cmd.last_task_status;
		mysql->update_worker_site_tb(ws);
	}

	//2 update slave_status
	void update_slave_status_in_memory(struct req_command& req_cmd) {
		//2.1 weather slave_id in map
		slave_status s_status;
		string tmp_slave_id = gh->num2str(req_cmd.slave_id);
		if (slave_map.find(tmp_slave_id) != slave_map.end()) {
			//slave exist
			s_status = slave_map[tmp_slave_id];
			s_status.status = 1;
			gh->log2("last_task_site:", s_status.last_task_site, s_work);
			if (req_cmd.last_task_status) {
				s_status.site_status[s_status.last_task_site].good++;
			} else {
				s_status.site_status[s_status.last_task_site].bad++;
			}
			slave_map[tmp_slave_id] = s_status;
		} else {
			//add_slave
			slave_status s_status1; //= (slave_status) malloc(t1);
			s_status1.slave_id = gh->num2str(req_cmd.slave_id);
			for (map<string, site_info>::iterator it = this->url_map.begin();
					it != this->url_map.end(); it++) {
				site_in_slave s1;
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

	void req_map_to_struct_cmd(struct req_command& req_cmd,
			map<string, string> req_cmd_map) {
		req_cmd.commd_id = atoi(req_cmd_map["commd_id"].c_str());
		req_cmd.app_version = atoi(req_cmd_map["application_version"].c_str());
		req_cmd.slave_id = atoi(req_cmd_map["slave_id"].c_str());
		req_cmd.last_task_status = atoi(
				req_cmd_map["last_task_status"].c_str());
		req_cmd.last_cmd_id = atoi(req_cmd_map["last_cmd_id"].c_str());
		req_cmd.available_disk_space = atof(
				req_cmd_map["available_disk_space"].c_str());
		req_cmd.slave_ip.push_back(req_cmd_map["slave_ip"]);
		req_cmd.rep_id.push_back(req_cmd_map["rep_id"]);
		req_cmd.worker_type.push_back(req_cmd_map["worker_type"]);
		//  req_cmd["rep_id"];
		// req_cmd["worker_type"];

	}

	void hand_slave_request(struct req_command& req_cmd,
			map<string, string>& response_cmd_map) {
		//  store
		if (need_store(req_cmd)) {
			//2 store
			init_store_cmd(req_cmd, response_cmd_map);
			return;
		}

		//  sleep
		if (need_sleep(req_cmd)) {
			init_sleep_cmd(response_cmd_map, req_cmd);

			return;
		}

		//  update
		if (req_cmd.app_version < this->current_version) {
			init_updata_cmd(response_cmd_map, req_cmd);
			return;
		}

		// assign
		init_assign_cmd(req_cmd, response_cmd_map);
		return;
	}

	//
	string hand_request(string request_str) {
		string respose_content;
		//==============================================
		//1  request string struct command
		//==============================================
		map<string, string> req_cmd_map;
		gh->command_str_to_map(request_str, &req_cmd_map);
		struct req_command req_cmd;
		req_map_to_struct_cmd(req_cmd, req_cmd_map);
		gh->log2("request_command:", gh->num2str(req_cmd.commd_id), "task");

		//==============================================
		//2 update slave_status
		//==============================================
		switch (req_cmd.commd_id) {
		case S2M_REQUEST: //request from slave
			update_slave_status_in_memory(req_cmd);
			update_slave_status_to_db(req_cmd);
			if (req_cmd.last_cmd_id == M2S_URL) {
				update_slave_site_status_to_db(req_cmd);
			} else if (req_cmd.last_cmd_id == M2S_STORE) {

			}

			break;
		case poseidon::R2S_FEEDBACK:
			this->updata_rep_status(req_cmd);
			break;
		}

		//==============================================
		//3 switch command type
		//==============================================
		map<string, string> response_cmd_map;
		switch (req_cmd.commd_id) {
		case poseidon::S2M_REQUEST: //request from slave
			//2.1.1 version
			hand_slave_request(req_cmd, response_cmd_map);
			respose_content = cmd_map_to_str(response_cmd_map);
			break;
		case poseidon::R2S_FEEDBACK: //feedback from rep
			init_feedback_cmd(response_cmd_map, req_cmd);
			respose_content = cmd_map_to_str(response_cmd_map);
			break;
		default:
			respose_content = "-_-!,bad request!\f";
			break;
		}

		return respose_content;
	}

	bool assign_request(int slave_id, string& request_url) {

		vector<string> vec_urls;
		//		string request_url;

		if (!read_urls_and_record(slave_id, &vec_urls)) {
			return 0;
		}

		for (vector<string>::iterator it = vec_urls.begin();
				it < vec_urls.end(); it++) {
			request_url.append(*it);
			request_url.append("#");
		}
		return 1;
	}
//================================================
//get the min bad of all site
	bool get_min_bad_of_sites(int slave_id, string& site_name) {
		bool is_get_site_ok = false;
		struct slave_status s_status;
		s_status = this->slave_map[gh->num2str(slave_id)];

		map<string, site_in_slave>::iterator it;
		it = s_status.site_status.begin();
		map<string, site_in_slave> site_status2;
		//int
		int badnum = -1;

		for (; it != s_status.site_status.end(); it++) {
			// weather site 's task is finished
			string s_n_str = ((*it).second.site_name);
			if (this->url_map.find(s_n_str) == this->url_map.end()) {
				continue;
			}
			if (url_map[s_n_str].isfinised) {
				continue;
			}
			if (badnum == -1) {
				badnum = (*it).second.bad;
				site_name = (*it).second.site_name;
			}
			if (badnum > ((*it).second.bad)) {
				badnum = (*it).second.bad;
				site_name = (*it).second.site_name;
			}
			is_get_site_ok = true;
			//cout << "key:" << (*it).first << " v:" << (*it).second.slave_id << endl;
		}

		//site_name = (*min_fail_site).second.site_name;
		return is_get_site_ok;
	}

	bool read_sitefile_lines(long int& pos, string site_name,
			vector<string>* vec_urls) {
		//
		string fname = "./urls/" + site_name;
		FILE *fp_now = fopen(fname.c_str(), "r");
		char *filep;
		string url;
		char read_buff[1024];
		bool isok = true;
		int read_line = this->assign_url_number;

		fseek(fp_now, pos, 0);
		while (read_line--) {
			filep = fgets(read_buff, 1024, fp_now);
			if (filep == NULL) {
				isok = false;
				break;
			}
			size_t line_length = strlen(read_buff);
			read_buff[line_length - 1] = '\0';
			//
			string url = read_buff;
			url = gh->replace(url, "", "\n");
			//filter space line without a char
			if (url.find('.') == string::npos) {
				read_line++;
				continue;
			}
			url = gh->replace(url, "", "http://");
			vec_urls->push_back(url);
		}
		pos = ftell(fp_now);
		fclose(fp_now);
		return isok;
	}
//real assign function
	bool read_urls_and_record(int slave_id, vector<string>* vec_urls) {
		//================================================
		//get the min bad of all site
		//================================================
		string site_name;
		bool is_get_site_ok = get_min_bad_of_sites(slave_id, site_name);
		if (!is_get_site_ok) {
			return false;
		}
		gh->log2("site_name:", site_name, s_work);

		//****************************************lock begin
		//================================================
		//read urls
		//================================================
		site_info si = this->url_map[site_name];
		bool is_read_ok = read_sitefile_lines(si.finished_length, site_name,
				vec_urls);

		//================================================
		//record read process to memory
		//================================================
		if (is_read_ok) {
			record_slave_read_urls(slave_id, site_name);
			this->url_map[site_name] = si;
		} else {
			si.isfinised = true;
			this->url_map[site_name] = si;
			//url_map delete the site name
		}

		//================================================
		//record read process to file
		//================================================
		map<string, site_info>::iterator it2;
		string cmd_str;
		for (it2 = url_map.begin(); it2 != url_map.end(); ++it2) {
			cmd_str.append(
					it2->first + ":" + gh->ld2str(it2->second.finished_length)
							+ "\n");
		}
		gh->log(gh->WEBSITE_CONF, cmd_str, ios::trunc);
		//****************************************lock end

		return is_read_ok;
	}
	void record_slave_read_urls(int slave_id, string site_name) {
		struct slave_status s_status;
		s_status = this->slave_map[gh->num2str(slave_id)];
		s_status.last_task_site = site_name;
		this->slave_map[gh->num2str(slave_id)] = s_status;
	}
//
	bool config_master() {
		map<string, string> config_map1;

		gh->read_config(gh->MASTER_CONF, config_map1);
		this->assign_url_number = atoi(
				(config_map1["assign_url_number"]).c_str());
		this->master_port = atoi((config_map1["master_port"]).c_str());
		//

		this->store_ip.push_back(config_map1["store_ip"]);
		this->store_port = atoi((config_map1["store_port"]).c_str());
		this->current_version = atoi((config_map1["app_version"]).c_str());

		this->slave_work_time_begin = atoi(
				(config_map1["slave_work_time_begin"]).c_str());
		this->slave_work_time_end = atoi(
				(config_map1["slave_work_time_end"]).c_str());
		this->receive_time_out = atoi(
				(config_map1["receive_time_out"]).c_str());
		this->send_time_out = atoi((config_map1["send_time_out"]).c_str());
		this->connect_time_out = atoi(
				(config_map1["connect_time_out"]).c_str());
		gh->show_config(config_map1, gh->MASTER_CONF);

		//read all file to a str.
		this->update_shell.push_back(config_map1["new_version_url"]);
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
			site_info si;
			si.finished_length = num;
			si.total_length = 0;
			si.isfinised = false;
			url_map[site_name] = si;

		}
		gh->show_config(config_map2, gh->WEBSITE_CONF);
		return 1;
	}
	void show_slave_status() {

	}
	//
	void set_socket(int socketfd) {
		int keepAlive = 1;
		setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, (void*) &keepAlive,
				sizeof(keepAlive));
		int keepIdle = 10; //开始首次KeepAlive探测前的TCP空闭时间
		int keepInterval = 10; // 两次KeepAlive探测间的时间间隔
		int keepCount = 1; // 判定断开前的KeepAlive探测次数
		setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPIDLE, (void *) &keepIdle,
				sizeof(keepIdle));
		setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPINTVL, (void *) &keepInterval,
				sizeof(keepInterval));
		setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPCNT, (void *) &keepCount,
				sizeof(keepCount));
		//
		int reuse0 = 1;
		setsockopt(socketfd, IPPROTO_TCP, SO_REUSEADDR, (void *) &reuse0,
				sizeof(reuse0));
		int reuse1 = 1;
		setsockopt(socketfd, IPPROTO_TCP, 15, (void *) &reuse1, sizeof(reuse1));
		//
		struct timeval tv1;
		tv1.tv_sec = this->send_time_out;
		tv1.tv_usec = 0;
		//发送时限
		setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv1,
				sizeof(tv1));
		//接收时限
		struct timeval tv2;
		tv2.tv_sec = this->receive_time_out;
		tv2.tv_usec = 0;
		setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv2,
				sizeof(tv2));
	}
	virtual ~master() {
		//fclose(fp);
	}

};
}
/* namespace poseidon */
#endif /* MASTER_H_ */
