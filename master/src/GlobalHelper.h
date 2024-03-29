/*
 * GlobalHelper.h
 *
 *  Created on: 2013-4-4
 *      Author: root
 */

#ifndef GLOBALHELPER_H_
#define GLOBALHELPER_H_
//current project

//network
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
//c
#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
//c++
#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>
#include "fstream"
//c++ 容器
#include <algorithm>    // std::find_if
#include <vector>       // std::vector
#include <map>

using namespace std;
namespace poseidon {

class GlobalHelper {
private:

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
protected:

public:

	int TASK_BUF_SIZE; //1448
	string MASTER_CONF;
	string WEBSITE_CONF;
	GlobalHelper() {
		TASK_BUF_SIZE = 1448;
		MASTER_CONF = "./conf/master.conf";
		WEBSITE_CONF = "./conf/website_grabing.log";
		config();
	}
	map<string, string> config_show_map;
	void config() {
		read_config("./conf/log.conf", config_show_map);

	}
	bool is_log_show(string type) {
		return (config_show_map.find(type) != config_show_map.end());

	}
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
	//
	void log(string filename, string content) {
		ofstream outfile(filename.c_str(), ios::app);
		outfile << content;
		outfile.close();
	}
	void log(string filename, string content, ios::openmode t1) {
		ofstream outfile(filename.c_str(), t1);
		outfile << content;
		outfile.close();
	}
	//
	void log2(string c1, string c2, string type) {
		string str = c1 + c2;
		log2(str, type);

	}
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
		log2("content", "socket");
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
	void time_test2() {
		timing_begin();

		timing_end();
		cout << "cast_time" << cast_time() << "秒" << endl;
	}
	//num to string
	string num2str(int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string float2str(float i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string ld2str(long int i) {
		stringstream ss;
		ss << i;
		return ss.str();
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

	//是否以给定字符串结尾
	bool tail_with_feature(char* buf, int count, const char* feature) {

		int feature_len = strlen(feature);

		if (count < feature_len || 0 >= feature_len) {
			return false;
		}
		for (int j = 0; j < feature_len; ++j) {
			if (buf[count - j - 1] != feature[feature_len - j - 1]) {
				return false;
			}
		}
		return true;
	}

	//
	void init_address(struct sockaddr_in *dest_addr, int sin_family, int port,
			in_addr_t ip) {
		bzero(dest_addr, sizeof(dest_addr));
		dest_addr->sin_family = sin_family;
		dest_addr->sin_port = htons(port);
		dest_addr->sin_addr.s_addr = ip;
		bzero(&(dest_addr->sin_zero), 8);
	}

	//按行分割
	void split_line(string in, vector<string> *vec) {
		istringstream ss(in);
		string t;
		while (ss >> t) {
			//	cout << t << endl;
			vec->push_back(t);
		}
	}
	//以 为分隔符 只能取前2个
	//str=slave_id:1
	void split_by_split_char(string str, vector<string> *vec, char split_char) {
		string::size_type pos = 0;
		pos = str.find_first_of(':', pos);
		vec->push_back(str.substr(0, pos));
		vec->push_back(str.substr(pos + 1, str.size()));
	}
	bool split_line_test() {
		vector<string> *vec = new vector<string>();
		vector<string> *vec2 = new vector<string>();
		string command1 = "commd_id:1\r\n"
				"slave_id:1\r\n"
				"application_version:1\r\n"
				"\f";
		split_line(command1, vec);
		split_by_split_char(vec->at(0), vec2, ':');
		int commd_id = atoi(vec2->at(1).c_str());
		cout << "split_line_test:" << (commd_id == 1) << endl;
		return commd_id == 1;

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
			(*command_map)[key] = value;
		}
	}
	//以"\r "为分隔符
	bool command_str_to_map_test() {

		map<string, string> command_map;
		string command1 = "commd_id:1\r\n"
				"slave_id:1\r\n"
				"application_version:1\r\n"
				"\f";

		command_str_to_map(command1, &command_map);

		for (map<string, string>::iterator it2 = command_map.begin();
				it2 != command_map.end(); ++it2) {
			std::cout << it2->first << " => " << it2->second << '\n';
		}
		//cout << "split_line_test:" << (commd_id == 1) << endl;
//==========================================
		/* */

		return 1;
	}

	//=====================================
	//"%Y%m%d%H%M%S"
	string get_time_str(string timeformart) {
		string time_str;
		struct tm *newtime;
		char tmpbuf[128];
		time_t lt1;
		time(&lt1);
		newtime = localtime(&lt1);
		strftime(tmpbuf, 128, timeformart.c_str(), newtime);
		time_str.append(tmpbuf);
		return time_str;
	}
	string get_time_str() {
		string time_str;

		struct tm *newtime;
		char tmpbuf[128];
		time_t lt1;
		time(&lt1);
		newtime = localtime(&lt1);
		strftime(tmpbuf, 128, "%Y%m%d%H%M%S", newtime);
		time_str.append(tmpbuf);

		struct timeval tmp_time;
		gettimeofday(&tmp_time, 0);
		string str_usec = ld2str(tmp_time.tv_usec);
//		time_str.append("_");
		time_str.append(str_usec);
		return time_str;
	}
	void get_time_str_test() {
		string s2;

		cout << get_time_str() << endl;
	}
	//==========================================
	bool read_config(string fname, map<string, string>& config_map) {
		//
		FILE *fp;
		char *filep;
		string url;
		if ((fp = fopen(fname.c_str(), "r")) == NULL) {
			printf("open file %s error!!\n", fname.c_str());
			perror("open file  error!!\n");
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
	//==========================================
	string replace_str(string str, string new_str, string special_char) {

		string::size_type pos(0);
		pos = str.find_first_of(special_char);
		string::size_type len = special_char.size();
		str = str.replace(pos, len, "");
		return str.insert(pos, new_str);
	}
	//
	//[20130412-12:03] change replace size to be special_char.size()
	//
	string replace(string str, string new_str, string special_char) {

		string::size_type pos(0);
		pos = str.find_first_of(special_char);
		if (pos == string::npos) {
			return str;
		}
		str = str.replace(pos, special_char.size(), "");
		return str.insert(pos, new_str);
	}
	void replace_test() {
		string s1 = "GET http://www.1.com/question/#.html Host:@\r\n";
		string s2 = replace(s1, "123", "#");
		cout << "s2:" << s2 << endl;
		string s3 = replace(s2, "345", "@");
		cout << "s3:" << s3 << endl;

		string s4 = replace(s2, "789", "http://www.1.com");
		cout << "s4:" << s4 << endl;

	}
	//
	void show_config(map<string, string> config_map1, string title) {
		std::cout << "==========" << title << "==========" << endl;
		for (map<string, string>::iterator it2 = config_map1.begin();
				it2 != config_map1.end(); ++it2) {
			std::cout << it2->first << " => " << it2->second << endl;
		}
		std::cout << "==========" << title << "==========" << endl;
	}

//
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
};

}
#endif /* GLOBALHELPER_H_ */
