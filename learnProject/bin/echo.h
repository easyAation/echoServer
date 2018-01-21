#ifndef _ECHO_H_
#define _ECHO_H_
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#define EXIT_STATUS -1

void handle_error(const char *err)
{
	perror(err);
	exit(EXIT_STATUS);
}

int create_socket(int server_port , int listen_num);


#endif
