/*
 * slaver.h
 *
 *  Created on: 2013-4-1
 *      Author: root
 */

#ifndef SLAVER_H_
#define SLAVER_H_

//network
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>

#include "GlobalHelper.h"

#define REQUEST 1
#define SLEEP 2
#define UPDATE 3
#define GRABPAGE 4
#define STORE 5

#define DEBUG

using namespace std;

namespace poseidon {
struct storetask {
	string request_ip;
	int request_port;
	//int index;
	//string http_req;
	vector<string> fnames_evc;

};
struct grabtask {
	string request_ip;
	int request_port;
	int index;
	string http_req;
	string task_id;
	string url;

};
typedef struct task {
	int sleep_time;

	vector<string> task_id;

	//string task_id;
	//long int task_id;
	int slave_id;
	int cmd_id;

	string store_ip;
	int store_port;

	string request_ip;
	int request_port;

	string url_header; //no use
	string urls;

	vector<string> urls_vec;
	vector<string> urls_http_req;
	vector<string> urls_sites;
	map<string, string> response_cmd_map;

	int version;

};
class slaver {
private:
	int master_port;
	string master_ip;
//command 1 salve 1
	string cmd_req_model;

	GlobalHelper *gh;
protected:

public:
	int slave_id;
	int app_version;
	int last_task_status;
	string cmd_req_2send;
	slaver() {
		slave_id = 1;
		app_version = 1;
		master_port = 9000;
		this->master_ip = "192.168.75.128";
		cmd_req_model =
				"commd_id:1\r\nslave_id:#\r\nlast_task_status:^\r\napplication_version:@\r\n\f";
		gh = new GlobalHelper();
		last_task_status = 2; //2 means no last task
	}

	virtual ~slaver();
	bool requestTask(task* mytask, string& str_cmd);
	bool grabpage_work(task& mytask);
	bool grab_page(grabtask gtask);

	bool remoteStorePage(storetask s_task, string cmd, string send_content);

	void work() {
		task mytask; //commad
		string task_str;
		int i = 2;
		while (i) {
			memset(&mytask, 0, sizeof(mytask));
			//1 requestTask
			if (!requestTask(&mytask, task_str)) {
				cout << "Master is sleep.so i will sleep 2 seconds." << endl;
				sleep(5);
				continue;
			}
			//2 str2task
			gh->log("str2task:*" + task_str + "*");
			str2task(task_str, mytask);

			//3 hand_response
			gh->log("hand_response:" + gh->num2str(mytask.cmd_id));
			hand_response(mytask);
		}

	}

