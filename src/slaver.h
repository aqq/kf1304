/*
 * slaver.h
 *
 *  Created on: 2013-4-1
 *      Author: root
 */

#ifndef SLAVER_H_
#define SLAVER_H_

#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

namespace poseidon
{
typedef struct task
{
	int sleep_time;

	int task_id;
	int command;

	string store_ip;
	int store_port;

	string url_header;
	vector<string> *url_body;

};
class slaver
{
protected:
	int slave_id;
	int pro_version;
	int application_version;
public:
	slaver();
	virtual ~slaver();
	bool requestTask(task *mytask);
	bool work(task mytask);
	bool grabpage(task mytask, int index);
	bool localStorePage();
	bool remoteStorePage();
	//

	string& replace_all(string& str, const string& old_value,
			const string& new_value);
	string getHttpHeader(string url_header, vector<string> url_body, int id);
};

} /* namespace poseidon */
#endif /* SLAVER_H_ */
