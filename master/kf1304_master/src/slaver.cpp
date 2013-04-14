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


#define DEBUG
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
#ifdef DEBUG
	cout << mytask.url_body->at(index) << endl;
#endif
	struct sockaddr_in dest_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket fd create fail...");
		return -1;
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(mytask.request_port);
	dest_addr.sin_addr.s_addr = inet_addr(mytask.request_ip.c_str());
	bzero(&(dest_addr.sin_zero), 8);

	if (-1
			== connect(sockfd, (struct sockaddr*) &dest_addr,
					sizeof(struct sockaddr)))
	{
		perror("socket fd connet fail...");
		return -1;
	}

	write(sockfd, mytask.url_body->at(index).c_str(),
			strlen(mytask.url_body->at(index).c_str()));
	char buf[4096];
	int count = 0;

	GlobalHelper *gh = new GlobalHelper();
	string response_content;
	//string::size_type response_content_size;
	//bool flag = false;

	string filename = "./temp" + gh->num2str(index) + ".txt";
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND);

#ifdef DEBUG
	cout << "begin while" << endl;
#endif

	while ((count = read(sockfd, buf, 1448)) > 0)
	{
#ifdef DEBUG
		//cout << buf << "****" << endl;
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
		if (gh->tail_with_feature(buf, count, "\r\n0\r\n\r\n"))
		{
			break;
		}

		if (gh->tail_with_feature(buf, count, "</html>\r\n\r\n"))
		{
			break;
		}
		if (gh->tail_with_feature(buf, count, "</html>"))
		{
			break;
		}

	}

	close(fd);

	close(sockfd);

	//尝试通过C＋＋函数写入本地文件，内容缺失，，猜测是STR.APPEND的时候加入了‘、0’
	//string filename2 = "./txt_" + gh->num2str(index) + ".txt";
	//TextLogger *logger = new TextLogger(filename2.c_str());
	//logger->LogContent(response_content);

	printf("Client Dying.....\n");

	return 0;
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
