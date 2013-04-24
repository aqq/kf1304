/*
 * GlobalHelper.h
 *
 *  Created on: 2013-4-4
 *      Author: root
 */

#ifndef GLOBALHELPER_H_
#define GLOBALHELPER_H_

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <sys/time.h>
#include "map"
#include <iostream>     // std::cout
#include <algorithm>    // std::find_if
#include <vector>       // std::vector
#include <iostream>
#include "fstream"
#include "assert.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
//disk
#include <sys/statfs.h>
using namespace std;

#define DEBUG
#define LOG
namespace poseidon {

class GlobalHelper {
private:
//	clock_t clockBegin;
//	clock_t clockEnd;
	struct timeval start, stop, diff;
	int tim_subtract(struct timeval *result, struct timeval *start,
			struct timeval *stop) {
		if (start->tv_sec > stop->tv_sec)
			return -1;
		if ((start->tv_sec == stop->tv_sec) && (start->tv_usec > stop->tv_usec))
			return -1;
		result->tv_sec = (stop->tv_sec - start->tv_sec);
		result->tv_usec = (stop->tv_usec - start->tv_usec);
		if (result->tv_usec < 0) {
			result->tv_sec--;
			result->tv_usec += 1000000;
		}
		return 0;
	}
public:
	//
	string SLAVE_CONF;
	string split_char_betwen_pages;
	string tmp_log_str;
	//
	GlobalHelper() {
		split_char_betwen_pages = "\a";
		SLAVE_CONF = "./conf/slave.conf";
		config();

	}

	map<string, string> config_show_map;
	void config() {
		read_config("./conf/log.conf", config_show_map);
	}
	bool is_log_show(string type) {
		return (config_show_map.find(type) != config_show_map.end());

	}
	virtual ~GlobalHelper();

