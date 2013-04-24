/*
 * sockethelper.h
 *
 *  Created on: 2013-4-24
 *      Author: root
 */

#ifndef SOCKETHELPER_H_
#define SOCKETHELPER_H_
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
namespace poseidon {
class socket_helper {
private:
	int send_time_out;
	int receive_time_out;
	int connect_time_out;
	GlobalHelper *gh;
public:
	socket_helper();
	virtual ~socket_helper();
	bool request_task(string master_ip, int master_port, string& str_cmd,
			string cmd_req_2send);

	bool socket_connect_timeout(int& sockfd, struct sockaddr_in& dest_addr,
			int time_out);
	//
	void init_address(struct sockaddr_in *dest_addr, int sin_family, int port,
			in_addr_t ip) {
		bzero(dest_addr, sizeof(dest_addr));
		dest_addr->sin_family = sin_family;
		dest_addr->sin_port = htons(port);
		dest_addr->sin_addr.s_addr = ip;
		bzero(&(dest_addr->sin_zero), 8);
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
	//

	//
};

#endif /* SOCKETHELPER_H_ */
}
