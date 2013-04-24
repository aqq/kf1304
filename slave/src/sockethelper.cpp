/*
 * sockethelper.cpp
 *
 *  Created on: 2013-4-24
 *      Author: root
 */

#include "sockethelper.h"
#define BUFSIZE 1449
#define READ_BUFF_SIZE 1448

namespace poseidon {
socket_helper::socket_helper() {
	// TODO Auto-generated constructor stub
	gh = new GlobalHelper();
}

socket_helper::~socket_helper() {
	// TODO Auto-generated destructor stub
}

bool socket_helper::socket_connect_timeout(int& sockfd,
		struct sockaddr_in& dest_addr, int time_out) {
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
bool socket_helper::request_task(string master_ip, int master_port,
		string& str_cmd, string cmd_req_2send) {

	gh->log2("connect to ", master_ip, ":", gh->num2str(master_port),
			poseidon::s_socket);

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
			gh->log2("socket fd create fail...", strerror(errno), s_socket); //perror("socket fd create fail...");
			break;
		}

		this->set_socket(socketfd);

		//3 prepare server address
		gh->init_address(&dest_addr, PF_INET, master_port,
				inet_addr(master_ip.c_str()));

		//4 connect to server
		if (0
				== socket_connect_timeout(socketfd, dest_addr,
						this->connect_time_out)) {
			gh->log2("socket fd connect fail...", strerror(errno), s_socket);
			break;
		}

		//5 write to master
		int bytes_count;
		//	string cmd_req_2send = ""; // = this->init_request_cmd_str(); //last_task_status

		int write_result = write(socketfd, cmd_req_2send.c_str(),
				strlen(cmd_req_2send.c_str()));

		if (write_result == -1) {
			gh->log2("socket fd write fail...", strerror(errno), s_socket); //perror
			break; //continue;
		}
		gh->log2("write:[" + cmd_req_2send, "]", s_socket);

		//6 read
		while ((bytes_count = read(socketfd, read_buf, READ_BUFF_SIZE)) > 0) {
			read_buf[bytes_count] = '\0';
			str_cmd += read_buf;
			//gh->log2(read_buf, "debug_read_from_socekt");

			if (bytes_count == 0) {
				req_seccess = 1;
				break;
			}
			if (bytes_count == -1) {
				gh->log("socket fd read faild  error...");

				break;
			}
			if (gh->tail_with_feature(read_buf, bytes_count, "\f")) {
				req_seccess = 1;
				break;
			}

		} //end while
		  //	gh->log2(str_cmd, "debug_read_from_socekt_str_cmd");
		break;
	} //end while

	//7 clear socket
	close(socketfd);

	return req_seccess;

}

}