	void timing_begin() {
		gettimeofday(&start, 0);
	}
	void timing_end() {
		gettimeofday(&stop, 0);
	}
	string cast_time() {
		tim_subtract(&diff, &start, &stop);
		return ld2str(diff.tv_sec) + "." + ld2str(diff.tv_usec);
	}
	bool is_over_cast_time(int mill_seconds) {
		tim_subtract(&diff, &start, &stop);
		return (diff.tv_sec > 0 || diff.tv_usec > mill_seconds);

	}
	void time_test2() {
		int i = 11;
		while (i--) {
			timing_begin();
			//usleep(1000 * 500); //500毫秒
			timing_end();
			cout << "cast_time" << cast_time() << "秒" << endl;
		}
	}
//num to string
	string num2str(int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string size_t2str(size_t i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string ld2str(long int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string bool2str(bool yes) {
		return yes ? "sucessed" : "failed";
	}
	//实现string到unsigned int的转换
	unsigned int string_to_unsigned_int(string str) {
		unsigned int result(0); //最大可表示值为4294967296（=2‘32-1）
		//从字符串首位读取到末位（下标由0到str.size() - 1）
		for (int i = str.size() - 1; i >= 0; i--) {
			unsigned int temp(0), k = str.size() - i - 1;
			//判断是否为数字
			if (isdigit(str[i])) {
				//求出数字与零相对位置
				temp = str[i] - '0';
				while (k--)
					temp *= 10;
				result += temp;
			} else {
				break;
			}
			//exit(-1);

		}
		return result;
	}

	//unit is byte
	int file_size(string filename) {
		FILE *stream;
		stream = fopen(filename.c_str(), "r");
		fseek(stream, 0L, SEEK_END);
		long length = ftell(stream);
		fclose(stream);
		return length;
	}
	void string_contain_test() {
		string s1 = "HTTP/1.1 200 OK\r\n"
				"Cache-Control: max-age=86400\r\n"
				"Date: Thu, 04 Apr 2013 10:34:04 GMT\r\n"
				"Content-Length: 20035\r\n"
				"Content-Type: text/html\r\n";

		string s2;
		if (string_contain(s1, "Content-Length", &s2)) {
			cout << s2 << endl;
		}
		string::size_type len;
		if (Content_length(s1, &len)) {
			cout << len << endl;
		}

	}
	bool Content_length(string s1, string::size_type *length) {
		string s2;
		if (string_contain(s1, "Content-Length", &s2)) {
			*length = this->string_to_unsigned_int(s2);
			return true;
		}
		return false;
	}
	bool string_contain(string str, string key, string *result) {
		unsigned int pos;
		unsigned int pos1;
		unsigned int pos2;
//STRING.FIND 查找，找到第一个取其：后\r\n 前的值。
		if ((pos = str.find(key, 0)) == string::npos) {
			return false;
		}
		if ((pos1 = str.find(":", pos)) == string::npos) {
			return false;
		}
		pos1++;
		if ((pos2 = str.find("\r", pos)) == string::npos) {
			return false;
		}
		cout << "pos=" << pos << " pos1=" << pos1 << " pos2=" << pos2 << endl;
		if (pos1 > pos2) {
			return false;
		}
		*result = str.substr(pos1, pos2 - pos1);
		return true;
	}

//是否以给定字符串结尾
	bool tail_with_feature(char* buf, size_t count, const char* feature) {

		size_t feature_len = strlen(feature);

		if (count < feature_len || 0 >= feature_len) {
			return false;
		}
		for (size_t j = 0; j < feature_len; ++j) {
			if (buf[count - j - 1] != feature[feature_len - j - 1]) {
				return false;
			}
		}
		return true;
	}

	bool split_char(char c) {
		return c == ':';
	}
	bool not_split_char(char c) {
		return c != ':';
	}

	//
	//按行分割
	void split_line(string in, vector<string> *vec) {
		istringstream ss(in);
		string t;
		while (ss >> t) {
			//		cout << "split_line:" << t << endl;
			vec->push_back(t);
		}
	}
	void split_line2(string s1, vector<string>& str_vec) {

		split(s1, "\n", str_vec);

	}
	//以 为分隔符 只能取前2个
	//str=slave_id:1
	void split_by_split_char(string str, vector<string> *vec, char split_char) {
		string::size_type pos = 0;
		pos = str.find_first_of(':', pos);
		string key = str.substr(0, pos);
		string value = str.substr(pos + 1, str.size());

		vec->push_back(key);
		vec->push_back(value);
		//	this->log2(key, "kv");
		//this->log2(value, "kv");

	}
	//以 为分隔符 尝试使用 算法模板 失败。待调试
	void split_4alg(string str, vector<string> *vec, char split_char) {
		typedef string::const_iterator iter;
		iter i = str.begin();
		while (i != str.end()) {
			//i =
			//std::find_if(i, str.end(), not_split_char);
		}
		//string t;
		//vec->push_back(t);
		std::vector<int> myvector;
		myvector.push_back(10);
		//typedef bool (poseidon::GlobalHelper::*p1)(int);
		//p1 cfp = &poseidon::GlobalHelper::IsOdd;
		//	bool (*IsOddfun)(int); //bool (poseidon::GlobalHelper::)(int)
		//	std::vector<int>::iterator it = std::find_if(myvector.begin(),
		//			myvector.end(), cfp);
		//std::cout << "The first odd value is " << *it << '\n';

	}

//以"\r "为分隔符
	bool split_line_test() {
		vector<string> *vec = new vector<string>();
		string command1 = "commd_id:1\r\n"
				"slave_id:1\r\n"
				"application_version:1\r\n"
				"\f";
		split_line(command1, vec);
		bool res = false;
		string s1 = "commd_id:1";
		res = (s1 == vec->at(0));
		if (!res) {
			cout << res << endl;
			return res;
		}
		vector<string> *vec2 = new vector<string>();
		split_by_split_char(s1, vec2, ':');
		res = (s1 == vec2->at(0));
		cout << vec2->at(0) << endl;
		cout << vec2->at(1) << endl;
		return res;

	}
	//==================================
	//无限分割入VEC
	//vector<string> str_vec;
	//string s1 = "1#www.baidu.com#2.com#";
	//const string s2 = "#";
	//split(s1, "#", str_vec);
	//
	void split(const string& src, const string& separator,
			vector<string>& dest) {
		string str = src;
		string substring;
		string::size_type start = 0, index;
		do {
			index = str.find_first_of(separator, start);
			if (index != string::npos) {
				substring = str.substr(start, index - start);
				dest.push_back(substring);
				start = str.find_first_not_of(separator, index);
				if (start == string::npos)
					return;
			}
		} while (index != string::npos);
		//the last token
		substring = str.substr(start);
		dest.push_back(substring);
	}
	bool split_test() {
		vector<string> str_vec;
		string s1 = "1#www.baidu.com#2.com#";
		const string s2 = "#";

		split(s1, "#", str_vec);
		cout << str_vec.at(0) << endl;
		cout << str_vec.at(1) << endl;
		cout << str_vec.at(2) << endl;
		cout << "SIZE:" << str_vec.size() << endl;
		return 1;
		//split(s1, &s2, &str_vec);
	}

	//==================================
	void command_str_to_map(string command1, map<string, string> *command_map) {
		vector<string> *vec = new vector<string>();
		vector<string>::iterator it1;
		split_line(command1, vec);
		for (it1 = vec->begin(); it1 != vec->end(); ++it1) {
			vector<string> vec2;
			split_by_split_char(*it1, &vec2, ':');
			string key = vec2.at(0);
			string value = vec2.at(1);
			//	log2("**************", "dg_command_str_to_map");
			//	log2("key:", key, "dg_command_str_to_map");
			//	log2("value:", value, "dg_command_str_to_map");
			//log2("**************", "dg_command_str_to_map");
			(*command_map)[key] = value;
		}
	}

	//以"\r "为分隔符
	bool command_str_to_map_test() {

		map<string, string> command_map;
		string command1 = "commd_id:1\r\n"
				"slave_id:1\r\n"
				"application_version:1\r\n"
				"urls:"
				"\f";

		string s1 =
				"commd_id:4\r\n"
						"slave_id:1\r\n"
						"task_id:20130416094957931480\r\n"
						"urls:s.114chn.com/s-115007009002-kÆ¬ÃæŒÓ.html#s.114chn.com/s-115007009002-kÅÌÏØÊÕ·ÑÍøÕŸ.html#s.114chn.com/s-115007009002-kÍøÖ·µÚËÄÉ«Å®2472670890.html#s.114chn.com/s-115007009002-kÁÄÁÄÓïÒôË«¿ªÈíŒþÎÒÊÇQQ2573292802.html#s.114chn.com/s-115007009002-kµ÷œÌÅ®ÆÍÔõÃŽÅ®.html#s.114chn.com/s-115007009002-k³ÃÈËÐ¡ËµŒÓ£œ³ÃÈËÐ¡Ëµ.html#s.114chn.com/s-115007009002-kžßÇåÄÚÒÂÐã±íÑÝÏÂÔØ.html#s.114chn.com/s-115007009003-101011-kÅ®ÈËºÈÄÐÈËµÄÄòÊÓÆµŒÓ.html#s.114chn.com/s-115007009003-k80ºóÐÔžñ80ºóÐÔžñ×îÐÂ.html#s.114chn.com/s-115007009003-kvodÖÐ³öŒÓ¡ÖvodÖÐ³ö.html#s.114chn.com/s-115007009003-kšÑ§ÁÄÓïÒô×îÐÂ.html#s.114chn.com/s-115007009003-k¡â¹ãÍ¬œ»ÓÑÐÂµØÖ·.html#s.114chn.com/s-115007009003-k²»ÓÃ×¢²á»ÆÖ·ŒÓ.html#s.114chn.com/s-115007009003-kÄãºÃÁÄÁÄºÃÂð1910847577-s1.html#s.114chn.com/s-115007009003-k²©¿ÍÊ×Ò³ŒÓ.html#s.114chn.com/s-115007009003-kÖÜÓÑÆœÖÜÓÑÆœ¹Ù·œÍøÕŸ.html#s.114chn.com/s-115007009003-kŽóµšÅ·ÖÞÈËÌåŒÓ.html#s.114chn.com/s-115007009003-kÈÕ±ŸÄÐÅ®ŒÓ.html#s.114chn.com/s-115007009003-kÑîÆåº­£ŸŒÓ.html#s.114chn.com/s-115007009003-kÑîÓñÓšÇéÏÝºìÂ¥.html#s.114chn.com/s-115007009003-kÉòÑôÁÄÌìÈºŽò²»¿ª.html#s.114chn.com/s-115007009003-k²š¶àÒ°œáÒÂ2472670890£Ÿ.html#s.114chn.com/s-115007009003-kŒ€ÇéÎåÔÂÅ®QQ2472670890©¶.html#s.114chn.com/s-115007009003-kÌØŒ°ÍøŒÓ.html#s.114chn.com/s-115007009003-kÊŸ·¶£œŒÓ.html#s.114chn.com/s-115007009003-kÍøÉ«Íõ³¯ŒÓ.html#s.114chn.com/s-115007009003-kžØœ»ÀÏÆÅŒÓ.html#s.114chn.c\r\n"
						"\f";
		command_str_to_map(s1, &command_map);

		for (map<string, string>::iterator it2 = command_map.begin();
				it2 != command_map.end(); ++it2) {
			std::cout << it2->first << " => " << it2->second << endl;
		}
		//cout << "split_line_test:" << (commd_id == 1) << endl;

		return 1;
	}
	//==========================================
	string replace(string str, string new_str, string special_char) {

		string::size_type pos(0);
		pos = str.find_first_of(special_char);

		str = str.replace(pos, special_char.size(), "");
		return str.insert(pos, new_str);
	}
	void replace_test() {
		string s1 = "GET /question/#.html Host:@\r\n";
		string s2 = replace(s1, "123", "#");
		cout << "s2:" << s2 << endl;
		string s3 = replace(s2, "345", "@");
		cout << "s3:" << s3 << endl;

	}
	//================================
	string get_sitename(string url) {
		string::size_type pos1; //  pos of  '/'
		pos1 = url.find_first_of('/', 0);
		string sitename = url.substr(0, pos1);
		return sitename;
	}
	string convert_url_to_urls(string url) {
		string req = "";
		string::size_type pos1; //  pos of  '/'
		pos1 = url.find_first_of('/', 0);
		string sitename = url.substr(0, pos1);
		string urlbody = url.substr(pos1);

		req = replace(req, urlbody, "#");
		req = replace(req, sitename, "@");
		req = replace(req, "", "$");
		//cout << "sitename:" << sitename << endl;
		//	cout << "urlbody:" << urlbody << endl;
		return req;
	}
	int file_lines(string filename) {
		ifstream ReadFile;
		int n = 0;
		char line[1024];
		ReadFile.open(filename.c_str(), ios::in); //ios::in 表示以只读的方式读取文件
		if (ReadFile.fail()) {
			return 0;
		}

		while (!ReadFile.eof()) {
			ReadFile.getline(line, 1024, '\n');
			n++;
		}
		ReadFile.close();
		return n;
	}

	string read_line(string fname, int index_line) {
		ifstream ReadFile;
		int n = 0;
		char line[1024];
		ReadFile.open(fname.c_str(), ios::in);
		if (ReadFile.fail()) {
			return 0;
		}
		while (!ReadFile.eof()) {
			ReadFile.getline(line, 1024, '\n');
			n++;
			if (n == index_line) {
				break;
			}
		}
		ReadFile.close();
		string res = line;
		return res;
	}
	void read_line_from_test() {
		string fpath = "./conf/user_agent";

		//	cout << file_lines(fpath) << endl;
		//	cout << read_line(fpath, 2) << endl;
		int i = 6;
		while (i--) {
			cout << random_get_one_line(fpath) << endl;
		}
	}

	string random_get_one_line(string fpath) {

		int flines = file_lines(fpath);
		int random_int = rand() % flines;
		string user_agent_str = read_line(fpath, random_int);
		//"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31";
		return user_agent_str;
	}

	string convert_url_to_http_req(string url) {
		string fpath_user_agent = "./http_req/User-Agent";
		string fpath_accept = "./http_req/Accept";
		string fpath_accept_language = "./http_req/Accept-Language";
		string fpath_accept_charset = "./http_req/Accept-Charset";
		string req =
				"GET # HTTP/1.1\r\n"
						"Host: #\r\n"
						"Connection: close\r\n"
						"Cache-Control: max-age=0\r\n"
						"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
						"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31\r\n"
						"Accept-Language: zh-CN,zh;q=0.8\r\n"
						"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n\r\n";
		log2(url, "convert_url_to_http_req");
		string::size_type pos1; //  pos of  '/'
		pos1 = url.find_first_of('/', 0);
		string host = url.substr(0, pos1);
		string urlbody = url.substr(pos1);
		req = replace(req, urlbody, "#");
		req = replace(req, host, "#"); //host

		//req = replace(req, "", "#"); //cookie
		return req;
	}
	string convert_url_to_http_req3(string url) {
		string fpath_user_agent = "./http_req/User-Agent";
		string fpath_accept = "./http_req/Accept";
		string fpath_accept_language = "./http_req/Accept-Language";
		string fpath_accept_charset = "./http_req/Accept-Charset";
		string req = "GET # HTTP/1.1\r\n"
				"Host: #\r\n"
				"Connection: close\r\n"
				//		"Cache-Control: max-age=0\r\n"
				"User-Agent: #\r\n"
				//			"Accept: #\r\n"
				//			"Accept-Language: #\r\n"
				//			"Accept-Charset: #\r\n"
				//"Cookie:#\r\n"
				"\r\n";
		log2(url, "convert_url_to_http_req");
		string::size_type pos1; //  pos of  '/'
		pos1 = url.find_first_of('/', 0);
		string host = url.substr(0, pos1);
		string urlbody = url.substr(pos1);
		string user_agent_str = random_get_one_line(fpath_user_agent);
		req = replace(req, urlbody, "#");
		req = replace(req, host, "#"); //host
		req = replace(req, random_get_one_line(fpath_user_agent), "#");
//		req = replace(req, random_get_one_line(fpath_accept), "#");
//		req = replace(req, random_get_one_line(fpath_accept_language), "#");
		//	req = replace(req, random_get_one_line(fpath_accept_charset), "#");
		//req = replace(req, "", "#"); //cookie
		return req;
	}
	string convert_url_to_http_req4(string url) {
		string fpath_user_agent = "./http_req/User-Agent";
		string fpath_accept = "./http_req/Accept";
		string fpath_accept_language = "./http_req/Accept-Language";
		string fpath_accept_charset = "./http_req/Accept-Charset";
		string req = "GET # HTTP/1.1\r\n"
				"Host: #\r\n"
				"Connection: close\r\n"
				"Cache-Control: max-age=0\r\n"
				"User-Agent: #\r\n"
				"Accept: #\r\n"
				"Accept-Language: #\r\n"
				"Accept-Charset: #\r\n"
				//"Cookie:#\r\n"
				"\r\n";
		log2(url, "convert_url_to_http_req");
		string::size_type pos1; //  pos of  '/'
		pos1 = url.find_first_of('/', 0);
		string host = url.substr(0, pos1);
		string urlbody = url.substr(pos1);
		string user_agent_str = random_get_one_line(fpath_user_agent);
		req = replace(req, urlbody, "#");
		req = replace(req, host, "#"); //host
		req = replace(req, random_get_one_line(fpath_user_agent), "#");
		req = replace(req, random_get_one_line(fpath_accept), "#");
		req = replace(req, random_get_one_line(fpath_accept_language), "#");
		req = replace(req, random_get_one_line(fpath_accept_charset), "#");
		//req = replace(req, "", "#"); //cookie
		return req;
	}
	bool convert_url_to_http_req_test() {
		string url = "wx.114chn.com/m/web/shop/index.aspx";
		cout << "url:" << url << endl;
		string http_req = convert_url_to_http_req(url);
		cout << "http_req:" << http_req << endl;
		return 1;
	}
	void log(string filename, string content) {
		ofstream outfile(filename.c_str(), ios::app);
		outfile << content;
		outfile.close();
	}
	void log4(string filename, string content) {
		ofstream outfile(filename.c_str(), ios::app);

		outfile << content << endl;
		outfile.close();
	}
	void log(string filename, string content, ios::openmode t1) {
		ofstream outfile(filename.c_str(), t1);
		outfile << content;
		outfile.close();
	}

//
//
	void log3(string& content, string type) {

		string time_str = get_string_time("%Y%m%d");

		string filename = "./log/slave_#_@.txt";
		filename = replace(filename, time_str, "#");
		filename = replace(filename, type, "@");

		ofstream outfile(filename.c_str(), ios::app);
		string str_con = time_str + " " + get_string_time("%T") + ":" + content;

		outfile << str_con << endl;
		outfile.close();

		if (this->is_log_show(type)) {
			cout << content << endl;
		}
		content = "";
	}
//
	void log2(string str1, string str2, string type) {
		string str = "";
		str += str1 + str2;
		log2(str, type);
	}
	void log2(string str1, string str2, string str3, string type) {

		string str = "";
		str += str1 + str2 + str3;
		log2(str, type);
	}
	void log2(string str1, string str2, string str3, string str4, string type) {

		string str = "";
		str += str1 + str2 + str3 + str4;
		log2(str, type);
	}
//
	void log2(string content, string type) {

		string time_str = get_string_time("%Y%m%d");

		string filename = "./log/slave_#_@.txt";
		filename = replace(filename, time_str, "#");
		filename = replace(filename, type, "@");

		ofstream outfile(filename.c_str(), ios::app);
		string str_con = time_str + " " + get_string_time("%T") + ":" + content;

		outfile << str_con << endl;
		outfile.close();

		if (this->is_log_show(type)) {
			cout << content << endl;
		}
	}
	void log2_test() {
		cout << is_log_show("task");
		this->log2("hi test", "task");
	}

	void log(string content) {
#ifdef DEBUG
		cout << content << endl;
#endif
		string time_str = get_string_time("%Y%m%d");
		//cout << time_str << endl;
		string filename = "./log/slave#.txt";
		filename = replace(filename, time_str, "#");

		ofstream outfile(filename.c_str(), ios::app);
		outfile << content << endl;
		outfile.close();
//#endif
	}
// %Y%m%d"
	string get_string_time(string formart) {

		struct tm *newtime;
		char tmpbuf[128];
		time_t lt1;
		time(&lt1);
		newtime = localtime(&lt1);
		strftime(tmpbuf, 128, formart.c_str(), newtime);
		string time_str = tmpbuf;
		return time_str;
	}
	void log_test() {
		//	log("111");
		this->config();
		log2("connect to ", "ip ", "port ", "socket");
	}
//=====================================
	void get_time_str(string& time_str) {
		struct tm *newtime;
		char tmpbuf[128];
		time_t lt1;
		time(&lt1);
		newtime = localtime(&lt1);
		strftime(tmpbuf, 128, "%Y%m%d_%H%M%S", newtime);
		time_str.append(tmpbuf);

		struct timeval tmp_time;
		gettimeofday(&tmp_time, 0);
		string str_usec = ld2str(tmp_time.tv_usec);
		time_str.append("_");
		time_str.append(str_usec);

	}
	void get_time_str_test() {
		string s2;
		get_time_str(s2);
		cout << s2 << endl;
	}
//==========================================

	string grab_page_filename(int index, string task_id) {
		string filename = "./download/" + task_id;
		return filename;
	}
//================
	bool is_html_end(char* buf, size_t count) {
		{
			while (1) {
				if (tail_with_feature(buf, count, "\r\n0\r\n\r\n")) {
					break;
				}
				if (tail_with_feature(buf, count, "</html>\r\n\r\n")) {
					break;
				}
				if (tail_with_feature(buf, count, "</HTML>\r\n\r\n")) {
					break;
				}
				if (tail_with_feature(buf, count, "</HTML>")) {
					break;
				}
				if (tail_with_feature(buf, count, "</HTML>\r\n")) {
					break;
				}
				if (tail_with_feature(buf, count, "</html>")) {
					break;
				}
				if (tail_with_feature(buf, count, "</html>\r\n")) {
					break;
				}
				if (tail_with_feature(buf, count, "</html>")) {
					break;
				}
				return 0;
			}
			return 1;
		}
	}
	void is_html_end_test() {
		string s2 = "</TD></TR></TABLE></BODY></html>\r\n";

		bool a1 = is_html_end((char *) s2.c_str(), s2.size());

		assert(a1 == true);
		cout << "</HTML>\r\n" << a1 << endl;
	}
//================
	void test_strlen_size_len() {
		//string s2 = "\a";
		string s2 = "123";
		cout << "size:" << s2.size() << "	strlen:" << strlen(s2.c_str())
				<< "	length:" << s2.length() << endl;

	}

//=======================

	string get_str_betwen_pages(string url) {
		string split_str = "\a\r\n";
		split_str += "url:" + url + "\r\n";
		return split_str;
	}
	void delete_self_test() {
		system("rm -f kf1304_slave");
	}
//
	int call_updata_shell(string url) {
		//cin >> result;
		string sh_str = "";
		//"wget -O kf1304_slave_1 # \n";
		//	sh_str += "ls \n";
		//sh_str += "cd ..\n";

		sh_str += "rm -f slave.tar.gz \n"; //删除原来的文件
		sh_str += "wget -O slave.tar.gz # \n";
		//sh_str += "mv kf1304_slave_1 kf1304_slave \n";
		//sh_str += "chmod 777 kf1304_slave \n";
		//	sh_str += "./kf1304_slave \n";
		sh_str = replace(sh_str, url, "#");
		sh_str = replace(sh_str, ":", "#"); //替换掉XX＃8080中的＃为:
		sh_str += "tar zxvf slave.tar.gz \n"; //解压
		sh_str += "cd Debug/ \n"; //编译
		sh_str += "./build.sh \n"; //

		log("./update.sh", sh_str, ios::trunc);
		//"chmod 777 kf1304_slave \n";
		system("chmod 777 update.sh");
		int result = system("./update.sh");
		exit(0); //退出程序！
		return result;
	}
	void call_updata_shell_test() {
		int result = call_updata_shell("www.csdn.net");
		cin >> result;
		cout << result << endl;
	}
//===================================
	void call_shell_test() {
		int result = system("./updata.sh");

		cin >> result;
		cout << result << endl;
	}
//
	void auto_restart_test() {
		//	int result = 1;
		//	cin >> result;
		//cout << result << endl;
		while (1) {
			this->log("./test", "yes i'm alive!");

		}
	}

	int trave_dir(char* path, int depth, char filename[256][256], int& len) {
		DIR *d; //声明一个句柄
		struct dirent *file; //readdir函数的返回值就存放在这个结构体中
		struct stat sb;

		if (!(d = opendir(path))) {
			printf("error opendir %s!!!/n", path);
			return -1;
		}
		while ((file = readdir(d)) != NULL) {
			//把当前目录.，上一级目录..及隐藏文件都去掉，避免死循环遍历目录
			if (strncmp(file->d_name, ".", 1) == 0)
				continue;
			strcpy(filename[len++], file->d_name); //保存遍历到的文件名
			//判断该文件是否是目录，及是否已搜索了三层，这里我定义只搜索了三层目录，太深就不搜了，省得搜出太多文件
			if (stat(file->d_name, &sb) >= 0 && S_ISDIR(sb.st_mode)
					&& depth <= 1) {
				trave_dir(file->d_name, depth + 1, filename, len);
			}
		}
		closedir(d);
		return 0;
	}
	int trave_dir_test() {
		char filename[256][256];
		int len = 0;

		int depth = 1;
		int i;
		trave_dir("./download/", depth, filename, len);
		for (i = 0; i < len; i++) {
			printf("%s\n", filename[i]);
		}
		printf("/n");
		return 0;
	}
//===================================
	bool trave_dir_into_vec(string path, vector<string>& fnames_evc) {
		DIR *d; //声明一个句柄
		struct dirent *file; //readdir函数的返回值就存放在这个结构体中
		if (!(d = opendir(path.c_str()))) {
			printf("error opendir %s./n", path.c_str());
			return 0;
		}
		while ((file = readdir(d)) != NULL) {
			//把当前目录.，上一级目录..及隐藏文件都去掉，避免死循环遍历目录
			if (strncmp(file->d_name, ".", 1) == 0)
				continue;
			//strcpy(filename[len++], file->d_name); //保存遍历到的文件名
			string fname = file->d_name;
			//过滤掉 包含.的文件
			bool filter1 = fname.find_first_of('.', 0) == string::npos;
			//过滤掉包含～的隐藏文件
			bool filter2 = fname.find_first_of('~', 0) == string::npos;
			if (filter1 && filter2) {
				fnames_evc.push_back(file->d_name);
			}
		}
		closedir(d);
		return 1;
	}

	void trave_dir_into_vec_test() {
		vector<string> fnames_evc;
		trave_dir_into_vec("./download/", fnames_evc);
		for (vector<string>::iterator it2 = fnames_evc.begin();
				it2 != fnames_evc.end(); ++it2) {
			std::cout << "   " << *it2 << endl;
		}

	}
//
	void read_file() {

	}
	void init_address(struct sockaddr_in *dest_addr, int sin_family, int port,
			in_addr_t ip) {
		bzero(dest_addr, sizeof(dest_addr));
		dest_addr->sin_family = sin_family;
		dest_addr->sin_port = htons(port);
		dest_addr->sin_addr.s_addr = ip;
		bzero(&(dest_addr->sin_zero), 8);
	}
//

	bool page_read_binary(string send_filename, char * page_buff,
			size_t& length) {
		//==============================
		//4.send content
		//==============================

		//read binary page and send. begin
		ifstream is_page(send_filename.c_str(), ios::binary);
		//char * page_buff;
		if (!is_page) {
			return false;
		}
		is_page.seekg(0, is_page.end);
		length = is_page.tellg();
		//char * page_buff; = is_page.tellg();
		is_page.seekg(0, is_page.beg);
		page_buff = (char *) malloc(length);
		is_page.read(page_buff, length);
		//read binary page and send. end
		ofstream os_page("./download/t3.tar.gz", ios::binary);
		os_page.write(page_buff, length);
		os_page.close();
		//
		//	free(page_buff);
		//}
		is_page.close();
		return 1;
	}
//not pass
	void page_read_binary_test() {
		string send_filename = "./download/t1.tar.gz";
		char * page_buff;
		size_t length;
		page_read_binary(send_filename, page_buff, length);
		cout << "length:" << length << endl;
		ofstream os_page("./download/t2.tar.gz", ios::binary);
		os_page.write(page_buff, length);
		os_page.close();
		//free(page_buff);
	}
//

//==========================================
	bool read_config(string fname, map<string, string>& config_map) {
		//
		FILE *fp;
		char *filep;
		string url;
		if ((fp = fopen(fname.c_str(), "r")) == NULL) {
			printf("open file %s error!!\n", fname.c_str());
		}
		//
		vector<string> *vec = new vector<string>();
		char read_buff[1024];
		while (1) {
			filep = fgets(read_buff, 1024, fp);
			if (filep == NULL) {
				break;
			}
			string s1 = read_buff;
			int f1 = s1.find_first_of('\n');
			string item = s1.substr(0, f1);
			vec->push_back(item);
		}
		fclose(fp);
		//
		vector<string>::iterator it1;
		for (it1 = vec->begin(); it1 != vec->end(); ++it1) {
			vector<string> vec2;
			split_by_split_char(*it1, &vec2, ':');
			string key = vec2.at(0);
			string value = vec2.at(1);
			(config_map)[key] = value;
		}
		return 1;
	}
//===rep special
	float available_disk_space() {
		struct statfs diskInfo;
		statfs("/", &diskInfo);
		unsigned long long blocksize = diskInfo.f_bsize; //每个block里包含的字节数
		unsigned long long availableDisk = diskInfo.f_bavail * blocksize; //可用空间大小
		unsigned int availableDisk2 = availableDisk >> 20;
		float availableDisk_ft = availableDisk2 / 1024.0;
		//cout << "availableDisk2=" << availableDisk_ft << endl;
		return availableDisk_ft;

	}
	void disk_space_test() {
		available_disk_space();
	}
//
//num to string
	string float2str(float i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	/////////////////////////////info_extract
	string read_all_file(string fname) {
		ifstream ifs;

		ifs.open(fname.c_str(), ios::in);
		int f_size = this->file_size(fname);
		char fcontent[f_size];
		ifs.read(fcontent, f_size);
		ifs.close();
		string res = fcontent;
		return res;
	}
	//==========================================
	string replace_infoextr(string str, string new_str, string special_char) {

		string::size_type pos(0);
		pos = str.find_first_of(special_char);
		if (pos == string::npos) {
			return str;
		}
		str = str.replace(pos, special_char.size(), "");
		return str.insert(pos, new_str);
	}
	string replace_all(string str, string new_str, string special_char) {
		string key1 = "";
		string key2 = "";
		key2 = str;
		do {
			key1 = key2;
			key2 = replace_infoextr(key1, new_str, special_char);
		} while (key1 != key2);
		return key2;
	}
	void replace_all_test() {
		string s1 = "    aa a aa  ";
		cout << replace_all(s1, "", " ") << endl;
	}
	void remove_space(string& str) {
		string buff(str);
		char space = ' ';
		str.assign(buff.begin() + buff.find_first_not_of(space),
				buff.begin() + buff.find_last_not_of(space) + 1);
	}
	//
	bool trave_dir_into_vec_ie(string path, vector<string>& fnames_evc) {
		DIR *d; //声明一个句柄
		struct dirent *file; //readdir函数的返回值就存放在这个结构体中
		if (!(d = opendir(path.c_str()))) {
			printf("error opendir %s./n", path.c_str());
			return 0;
		}
		while ((file = readdir(d)) != NULL) {
			//把当前目录.，上一级目录..及隐藏文件都去掉，避免死循环遍历目录
			if (strncmp(file->d_name, ".", 1) == 0)
				continue;
			//strcpy(filename[len++], file->d_name); //保存遍历到的文件名
			string fname = file->d_name;
			//过滤掉 包含.的文件
			//	bool filter1 = fname.find_first_of('.', 0) == string::npos;
			//过滤掉包含～的隐藏文件
			bool filter2 = fname.find_first_of('~', 0) == string::npos;
			if (filter2) {
				fnames_evc.push_back(file->d_name);
			}
		}
		closedir(d);
		return 1;
	}
	//

//
//==========================================
// . connect to server
	/*
	 for (vector<string>::iterator it2
	 = fnames_evc.begin();
	 it2 != fnames_evc.end(); ++it2) {
	 std::cout << it2->first << " => " << it2->second << '\n';
	 }

	 for (map<string, string>::iterator it2 = command_map.begin();
	 it2 != command_map.end(); ++it2) {
	 std::cout << it2->first << " => " << it2->second << '\n';
	 }


	 * */
//
}
;
}
#endif /* GLOBALHELPER_H_ */
