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
	FILE *fp;
	//char *filep;
	char thefileName[1024];

	if ((fp = fopen("./conf/mysqlconn.conf", "r")) == NULL) {
		printf("open file mysqlconn.config error!!\n");
		return;
	}
	string content;

	fgets(thefileName, 1024, fp); //每次调用文件指针fp会自动后移一行
	LineOpt(thefileName);
	content = thefileName;
	this->host = content.c_str();

	fgets(thefileName, 1024, fp);
	LineOpt(thefileName);
	content = thefileName;
	this->user = content.c_str();

	fgets(thefileName, 1024, fp);
	LineOpt(thefileName);
	content = thefileName;
	this->psw = content.c_str();

	fgets(thefileName, 1024, fp);
	LineOpt(thefileName);
	content = thefileName;
	this->dbname = content.c_str();

	fgets(thefileName, 1024, fp);
	LineOpt(thefileName);
	content = thefileName;
	this->port = content.c_str();
	fclose(fp);

	result = NULL;
	init();

}
Cpp2mysql::Cpp2mysql(string host, string user, string psw, string port,
		string dbname) {
	this->host = host;
	this->user = user;
	this->psw = psw;
	this->port = port;
	this->dbname = dbname;
	result = NULL;
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
	if (!mysql_real_connect(&mysql, host.c_str(), user.c_str(), psw.c_str(),
			dbname.c_str(), atoi(port.c_str()), 0, 0)) {
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

bool Cpp2mysql::preparedExcute() {
	MYSQL_STMT *stmt;
//	if (!mysql_real_connect(&mysql, host.c_str(), user.c_str(), psw.c_str(),
//			dbname.c_str(), atoi(port.c_str()), 0, 0))
	stmt = mysql_stmt_init(&mysql);
	string sql = "INSERT b_test(ID,Type,Value) values('idtest',1,?)";
	int res1 = mysql_stmt_prepare(stmt,
			"INSERT b_test(ID,Type,Value) values('idtest',1,?)",
			strlen("INSERT b_test(ID,Type,Value) values('idtest',1,?)"));
	cout << res1 << endl;
	if (res1 != 0) {
		fprintf(stderr, " mysql_stmt_prepare(), insert failed\r\n");
		fprintf(stderr, " %s\r\n", mysql_stmt_error(stmt));
		return false;
	}

	MYSQL_BIND bind[1];
	unsigned long length[1];
	my_bool is_null[1];

	memset(bind, 0, sizeof(bind));
	memset(length, 0, sizeof(length));
	memset(is_null, 0, sizeof(is_null));

	char *st = "test2";
	bind[0].buffer_type = MYSQL_TYPE_BLOB;
	bind[0].buffer = (char *) &st;
	bind[0].buffer_length = sizeof(st);
	length[0] = sizeof(st);
	bind[0].length = &length[0];
	bind[0].is_null = &is_null[0];

	if (mysql_stmt_bind_param(stmt, bind)) {
		fprintf(stderr, "mysql_stmt_bind_param failed,err = %s\r\n",
				mysql_stmt_error(stmt));
		return false;
	}

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, "mysql_stmt_excute failed,err = %s\r\n",
				mysql_stmt_error(stmt));
		return false;
	}
	return 1;
}

void Cpp2mysql::close() {

	this->host = "";
	this->user = "";
	this->psw = "";
	this->port = "";
	this->dbname = "";
	mysql_free_result(result);
	result = NULL;
	mysql_close(&mysql);

}

} /* namespace sisyphus */
