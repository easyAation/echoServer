#include "epoll.h"
#include "heart.h"
#include "config.h"
#include "handle_echo.h"
#include "callbackfunc.h"

extern struct echo_conf_t conf;

int create_timerfd() {
	struct itimerspec new_value;
	struct timespec now;
	if(clock_gettime(CLOCK_MONOTONIC , &now) == -1) {
		perror("ERROR: create_timerfd->clock_gettime faild");
		exit(-1);
	}

	new_value.it_value.tv_sec = now.tv_sec + conf.max_interval_time.tv_sec;
	new_value.it_value.tv_nsec = 0;
	
	new_value.it_interval.tv_sec = conf.max_interval_time.tv_sec;
	new_value.it_interval.tv_nsec = 0;
	
	int fd = timerfd_create(CLOCK_MONOTONIC , 0);
	if(fd == -1) {
		perror("ERROR: create_timerfd->timerfd_create faild");
		exit(-1);
	}
	if(timerfd_settime(fd , TFD_TIMER_ABSTIME , &new_value , NULL) == -1) {
		perror("ERROR: create_timerfd->timerfd_settime faild");
		exit(-1);
	}
	return fd;
}
void *wait_task(void *arg)
{
	int epfd = *(int *)arg;
	printf("new epfd : %d\n" , epfd);
	struct epoll_event *evlist = NULL;
  	printf("wait_task: %d, join work thead\n", epfd);	
	evlist = (struct epoll_event*)malloc(sizeof(struct epoll_event) * conf.event_list_num);
	if(evlist == NULL)
	{
		perror("malloc evlist failed");
		exit(-1);
	}
    /*
 	*注册timefd到epoll和初始化一颗新avl tree来管理时间。
	*/
	echo_request_t *ptr = (echo_request_t*)malloc(sizeof(echo_request_t));
	ptr->epfd = epfd;
	ptr->fd = create_timerfd();
	ptr->OnMessageCall = OnMessageCall_heart; //赋值心跳回调函数
	epoll_add(epfd , ptr , EPOLLIN);

	avl_node *root = NULL; //创建avl tree 维护时间。

	int event_cnt , i;
	char recv_buf[RECV_BUF];
	while(1)
	{
		event_cnt = epoll_wait(epfd , evlist , conf.event_list_num-1 , -1);
		if(event_cnt == -1)
		{
			puts("epoll_wait error!");
			continue;
		}
		for(i=0; i<event_cnt; i++)
		{
			echo_request_t *echo = (echo_request_t*)evlist[i].data.ptr;
			if(evlist[i].events == EPOLLIN)
			{	
				root = (avl_node *)echo->OnMessageCall(echo , root); //运行时多态，并维护time tree
				if(NULL == root)
					printf("handle_func()-> root is NULL\n");
				/*int len_str = recv_message(ptr , recv_buf);
				if(len_str <= 0)
				{
					epoll_del(epfd , ptr->fd , EPOLLIN);
					close(ptr->fd); 
					printf("closed client: %d\n" , ptr->fd);
				}
				else
				{	
					send_message(ptr , recv_buf , len_str);
				}*/
			}
			/*else if(evlist[i].events == EPOLLOUT)
			{
				printf("fd : %d id long time don't anyting\n" , ptr->fd);
				epoll_del(epfd , ptr->fd , EPOLLIN);

			}*/
		}
	}
	free(evlist);
	close(epfd);
	return ;
}