	void hand_response(task& mytask) {

		switch (mytask.cmd_id) {
		case SLEEP:
			cout << "Sleep :" << mytask.sleep_time << " secoonds." << endl;
			sleep(mytask.sleep_time); //sleep 1 second;
			break;

		case UPDATE:
			//update app begin
			//gh->call_updata_shell();
			//update app end
			gh->log("Updata version:" + mytask.version);
			//in real updata,the app_version will be write in code.
			this->app_version = mytask.version;
			break;

		case GRABPAGE:
			cout << "Grab page totals:" << mytask.urls_http_req.size() << endl;
			grabpage_work(mytask);
			break;

		case STORE:
			//	store_page_work();
			storetask s_task;
			s_task.request_ip = "192.168.75.128";
			s_task.request_port = 9001;
			store_page(s_task);
			break;
		}
	}
	void tar_pages_and_get_fnames_vec(vector<string>& fnames_evc) {
		//vector<string> fnames_evc;

		//1.tar
		//	string tar_fname = "./download/";

		gh->trave_dir_into_vec("download/", fnames_evc);

		string tar_download_sh_fname = "./script/tar_download.sh";
		ofstream outfile(tar_download_sh_fname.c_str(), ios::trunc);
		outfile << "cd  download" << endl;

		for (vector<string>::iterator it2 = fnames_evc.begin();
				it2 != fnames_evc.end(); ++it2) {
			string it_tar_cmd = "tar -zcvf  " + *it2 + ".tar.gz " + *it2;
			//tar -zcvf 1.tar.gz ../download/1
			outfile << it_tar_cmd << endl;
		}
		outfile.close();
		system("chmod 777 script/tar_download.sh");
		system("./script/tar_download.sh");

	}
	void store_page(storetask s_task) {
		//string tar_fname = "./download/";
		//2. read file in menu
		//vector<string> fnames_evc;
		tar_pages_and_get_fnames_vec(s_task.fnames_evc);

		//3. connect to server
		//begin
		string send_filename;
		for (vector<string>::iterator it2 = s_task.fnames_evc.begin();
				it2 != s_task.fnames_evc.end(); ++it2) {
			string cmd_str;
			map<string, string> cmd_map;

			cmd_map["commd_id"] = "5";
			cmd_map["slave_id"] = this->slave_id;
			cmd_map["task_id"] = *it2;
			cmd_map["content_size"] = "#";

			for (map<string, string>::iterator it3 = cmd_map.begin();
					it3 != cmd_map.end(); ++it3) {
				cmd_str.append(it3->first + ":" + it3->second + "\r\n");
			}
			cmd_str.append("\f");

			send_filename = "./download/" + *it2 + ".tar.gz";

			//is_page << send_content;
			//send_filename.append("\f");
			gh->log("enter remoteStorePage");
			this->remoteStorePage(s_task, cmd_str, send_filename);

		}
		//end

	}
	void grab_page_log_time(grabtask gt) {

		string http_req = gt.http_req; //= mytask.urls_http_req.at(index);
		string request_ip = gt.request_ip; // = dest_ip;
		int request_port = gt.request_port;
		int index = gt.index;
		string task_id = gt.task_id; // = mytask.task_id.at(0);
		string url = gt.url; //mytask.urls.at(index);

		gh->timing_begin();
		grab_page(gt);
		gh->timing_end();

		string filename = gh->grab_page_filename(index, task_id);

		string info = "" + gh->num2str(index);
		info += " " + gh->cast_time();
		info += " " + gh->num2str(gh->file_size(filename));
		info += "\n";

		string filename2 = "log/log";
		filename2 += request_ip;
		filename2 += "_";
		filename2 += request_port;
		filename2 += ".log";

		gh->log(filename2, info);

	}
	void urls_str_to_http_reqs(task& mytask, string urls) {
		vector<string> str_vec;
		gh->split(urls, "#", str_vec);

		vector<string>::iterator url_it;
		string req;
		//urls_http_req
		for (url_it = str_vec.begin(); url_it != str_vec.end(); url_it++) {
			string str1 = gh->convert_url_to_http_req(*url_it);
			mytask.urls_http_req.push_back(str1);
		}
		//urls_sites
		for (url_it = str_vec.begin(); url_it != str_vec.end(); url_it++) {
			string str1 = gh->get_sitename(*url_it);
			mytask.urls_sites.push_back(str1);
		}
		//urls
		for (url_it = str_vec.begin(); url_it != str_vec.end(); url_it++) {
			//		string str1 = gh->convert_url_to_urls(*url_it);
			mytask.urls_vec.push_back(*url_it);
		}
	}
//
	map<string, string> dns_map;
	bool lookup_ip(string sitename, string& dest_ip) {
//1 check map
		dest_ip = dns_map[sitename];
		if (dest_ip != "") {
			cout << "get" << sitename << " from cache!" << endl;
			return true;
		}

		//2 lookup ip
		if (get_ip_by_name(sitename, dest_ip)) {
			dns_map[sitename] = dest_ip;
			return true;
		} else {
// not find
			return false;
		}

	}
	bool get_ip_by_name(string hostname, string& ip) {
		struct hostent *pt;
		char **ptr;
		char str[INET_ADDRSTRLEN];

		if ((pt = gethostbyname(hostname.c_str())) == NULL) {
			cout << "gethostname " << hostname << " error\n" << endl;

		}

		switch (pt->h_addrtype) {
		case AF_INET:
			ptr = pt->h_addr_list;
			for (; *ptr != NULL; ptr++) {
				ip = inet_ntop(pt->h_addrtype, *ptr, str, sizeof(str));
				//printf("IP addrss: %s\n", ip);
			}
			break;
		default:
			//printf("unknown address type");
			return 0;
			break;
		}

		return 1;
	}
	//
	string& replace_all(string& str, const string& old_value,
			const string& new_value);
	string getHttpHeader(string url_header, vector<string> url_body, int id);
	//help

	void init_address(struct sockaddr_in *dest_addr, int sin_family, int port,
			string ip) {
		bzero(dest_addr, sizeof(dest_addr));
		dest_addr->sin_family = sin_family;
		dest_addr->sin_port = htons(port);
		dest_addr->sin_addr.s_addr = inet_addr(ip.c_str());
		bzero(&(dest_addr->sin_zero), 8);
	}
	//
	void str2task(string response_command, task& mytask) {
		map<string, string> response_cmd_map;

		gh->command_str_to_map(response_command, &response_cmd_map);

		mytask.cmd_id = atoi(response_cmd_map["commd_id"].c_str());
		mytask.response_cmd_map = response_cmd_map;
		mytask.sleep_time = atoi(response_cmd_map["time"].c_str());
		mytask.slave_id = atoi(response_cmd_map["slave_id"].c_str());
		mytask.version = atoi(response_cmd_map["version"].c_str());
		//  can't read urls if we don't have.but it work when it's
		//others,eg it's "a12"
		//it's a c++ string copy on wirite technology!

		//cout << mytask.response_cmd_map["task_id"] << endl;

		//		cout << "mytask.task_id_str:" << mytask.task_id << endl;

		if (mytask.cmd_id == 4) {

			string urls = mytask.response_cmd_map["urls"];
			urls_str_to_http_reqs(mytask, urls);

			string task_id = mytask.response_cmd_map["task_id"];
			cout << task_id << endl;
			mytask.task_id.push_back(task_id);

			cout << mytask.task_id.at(0) << endl;
			// covert_fuck_bug_string(mytask.task_id, task_id);
			//	mytask.task_id = mytask.response_cmd_map["task_id"];
		}

	}

	void prepare_req_cmd() {
		string cmd_req_str = this->cmd_req_model;

		//	cout << "*app_version:" << app_version << endl;
		cmd_req_str = gh->replace(cmd_req_str, gh->num2str(this->slave_id),
				"#");
		cmd_req_str = gh->replace(cmd_req_str, gh->num2str(this->app_version),
				"@");
		cmd_req_str = gh->replace(cmd_req_str,
				gh->num2str(this->last_task_status), "^");
		this->cmd_req_2send = cmd_req_str;
	}
//

//

};

} /* namespace poseidon */
#endif /* SLAVER_H_ */
