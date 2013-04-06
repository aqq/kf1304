/*
 * slaver_test.cpp
 *
 *  Created on: 2013-4-1
 *      Author: root
 */
#ifndef slaver_test_H_
#define slaver_test_H_

#include "slaver.h"
#include <stdio.h>

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include "GlobalHelper.h"
#include "TextLogger.h"

#include <iostream>
using namespace std;

namespace poseidon {
//slaver_test_grab
//==========================================================================
void init_grabpage_iis(task *mytask) {
	//csdn 117.79.93.222
	//iis 10.2.112.11
	mytask->request_ip = "10.2.112.11";
	mytask->request_port = 81;
	mytask->url_body = new vector<string>();
	mytask->url_body->push_back(
			"GET / HTTP/1.1\r\n"
					"Host: 10.2.112.11:81\r\n"
					"Connection: close\r\n"
					"Cache-Control: max-age=0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
					"Accept-Encoding: gzip,deflate,sdch\r\n"
					"Accept-Language: zh-CN,zh;q=0.8\r\n"
					"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n\r\n");
}
//===============================================================
void init_task(task *mytask) {
	mytask->sleep_time = 1000;
	mytask->store_ip = "180.149.131.104";
	mytask->cmd_id = GRABPAGE;
	mytask->store_port = 80;
	mytask->task_id = 1;
	mytask->url_body = new vector<string>();
	mytask->url_header =
			"GET /question/#.html HTTP/1.1\r\nHost: zhidao.baidu.com\r\nConnection: close\r\nCache-Control: max-age=0\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.84 Safari/535.11 SE 2.X MetaSr 1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: utf-8;q=0.7,*;q=0.3\r\n%s\r\n\r\n";

	mytask->url_body->push_back("1");
	mytask->url_body->push_back("537211931");

}

//slaver_test_grab
//==========================================================================
void init_task_4_grabpage_114_wx_1(task *mytask) {
	//119.75.220.34 zhidao
	//"122.115.32.51" wx 114
	mytask->request_ip = "122.115.32.51";
	mytask->request_port = 80;

	mytask->url_body = new vector<string>();
	//子站首页
	mytask->url_body->push_back(
			"GET /tradehtml/320200/3202001212190001/index.html HTTP/1.1\r\n"
					"Host: wx.114chn.com\r\n"
					"Connection: keep-alive\r\n"
					"Cache-Control: max-age=0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
					"Accept-Encoding: gzip,deflate,sdch\r\n"
					"Accept-Language: zh-CN,zh;q=0.8\r\n"
					"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
					"\r\n");
	//子站列表页
	mytask->url_body->push_back(
			"GET /tradehtml/320200/3202001212190001/News.html HTTP/1.1\r\n"
					"Host: wx.114chn.com\r\n"
					"Connection: close\r\n"
					"Cache-Control: max-age=0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
					"Accept-Encoding: gzip,deflate,sdch\r\n"
					"Accept-Language: zh-CN,zh;q=0.8\r\n"
					"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
					"\r\n");
	//子站文章内容页
	mytask->url_body->push_back(
			"GET /TradeWeb/web/NewIndex/NewsShow.aspx?ID=3202001212190001&channelID=05&NewsID=130890 HTTP/1.1\r\n"
					"Host: wx.114chn.com\r\n"
					"Connection: close\r\n"
					"Cache-Control: max-age=0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
					"Accept-Encoding: gzip,deflate,sdch\r\n"
					"Accept-Language: zh-CN,zh;q=0.8\r\n"
					"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
					"\r\n");
}
void slaver_test_grab_local(int index) {
	//timing_begin();

	task mytask; //commad
	init_grabpage_iis(&mytask);
	//	init_grabpage_iis(&mytask);
	//bool request_result;
	slaver * worker = new slaver();

	worker->grabpage(mytask, index);

	string filename = "./temp";
	filename += (index + 48);
	filename.append(".txt");

	FILE *stream;
	stream = fopen(filename.c_str(), "r");
	long length;
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fclose(stream);

	//clock_t cast_time = timing_end();
#ifdef DEBUG
	cout << "filename" << filename << " slaver_test_grab :" << (length > 0)
	<< "; size: " << length << endl;
#endif

}
//========================================
void slaver_test_grab(int index) {
	GlobalHelper *gh = new GlobalHelper();
	gh->timing_begin();

	task mytask; //commad
	init_task_4_grabpage_114_wx_1(&mytask);

	slaver * worker = new slaver();
	worker->grabpage(mytask, index);

	string filename = "./temp" + gh->num2str(index) + ".txt";

	//clock_t cast_time = timing_end();
#ifdef DEBUG
	cout << "filename" << filename << "slaver_test_grab :" << (length > 0)
	<< "; size: " << length << endl;
#endif
	gh->timing_end();
	TextLogger *logger = new TextLogger("log/grab_114.txt");

	string info = "no:" + gh->num2str(index);
	info += " castTime:" + gh->cast_time();
	info += " pagesize:" + gh->num2str(gh->file_size(filename));
	info += " \n:";

	logger->LogContent(info);

}
void slaver_test_grab_3() {
	for (int i = 0; i < 3; i++) {
		slaver_test_grab(i);
	}

}

//slaver_test_request
void slaver_test_request() {
	slaver * worker = new slaver();
	task mytask; //commad
	//int i = 2;
//	while (i--) {
	worker->requestTask(&mytask);
//	}
}
//slaver_test
//==========================================================================
void slaver_test() {
	slaver * worker = new slaver();
	worker->work();
}

bool str2task_test() {
	task mytask;
	GlobalHelper *gh;
	gh = new GlobalHelper();
	slaver * worker = new slaver();
	string command1 = "commd_id:1\r\n"
			"slave_id:2\r\n"
			"application_version:3\r\n"
			"urls:www.1.com/1.html#www.2.com/2.html#www.3.com/3.html#\r\n"
			"\f";
	worker->str2task(command1, &mytask);

	cout << mytask.cmd_id << endl;
	cout << mytask.response_cmd_map["slave_id"] << endl;
	cout << mytask.response_cmd_map["application_version"] << endl;
	for (vector<string>::iterator it = mytask.url_body->begin();
			it != mytask.url_body->end(); it++) {
		mytask.url_body->push_back(gh->convert_url_to_http_req(*it));
	}
//str
	return 1;
}
bool slaver_test_prepare_req_cmd() {
	slaver * worker = new slaver();
	worker->prepare_req_cmd();
	cout << "worker->cmd_req_2send: " << worker->cmd_req_2send << endl;
	worker->app_version += 1;
	worker->prepare_req_cmd();
	cout << "worker->cmd_req_2send: " << worker->cmd_req_2send << endl;
	return 1;
}

void hand_response_test() {
	slaver * worker = new slaver();
	task mytask; //commad
	mytask.sleep_time = 1;
	mytask.store_ip = "180.149.131.104";
	mytask.cmd_id = GRABPAGE;
	mytask.store_port = 80;
	mytask.task_id = 1;
	mytask.url_body = new vector<string>();
	mytask.url_body->push_back("1");
	mytask.url_body->push_back("537211931");
	mytask.version = 3;

	mytask.cmd_id = SLEEP;
	worker->hand_response(mytask);

	mytask.cmd_id = UPDATE;
	worker->hand_response(mytask);

	mytask.cmd_id = GRABPAGE;
	worker->hand_response(mytask);

}
} /* namespace poseidon */
#endif /* slave_test_H_ */
