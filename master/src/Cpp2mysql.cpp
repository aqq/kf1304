/*
 * Cpp2mysql.cpp
 *
 *  Created on: 2012-5-16
 *      Author: root
 */

#include "Cpp2mysql.h"

#include<stdio.h>

#include<iostream>
#include<string>
#include <stdlib.h>
using namespace std;

#include<mysql/mysql.h>

namespace poseidon {
/*
 *usage:
 * 	Cpp2mysql* ms = new Cpp2mysql();
 * 	ms->execute(sqlquery2);
 *
 * */
Cpp2mysql::Cpp2mysql() {
	gh = new GlobalHelper();
	map<string, string> mysql_config_map;
	gh->read_config("./conf/mysqlconn.conf", mysql_config_map);
	//mysql_ip:192.168.60.22
	//mysql_port:3306
	//mysql_account:root
	//mysql_password:andrew
	//mysql_database:cjt

	this->host.push_back(mysql_config_map["mysql_ip"].c_str());
	this->user.push_back(mysql_config_map["mysql_account"].c_str());
	this->psw.push_back(mysql_config_map["mysql_password"].c_str());
	this->dbname.push_back(mysql_config_map["mysql_database"].c_str());
	this->port = atoi(mysql_config_map["mysql_port"].c_str());

	init();

}

Cpp2mysql::~Cpp2mysql() {

	this->close();
}
void Cpp2mysql::init() {
	mysql_init(&mysql);
	this->connect();
}
void Cpp2mysql::connectTest() {
	MYSQL_ROW row;
	Cpp2mysql* mysql = new Cpp2mysql();
	if (mysql->connect()) {
		string sql = "select * from test limit 100";
		MYSQL_RES* result = mysql->query(sql);
		if (result != NULL) {
			while ((row = mysql_fetch_row(result))) {
				cout << row[0] << "\t" << row[1] << "\t" << endl;
			}
		}
	} else {
		cout << "connect error!" << endl;
	}
	mysql->close();

}
bool Cpp2mysql::connect() {
	if (!mysql_real_connect(&mysql, host[0].c_str(), user[0].c_str(),
			psw[0].c_str(), dbname[0].c_str(), port, 0, 0)) {
		//如果处理失败，返回出错信息
		mysql_errno(&mysql);
		const char * s = mysql_error(&mysql);
		cout << s << endl;
		return false;
	}
	mysql_set_character_set(&mysql, "gbk");
	return true;
}

MYSQL_RES* Cpp2mysql::query(string sql) {
	int status = 0;
	gh->log2(sql, "sql");
	status = mysql_query(&mysql, sql.c_str());

	if (status != 0) {
		const char * sql_error_str = mysql_error(&mysql);
		cout << sql_error_str << endl;
		gh->log2(sql_error_str, "sql");
		return NULL;
	} else {
		result = mysql_store_result(&mysql);
		return result;
	}
}

void Cpp2mysql::close() {

	this->host.clear();
	this->user.clear();
	this->psw.clear();
	this->port = 0;
	this->dbname.clear();
	mysql_free_result(result);
	result = NULL;
	mysql_close(&mysql);

}

} /* namespace sisyphus */
