#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/sendfile.h> /* sendfile */

#include <sys/types.h>
#include <unistd.h>
#include "utility.h"
#include "lock_free_queue.h"

#include <sstream>
#include <string>

#include <iostream>
using namespace std;

#define MAXEVENTS 64
#define MAX_THREAD 6

extern bool have_signal;

void debugxxx()
{}; 

void* worker_func(void* queue)
{
	fprintf(stderr,"spawn a thread...\n");
	lock_free_queue* lfq = (lock_free_queue*)queue;
	while(!have_signal)
	{
		int socket;
		if(lfq->pop(socket))
		{
			char request[4096]={0};
			int request_size = 0;
			request_size = read(socket,request, 4096);
			cout<<request;
			debugxxx();
                        if(request_size <= 1 && request_size > 2048)
                        {
                                write(socket,"error",5);
                                close(socket);
                                continue;
                        }
			
			write(socket,request,request_size);
			close(socket);
		}
	}
};
struct server
{
	int sfd, s;
        int efd;
        struct epoll_event event;
        struct epoll_event *events;
	
	lock_free_queue lfq;
	pthread_t* worker;
	
	server()
	{
		events = NULL;
	}
	bool sopen(char* port)
	{
		worker = (pthread_t*) malloc(MAX_THREAD*sizeof( pthread_t));
                for(int i=0;i<MAX_THREAD;++i)
                {
                        pthread_create(&worker[i],NULL,worker_func,&lfq);
                }

		sfd = create_and_bind (port); if (sfd == -1)	return false;
		s = make_socket_non_blocking (sfd); if (s == -1) return false;

		s = listen (sfd, SOMAXCONN);  // echo 262144 > /proc/sys/net/core/somaxconn  
		if (s == -1) return false;

		efd = epoll_create (64); if (efd == -1) return false;

		event.data.fd = sfd;
		event.events = EPOLLIN | EPOLLET;
		s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);  if (s == -1) { return false;}
		
		events = (epoll_event*)calloc (MAXEVENTS, sizeof event);
		
		return true;
	};
	bool srun()
	{
		while(!have_signal)
		{
			int n, i;

			n = epoll_wait (efd, events, MAXEVENTS, -1);
			for (i = 0; i < n; i++)
			{
				if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
				{
					perror("epoll error");
					close (events[i].data.fd);
					continue;
				}
				else if (sfd == events[i].data.fd )
				{
					while (!have_signal)
					{
						struct sockaddr in_addr;
						socklen_t in_len;
						int infd;
						char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

						in_len = sizeof in_addr;
						infd = accept (sfd, &in_addr, &in_len);
						if (infd == -1)
						{
							if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
							{
								/* We have processed all incoming connections. */							
								break;
							}
							else
							{
								perror ("accept");
								break;
							}
						}
						/*
						s = getnameinfo (&in_addr, in_len,
								hbuf, sizeof hbuf,
								sbuf, sizeof sbuf,
								NI_NUMERICHOST | NI_NUMERICSERV);
						if (s == 0)
						{
							printf("Accepted connection on descriptor %d (host=%s, port=%s)\n", infd, hbuf, sbuf);
						}*/
						s = make_socket_blocking (infd);
						if (s == -1) return false;	
						event.data.fd = infd;
						event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
						s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
						if (s == -1)
						{
							perror ("epoll_ctl");
							return false;
						}
					}
					continue;
				}
				else 
				{
					printf("push socket %d into queue...\n",events[i].data.fd);
					if(!lfq.push(events[i].data.fd))
					{
						printf("%d\n",lfq.size());
						int done = 0;
						char buf[512];
						read (events[i].data.fd, buf, sizeof buf);
						off_t off = 0;
						int fd = open("./result.log",O_RDONLY);
						int cnt;
						size_t count = lseek(fd, 0L, SEEK_END);
						lseek(fd, 0L, SEEK_SET);
						if((cnt = sendfile(events[i].data.fd,fd,&off, count)) < 0){
							perror("sendfile");
							exit(errno);
						}
						close(fd);
						close ( events[i].data.fd ) ;
					}
					continue;
				}
			}
		}
		return true;
	};
	bool sclose()
	{
		printf("socket quit...\n");
		if(NULL != events)
		{
			free (events);
			events = NULL;
		}
		close (sfd);
		for(int i=0;i<MAX_THREAD;++i)		//make push end
		{
			pthread_join(worker[i],NULL);
		}
		if( NULL != worker )
		{
			free(worker);
			worker = NULL;
		}


		return true;
	};
};


