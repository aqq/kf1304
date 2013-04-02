/*
 * slaver_test.cpp
 *
 *  Created on: 2013-4-1
 *      Author: root
 */

#include "slaver.h"
#include <stdio.h>

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define SLEEP 1
#define UPDATE 2
#define GRABPAGE 3

#include <iostream>
using namespace std;

namespace poseidon
{

void init_task(task *mytask)
{
	mytask->sleep_time = 1000;
	mytask->store_ip = "180.149.131.104";
	mytask->command = GRABPAGE;
	mytask->store_port = 80;
	mytask->task_id = 1;
	mytask->url_body = new vector<string>();
	mytask->url_header =
			"GET /question/#.html HTTP/1.1\r\nHost: zhidao.baidu.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.84 Safari/535.11 SE 2.X MetaSr 1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: utf-8;q=0.7,*;q=0.3\r\n%s\r\n\r\n";

	mytask->url_body->push_back("1");
	mytask->url_body->push_back("537211931");

}
void slaver_test_getHttpHeader()
{
	task mytask; //commad
	init_task(&mytask);
	slaver * worker = new slaver();
	string s1 = worker->getHttpHeader(mytask.url_header, *mytask.url_body, 0);
	string s2 = worker->getHttpHeader(mytask.url_header, *mytask.url_body, 1);
	cout << "!!!Hello World!!!" << endl;
	bool b1 =
			s1
					== "GET /question/1.html HTTP/1.1\r\nHost: zhidao.baidu.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.84 Safari/535.11 SE 2.X MetaSr 1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: utf-8;q=0.7,*;q=0.3\r\n%s\r\n\r\n";
	bool b2 =
			s2
					== "GET /question/537211931.html HTTP/1.1\r\nHost: zhidao.baidu.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.84 Safari/535.11 SE 2.X MetaSr 1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: utf-8;q=0.7,*;q=0.3\r\n%s\r\n\r\n";
	cout << b1 <<
	//":"<< s1<<
			endl;
	cout << b2 <<
	//":"<< s2<<
			endl;
}

void slaver_test_grab()
{
	task mytask; //commad
	init_task(&mytask);
	//bool request_result;
	slaver * worker = new slaver();
	int index = 0;
	worker->grabpage(mytask, index);

	string filename = "./";
	filename.append(mytask.url_body->at(index));
	filename.append(".txt");
	//File* fp = fopen(filename.c_str() );

	FILE *stream;
	stream = fopen(filename.c_str(), "r");
	//fprintf(stream, "This is a test");

	long  length;
	//curpos = ftell(stream);
 fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	//fseek(stream, curpos, SEEK_SET);
	fclose(stream);
	cout << "slaver_test_grab :" << (length > 0) <<"  "<<length<< endl;

}
void slaver_test()
{
	task mytask; //commad
	//bool request_result;
	slaver * worker = new slaver();
	while (true)
	{
		//request_result =
		memset(&mytask, 0, sizeof(mytask));
		worker->requestTask(&mytask);
		switch (mytask.command)
		{
		case SLEEP:
			sleep(1000); //sleep 1 second;
			break;
		case UPDATE:

			break;
		case GRABPAGE:
			worker->work(mytask);
			break;
		default:
			break;
		}
	}
//slaver_worker

}

} /* namespace poseidon */
