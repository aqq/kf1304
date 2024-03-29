//============================================================================
// Name        : t3.cpp
// Author      : andrew
// Version     :
// Copyright   : newplayer
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
//disk
#include <sys/statfs.h>
using namespace std;

//是否以给定字符串结尾
bool tail_with_feature(const char* buf, size_t count, const char* feature) {

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
void log(string filename, string content) {
	ofstream outfile(filename.c_str(), ios::app);
	outfile << content << endl;
	outfile.close();
}

void store_site_url2(string sub_item2, string url, string fname) {
	string site_name;
	site_name = "Webpub";
	if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
			site_name.c_str())) {
		log(fname + "_" + site_name, url);
		return;
	}
	site_name = "tradehtml";
	if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
			site_name.c_str())) {
		log(fname + "_" + site_name, url);
		return;
	}
	site_name = "m";
	if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
			site_name.c_str())) {
		log(fname + "_" + site_name, url);
		return;
	}
	site_name = "others";
	log(fname + "_" + site_name, url);
}
void store_site_url(string sub_item2, string url) {
	string site_name;
	site_name = "1688.com";
	if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
			site_name.c_str())) {
		log(site_name, url);
		return;
	}
	site_name = "114chn.com";
	if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
			site_name.c_str())) {
		log(site_name, url);
		return;
	}
	site_name = "hc360.com";
	if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
			site_name.c_str())) {
		log(site_name, url);
		return;
	}

	log("bad", url);
}

string replace_str(string str, string new_str, string special_char) {

	string::size_type pos(0);
	pos = str.find_first_of(special_char);
	str = str.replace(pos, special_char.size(), "");
	return str.insert(pos, new_str);
}
//splite
int main_split(int argc, char *argv[]) {
	cout << argc << endl;

	string fname = "to.cjt"; // argv[1];
//
	FILE *fp;
	char *filep;
	string url;
	if ((fp = fopen(fname.c_str(), "r")) == NULL) {
		printf("open file %s error!!\n", fname.c_str());
	}
//
	char read_buff[1024];
	while (1) {
		filep = fgets(read_buff, 1024, fp);
		if (filep == NULL) {
			break;
		}

		string url = read_buff;
		//cout << url << endl;
		string item = url.substr(0, url.find_first_of('\n'));
		cout << item << endl;
		string sub_item = replace_str(item, "", "http://");
		cout << sub_item << endl;
		string sub_item2 = sub_item.substr(0, sub_item.find_first_of("/", 0));
		cout << sub_item2 << endl;
//
		store_site_url(sub_item2, url.substr(0, url.find(' ', 0)));

	}
	fclose(fp);
	return 1;
}
void store_str(string sub_item2, string url, vector<string> key_vec) {
	string site_name;
//	site_name = "Accept";
	vector<string>::iterator str_it = key_vec.begin();

	for (; str_it != key_vec.end(); str_it++) {
		site_name = *str_it;
		if (tail_with_feature(sub_item2.c_str(), sub_item2.size(),
				site_name.c_str())) {
			log(site_name, url);
			return;
		}
	}

}
//
int main_split_http_req(int argc, char *argv[]) {
	cout << argc << endl;
	cout << "usage: filename   " << endl;
	string fname = "http_req_record.log"; // argv[1];
//
	FILE *fp;
	char *filep;
	string url;
	if ((fp = fopen(fname.c_str(), "r")) == NULL) {
		printf("open file %s error!!\n", fname.c_str());
	}
//
	char read_buff[1024];
	while (1) {
		filep = fgets(read_buff, 1024, fp);
		if (filep == NULL) {
			break;
		}

		string url = read_buff;
		//cout << url << endl;
		string sub_item = url.substr(0, url.find_first_of('\n'));
		//	cout << item << endl;
		//	string sub_item = replace_str(item, "", "http://");
		//	cout << sub_item << endl;
		if (sub_item.find(':', 0) == string::npos) {
			continue;
		}
		string sub_item2 = sub_item.substr(0, sub_item.find_first_of(":", 0));
		//	cout << sub_item2 << endl;
		vector<string> key_vec;
		key_vec.push_back("Accept");
		key_vec.push_back("Accept-Language");
		key_vec.push_back("Accept-Charset");
		key_vec.push_back("User-Agent");
		string v1 = url.substr(url.find(':', 0) + 1, url.find('\n', 0) - 1);
//		cout <<"@" <<v1 << "@"<<endl;
		store_str(sub_item2, v1, key_vec);

	}
	fclose(fp);
	return 1;
}
//
int main(int argc, char *argv[]) {

	if (argc != 2) {
		cout << "usage: main {filename}" << endl;
		return 1;
	}
	string fname = argv[1]; // argv[1];
//
	FILE *fp;
	char *filep;
	string url;
	if ((fp = fopen(fname.c_str(), "r")) == NULL) {
		printf("open file %s error!!\n", fname.c_str());
	}
//
	char read_buff[1024];
	while (1) {
		filep = fgets(read_buff, 1024, fp);
		if (filep == NULL) {
			break;
		}

		string url = read_buff;
		//	cout << url << endl; //http://www.1.com/1
		string item = url.substr(0, url.find_first_of('\n'));
		string sub_item = replace_str(item, "", "http://");
		//cout << sub_item << endl;

		string key_words = "";
		//CASE1 and CASSE 2:	 www.1.com/Webpub/   yw.114chn.com/tradehtml/
		size_t it = sub_item.find_first_of("/", 0);
		if (it == string::npos) {
			continue;
		}
		//	cout << it << endl;

		size_t it2 = sub_item.find_first_of("/", it + 1);
//		cout << it2 << endl;
		if (it2 == string::npos) {
			continue;
		}

		key_words = sub_item.substr(it + 1, it2 - it - 1);
		cout << key_words << endl;
		//CASE3:tradehtml
//luzhou.114chn.com/m/web/shop/Index.aspx?shopid=5105001303200001

		store_site_url2(key_words, item, fname);
		//cout<<endl;

	}
	fclose(fp);
	return 1;
}
