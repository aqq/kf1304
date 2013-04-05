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
#include "GlobalHelper.h"
#include "slaver_test.h"
using namespace std;

namespace poseidon {

class master {

private:
	int master_port; //80
	string master_ip; //"192.168.75.128";
	//command 1 salve 1
	string command_m2s_1; //"2\r\n1\r\n\f";
	GlobalHelper *gh;
	bool isworktime;
	int current_version;
public:
	master() {
		master_port = 9000;
		master_ip = "192.168.75.128";
		command_m2s_1 = "2\r\n1\r\n\f";
		gh = new GlobalHelper();
		isworktime = false;
		current_version = 2;
	}

	bool is_worktime() {
		isworktime = !isworktime;
		return isworktime;
	}
	int service() {
		//1 init variable
		int socketfd;
		socklen_t size;
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
		//5   revice str
		while (1) {

			size = sizeof(client_addr);
			//accept
			if ((new_fd = accept(socketfd, (struct sockaddr *) (&client_addr),
					&size)) == -1) {
				perror("socket new_fd accept fail...");
				return -1;
			}
			cout << "conntection from" << inet_ntoa(client_addr.sin_addr)
					<< endl;
			int read_count;
			//
			while (1) {
				read_count = read(new_fd, request_buff, sizeof request_buff);

				if (read_count == 0) {
					perror("socket fd read fail...");
					break;
				}
				if (gh->tail_with_feature(request_buff, read_count, "\f")) {
					break;
				}

			} //hand request
			request_buff[read_count] = '\0';

			cout << "request is :" << request_buff << endl;

			string request_str = request_buff;
			vector<string> *vec = new vector<string>();
			vector<string> *vec2 = new vector<string>();
			string respose_content = "";

			gh->split_line(request_str, vec);
			gh->split_by_split_char(vec->at(0), vec2, ':');
			int commd_id = atoi(vec2->at(1).c_str());
			gh->split_by_split_char(vec->at(2), vec2, ':');
			int app_version = atoi(vec2->at(1).c_str());
			cout << "request_command:" << commd_id << endl;
			switch (commd_id) {
			case 1: //request_task
				//if version current version
				if (app_version < this->current_version) {
					respose_content = "commd_id:3\r\n"
							"slave_id:1\r\n"
							"version:2\r\n"
							"url:192.168.1.1/slave_v2.gz\r\n"
							"\f";
				} else if (is_worktime()) {
					task *mytask;
					init_task_4_grabpage_114_wx_1(mytask);
					respose_content = "commd_id:4[\r\n]"
							"slave_id:1[\r\n]"
							"task_id:1[\r\n]"
							"url:"
							//-------------------command header
							"\a"
							//---------------1
							"GET /tradehtml/320200/3202001212190001/index.html HTTP/1.1\r\n"
							"Host: wx.114chn.com\r\n"
							"Connection: keep-alive\r\n"
							"Cache-Control: max-age=0\r\n"
							"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
							"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
							"Accept-Encoding: gzip,deflate,sdch\r\n"
							"Accept-Language: zh-CN,zh;q=0.8\r\n"
							"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
							"\r\n"
							"\a"
							//---------------2
							"www.2.com#GET /2.html HTTP/1.1\r\nHost: zhidao.baidu.com\r\nConnection: keep-alive\r\nAccept-Encoding: gzip,deflate\r\n%s\r\n\r\n"
							"\a"
							"\f";

				} else {
					respose_content = "commd_id:4\r\n"
							"slave_id:1\r\n"
							"time:60\r\n"
							"\f";
				}
				break;
			case 6:
				break;
			default:
				respose_content = "-_0!,bad request!\f";
				break;
			}

			//write to request
			int write_result = write(new_fd, respose_content.c_str(),
					strlen(respose_content.c_str()));
			if (write_result < 0) {
				perror("socket fd write fail...");
			}
			cout << "response:" << respose_content << endl;
			//
			//6 sleep
			//sleep(3);
			//	close(socketfd);
			//	string result = "";

		}
		return 0;
	}
	virtual ~master();
};

}
/* namespace poseidon */
#endif /* MASTER_H_ */