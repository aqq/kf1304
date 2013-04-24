/*
 * rep.h
 *
 *  Created on: 2013-4-10
 *      Author: root
 */

#ifndef REP_H_
#define REP_H_

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
#include <linux/tcp.h>
#include "errno.h"
#include "map"
#include "vector"
#include "GlobalHelper.h"
#define DEBUG
using namespace std;

namespace poseidon {
struct rep_command {
	int commd_id;
	int slave_id;
	vector<string> task_id;
	unsigned long content_size;
	int last_task_status;
};
struct req_task {
	int sleep_time;

	vector<string> task_id;

	int slave_id;
	int cmd_id;

	string store_ip;
	int store_port;

	string request_ip;
	int request_port;

	string urls;

	vector<string> urls_vec;
	vector<string> urls_http_req;
	vector<string> urls_sites;
	map<string, string> response_cmd_map;
	vector<string> new_version_url;
	int version;

};
const string s_socket = "socket";

class rep {
	GlobalHelper *gh;
public:
	int rep_port;
	vector<string> rep_ip;

	int master_port;
	vector<string> master_ip;

	vector<string> worker_type;
	vector<string> rep_id;

	rep() {

		gh = new GlobalHelper();
		config();
	}

	virtual ~rep();
	void config() {
		map<string, string> config_map1;
		gh->read_config(gh->REP_CONF, config_map1);
		this->rep_port = atoi((config_map1["rep_port"]).c_str());
		this->rep_ip.push_back(config_map1["rep_ip"]);
		this->master_port = atoi((config_map1["master_port"]).c_str());
		this->master_ip.push_back(config_map1["master_ip"]);

		this->worker_type.push_back(config_map1["worker_type"]);
		this->rep_id.push_back(config_map1["rep_id"]);
		gh->show_config(config_map1, gh->REP_CONF);

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
		gh->init_address(&local_addr, PF_INET, this->rep_port, INADDR_ANY);

		this->set_socket(socketfd);
		//4 bind and listen
		if (-1
				== bind(socketfd, (struct sockaddr*) (&local_addr),
						sizeof(struct sockaddr))) {
			perror("socket fd connet fail...");
			return -1;
		}
		if (listen(socketfd, 128) == -1) {
			perror("socket fd listen fail...");
			return -1;
		}
		cout << "rep:waiting for your connection!" << endl;

		//5  service is receive cmd and response cmd
		socklen_t client_addr_size;
		int read_totals = 0;
		client_addr_size = sizeof(client_addr);
		while (1) {

			//5.1 accept
			if ((new_fd = accept(socketfd, (struct sockaddr*) (&client_addr),
					&client_addr_size)) == -1) {
				//		perror("socket new_fd accept fail...");
				continue;
			}
			string str_log = "conntection from:";
			str_log += inet_ntoa(client_addr.sin_addr);
			gh->log2(str_log, "socket");

			//5.2   accept until \f
			ssize_t read_count;
			bool read_error = false;
			string req_str = "";
			while (1) {
				bzero(request_buff, sizeof request_buff);
				read_count = read(new_fd, request_buff, sizeof request_buff);
				if (read_count == -1) {
					cout << "socket fd read fail..." << endl;
					read_error = true;
					break;
				}
				request_buff[read_count] = '\0';
				req_str += request_buff;
				if (gh->tail_with_feature(request_buff, read_count, "\f")) {
					break;
				}
				if (read_count == 0) {
					break;
				}
			}

			if (read_error) {
				continue;
			}
			string cog_str1 = "request is:";
			//cout<<(cog_str1 + request_buff)<<endl;
			gh->log2(cog_str1 + req_str, "socket");

			//5.3 hand request
			string request_str = req_str;
			struct rep_command req_cmd;

			if (!is_req_store(request_str, req_cmd)) {
				continue;
			}
			gh->log2("5.4 write to request:\f ", "socket");
			//5.4 write to request
			string respose_content = "\f";
			int write_result = write(new_fd, respose_content.c_str(),
					strlen(respose_content.c_str()));
			if (write_result < 0) {
				perror("socket fd write fail...");
			}

			//5.5 accept the real content with gzip
			string filename = "./pages_gzip/" + req_cmd.task_id.at(0)
					+ ".tar.gz";

			gh->log2("5.5 filename:" + filename, "task");
			int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC);
			unsigned long total = 0;

			//5.6 read  gzeip page
			while (1) {
				bzero(request_buff, sizeof request_buff);
				read_count = read(new_fd, request_buff, sizeof request_buff);
				if (read_count == -1) {
					perror("socket fd read fail...");
					break;
				}
				request_buff[read_count] = '\0';
				write(fd, request_buff, read_count);
				total += read_count;
				if (total == req_cmd.content_size) {
					break;
				}
				if (read_count == 0) {
					gh->log("receive eof ,so total is bad ");
					break;
				}

			} //end accept data
			close(fd);
			gh->log("5.7 ok,file saved! ");

			read_totals++;
			if (read_totals > 5) {
				read_totals = 1;
				req_task r_task;
				string result;
				this->feedback_status(&r_task, result);
			}

		} //end step 5

