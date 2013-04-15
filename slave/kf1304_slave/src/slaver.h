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

#include "errno.h"
#include "GlobalHelper.h"

#define REQUEST 1
#define SLEEP 2
#define UPDATE 3
#define GRABPAGE 4
#define STORE 7

#define DEBUG

using namespace std;

namespace poseidon {
const string s_moudle = "moudle";
const string s_socket = "socket";
const string s_normal = "normal";
const string s_sql = "sql";
const string s_work = "task";

const string slave_private_conf = "./conf/slave_private.conf";
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
struct req_task {
	int sleep_time;

	vector<string> task_id;

	int slave_id;
	int cmd_id;

	string store_ip;
	int store_port;

	string request_ip;
	int request_port;

	string urls;

	vector<string> urls_vec;
	vector<string> urls_http_req;
	vector<string> urls_sites;
	map<string, string> response_cmd_map;
	vector<string> new_version_url;
	int version;

};
class slaver {
private:

	GlobalHelper *gh;
	int grab_interval;//毫秒
protected:

public:
	vector<string> store_ip;
	int store_port;
	int master_port;
	vector<string> master_ip;
	int slave_id;
	int app_version;
	string cmd_req_2send;

	//store
	int sleep_time;
	int loop_try_time_in_store;

	//feed back
	int last_task_status;
	int last_cmd_id;
	slaver() {
		gh = new GlobalHelper();
		sleep_time = 1;
		last_task_status = -1; //-1 means no last task
		this->last_cmd_id = -1;
		loop_try_time_in_store = 5;
		gh->log2("//====Start", s_moudle);
		gh->log2("config().", s_moudle);
		config();

		show();
	}
	void show() {
		gh->log2("Slave " + gh->num2str(slave_id) + " wake up.", "normal");
		gh->log2("app_version:" + gh->num2str(app_version), "normal");
	}
	void config() {
		map<string, string> config_map1;
		gh->read_config(gh->SLAVE_CONF, config_map1);
		this->master_port = atoi((config_map1["master_port"]).c_str());
		this->master_ip.push_back(config_map1["master_ip"]);

		this->app_version = atoi((config_map1["app_version"]).c_str());
		this->store_ip.push_back(config_map1["store_ip"]);
		this->store_port = atoi((config_map1["store_port"]).c_str());
		this->grab_interval = atoi((config_map1["grab_interval"]).c_str());

		map<string, string> config_map2;
		gh->read_config(slave_private_conf, config_map2);
		this->slave_id = atoi((config_map2["slave_id"]).c_str());

	}

	virtual ~slaver();
	bool request_task(req_task* mytask, string& str_cmd);
	bool grabpage_work(req_task& mytask);
	bool grab_page(grabtask gtask);

	bool remoteStorePage(storetask s_task, string cmd, string send_content);

	void work() {
		req_task mytask;
		string task_str;
		int i = 2;

		while (i) {
			memset(&mytask, 0, sizeof(mytask));

			//1 requestTask
			gh->log2("requestTask().", s_moudle);

			if (!request_task(&mytask, task_str)) {
				cout << "Master is sleeping,so i will sleep 5 seconds." << endl;
				sleep(5);
				continue;
			}

			//2 str2task
			gh->log2("str2task().", s_moudle);
			gh->log2("str2task:" + task_str, s_normal);
			str2task(task_str, mytask);

			//3 hand_response
			gh->log2("hand_task().", s_moudle);
			gh->log2("hand_response:" + gh->num2str(mytask.cmd_id), s_normal);
			hand_task(mytask);
		}

	}

