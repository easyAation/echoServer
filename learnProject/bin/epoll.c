#include "epoll.h"
#include <stdio.h>

void epoll_add(int epfd , echo_request_t *ptr , int status)
{
	struct epoll_event ev;
	memset(&ev , 0 , sizeof(ev));
	ev.events = status;
	ev.data.ptr = ptr;
	set_sock_nonblock(ptr->fd);
	if(epoll_ctl(epfd , EPOLL_CTL_ADD , ptr->fd , &ev) < 0)
	{
		perror("epoll ctl_add failed");
	}
	
	return ;
}

void epoll_mod(int epfd , echo_request_t *ptr , int status)
{
	struct epoll_event ev;
	memset(&ev , 0 , sizeof(ev));
	ev.events = status;
	ev.data.ptr = ptr;
	if(epoll_ctl(epfd , EPOLL_CTL_MOD , ptr->fd , &ev) < 0)
	{
		perror("epoll ctl_mod failed");
	}

	return ;
}

void epoll_del(int epfd , int fd , int status)
{	
	/*struct epoll_event ev;
	memset(&ev , 0 , sizeof(ev));
	ev.events = status;
	ev.data.ptr = ptr;*/
	printf("run epoll del epfd : %d , fd : %d\n", epfd , fd);
	if(epoll_ctl(epfd , EPOLL_CTL_DEL , fd , NULL) < 0)
	{
		perror("epoll ctl_del failed");
	}
	
	return ;
}

void set_sock_nonblock(int sockfd)
{
	int flag = fcntl(sockfd , F_GETFL , 0);
	fcntl(sockfd , F_SETFL , flag | O_NONBLOCK);
}
