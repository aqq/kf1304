/*
 * rep.h
 *
 *  Created on: 2013-4-10
 *      Author: root
 */

#ifndef REP_TEST_H_
#define REP_TEST_H_
#include "rep.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/statfs.h>
#include "map"
#include "vector"
#include "GlobalHelper.h"
#define DEBUG
using namespace std;

namespace poseidon {

void rep_test() {

	rep *rep1 = new rep();
	rep1->service();
}
void read_config_test() {
	rep *rep1 = new rep();
	GlobalHelper *gh;
	gh = new GlobalHelper();
	cout << rep1->master_ip[0] << endl;
	cout << rep1->master_port << endl;
	assert( rep1->master_ip[0]==gh->MASTER_IP);
	assert( rep1->master_port==gh->MASTER_PORT);

}
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
void report_disk_space() {
	//float available_dk = available_disk_space();

//create socket and connect to master
}
void report_disk_space_test() {

}
void service_test_5_5() {
	/*GlobalHelper *gh = new GlobalHelper();

	 //5.5 accept the real content with gzip
	 string filename = "./pages_gzip/" + gh->ld2str(req_cmd.task_id) + ".tar.gz";
	 int fd = open(gh->ld2str(req_cmd.task_id).c_str(),
	 O_CREAT | O_WRONLY | O_TRUNC);
	 long total = 0;

	 while (1) {
	 read_count = read(new_fd, request_buff, sizeof request_buff);
	 if (read_count == -1) {
	 perror("socket fd read fail...");
	 break;
	 }
	 write(fd, request_buff, read_count);
	 total += read_count;
	 if (total == req_cmd.content_size) {
	 break;
	 }
	 if (read_count == 0) {
	 gh->log("receive eof ,so total is bad ");
	 break;

	 read_count = read(new_fd, request_buff, sizeof request_buff);
	 if (read_count == -1) {
	 perror("socket fd read fail...");
	 break;
	 }
	 write(fd, request_buff, read_count);
	 total += read_count;
	 if (total == req_cmd.content_size) {
	 break;
	 }
	 if (read_count == 0) {
	 gh->log("receive eof ,so total is bad ");
	 break;
	 }
	 close(fd);
	 }

	 } //
	 */
}
//===============
}
#endif
