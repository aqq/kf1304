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
#include <linux/tcp.h>
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

#include <sys/types.h>

#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>

#include "errno.h"
#include "GlobalHelper.h"

#include <signal.h>

#define REQUEST 1
#define SLEEP 2
#define UPDATE 3
#define GRABPAGE 4
#define STORE 7

#define DEBUG
#define BUFSIZE 1449
#define READ_BUFF_SIZE 1448
using namespace std;

namespace poseidon {
const string s_moudle = "moudle";
const string s_socket = "socket";
const string s_normal = "normal";
const string s_sql = "sql";
const string s_work = "task";

const string slave_private_conf = "./private/slave_private.conf";
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
	vector<string> update_shell;
	int version;

};

//extern ssize_t read (int __fd, void *__buf, size_t __nbytes) __wur;
static void read_alarm(int signo) {
	cout << "read_alarm:time out!" << endl;
	exit(1);
	return;
}

class slaver {
private:

	GlobalHelper *gh;
	int grab_interval; //毫秒
	int receive_time_out;
	int send_time_out;
	int connect_time_out;
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

	string local_ip_str;
	//feed back
	int last_task_status;
	int last_cmd_id;
	map<string, string> dns_map;
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
		this->master_ip.clear();
		this->master_ip.push_back(config_map1["master_ip"]);
		this->app_version = atoi((config_map1["app_version"]).c_str());
		//	this->store_ip.push_back(config_map1["store_ip"]);
		//	this->store_port = atoi((config_map1["store_port"]).c_str());
		this->grab_interval = atoi((config_map1["grab_interval"]).c_str());
		this->receive_time_out = atoi(
				(config_map1["receive_time_out"]).c_str());
		this->send_time_out = atoi((config_map1["send_time_out"]).c_str());
		this->connect_time_out = atoi(
				(config_map1["connect_time_out"]).c_str());
		gh->show_config(config_map1, gh->SLAVE_CONF);

