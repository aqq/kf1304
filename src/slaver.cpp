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

namespace poseidon
{

slaver::slaver()
{
	// TODO Auto-generated constructor stub
//初始化 获取自身的SLAVE ID
}

slaver::~slaver()
{

}

bool slaver::requestTask(task *mytask)
{

	return 1;
}

bool slaver::work(task mytask)
{

	uint count = mytask.url_body->size();
	for (uint k = 0; k < count; k++)
	{
		grabpage(mytask, k);
	}
	return 1;
}
bool slaver::grabpage(task mytask, int index)
{

	//create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket fd create fail...");
		return -1;
	}
	//prepare socket's dest ip and port
	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(mytask.store_port);
	dest_addr.sin_addr.s_addr = inet_addr(mytask.store_ip.c_str());
	bzero(&(dest_addr.sin_zero), 8);
	// connect
	if (-1
			== connect(sockfd, (struct sockaddr*) &dest_addr,
					sizeof(struct sockaddr)))
	{
		perror("socket fd connet fail...");
		return -1;
	}
	//prepare http_request_header
	string http_request_header = this->getHttpHeader(mytask.url_header,
			*mytask.url_body, index);
	write(sockfd, http_request_header.c_str(), http_request_header.length());
	char read_buf[4096]; //read_buf
	int count = 0;
	//
	ostringstream ret_ostring;
	string filename = "./";
	filename.append(mytask.url_body->at(index));
	filename.append(".txt");
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND);
	//int fd = open("./result.log", O_CREAT | O_WRONLY | O_APPEND);
	while ((count = read(sockfd, read_buf, sizeof(read_buf))) > 0)
	{
		write(fd, read_buf, count);
		//	if (tail_with_feature(buf, count, "\r\n0\r\n\r\n"))
		//	{
		//		break;
		//	}

	}
	close(fd);
	//close socket
	close(sockfd);

	return 1;
}
bool slaver::localStorePage()
{
	return 1;
}
bool slaver::remoteStorePage()
{
	return 1;
}

string slaver::getHttpHeader(string url_header, vector<string> url_body, int id)
{
	//url_header.replace(url_header.find_first_of('#'),,url_body);

	string::size_type pos(0);

	const string special_char = "#";
	const string s4 = url_body.at(id);

	pos = url_header.find_first_of('#');
//	if ((pos = url_header.find(special_char)) != string::npos)

	url_header = url_header.replace(pos, 1, "");
	return url_header.insert(pos, s4);

}

} /* namespace poseidon */
