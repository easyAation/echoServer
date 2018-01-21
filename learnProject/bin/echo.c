#include "config.h"
#include "echo.h"
#include "epoll.h"
#include "heart.h"
#include "handle_echo.h"
#include "callbackfunc.h"

struct echo_conf_t conf;

int create_socket(int server_port , int listen_num)
{
    struct sockaddr_in server_addr;
	int sock_server = socket(AF_INET, SOCK_STREAM , 0);
	if(sock_server == -1)
	{
		handle_error("socket failed");
	}
	memset(&server_addr , 0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(conf.port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int option = 1;
	if(setsockopt(sock_server , SOL_SOCKET , SO_REUSEADDR , (void*)&option , sizeof(option)) < 0)
		perror("setsockopt failed!");
	if(bind(sock_server , (struct sockaddr*)&server_addr , sizeof(struct sockaddr_in)) == -1)
	{
		handle_error("bind failed");	
	}
	if(listen(sock_server , conf.listen_num) == -1)
	{
		handle_error("listen failed");
	}
	return sock_server;
}

int main()
{
    int sock_client , sock_server;
	struct sockaddr_in addr_client;
	int *epfds = NULL;
	pthread_t *threads = NULL;	
	pthread_t heart_thread;
	init_conf(&conf);	//初始化配置信息
	epfds = (int *)malloc(sizeof(int) * conf.thread_num);
	threads = (pthread_t*)malloc(sizeof(pthread_t) * conf.thread_num);
	
	if(epfds == NULL)
	{
		perror("epfds malloc fails");
		exit(-1);
	}
	if(threads == NULL)
	{
		perror("threads malloc fails");
		exit(-1);
	}

	int i;
	for(i=0; i<conf.thread_num; i++)
	{
		if((epfds[i] = epoll_create1(0)) < 0)
		{
			perror("epoll create failed");
			exit(EXIT_STATUS);
		}
	}
	for(i=0; i<conf.thread_num; i++)
	{
		if(pthread_create(&threads[i] , NULL , wait_task , (void *)(epfds + i)) != 0)
		{
			perror("pthread create failed");
			exit(EXIT_STATUS);
		}
	}
	sock_server = create_socket(conf.port , conf.listen_num);

    int len_client_addr = sizeof(addr_client);	
	while(1)
	{
		for(i=0; i<conf.thread_num; i++)
		{
			if((sock_client = accept(sock_server , (struct sockaddr*)&addr_client , &len_client_addr)) < 0)
			{
				perror("accpet failed");
				continue;
			} 
			echo_request_t *ptr = (echo_request_t*)malloc(sizeof(echo_request_t));

			TYPE key = create_heart_package(sock_client , epfds[i]);

			ptr->fd = sock_client;
			ptr->epfd = epfds[i];
			ptr->curr = key.curr;
			printf("新连接的fd和时间分别为: %d , %lu\n" ,ptr->fd, ptr->curr);
			ptr->OnMessageCall = OnMessageCall_handle;
			printf("New connect : %d\n" , sock_client);	
			epoll_add(epfds[i] , ptr , EPOLLIN);
		}		

	}	
	printf("all is completed\n");
	return 0;
}
	







	
