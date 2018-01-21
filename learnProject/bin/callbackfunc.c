#include "callbackfunc.h"


void* OnMessageCall_handle(struct echo_request *this , void *arg) {
	printf("进入到OnMessage函数.\n");
	char msg[1024];
	int sock_client = this->fd;
	int len_str = read(sock_client , msg , 1024);

	msg[len_str] = 0;

	avl_node *root = (avl_node *)arg;

	if(len_str <= 0) {

		printf("client: %d is disconnect\n", this->fd);
		//TYPE key = create_avl_key(this->fd , this->epfd , &this->curr);

	    root = delete_avl_by_key(root , create_avl_key(this->fd , this->epfd , &this->curr));
		if(this->fd != -1)
			epoll_del(this->epfd , this->fd , 0); 
		arg = (void *)root;
		return arg;
	}
	write(sock_client , msg , len_str);
	/*
 	*更新这个连接的最后响应时间
 	*/
	root = push_back(root , this);	
	if(NULL == root) {
		printf("push_back 后 root is NULL!!!!!\n");
	}
	arg = (void *)root;
	return arg;
}

void *OnMessageCall_heart(struct echo_request *this , void *arg) { 	
	avl_node *root = (avl_node *)arg;
	printf("进入心跳回调函数\n");
	uint64_t exp;
    int sock_timefd = this->fd;
	/*
 	* timefd 定时发送一个八字节的uint64 ， 在这个程序所用到的非阻塞IO里面如果不接收
 	* 这个uint64 , timefd就会连续触发EPOLLIN事件，造成 busy loop.
 	*/
	read(sock_timefd , &exp , sizeof(uint64_t));//timefd 定时发送一个八字节的uint64.	
	printf("exp : %llu\n" , exp);
	if(root != NULL) {
		root = pop(root);
		if(NULL  == root) {
			printf("OnMessageCall_heart-> root is NULL\n");
		}	
	}
	arg = (void *)root;
	return arg;
}

