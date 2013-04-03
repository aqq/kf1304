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
cout<<mytask.url_body->at(index)<<endl;

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

	//char http_request[] =			"GET /question/530490556.html HTTP/1.1\r\nHost: zhidao.baidu.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.84 Safari/535.11 SE 2.X MetaSr 1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: utf-8;q=0.7,*;q=0.3\r\n%s\r\n\r\n";
	//char cookie[] = "Cookie: ";
	//char cookie[]="Cookie: Hm_lvt_0e4a6d78858c1a36901881c93bd86067=1352537642364,1352699807358,1352864424733,1353231533848; Hm_lvt_ced1e79e7fa08b57cae5394a93748264=1351828040154,1352439563852,1352539827418,1353301419176; "
	//"bdshare_firstime=1353718390129;BDREFER=%7Burl%3A%22http%3A//news.baidu.com/%22%2Cword%3A%22%22%7D; BAIDUID=8EDAAD9053BEA3C4F1A021013F88C5C9:FG=1; BAIDU_WISE_UID=AA1658B98730EA255C698C58D3D5210E; MCITY=-131%3A; BAIDULXBCB=40B588D2439B02F1D36A9824FFAFE1CE82E3C21B9B5C1156F5A4564C424E77E20E6FEA0F362DCCF8691571DACAB4AD4ED81C9F2C6082612485F72776FEFABC24F44538870A759B1DF2BEA7BB60F7AA33DB1D5937E1";
	//char request[4096];
//	snprintf(request, 4096, http_request, cookie);
	//int w =
	write(sockfd, mytask.url_body->at(index).c_str(),
			strlen(mytask.url_body->at(index).c_str()));
	char buf[4096];
	int count = 0;

	ostringstream ret_ostring;
	string filename = "./page";
	filename += (index+48);
	filename.append(".txt");
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND);

	while ((count = read(sockfd, buf, 1448)) > 0)
	{
		write(fd, buf, count);
		if (tail_with_feature(buf, count, "\r\n0\r\n\r\n"))
		{
			break;
		}

	}
	close(fd);
	close(sockfd);

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
