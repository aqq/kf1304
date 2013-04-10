/*
 * repertory.h
 *
 *  Created on: 2013-4-10
 *      Author: andrew
 */

#ifndef REPERTORY_H_
#define REPERTORY_H_

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

struct rep_command {
	int commd_id;
	int slave_id;
	long task_id;
	long content_size;
	int last_task_status;
};

class repertory {
private:
	int master_port;
	GlobalHelper *gh;
public:
	repertory() {
		 master_port = 9001;
		 	gh = new GlobalHelper();

	}
	virtual ~repertory();
	void config() {

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
		cout << "rep:waiting for your connection!" << endl;
		//5  service is receive cmd and response cmd
		socklen_t client_addr_size;
		while (1) {
			client_addr_size = sizeof(client_addr);
			//5.1 accept
			if ((new_fd = accept(socketfd, (struct sockaddr*) (&client_addr),
					&client_addr_size)) == -1) {
				perror("socket new_fd accept fail...");
				return -1;
			}
			cout << "conntection from:" << inet_ntoa(client_addr.sin_addr)
					<< endl;
			//5.2   accept until \f
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

			string cog_str1 = "request is:";
			gh->log(cog_str1 + request_buff);

			//5.3 hand request
			string request_str = request_buff;
			struct rep_command req_cmd;

			if (!is_req_store(request_str, req_cmd)) {
				break;
			}
			//5.4 write to request
			string respose_content = "\f";
			int write_result = write(new_fd, respose_content.c_str(),
					strlen(respose_content.c_str()));
			if (write_result < 0) {
				perror("socket fd write fail...");
			}

			//5.5 accept the real content with gzip
			string filename = "./pages_gzip/" + gh->ld2str(req_cmd.task_id)
					+ ".tar.gz";
			int fd = open(gh->ld2str(req_cmd.task_id).c_str(),
					O_CREAT | O_WRONLY | O_TRUNC);
			long total = 0;

			while (1) {
				read_count = read(new_fd, request_buff, sizeof request_buff);
				if (read_count == -1) {
					perror("socket fd read fail...");
					break;
				}
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

		} //end step 5

		return 0;
	}
	//
	bool is_req_store(string request_str, struct rep_command& req_cmd) {
		bool is_store;
		//==============================================
		//1  request string struct command
		//==============================================
		map<string, string> req_cmd_map;
		gh->command_str_to_map(request_str, &req_cmd_map);
		//struct command req_cmd;
		req_cmd.commd_id = atoi(req_cmd_map["commd_id"].c_str());
		req_cmd.task_id = atol(req_cmd_map["task_id"].c_str());
		req_cmd.slave_id = atoi(req_cmd_map["slave_id"].c_str());
		req_cmd.content_size = atoi(req_cmd_map["content_size"].c_str());
		//req_cmd_map["pagecontent"]
		cout << "request_command:" << req_cmd.commd_id << endl;
		//==============================================
		//3 switch command type
		//==============================================
		map<string, string> response_cmd_map;
		switch (req_cmd.commd_id) {
		case 5: //slave to rep
			store_message(request_str);
			is_store = 1;
			break;
		default:
			is_store = 0;
			break;
		}

		return is_store;
	}
	//======================
	void store_message(string request_str) {
		cout << endl;
	}

	//================
};
//
}
#endif /* REPERTORY_H_ */