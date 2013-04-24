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
#include "assert.h"
//disk
#include <sys/statfs.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

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
	string REP_CONF;
	string WEBSITE_CONF;
	string MASTER_IP;
	int MASTER_PORT;
	string MASTER_CONF;
	GlobalHelper() {

		TASK_BUF_SIZE = 1448;
		REP_CONF = "./conf/repertory.conf";
		MASTER_CONF = "./conf/master.conf";
		MASTER_IP = "192.168.75.128";
		MASTER_PORT = 9001;
		config();
	}
	map<string, string> config_show_map;
	void config() {
		string confname = "./conf/log.conf";
		read_config(confname, config_show_map);
		this->show_config(config_show_map, confname);
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
	//.
	bool is_log_to_file() {
		string is_log = config_show_map["log"];
		return (is_log == "1");
	}
	void log2(string content, string type) {

		if (is_log_to_file()) {
			string time_str = get_string_time("%Y%m%d");
			string filename = "./log/slave_#_@.txt";
			filename = replace(filename, time_str, "#");
			filename = replace(filename, type, "@");
			ofstream outfile(filename.c_str(), ios::app);
			string str_con = time_str + " " + get_string_time("%T") + ":"
					+ content;
			outfile << str_con << endl;
			outfile.close();
		}

		if (this->is_log_show(type)) {
			cout << content << endl;
		}
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
		usleep(1);
		timing_end();
		cout << "cast_time" << cast_time() << "秒" << endl;
	}
	//num to string
	string num2str(int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string ld2str(long int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}
	//num to string
	string float2str(float i) {
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
	//unit is byte
	long file_size_long(string filename) {
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
	string replace(string str, string new_str, string special_char) {

		string::size_type pos(0);
		pos = str.find_first_of(special_char);
		str = str.replace(pos, 1, "");
		return str.insert(pos, new_str);
	}
	//==========================================
	void log(string content) {

		cout << content << endl;

//#ifdef LOG
		struct tm *newtime;
		char tmpbuf[128];
		time_t lt1;
		time(&lt1);
		newtime = localtime(&lt1);
		strftime(tmpbuf, 128, "%Y%m%d", newtime);
		string time_str = tmpbuf;
		//cout << time_str << endl;
		string filename = "./log/slave#.txt";
		filename = replace(filename, time_str, "#");

		ofstream outfile(filename.c_str(), ios::app);
		outfile << content << endl;
		outfile.close();
//#endif
	}
	void log(string filename, string content, ios::openmode t1) {
		ofstream outfile(filename.c_str(), t1);
		outfile << content;
		outfile.close();
	}
	//===rep special
	float available_disk_space() {
		struct statfs diskInfo;
		statfs("/", &diskInfo);
		unsigned long long blocksize = diskInfo.f_bsize; //每个block里包含的字节数
		unsigned long long availableDisk = diskInfo.f_bavail * blocksize; //可用空间大小
		unsigned int availableDisk2 = availableDisk >> 20;
		float availableDisk_ft = availableDisk2 / 1024.0;
		cout << "availableDisk2=" << availableDisk_ft << endl;
		return availableDisk_ft;

	}
	void disk_space_test() {
		available_disk_space();
	}
	//
	//==================================
	//无限分割入VEC
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
	//================================

	void split_file(string send_filename) {
		string fname;
		string char_str;

		//1 read file
		ifstream is_page(send_filename.c_str(), ios::in);

		long length = this->file_size_long(send_filename);

		//page_buff = (char *) malloc(length);
		char page_buff[length];
		is_page.read(page_buff, length);
		//2 split
		vector<string> str_vec;
		string s1 = page_buff;
		const string s2 = "\a";

		split(s1, s2, str_vec);
		//3 output
		int i = 0;
		for (vector<string>::iterator it_str = str_vec.begin();
				it_str != str_vec.end(); it_str++, i++) {
			string fname = send_filename + "_" + this->num2str(i) + ".html";
			if ((*it_str).size() < 2) {
				continue;
			}
			this->log(fname, *it_str, ios::trunc);
		}

	}
	void split_file_test2() {
		string page_fname = "./pages_gzip/201304201507065173";
		split_file(page_fname);
	}
	//
	//===================================
	//找出以tar.gz结尾的
	bool trave_dir_into_vec_rep(string path, vector<string>& fnames_evc) {
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

			string fname = file->d_name;
			//过滤掉包含～的隐藏文件
			bool filter2 = fname.find_first_of('~', 0) == string::npos;
			bool filter3 = this->tail_with_feature((char *) (fname.c_str()),
					fname.size(), ".tar.gz");
			if (filter3 && filter2) {
				fnames_evc.push_back(file->d_name);
			}
		}
		closedir(d);
		return 1;
	}
	void trave_dir_into_vec_test() {
		vector<string> fnames_evc;
		trave_dir_into_vec_rep("./download/", fnames_evc);
		for (vector<string>::iterator it2 = fnames_evc.begin();
				it2 != fnames_evc.end(); ++it2) {
			std::cout << "   " << *it2 << endl;
		}

	}

	//
	void split_file_test() {
		//1 get file_names
		vector<string> fnames_evc;
		string path = "./pages_gzip/";
		trave_dir_into_vec_rep(path, fnames_evc);
		//2 tar uncompress
		for (vector<string>::iterator it2 = fnames_evc.begin();
				it2 != fnames_evc.end(); ++it2) {
			//	std::cout << "   " << *it2 << endl;
			string cmd1 = "tar -zxvf  ";
			//cmd1 += path + *it2;
			string gzip_fname = path + *it2;
			string page_fname = "";
			page_fname = path + (*it2).substr(0, (*it2).find('.', 0));
			cmd1 += gzip_fname;
			cmd1 += " -C " + path;
			log2(cmd1, "shell");
			system(cmd1.c_str());

			//3 split and store
			split_file(page_fname);

			//4 delete uncompressed file
			string cmd2 = "rm -f  ";
			cmd2 += page_fname;
			log2(cmd2, "shell");
			system(cmd2.c_str()); //rm -f fname

		}

	}
	//
	//
	void show_config(map<string, string> config_map1, string title) {
		std::cout << "==========" << title << "==========" << endl;
		for (map<string, string>::iterator it2 = config_map1.begin();
				it2 != config_map1.end(); ++it2) {
			std::cout << it2->first << " => " << it2->second << endl;
		}
		std::cout << "==========" << title << "==========" << endl;
	}
};

}
#endif /* GLOBALHELPER_H_ */
