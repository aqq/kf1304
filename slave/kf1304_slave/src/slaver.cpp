/*
 * slaver.cpp
 *
 *  Created on: 2013-4-1
 *      Author: root
 */

#include "slaver.h"

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include <unistd.h>
//#include "utility.h"
#include <sstream>
#include <string>
#include <iostream>
#include "unistd.h"
#include "GlobalHelper.h"

#define DEBUG
#define DEBUG_GRAB
#define BUFSIZE 1449
#define READ_BUFF_SIZE 1448
namespace poseidon {

slaver::~slaver() {
	gh->log2("End!", s_moudle);
	gh->log2("//==================================", "MOUDLE");

}

bool slaver::request_task(req_task *mytask, string& str_cmd) {

	gh->log2("connect to ", this->master_ip[0], ":",
			gh->num2str(this->master_port), s_socket);
	//1 init variable
	str_cmd = "";
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

		//3 prepare server address
		this->init_address(&dest_addr, PF_INET, this->master_port,
				this->master_ip[0]);

		//4 connect to server
		if (-1
				== connect(socketfd, (struct sockaddr*) &dest_addr,
						sizeof(struct sockaddr))) {
			log_str = "socket fd connect fail...";
			log_str += strerror(errno);
			gh->log2(log_str, s_socket);
			break;
		}

		//5 write to master
		int bytes_count;
		this->cmd_req_2send = this->init_request_cmd_str(); //last_task_status
		int write_result = write(socketfd, this->cmd_req_2send.c_str(),
				strlen(this->cmd_req_2send.c_str()));
		if (write_result == -1) {
			log_str = "socket fd write fail...";
			log_str += strerror(errno);
			gh->log2(log_str, s_socket); //perror
			break; //continue;
		}
		gh->log2("write:[" + this->cmd_req_2send, "]", s_socket);

		//6 read
		int total = 0;

		while ((bytes_count = read(socketfd, read_buf, READ_BUFF_SIZE)) > 0) {
			total += bytes_count;
			read_buf[bytes_count] = '\0';
			str_cmd += read_buf;
			gh->log2(read_buf, "debug_read_from_socekt");

			if (bytes_count == 0) {
				break;
			}
			if (gh->tail_with_feature(read_buf, bytes_count, "\f")) {
				break;
			}

		}
		gh->log2(str_cmd, "debug_read_from_socekt_str_cmd");
		req_seccess = 1;
		break;
	}
	//}
	//7 clear socket
	close(socketfd);
	//8  init task
//	cout << "receive:" << read_buf << endl;
//	str_cmd = t_str_cmd;
//	gh->log(str_cmd);
	return req_seccess;

}

bool slaver::grabpage_work(req_task& mytask) {
	string dest_ip;

	this->last_task_status = 0;
	uint count = mytask.urls_http_req.size();
	bool is_grab_ok = 0;
	for (vector<string>::size_type index = 0; index < count; index++) {

		if (lookup_ip(mytask.urls_sites.at(index), dest_ip)) {
			grabtask gt;
			gt.http_req = mytask.urls_http_req.at(index);
			gt.request_ip = dest_ip;
			gt.request_port = 80;
			gt.index = index;
			gt.task_id = mytask.task_id.at(0);
			gt.url = mytask.urls_vec.at(index);

			is_grab_ok = grab_page_log_time(gt);
			//500毫秒=500*1000=500000
			gh->timing_begin();

			gh->timing_end();
			while (!gh->is_over_cast_time(this->grab_interval)) {
				gh->timing_end();
			}
			//gh->log2("cast_time", gh->cast_time(), "cast_time");
			this->last_task_status = is_grab_ok;
			gh->log2("task is " + gh->bool2str(is_grab_ok), s_work);
		}

	}
	return is_grab_ok;
}
//ip:port,http_req,index
bool slaver::grab_page(grabtask gt) {

	string http_req = gt.http_req; //= mytask.urls_http_req.at(index);
	string request_ip = gt.request_ip;
	int request_port = gt.request_port;
	int task_index = gt.index;
	string task_id = gt.task_id; // = mytask.task_id.at(0);
	string url = gt.url; //mytask.urls.at(index);

	string log_str = "grab index: ";
	log_str += gh->num2str(task_index) + " " + request_ip + ":"
			+ gh->num2str(request_port);
	gh->log2(log_str, s_work);
	gh->log2(http_req.substr(0, http_req.find('\r', 0)), s_socket);
	gh->log2(http_req, "http_req");

	struct sockaddr_in dest_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	while (1) {
		if (-1 == sockfd) {
			perror("socket fd create fail...");
			break;
		}

		gh->init_address(&dest_addr, AF_INET, request_port,
				inet_addr(request_ip.c_str()));

		if (-1
				== connect(sockfd, (struct sockaddr*) &dest_addr,
						sizeof(struct sockaddr))) {
			log_str = "socket fd connect fail...";
			log_str += strerror(errno);
			gh->log2(log_str, s_socket);
			break;
		}

		if (-1 == write(sockfd, http_req.c_str(), http_req.size())) {
			perror("socket fd write fail...");
			gh->log2("socket fd write fail...", s_socket);
			break;
		}
		char buf[1449];
		int count = 0;
		string response_content;

		//========================================
		//open grab_page_file and write split_str into it
		//========================================
		gh->log2("wait to reading from socket:", s_socket);

		int total = 0;
		string page_content = "";
		while ((count = read(sockfd, buf, READ_BUFF_SIZE)) > 0) {
			total += count;
			if (count == -1) {
				gh->log("socket fd read faild...");
				break;
			}
			if (count == 0) {
				gh->log("socket fd read end...");
				break;
			}
			buf[count] = '\0';
			page_content += buf;
			//	if (gh->is_html_end(buf, count)) {
			//		break;
			//		}
		}

		string filename = gh->grab_page_filename(task_index, task_id);
		string split_s1 = gh->get_str_betwen_pages(gt.url);
		gh->log(filename, split_s1 + page_content);
		string head = page_content.substr(0, page_content.find('\r', 0));
		gh->log2(gt.url + " " + head, s_socket);
		gh->log2(gt.url + " " + head, s_work);
		this->last_task_status = 1; //
		break;
	}
	close(sockfd);
	gh->log2("Socket close!", s_socket);
	return this->last_task_status;
}

