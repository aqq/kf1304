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
#include "zlib.h"

#include "GlobalHelper.h"
#include "TextLogger.h"

#define DEBUG
#define BUFSIZE 1449
#define READ_BUFF_SIZE 1448
namespace poseidon {

slaver::~slaver() {

}

bool slaver::requestTask(task *mytask, string& str_cmd) {
	//1 init variable
	int socketfd;
	struct sockaddr_in dest_addr;
	char read_buf[BUFSIZE];
	bzero(&read_buf, sizeof read_buf);
	bool req_seccess = 0;
	while (1) {
		//2 create socket
		if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
			gh->log("socket fd create fail..."); //perror("socket fd create fail...");
			break;
		}

		//3 prepare server address
		this->init_address(&dest_addr, PF_INET, this->master_port,
				this->master_ip);

		//4 connect to server
		if (-1
				== connect(socketfd, (struct sockaddr*) &dest_addr,
						sizeof(struct sockaddr))) {
			printf("socket fd connet fail..."); //perror("socket fd connet fail...");
			break;
		}

		gh->log(
				string(
						"connect to " + this->master_ip + ":"
								+ gh->num2str(this->master_port)));

		//5 talk with server
		int bytes_count;
		//	while (1) { //loop if request failed		//prepare_req_cmd();
		prepare_req_cmd();
		int write_result = write(socketfd, this->cmd_req_2send.c_str(),
				strlen(this->cmd_req_2send.c_str()));
		if (write_result < 0) {
			gh->log("socket fd write fail..."); //perror
			break; //continue;
		}

		cout << "write:" << this->cmd_req_2send << endl;
		while ((bytes_count = read(socketfd, read_buf, READ_BUFF_SIZE)) > 0) {
			if (bytes_count == 0) {
				gh->log("socket fd read fail..."); //perror
				break;
			}

			if (gh->tail_with_feature(read_buf, bytes_count, "\f")) {
				break;
			}
			read_buf[bytes_count] = '\0';
		}
		req_seccess = 1;
		break;
	}
	//}
	//6 clear socket
	close(socketfd);
	//7  init task
#ifdef DEBUG
	cout << "receive:" << read_buf << endl;
#endif
	str_cmd = read_buf;
	gh->log(str_cmd);
	return req_seccess;
//cout << "cmd_id:" << (*mytask).cmd_id << endl;
	//cout << "slave_id:" << (*mytask).response_cmd_map["slave_id"] << endl;
//	cout << "version" << (*mytask).response_cmd_map["version"] << endl;

}

bool slaver::grabpage_work(task& mytask) {
	string dest_ip;
	//1 dns ip 208.87.35.103
	//for (vector<string>::iterator it = mytask.urls_sites.begin();
	//		it != mytask.urls_sites.end(); it++) {

	//}
//2
	uint count = mytask.urls_http_req.size();
	for (vector<string>::size_type index = 0; index < count; index++) {
		if (lookup_ip(mytask.urls_sites.at(index), dest_ip)) {
			grab_page_log_time(dest_ip, 80, index,
					mytask.urls_http_req.at(index));
			//grab_page
		}

	}
	return 1;
}
//ip:port,http_req,index
bool slaver::grab_page(string request_ip, int request_port, int index,
		string http_req) {
#ifdef DEBUG
	cout << "grab index:" << index << ";" << request_ip << ":" << request_port
			<< ";" << "http_req:\n" << http_req << endl;
#endif
	struct sockaddr_in dest_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		perror("socket fd create fail...");
		return -1;
	}
//[002] prepare 2 reflactor
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(request_port);
	dest_addr.sin_addr.s_addr = inet_addr(request_ip.c_str());
	bzero(&(dest_addr.sin_zero), 8);
//[002] prepare 2 reflactor end
	if (-1
			== connect(sockfd, (struct sockaddr*) &dest_addr,
					sizeof(struct sockaddr))) {
		perror("socket fd connet fail...");
		return -1;
	}

	write(sockfd, http_req.c_str(), strlen(http_req.c_str()));
	char buf[4096];
	int count = 0;

	string response_content;
	//string::size_type response_content_size;
	//bool flag = false;

	string filename = gh->grab_page_filename(index);
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND);

#ifdef DEBUG
	cout << "wait to reading from socket" << endl;
#endif

	while ((count = read(sockfd, buf, READ_BUFF_SIZE)) > 0) {
#ifdef DEBUG
		//	cout << "READ_BUFF:" << buf << endl;
		//	cout << "===================" << endl;
#endif
		//response_content.append(buf);
		write(fd, buf, count);
		//[001]尝试通过CONTENT－LEN来判断 BEGIN
		//if (!flag)		{			flag = gh->Content_length(response_content, &response_content_size);		}
		//string::size_type current_size =
		//response_content.size();
		//cout << "current_size:" << current_size << "total_size:"				<< response_content_size << endl;
		/*	if (flag && (current_size == response_content_size))
		 {
		 #ifdef DEBUG
		 cout << response_content << endl;
		 #endif
		 break;
		 }
		 */
		//[001]尝试通过CONTENT－LEN来判断 END

		if (gh->tail_with_feature(buf, count, "\r\n0\r\n\r\n")) {
			break;
		}
		if (gh->tail_with_feature(buf, count, "</html>\r\n\r\n")) {
			break;
		}
		if (gh->tail_with_feature(buf, count, "</HTML>\r\n\r\n")) {
			break;
		}

		if (gh->tail_with_feature(buf, count, "</html>")) {
			break;
		}

	}

	close(fd);

	close(sockfd);

	//尝试通过C＋＋函数写入本地文件，内容缺失，，猜测是STR.APPEND的时候加入了‘、0’
	//string filename2 = "./txt_" + gh->num2str(index) + ".txt";
	//TextLogger *logger = new TextLogger(filename2.c_str());
	//logger->LogContent(response_content);

	printf("Socket close!\n");

	return 0;
}
bool slaver::localStorePage() {
	return 1;
}
bool slaver::remoteStorePage() {
	return 1;
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
