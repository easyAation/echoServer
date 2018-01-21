#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
typedef struct echo_request
{
	int fd;
	int epfd;
	struct timespec curr;
	void* (*OnMessageCall) (struct echo_request *this , void *arg);
}echo_request_t;

void epoll_add(int epfd , echo_request_t *ptr , int status);

void epoll_mod(int epfd , echo_request_t *ptr , int status);

void epoll_del(int epfd , int fd , int status);

void set_sock_nonblock(int sockfd);

#endif