		return 0;
	}
//===========
	bool is_req_store(string request_str, struct rep_command& req_cmd) {
		bool is_store;
		//==============================================
		//1  request string struct command
		//==============================================
		map<string, string> req_cmd_map;
		gh->command_str_to_map(request_str, &req_cmd_map);
		//struct command req_cmd;
		req_cmd.commd_id = atoi(req_cmd_map["commd_id"].c_str());
		req_cmd.task_id.push_back(req_cmd_map["task_id"]);
		req_cmd.slave_id = atoi(req_cmd_map["slave_id"].c_str());
		req_cmd.content_size = strtoul(req_cmd_map["content_size"].c_str(),
				NULL, 0);
		//req_cmd_map["pagecontent"]
		cout << "request_command:" << req_cmd.commd_id << endl;
		//==============================================
		//3 switch command type
		//==============================================
		map<string, string> response_cmd_map;
		switch (req_cmd.commd_id) {
		case 5: //slave to rep
			//store_message(request_str);
			is_store = 1;
			break;
		default:
			is_store = 0;
			break;
		}

		return is_store;
	}
	//=========
	string init_request_cmd_str() {
		//init_map
		map<string, string> cmd_map;
		cmd_map["commd_id"] = "6";
		cmd_map["rep_id"] = this->rep_id[0];
		cmd_map["worker_type"] = this->worker_type[0];
		cmd_map["available_disk_space"] = gh->float2str(
				gh->available_disk_space());

		//init str
		string cmd_str = "";
		for (map<string, string>::iterator it3 = cmd_map.begin();
				it3 != cmd_map.end(); ++it3) {
			cmd_str.append(it3->first + ":" + it3->second + "\r\n");
		}
		cmd_str.append("\f");
		return cmd_str;

	}
#define BUFSIZE 1449
#define READ_BUFF_SIZE 1448
	bool feedback_status(req_task *mytask, string& str_cmd) {
		//1 init variable
		int socketfd;
		struct sockaddr_in dest_addr;
		char read_buf[BUFSIZE];
		bzero(&read_buf, sizeof read_buf);
		bool req_seccess = 0;
		string log_str;
		while (1) {

			//2 create socket
			if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
				log_str = "socket fd create fail...";
				log_str += strerror(errno);
				gh->log2(log_str, s_socket); //perror("socket fd create fail...");

				break;
			}
			this->set_socket(socketfd);
			//3 prepare server address
			gh->init_address(&dest_addr, PF_INET, this->master_port,
					inet_addr(this->rep_ip[0].c_str()));

			//4 connect to server
			if (-1
					== connect(socketfd, (struct sockaddr*) &dest_addr,
							sizeof(struct sockaddr))) {
				log_str = "socket fd connect fail...";
				log_str += strerror(errno);
				gh->log2(log_str, s_socket);
				break;
			}
			gh->log2(
					"Connect ot:" + this->rep_ip[0] + ":"
							+ gh->num2str(this->rep_port), s_socket);

			//5 write to master
			int bytes_count;
			string cmd_req_2send = "";
			cmd_req_2send = this->init_request_cmd_str(); //last_task_status
			int write_result = write(socketfd, cmd_req_2send.c_str(),
					strlen(cmd_req_2send.c_str()));
			if (write_result == -1) {
				log_str = "socket fd write fail...";
				log_str += strerror(errno);
				gh->log2(log_str, s_socket); //perror
				break; //continue;
			}
			gh->log2("write:" + cmd_req_2send, s_socket);

			//6 read
			int total = 0;
			//	string t_str_cmd = "";
			str_cmd = "";
			while ((bytes_count = read(socketfd, read_buf, READ_BUFF_SIZE)) > 0) {
				total += bytes_count;
				if (bytes_count == -1) {
					break;
				}
				if (bytes_count == 0) {
					break;
				}
				if (gh->tail_with_feature(read_buf, bytes_count, "\f")) {
					break;
				}
				read_buf[bytes_count] = '\0';
				str_cmd += read_buf;
			}
			req_seccess = 1;
			break;
		}
		//}
		//7 clear socket
		close(socketfd);

//	str_cmd = read_buf;

		return req_seccess;

	}

	//=============
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
		struct timeval tv1;
		tv1.tv_sec = 10;
		tv1.tv_usec = 0;
		//发送时限
		setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv1,
				sizeof(tv1));
		//接收时限
		struct timeval tv2;
		tv2.tv_sec = 10;
		tv2.tv_usec = 0;
		setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv2,
				sizeof(tv2));
	}

	//
};

}
#endif /* REP_H_ */
