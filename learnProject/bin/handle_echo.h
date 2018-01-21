#ifndef _HANDLE_ECHO_
#define _HANDLE_ECHO_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "epoll.h"
#include "heart.h"
#include "config.h"
#define RECV_BUF 1024
#define SEND_BUF 1024


void *wait_task(void *arg);

int create_timerfd();


#endif