bool slaver::remoteStorePage(storetask s_task, string send_cmd,
		string send_filename) {
	bool is_store_ok = 0;
	string request_ip = s_task.request_ip;
	int request_port = s_task.request_port;
	//==============================
	//1.init var and connect to rep
	//==============================
	gh->log("connect to:" + request_ip + ":" + gh->num2str(request_port));
	struct sockaddr_in dest_addr;
	string log_str = "";
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	ifstream is_page(send_filename.c_str(), ios::binary);
	//char * page_buff;
	while (1) {
		if (-1 == sockfd) {
			//	perror("socket fd create fail...");.
			log_str.append("socket fd socket fail...").append(strerror(errno));
			gh->log2(log_str, s_socket);
			break;
		}

		gh->init_address(&dest_addr, AF_INET, request_port,
				inet_addr(request_ip.c_str()));

		if (-1
				== connect(sockfd, (struct sockaddr*) &dest_addr,
						sizeof(struct sockaddr))) {
			log_str.append("socket fd connect fail...").append(strerror(errno));
			gh->log2(log_str, s_socket);
			break;
		}
		//==============================
		//2.write cmd to rep
		//==============================
		//[001]read binary page and prepare send. begin
		if (!is_page) {
			break;
		}
		is_page.seekg(0, is_page.end);
		size_t length = is_page.tellg();
		is_page.seekg(0, is_page.beg);
		//page_buff = (char *) malloc(length);
		char page_buff[length];
		is_page.read(page_buff, length);
		//[001]read binary page and prepare send. end

		send_cmd = gh->replace(send_cmd, gh->size_t2str(length), "#");
		gh->log(send_cmd);
		if (-1 == write(sockfd, send_cmd.c_str(), send_cmd.size())) {
			log_str.append("socket fd write fail...").append(strerror(errno));
			gh->log2(log_str, s_socket);
			break;
		}
		//==============================
		//3.revice message
		//==============================
		int count = 0;
		string response_content;
		char buf[1449];
		while ((count = read(sockfd, buf, READ_BUFF_SIZE)) > 0) {
			buf[count] = '\0';
			//cout << "3.revice message buf :" << buf << endl;
			if (count == 0) {
				break;
			}
			if (count < 0) {
				gh->log2("read error", s_socket);
				break;
			}
			if (gh->tail_with_feature(buf, count, "\f")) {
				break;
			}
		}
		//==============================
		//4.prepare send content
		//==============================
//	cout << "4.prepare send content:*\n" << page_buff << "*" << endl;
		//write
		if (-1 == write(sockfd, page_buff, length)) {
			//break; //simple lose
			log_str.append("socket fd write fail...").append(strerror(errno));
			gh->log2(log_str, s_socket);
			break;
		}
		is_store_ok = 1;
		break;
	}
	//free(page_buff);
	//}
//	try {
	is_page.close();
	close(sockfd);
//	} catch (std::exception& e) {
//	gh->log(e.what());
//	}
	gh->log2("Socket close!", s_socket);
	return is_store_ok;

}
/*
 string slaver::getHttpHeader(string url_header, vector<string> url_body,
 int id) {
 //url_header.replace(url_header.find_first_of('#'),,url_body);

 string::size_type pos(0);

 const string special_char = "#";
 const string s4 = url_body.at(id);

 pos = url_header.find_first_of('#');
 //	if ((pos = url_header.find(special_char)) != string::npos)

 url_header = url_header.replace(pos, 1, "");
 return url_header.insert(pos, s4);

 }*/

} /* namespace poseidon */
