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

#include <iostream>

#include <assert.h>
using namespace std;

namespace poseidon {
//slaver_test_grab
//==========================================================================
void init_grabpage_iis(req_task *mytask) {
	//csdn 117.79.93.222
	//iis 10.2.112.11
	mytask->request_ip = "10.2.112.11";
	mytask->request_port = 81;

	mytask->urls_http_req.push_back(
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
void init_task(req_task *mytask) {
	mytask->sleep_time = 1000;
	mytask->store_ip = "180.149.131.104";
	mytask->cmd_id = GRABPAGE;
	mytask->store_port = 80;
	mytask->task_id.push_back("1");

	mytask->urls_http_req.push_back("1");
	mytask->urls_http_req.push_back("537211931");

}

//slaver_test_grab
//==========================================================================
void init_task_4_grabpage_114_wx_1(req_task *mytask) {
	//119.75.220.34 zhidao
	//"122.115.32.51" wx 114
	mytask->request_ip = "122.115.32.51";
	mytask->request_port = 80;

	//mytask->urls_http_req;
	//子站首页
	mytask->urls_http_req.push_back(
			"GET /tradehtml/320200/3202001212190001/index.html HTTP/1.1\r\n"
					"Host: wx.114chn.com\r\n"
					"Connection: close\r\n"
					"Cache-Control: max-age=0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
					"Accept-Encoding: gzip,deflate,sdch\r\n"
					"Accept-Language: zh-CN,zh;q=0.8\r\n"
					"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
					"\r\n");
	//子站列表页
	mytask->urls_http_req.push_back(
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
	mytask->urls_http_req.push_back(
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
	/*/timing_begin();

	 task mytask; //commad
	 init_grabpage_iis(&mytask);
	 //	init_grabpage_iis(&mytask);
	 //bool request_result;
	 slaver * worker = new slaver();

	 worker->grab_page(mytask.request_ip, mytask.request_port, index,
	 mytask.urls_http_req.at(index), "1");
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
	 */
}
//========================================
void slaver_test_grab() {
	GlobalHelper *gh = new GlobalHelper();
	gh->timing_begin();

	slaver * worker = new slaver();
	grabtask gt;
	gt.http_req =
			"GET / HTTP/1.1\r\n"
					"Host: wx.114chn.com\r\n"
					"Connection: close\r\n"
					"Cache-Control: max-age=0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
					"Accept-Encoding: gzip,deflate,sdch\r\n"
					"Accept-Language: zh-CN,zh;q=0.8\r\n"
					"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
					"\r\n";
	gt.request_ip = "122.115.32.51";
	gt.request_port = 80;
	gt.index = 1;
	gt.task_id = "slaver_test_grab_1";
	gt.url = "www.test.t";
	worker->grab_page(gt);

}
void slaver_test_grab_for_login() {
	GlobalHelper *gh = new GlobalHelper();
	gh->timing_begin();

	slaver * worker = new slaver();
	grabtask gt;
	gt.http_req =
			"POST /login.aspx?ReturnUrl=http%3a%2f%2fwww.cnblogs.com%2f HTTP/1.1\r\n"
					"Host: passport.cnblogs.com\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:20.0) Gecko/20100101 Firefox/20.0\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
					"Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n"
					"Accept-Encoding: gzip, deflate\r\n"
					"Cookie: __utma=226521935.761821559.1366532875.1366532875.1366532875.1; __utmb=226521935.1.10.1366532875; __utmc=226521935; __utmz=226521935.1366532875.1.1.utmcsr=google|utmccn=(organic)|utmcmd=organic|utmctr=cnblogs; __gads=ID=c7f669b811bc0b65:T=1366532936:S=ALNI_MYXTfQ4GJe13K4exXBjecxzcONgYg\r\n"
					"Connection: keep-alive\r\n\r\n";

	gt.request_ip = "100.100.100.100";
	gt.request_port = 80;
	gt.index = 1;
	gt.task_id = "slaver_test_grab_for_login";
	gt.url = "slaver_test_grab_for_login";
	worker->grab_page(gt);

}

void slaver_test_grab_3() {
	slaver_test_grab();
	slaver_test_grab();
	slaver_test_grab();

}
//=======================
//slaver_test_request
void slaver_test_request() {
	slaver * worker = new slaver();
	req_task mytask; //commad
	string str;
	//int i = 2;
//	while (i--) {
	worker->request_task(&mytask, str);
//	}
}
//slaver_test
//==========================================================================
void slaver_work_test() {
	slaver * worker = new slaver();
	worker->work();
}

bool str2task_test() {
	req_task mytask;
	//GlobalHelper *gh = new GlobalHelper();
	slaver * worker = new slaver();
	string command1 =
			"commd_id:4\r\n"
					"slave_id:1\r\n"
					"task_id:20130408132500283768\r\n"
					"version:3\r\n"
					"urls:wx.114chn.com/TradeWeb/web/NewIndex/JobShow.aspx?ID=3202001212190001&channelID=06&JobID=1302280014#wx.114chn.com/tradehtml/320200/3202001212190001/index.html#\r\n"
					"\f";
	string s1 =
			"commd_id:4\r\n"
					"slave_id:1\r\n"
					"task_id:20130416094957931480\r\n"
					"urls:s.114chn.com/s-115007009002-kÆ¬ÃæŒÓ.html#s.114chn.com/s-115007009002-kÅÌÏØÊÕ·ÑÍøÕŸ.html#s.114chn.com/s-115007009002-kÍøÖ·µÚËÄÉ«Å®2472670890.html#s.114chn.com/s-115007009002-kÁÄÁÄÓïÒôË«¿ªÈíŒþÎÒÊÇQQ2573292802.html#s.114chn.com/s-115007009002-kµ÷œÌÅ®ÆÍÔõÃŽÅ®.html#s.114chn.com/s-115007009002-k³ÃÈËÐ¡ËµŒÓ£œ³ÃÈËÐ¡Ëµ.html#s.114chn.com/s-115007009002-kžßÇåÄÚÒÂÐã±íÑÝÏÂÔØ.html#s.114chn.com/s-115007009003-101011-kÅ®ÈËºÈÄÐÈËµÄÄòÊÓÆµŒÓ.html#s.114chn.com/s-115007009003-k80ºóÐÔžñ80ºóÐÔžñ×îÐÂ.html#s.114chn.com/s-115007009003-kvodÖÐ³öŒÓ¡ÖvodÖÐ³ö.html#s.114chn.com/s-115007009003-kšÑ§ÁÄÓïÒô×îÐÂ.html#s.114chn.com/s-115007009003-k¡â¹ãÍ¬œ»ÓÑÐÂµØÖ·.html#s.114chn.com/s-115007009003-k²»ÓÃ×¢²á»ÆÖ·ŒÓ.html#s.114chn.com/s-115007009003-kÄãºÃÁÄÁÄºÃÂð1910847577-s1.html#s.114chn.com/s-115007009003-k²©¿ÍÊ×Ò³ŒÓ.html#s.114chn.com/s-115007009003-kÖÜÓÑÆœÖÜÓÑÆœ¹Ù·œÍøÕŸ.html#s.114chn.com/s-115007009003-kŽóµšÅ·ÖÞÈËÌåŒÓ.html#s.114chn.com/s-115007009003-kÈÕ±ŸÄÐÅ®ŒÓ.html#s.114chn.com/s-115007009003-kÑîÆåº­£ŸŒÓ.html#s.114chn.com/s-115007009003-kÑîÓñÓšÇéÏÝºìÂ¥.html#s.114chn.com/s-115007009003-kÉòÑôÁÄÌìÈºŽò²»¿ª.html#s.114chn.com/s-115007009003-k²š¶àÒ°œáÒÂ2472670890£Ÿ.html#s.114chn.com/s-115007009003-kŒ€ÇéÎåÔÂÅ®QQ2472670890©¶.html#s.114chn.com/s-115007009003-kÌØŒ°ÍøŒÓ.html#s.114chn.com/s-115007009003-kÊŸ·¶£œŒÓ.html#s.114chn.com/s-115007009003-kÍøÉ«Íõ³¯ŒÓ.html#s.114chn.com/s-115007009003-kžØœ»ÀÏÆÅŒÓ.html#s.114chn.c\r\n"
					"\f";
	worker->str2task(s1, mytask);

///	cout << "cmd_id:" << mytask.cmd_id << endl;
	//cout << "task_id:" << mytask.task_id_str << endl;

	//cout << "urls:\n" << mytask.urls << endl;
	return 1;
	assert(
			mytask.urls=="wx.114chn.com/TradeWeb/web/NewIndex/JobShow.aspx?ID=3202001212190001&channelID=06&JobID=1302280014#wx.114chn.com/tradehtml/320200/3202001212190001/index.html#");
	assert( mytask.cmd_id==4);
	assert( mytask.task_id.at(0)=="20130408132500283768");
	assert( mytask.slave_id==1);
	assert( mytask.version==3);

	return 1;
}

void prepare_urls_test() {
	string urls = "www.1.com/1.html#www.2.com/2.html#www.3.com/3.html#";
	req_task mytask;
//	GlobalHelper *gh = new GlobalHelper();
	//notice! the http request header is not set here
	(new slaver())->urls_str_to_http_reqs(mytask, urls);
	assert( mytask.urls_http_req.size()==3);
	/*for (vector<string>::iterator it = mytask.urls_http_req.begin();
	 it != mytask.urls_http_req.end(); it++) {
	 cout << *it << endl;
	 }*/
	for (vector<string>::iterator it2 = mytask.urls_vec.begin();
			it2 != mytask.urls_vec.end(); it2++) {
		cout << ":" << *it2 << endl;
	}

}

bool slaver_test_prepare_req_cmd() {
	slaver * worker = new slaver();

	worker->init_request_cmd_str();
	cout << "worker->cmd_req_2send: " << worker->cmd_req_2send << endl;
	worker->app_version += 1;
	worker->init_request_cmd_str();
	cout << "worker->cmd_req_2send: " << worker->cmd_req_2send << endl;
	return 1;
}

void hand_response_test() {
	slaver * worker = new slaver();
	req_task mytask; //commad
	mytask.sleep_time = 1;
	mytask.cmd_id = GRABPAGE;
	mytask.task_id.push_back("1");
	string urls =
			"urls:"
					"wx.114chn.com/TradeWeb/web/NewIndex/JobShow.aspx?ID=3202001212190001&channelID=06&JobID=1302280014#"
					"wx.114chn.com/tradehtml/320200/3202001212190001/index.html#"
					//		"wx.114chn.com/TradeWeb/web/NewIndex/NewsShow.aspx?ID=3202001212190001&channelID=05&NewsID=125999#"
					//		"shop.114chn.com/mallhtml/320200/3202000911190002/freeindex.html#"
					//		"py.114chn.com/mallhtml/410900/4109001211210001/product41090012112100010009.html#"
					//		"smt.114chn.com/Webpub/320200/090331000004/web/index.shtml#"
					//	"smt.114chn.com/web/CategoryProduct.aspx?profileId=090331000004&aid=320200&cid=08#"
					//		"smt.114chn.com/Webpub/320200/090331000004/ConPD090410000085.shtml#"
					"wx.114chn.com/m/web/shop/index.aspx?shopid=3202001009190002#";
	worker->urls_str_to_http_reqs(mytask, urls);
	//mytask.urls_http_req.push_back("1");
//	mytask.urls_http_req.push_back("537211931");
	mytask.version = 3;
//	208.87.35.103

	mytask.cmd_id = SLEEP;
	worker->hand_task(mytask);

	mytask.cmd_id = UPDATE;
	worker->hand_task(mytask);

	mytask.cmd_id = GRABPAGE;
	worker->hand_task(mytask);

}

//
void get_ip_by_name_test() {
	string ip;
	(new slaver())->get_ip_by_name("www.114.com", ip);
	assert(ip=="208.87.35.103");
}

void lookup_ip_test() {
	slaver *worker = new slaver();
	string ip;
	worker->lookup_ip("www.114.com", ip);
	cout << "ip:" << ip << endl;
	worker->lookup_ip("www.114.com", ip);
	cout << "ip:" << ip << endl;
}
//

void grabpage_work_test() {
	slaver * worker = new slaver();
	req_task mytask; //commad
	mytask.sleep_time = 1;
	//mytask.request_ip = "208.87.35.103";
	//mytask.request_port = 80;
	mytask.cmd_id = GRABPAGE;
	mytask.store_port = 80;
	mytask.task_id.push_back("1");
	string urls =
	//"urls:"
	//		"10.2.112.11/index.html#"
			/*		"biz.ppsj.com.cn/2010-4-6/2536285740.html#"
			 "wx.114chn.com/tradehtml/320200/3202001212190001/index.html#"
			 "wx.114chn.com/TradeWeb/web/NewIndex/NewsShow.aspx?ID=3202001212190001&channelID=05&NewsID=125999#"
			 "shop.114chn.com/mallhtml/320200/3202000911190002/freeindex.html#"
			 "py.114chn.com/mallhtml/410900/4109001211210001/product41090012112100010009.html#"
			 "smt.114chn.com/Webpub/320200/090331000004/web/index.shtml#"
			 "smt.114chn.com/web/CategoryProduct.aspx?profileId=090331000004&aid=320200&cid=08#"
			 "smt.114chn.com/Webpub/320200/090331000004/ConPD090410000085.shtml#"
			 "wx.114chn.com/m/web/shop/index.aspx?shopid=3202001009190002#";
			 */
			"s.hc360.com/supply/%B7%C0%CB%AE%C5%C1%B5%C6.html#"
					"gzxianyou.b2b.hc360.com/supply/183794961.html#"
					"hlwtdgsb.b2b.hc360.com/supply/166144361.html#"
					"ankdled.b2b.hc360.com/supply/174876212.html#"
					"yinlanglight.b2b.hc360.com/supply/175241565.html#"
					"gdh1.b2b.hc360.com/supply/117295913.html#"
					"www.chat.hc360.com/intro/74301.html#"
					"info.tele.hc360.com/2013/04/070800420997.shtml#"
					"info.tele.hc360.com/2013/04/070740420990.shtml#";
	/*
	 "detail.china.alibaba.com/offer/1136008016.html?spm=a260k.635.5702053.0#"
	 "detail.china.alibaba.com/offer/1199146961.html#"
	 "search.china.alibaba.com/selloffer/--1031639.html?spm=a260k.635.5746625.2#"
	 "detail.china.alibaba.com/offer/1212538949.html?tracelog=p4p#"
	 "home.china.alibaba.com/huodong/15baohuan.html?spm=a260k.635.5702057.19&tracelog=hpcenterzc_mr_a3_jj#"
	 "detail.china.alibaba.com/offer/1194852693.html?spm=a260f.2176813.5690225.5#"
	 "search.china.alibaba.com/selloffer/-CDE2CCD7.html?spm=a260k.635.5746617.20#"
	 "detail.china.alibaba.com/offer/1191346235.html?tracelog=p4p#"
	 "yulifushi.cn.alibaba.com/page/offerlist_15236427.htm?sortType=showcase#"
	 */
	;
	//	"zhidao.baidu.com/question/211231.html#";

	worker->urls_str_to_http_reqs(mytask, urls);
	worker->grabpage_work(mytask);
}

//==================================================
void grab_url_fail() {

}
//

void tar_pages_and_get_fnames_vec_test() {

	vector<string> v1;
	slaver * worker = new slaver();
	worker->tar_pages_and_get_fnames_vec(v1);

	string tar_download_sh_fname = "./download/tar_download.sh";
	//ofstream outfile(tar_download_sh_fname.c_str(), ios::trunc);
	for (vector<string>::iterator it2 = v1.begin(); it2 != v1.end(); ++it2) {
		//outfile << *it2 << endl;
		cout << "filename:" << *it2 << endl;
	}
//	outfile.close(); /**/
}
void remoteStorePage_test() {
	slaver * worker = new slaver();
	storetask s_task;
	s_task.request_ip = "192.168.75.128";
	s_task.request_port = 9001;

	string send_filename = "./download/20130409181506335224.tar.gz";
	string cmd = "commd_id:5\r\n"
			"slave_id:1\r\n"
			"task_id:20130409181510256411\r\n"
			"content_size:#\r\n"
			"\f";
	worker->remote_store_page(s_task, cmd, send_filename);
}

void store_page_test() {
	slaver * worker = new slaver();
	storetask s_task;

	s_task.request_ip = worker->store_ip[0];
	s_task.request_port = 9001;
	worker->store_page(s_task);
}
bool page_read_binary_test() {
	//==============================
	//4.send content
	//==============================
	string send_filename = "./download/t1.tar.gz";
	//read binary page and send. begin
	ifstream is_page(send_filename.c_str(), ios::binary);
	char * page_buff;
	if (!is_page) {
		return false;
	}
	is_page.seekg(0, is_page.end);
	size_t length = is_page.tellg();
	is_page.seekg(0, is_page.beg);
	page_buff = (char *) malloc(length);
	is_page.read(page_buff, length);
	//read binary page and send. end
	ofstream os_page("./download/t2.tar.gz", ios::binary);
	os_page.write(page_buff, length);
	os_page.close();
	//
	free(page_buff);
	//}
	is_page.close();
	return 1;
}

void slave_config_test() {
	slaver * worker = new slaver();
	assert(worker->master_ip[0]=="192.168.75.128");
	assert(worker->slave_id==1);
	assert(worker->master_port==9000);
}
//
void local_ip_test() {
	slaver * worker = new slaver();
	cout << worker->local_ip() << endl;
}

void split_response_status() {

	string page_content =
			"ja.114chn.com/chtml/360800/HotInfoView100104000173.html HTTP/1.1 200 OK\r\n";
	GlobalHelper *gh = new GlobalHelper();

	vector<string> str_vec;
	gh->split(page_content.c_str(), " ", str_vec);
	cout << str_vec[2] << endl;

	//ja.114chn.com/chtml/360800/HotInfoView100104000173.html HTTP/1.1 200 OK
}
} /* namespace poseidon */
#endif /* slave_test_H_ */