		map<string, string> config_map2;
		gh->read_config(slave_private_conf, config_map2);
		this->slave_id = atoi((config_map2["slave_id"]).c_str());
		this->local_ip_str = local_ip();

	}

	string local_ip() {
		int sock_get_ip;
		char ipaddr[50];
		struct sockaddr_in* sin;
		struct ifreq ifr_ip;
		if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			printf("socket create failse...GetLocalIp!/n");
			return "can't get ip";
		}
		memset(&ifr_ip, 0, sizeof(ifr_ip));
		strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);
		if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0) {
			return "can't get ip";
		}
		sin = (struct sockaddr_in *) &ifr_ip.ifr_addr;
		strcpy(ipaddr, inet_ntoa(sin->sin_addr));
		//printf("local ip:%s /n", ipaddr);
		close(sock_get_ip);
		return string(ipaddr);
	}

	virtual ~slaver();
	bool request_task(req_task* mytask, string& str_cmd);
	bool grabpage_work(req_task& mytask);
	bool grab_page(grabtask gtask);
	bool remote_store_page(storetask s_task, string cmd, string send_content);

	void work() {
		req_task mytask;
		string task_str;
		int i = 1;
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
			gh->log2("str2task:[" + task_str, "]", s_normal);
			str2task(task_str, mytask);
			//3 hand_response
			gh->log2("hand_task().", s_moudle);
			gh->log2("hand_response:［" + gh->num2str(mytask.cmd_id), "]",
					s_normal);
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
			gh->log("Updata version:" + mytask.version);
			cout << mytask.update_shell[0] << endl;
			gh->call_updata_shell(mytask.update_shell[0]);
			//update app end

			//in real updata,the app_version will be write in code.
			this->app_version = mytask.version;
			this->last_task_status = 0;
			break;

		case GRABPAGE:

			gh->log2("Grab page totals:",
					gh->size_t2str(mytask.urls_http_req.size()), s_work);
			is_task_finished = grabpage_work(mytask);
			this->last_task_status = is_task_finished;

			break;

		case STORE:
			//	store_page_work();
			storetask s_task;
			s_task.request_ip = this->store_ip[0];
			s_task.request_port = this->store_port;
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
			string s1 = "commit page   " + send_filename;
			gh->log2(s1, s_work);
			bool is_store_ok = false;
			while (!is_store_ok) {
				is_store_ok = this->remote_store_page(s_task, cmd_str,
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
		struct hostent* pt;
		char** ptr;
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
	//
	void str2task(string response_command, req_task& mytask) {
		map<string, string> response_cmd_map;
		gh->command_str_to_map(response_command, &response_cmd_map);
		//
		//
		mytask.cmd_id = atoi(response_cmd_map["commd_id"].c_str());
		mytask.response_cmd_map = response_cmd_map;
		mytask.sleep_time = atoi(response_cmd_map["time"].c_str());
		mytask.slave_id = atoi(response_cmd_map["slave_id"].c_str());
		mytask.version = atoi(response_cmd_map["version"].c_str());
		mytask.update_shell.push_back(response_cmd_map["update_shell"].c_str());
		if (mytask.cmd_id == 7) {
			this->store_ip.clear();
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
		cmd_map["slave_ip"] = local_ip_str;
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
	void set_socket(int socketfd) {
		int keepAlive = 1;
		setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, (void*) &keepAlive,
				sizeof(keepAlive));
		int keepIdle = 10; //开始首次KeepAlive探测前的TCP空闭时间
		int keepInterval = 10; // 两次KeepAlive探测间的时间间隔
		int keepCount = 1; // 判定断开前的KeepAlive探测次数
		setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPIDLE, (void *) &keepIdle,
				sizeof(keepIdle));
		setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPINTVL, (void *) &keepInterval,
				sizeof(keepInterval));
		setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPCNT, (void *) &keepCount,
				sizeof(keepCount));
		struct timeval tv1;
		tv1.tv_sec = this->send_time_out;
		tv1.tv_usec = 0;
		//发送时限
		setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv1,
				sizeof(tv1));
		//接收时限
		struct timeval tv2;
		tv2.tv_sec = this->receive_time_out;
		tv2.tv_usec = 0;
		setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv2,
				sizeof(tv2));
	}

	//1．建立socket
	//2．将该socket设置为非阻塞模式
	//3．调用connect()
	//4．使用select()检查该socket描述符是否可写（注意，是可写）
	//5．根据select()返回的结果判断connect()结果
	//6．将socket设置为阻塞模式（如果你的程序不需要用阻塞模式的，这步就省了，不过一般情况下都是用阻塞模式的，这样也容易管理）
	bool socket_connect_timeout(int& sockfd, struct sockaddr_in& dest_addr,
			int time_out) {
		int error = -1;
		int len = sizeof(int);
		struct timeval tm;
		fd_set set;
		unsigned long ul = 1;
		ioctl(sockfd, FIONBIO, &ul);
		bool ret = 0;
		if (connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr))
				== -1) {
			tm.tv_sec = time_out;
			tm.tv_usec = 0;
			FD_ZERO(&set);
			FD_SET(sockfd, &set);
			if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0) {

				getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error,
						(socklen_t *) &len);
				if (error == 0) {
					ret = true;
				} else {
					ret = false;
				}

			} else {
				ret = false;
			} //end select if

		} else {
			ret = true; //end select
		}
		ul = 0;
		ioctl(sockfd, FIONBIO, &ul);
		if (!ret) {
			//	close(sockfd);
			//fprintf(stderr, "Cannot Connect the server!/n");
			return 0;
		}
		//	fprintf(stderr, "Connected!/n");
		return 1;
	}

	/////////////////////////test
	void down_test() {
	}

	bool request_task_timeo(req_task* mytask, string& str_cmd) {
		gh->log2("connect to ", this->master_ip[0], ":",
				gh->num2str(this->master_port), s_socket);
		//1 init variable
		str_cmd = "";
		int socketfd;
		struct sockaddr_in dest_addr;
		char read_buf[BUFSIZE];
		bzero(&read_buf, sizeof read_buf);
		bool req_seccess = 0;
		string log_str;
		while (1) {

			//2 create socket
			if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
				log_str = "socket fd create fail...";
				log_str += strerror(errno);
				gh->log2(log_str, s_socket); //perror("socket fd create fail...");

				break;
			}
			//
			//this->set_socket(socketfd);
			//
			//3 prepare server address
			gh->init_address(&dest_addr, PF_INET, this->master_port,
					inet_addr(this->master_ip[0].c_str()));

			//4 connect to server
			if (0
					== socket_connect_timeout(socketfd, dest_addr,
							this->connect_time_out)) {
				log_str = "socket fd connect fail...";
				log_str += strerror(errno);
				gh->log2(log_str, s_socket);
				break;
			}

			//5 write to master
			int bytes_count;
			//this->cmd_req_2send = this->init_request_cmd_str(); //last_task_status
			this->cmd_req_2send = "hi test!"; //last_task_status
			int write_result = write(socketfd, this->cmd_req_2send.c_str(),

			strlen(this->cmd_req_2send.c_str()));
			if (write_result == -1) {
				log_str = "socket fd write fail...";
				log_str += strerror(errno);
				gh->log2(log_str, s_socket); //perror
				break; //continue;
			}
			gh->log2("write:[" + this->cmd_req_2send, "]", s_socket);

			//6 read
			int total = 0;
			///
			__sighandler_t sigfunc1;
			int nsec = 10;
			sigfunc1 = signal(SIGALRM, read_alarm);
			if (alarm(nsec) != 0) {
				cout << "connect_timeo: alarm was already set" << endl;
			}
			///
			while ((bytes_count = read(socketfd, read_buf, READ_BUFF_SIZE)) > 0) {
				if (errno == EINTR) {
					cout << "error and timeout." << endl;
					break;
				}
				total += bytes_count;
				read_buf[bytes_count] = '\0';
				str_cmd += read_buf;
				//gh->log2(read_buf, "debug_read_from_socekt");
				if (bytes_count == 0) {
					break;
				}
				if (gh->tail_with_feature(read_buf, bytes_count, "\f")) {
					break;
				}

				cout << bytes_count << endl;

			}
			alarm(0); //  turn off the alarm  /
			signal(SIGALRM, sigfunc1); //restore previous signal handler
			///
			//	gh->log2(str_cmd, "debug_read_from_socekt_str_cmd");

			req_seccess = 1;
			break;
		}
		//}
		//7 clear socket
		close(socketfd);
		//8  init task
		return req_seccess;
	}

	//
	void request_task_timeo_test() {
		req_task mytask; //commad
		string str;
		this->request_task_timeo(&mytask, str);
		cout << "over request_task_timeo_test" << endl;
	}

	//
	void request_task_timeo_test2() {
		__sighandler_t sigfunc1;
		int nsec = 11;
		int i = 0;
		sigfunc1 = signal(SIGALRM, read_alarm);
		if (alarm(nsec) != 0) {
			cout << "connect_timeo: alarm was already set" << endl;
		}
		scanf("%d", &i);
		if (errno == EINTR) {
			cout << "error and timeout." << endl;
		}
		alarm(0); //  turn off the alarm  /
		signal(SIGALRM, sigfunc1);
		cout << " over 2." << endl;
	}
	void request_task_timeo_test3() {
		int j = 3;
		while (j--) {
			request_task_timeo_test2();
		}
	}
	//

	//
	bool request_task_expection(req_task *mytask, string& str_cmd) {

		gh->log2("connect to ", this->master_ip[0], ":",
				gh->num2str(this->master_port), s_socket);
		//1 init variable
		str_cmd = "";
		int socketfd;
		struct sockaddr_in dest_addr;
		char read_buf[BUFSIZE];
		bzero(&read_buf, sizeof read_buf);
		bool req_seccess = 0;
		string log_str;

		//2 create socket
		if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
			log_str = "socket fd create fail...";
			log_str += strerror(errno);
			gh->log2(log_str, s_socket); //perror("socket fd create fail...");

			//	break;
			close(socketfd);
			return 0;
		}

		//
		this->set_socket(socketfd);
		//

		//3 prepare server address
		gh->init_address(&dest_addr, PF_INET, this->master_port,
				inet_addr(this->master_ip[0].c_str()));

		//4 connect to server
		//
		if (0 == this->socket_connect_timeout(socketfd, dest_addr, 5)) {
			log_str = "socket fd connect fail...";
			log_str += strerror(errno);
			gh->log2(log_str, s_socket);
			gh->log2(" timeout socket_connect_timeout ", s_socket);

			close(socketfd);
			return 0;
		}
		//	gh->log2(" socket_connect ok ", s_socket);

		/*	if (-1
		 == connect(socketfd, (struct sockaddr*) &dest_addr,
		 sizeof(struct sockaddr))) {
		 log_str = "socket fd connect fail...";
		 log_str += strerror(errno);
		 gh->log2(log_str, s_socket);
		 gh->log2(" timeout socket_connect_timeout ", s_socket);
		 break;
		 }*/

		//
		//5 write to master
		int bytes_count;
		this->cmd_req_2send = this->init_request_cmd_str(); //last_task_status
		//	this->cmd_req_2send = "hi test!";
		///
		int write_result = write(socketfd, this->cmd_req_2send.c_str(),
				strlen(this->cmd_req_2send.c_str()));
		if (write_result == -1) {
			log_str = "socket fd write fail...";
			log_str += strerror(errno);
			gh->log2(log_str, s_socket); //perror
			close(socketfd);
			return 0;
		}
		gh->log2("write:[" + this->cmd_req_2send, "]", s_socket);

		//6 read
		int total = 0;
		bool read_sucessed = 0;
		while ((bytes_count = read(socketfd, read_buf, READ_BUFF_SIZE)) > 0) {
			total += bytes_count;
			read_buf[bytes_count] = '\0';
			str_cmd += read_buf;
			if (bytes_count == -1) {
				//gh->log2(read_buf, "debug_read_from_socekt");
				cout << "time_out or bad" << endl;

			}

			if (bytes_count == 0) {
				read_sucessed = 1;
				break;
			}
			if (gh->tail_with_feature(read_buf, bytes_count, "\f")) {
				read_sucessed = 1;
				break;
			}
		}

		gh->log2(str_cmd, "debug_read_from_socekt_str_cmd");

		req_seccess = read_sucessed; //

		//}
		//7 clear socket
		close(socketfd);

		return req_seccess;

	}
	void request_task_setsocketop_expectin() {
		req_task mytask; //commad
		string str;
		this->request_task_expection(&mytask, str);
		//	this->request_task(&mytask, str);
		cout << "over request_task_setsocketop_expectin " << endl;
	}

	int socket_readable_timeo(int fd, int sec) {
		fd_set rset;
		struct timeval tv;

		FD_ZERO(&rset);
		FD_SET(fd, &rset);

		tv.tv_sec = sec;
		tv.tv_usec = 0;

		return (select(fd + 1, &rset, NULL, NULL, &tv));
		/* 4> 0 if descriptor is readable */
	}
	int socket_readable_timeo_test(int fd, int sec) {
		int n;

		if ((n = socket_readable_timeo(fd, sec)) < 0)
			printf("readable_timeo error");
		return (n);
	}
//
	/* end readable_timeo */
};

} /* namespace poseidon */
#endif /* SLAVER_H_ */