	bool hand_task(req_task& mytask) {
		bool is_task_finished = 1;
		this->last_cmd_id = mytask.cmd_id;
		switch (mytask.cmd_id) {
		case SLEEP:
			cout << "Sleep :" << mytask.sleep_time << " secoonds." << endl;
			sleep(mytask.sleep_time); //sleep 1 second;
			this->last_task_status = 1;
			break;

		case UPDATE:
			//update app begin
			gh->call_updata_shell(mytask.new_version_url[0]);
			//update app end
			gh->log("Updata version:" + mytask.version);
			//in real updata,the app_version will be write in code.
			this->app_version = mytask.version;
			this->last_task_status = 0;
			break;

		case GRABPAGE:
			gh->tmp_log_str += "Grab page totals:" // << mytask.urls_http_req.size() << endl;
			+ gh->size_t2str(mytask.urls_http_req.size());
			gh->log3(gh->tmp_log_str, s_work);
			is_task_finished = grabpage_work(mytask);
			this->last_task_status = is_task_finished;

			break;

		case STORE:
			//	store_page_work();
			storetask s_task;
			s_task.request_ip = this->store_ip[0]; //"192.168.75.128";
			s_task.request_port = this->store_port; //9001;
			is_task_finished = store_page(s_task);
			this->last_task_status = is_task_finished;
			break;
		}
		return is_task_finished;
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
	bool store_page(storetask s_task) {

		//1. read file in menu
		//vector<string> fnames_evc;
		tar_pages_and_get_fnames_vec(s_task.fnames_evc);

		//2 foreach pages and send  to rep
		string send_filename;
		for (vector<string>::iterator it2 = s_task.fnames_evc.begin();
				it2 != s_task.fnames_evc.end(); ++it2) {
			// 3 init cmd
			string fname = *it2;
			string cmd_str;
			map<string, string> cmd_map;
			cmd_map["commd_id"] = "5";
			cmd_map["slave_id"] = this->slave_id;
			cmd_map["task_id"] = fname;
			cmd_map["content_size"] = "#";
			int loop_store_total = 0; //this->loop_store_time;

			for (map<string, string>::iterator it3 = cmd_map.begin();
					it3 != cmd_map.end(); ++it3) {
				cmd_str.append(it3->first + ":" + it3->second + "\r\n");
			}
			cmd_str.append("\f");

			// 4.b
			send_filename = "./download/" + fname + ".tar.gz";

			string s1 = "enter remoteStorePage with " + send_filename;
			gh->log2(s1, s_moudle);
			gh->log2(s1, s_work);
			bool is_store_ok = false;
			while (!is_store_ok) {
				is_store_ok = this->remoteStorePage(s_task, cmd_str,
						send_filename);

				if (is_store_ok) {
					this->sleep_time = 1;
					//delete file
					string del_cmd_str = "";
					del_cmd_str = "rm download/" + fname + " -f";
					gh->log2(del_cmd_str, "shell");
					system(del_cmd_str.c_str());

					del_cmd_str = "rm download/" + fname + ".tar.gz -f";
					gh->log2(del_cmd_str, "shell");
					system(del_cmd_str.c_str());
					break;
				} else {
					//
					if (this->sleep_time < 500) {
						this->sleep_time = this->sleep_time * 2;
					}
					string s1 = "can't store to rep ,i will wait seconds:";
					s1 += gh->num2str(this->sleep_time);
					gh->log2(s1, s_work);
					sleep(this->sleep_time);
					loop_store_total++;
					if (loop_store_total > this->loop_try_time_in_store) {
						return false;
					}
					//
				}

			} //end store one

		} //end store all
		return true;
	}
	bool grab_page_log_time(grabtask gt) {
		bool isok = false;
		string http_req = gt.http_req; //= mytask.urls_http_req.at(index);
		string request_ip = gt.request_ip; // = dest_ip;
		int request_port = gt.request_port;
		int index = gt.index;
		string task_id = gt.task_id; // = mytask.task_id.at(0);
		string url = gt.url; //mytask.urls.at(index);

		gh->timing_begin();
		isok = grab_page(gt);
		gh->timing_end();

		string filename = gh->grab_page_filename(index, task_id);

		string info = "" + gh->num2str(index);
		info += " " + gh->cast_time();
		//info += " " + gh->num2str(gh->file_size(filename));
		info += "\n";

		string filename2 = "log/cast_time_";
		filename2 += request_ip;
		filename2 += "_";
		filename2 += gh->num2str(request_port);
		filename2 += ".log";

		gh->log(filename2, info);
		return isok;

	}
	void urls_str_to_http_reqs(req_task& mytask, string urls) {
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
	void str2task(string response_command, req_task& mytask) {
		map<string, string> response_cmd_map;

		gh->command_str_to_map(response_command, &response_cmd_map);

		mytask.cmd_id = atoi(response_cmd_map["commd_id"].c_str());
		mytask.response_cmd_map = response_cmd_map;
		mytask.sleep_time = atoi(response_cmd_map["time"].c_str());
		mytask.slave_id = atoi(response_cmd_map["slave_id"].c_str());
		mytask.version = atoi(response_cmd_map["version"].c_str());

		mytask.new_version_url.push_back(
				response_cmd_map["new_version_url"].c_str());
		if (mytask.cmd_id == 7) {
			this->store_ip.push_back(response_cmd_map["store_ip"]);
			this->store_port = atoi(response_cmd_map["store_port"].c_str());
		} else if (mytask.cmd_id == 4) {
			string urls = mytask.response_cmd_map["urls"];
			urls_str_to_http_reqs(mytask, urls);
			string task_id = mytask.response_cmd_map["task_id"];
			mytask.task_id.push_back(task_id);
		}

	}

	string init_request_cmd_str() {
		//init_map
		map<string, string> cmd_map;
		cmd_map["commd_id"] = "1";
		cmd_map["slave_id"] = gh->num2str(this->slave_id);
		cmd_map["application_version"] = gh->num2str(this->app_version);
		cmd_map["last_task_status"] = gh->num2str(this->last_task_status);
		cmd_map["last_cmd_id"] = gh->num2str(this->last_cmd_id);
		cmd_map["available_disk_space"] = gh->float2str(
				gh->available_disk_space());
		//init str
		string cmd_str = "";
		for (map<string, string>::iterator it3 = cmd_map.begin();
				it3 != cmd_map.end(); ++it3) {
			cmd_str.append(it3->first + ":" + it3->second + "\r\n");
		}
		cmd_str.append("\f");
		this->cmd_req_2send = cmd_str;
		return cmd_str;

	}
//

//

};

} /* namespace poseidon */
#endif /* SLAVER_H_ */
