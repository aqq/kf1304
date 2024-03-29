/*
 * Cpp2mysql.h
 *
 *  Created on: 2012-5-16
 *      Author: root
 */

#ifndef CPP2MYSQL_H_
#define CPP2MYSQL_H_
#include<mysql/mysql.h>
#include<iostream>
#include<string>
#include "GlobalHelper.h"
using namespace std;

namespace poseidon {
struct worker_tb {
	int slave_id;
	string last_request_time;
	string last_request_ip;
	float available_disk_space;
	string worker_type;
};
struct worker_site_tb {
	int ws_id;
	int slave_id;
	string site_id;
	bool site_task_sucess;
//	bool site_task_failed;
};
class Cpp2mysql {
protected:
	vector<string> host;
	vector<string> user;
	vector<string> psw;
	vector<string> dbname;
	int port;

	MYSQL mysql;
	MYSQL_RES* result;
	MYSQL_ROW row;
	GlobalHelper *gh;
protected:
	void init();

public:
	Cpp2mysql();
	//Cpp2mysql(string host, string user, string psw, string port, string dbname);
	void connectTest();
	virtual ~Cpp2mysql();

	void close();
	bool connect();
	bool connect(string host, string user, string psw, string port,
			string dbname);
	MYSQL_RES* query(string sql);
	int execute(string sql, MYSQL_RES* result);
	int execute(string sql);

	//================================
	void update_worker_tb(worker_tb worker) {
		char sqlquery1[500] =
				"	update worker_tb set "
						"last_request_time='%s',last_ip='%s',available_disk_space='%f',worker_type='%s' "
						"where slave_id=%d";
		char sqlquery2[500];
		sprintf(sqlquery2, sqlquery1, worker.last_request_time.c_str(),
				worker.last_request_ip.c_str(), worker.available_disk_space,
				worker.worker_type.c_str(), worker.slave_id);

	//	gh->log2(sqlquery2, "sql");
		this->query(sqlquery2);
	}
	void update_worker_tb_test() {
		worker_tb w;
		w.slave_id = 1;
		//
		w.last_request_time = gh->get_time_str("%Y-%m-%d %H:%M:%S");
		update_worker_tb(w);
	}
	//================================

	void update_worker_site_tb(worker_site_tb w_s) {
		string site_task_str = "";
		if (w_s.site_task_sucess) {
			site_task_str = "site_task_sucess";
		} else {
			site_task_str = "site_task_failed";
		}
		char sqlquery1[500] = "	update worker_site_tb"
				" set %s=%s+1"
				" where slave_id=%d and site_id='%s'";
		char sqlquery2[500];
		sprintf(sqlquery2, sqlquery1, site_task_str.c_str(),
				site_task_str.c_str(), w_s.slave_id, w_s.site_id.c_str());

		//	gh->log2(sqlquery2, "sql");
		this->query(sqlquery2);
	}
	void update_worker_site_tb_test() {
		worker_site_tb w;
		w.slave_id = 1;
		w.site_id = "bbs_csdn";
		//

		update_worker_site_tb(w);
	}
	//================================

};

} /* namespace poseidon */
#endif /* CPP2MYSQL_H_ */
