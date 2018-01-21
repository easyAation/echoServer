#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timerfd.h>
#include <time.h>
#define default_port 8080
#define default_thread_num 3
#define default_listen_num 1024
#define default_event_list_num 2048
#define default_heart_time 60
#define CONFIG_FILE_NAME "../etc/echo.conf"

struct echo_conf_t
{
    int port;
	int listen_num;
    int thread_num;
//	int heart_time;
    struct timespec max_interval_time;

	int event_list_num;
};

void init_conf(struct echo_conf_t *);
void default_conf(struct echo_conf_t *);

#endif

