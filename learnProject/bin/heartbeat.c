#include "heartbeat.h"
#include "epoll.h"


extern struct echo_conf_t conf;

struct heart_channe* create_Heart_Channe(int max_num) {
	struct heart_channe* channe = (struct heart_channe*)malloc(sizeof(struct heart_channe) * conf.event_list_num);
	channe->heart_ = (struct heart_package*)malloc(sizeof(struct heart_package));
	channe->n = 0;
	return channe;
}

struct heart_package* heart_first_package(struct heart_channe *channe) {
	if(channe->n == 0)
		return NULL;
	return &channe->heart_[1];
}

static int heart_compare(struct heart_package *s1 , struct heart_package *s2) {
	if(s1->curr_.tv_sec != s2->curr_.tv_sec) {
		return s1->curr_.tv_sec > s2->curr_.tv_sec;
	}
	if(s1->curr_.tv_nsec != s2->curr_.tv_nsec) {
		return s1->curr_.tv_nsec > s2->curr_.tv_nsec;
	}
	return 0;
}


static void heart_assignment(struct heart_package *s1 , struct heart_package *s2) {
	s1->curr_ = s2->curr_;
	s1->fd   = s2->fd;
	s1->epfd = s2->epfd;
}

static void siftup(struct heart_channe *channe , int i)	{
	if(i == 1) {
		return ;
	}
	while(i != 1)
    {
		if(heart_compare(&channe->heart_[i] , &channe->heart_[i/2]) == 0) {
			struct heart_package now;
			heart_assignment(&now , &channe->heart_[i]);
			heart_assignment(&channe->heart_[i] , &channe->heart_[i/2]);
			heart_assignment(&channe->heart_[i/2] , &now);
		}
		else {
			break;
		}
		i = i / 2;
	}
}
	
static void siftdown(struct heart_channe *channe , int i) {
	int t;
	while(i*2 <= channe->n)
	{
		if(heart_compare(&channe->heart_[i] , &channe->heart_[i*2])) {
			t = i * 2;
		}
		else {
			t = i;
		}

		if(i*2+1 <= channe->n && heart_compare(&channe->heart_[t] , &channe->heart_[i*2+1])) {
			t = i * 2 + 1;
		}
		if(t != i) {
			struct heart_package now;
			heart_assignment(&now , &channe->heart_[i]);
			heart_assignment(&channe->heart_[i] , &channe->heart_[t]);
			heart_assignment(&channe->heart_[t] , &now);
		}
		else {
		  	break;
		}
	}
}

void heart_push(struct heart_channe *channe , int fd , int epfd) {
	if(channe->n + 1 > conf.event_list_num) {
		fprintf(stderr , "epoll : %d listen number is %d , so can't add more heart_package\n", epfd , conf.event_list_num);
		return ;
	}
	struct timespec curr;
	if(clock_gettime(CLOCK_MONOTONIC , &curr) == -1) {
		fprintf(stderr , "function: heart_push, clock_gettime failed!\n");	
	}
	channe->n += 1;
	channe->heart_[channe->n].fd = fd;
	channe->heart_[channe->n].epfd = epfd;
	channe->heart_[channe->n].curr_ = curr;
	siftup(channe , channe->n);
}


void heart_pop(struct heart_channe *channe)	{
	if(!channe->n) {
		return ;
	}
	heart_assignment(&channe->heart_[1] , &channe->heart_[channe->n]);
	channe->n -= 1;
	if(channe->n > 1) {
		siftdown(channe , 1);
	}
}
