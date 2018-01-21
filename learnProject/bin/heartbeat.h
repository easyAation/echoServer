#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_

#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> // Definition of uint64_t

#include "config.h"
#include "epoll.h"

/* author : lianxm , a student of cs。
 * school : zzuli。
 * graduation date : 2019.06.
 *
 * 心跳模块，用最小堆维护heart_package，每个heart_channe就是一个最小堆
 * 在程序中，每个子线程(事务处理线程)都有一个heart_channe来维护所在线
 * 程的所有连接。
 */
extern struct echo_conf_t conf;

typedef struct heart_package {
	int fd;
	int epfd;
	struct timespec curr_;
}TYPE;

struct heart_channe {
	struct heart_package *heart_;
	int n;
};
	
static int heart_compare(struct heart_package *s1 , struct heart_package *s2);
static void heart_assignment(struct heart_package *s1 , struct heart_package *s2);
static void siftup(struct heart_channe *channe , int i);
static void siftdown(struct heart_channe *channe , int i);
 
struct heart_channe* create_Heart_Channe(int max_num);
struct heart_package* heart_first_package(struct heart_channe *channe);
void heart_push(struct heart_channe *channe , int fd , int epfd);
void heart_pop(struct heart_channe *channe);

#endif
