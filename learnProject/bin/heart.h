#ifndef _HEART_H_
#define _HEART_H_

#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "epoll.h"
extern struct echo_conf_t conf; 
/*
 * 定时器模块，用avl tree来管理时间。向外暴露四个功能函数
 */
typedef struct heart_package {
	int fd;
	int epfd;
	struct timespec curr;
}TYPE;

typedef struct avl_tree {
	TYPE key;
	int height;
	struct avl_tree *lchild;
	struct avl_tree *rchild;
}avl_node;

avl_node *create_heart_tree(TYPE key);

TYPE create_heart_package(int fd , int epfd);

TYPE create_avl_key(int fd , int epfd , struct timespec *curr);

avl_node *pop(avl_node *root);

avl_node *push_back(avl_node *root , struct echo_request *this);

avl_node *delete_avl_by_key(avl_node *root , TYPE key);

static int Height(avl_node *node);

static int max(int a , int b);

static int heart_compare(TYPE *s1 , TYPE *s2);

static avl_node *LL_rotation(avl_node *root);

static avl_node *RR_rotation(avl_node *root);

static avl_node *LR_rotation(avl_node *root);

static avl_node *RL_rotation(avl_node *root);

static avl_node *insert_avlnode(avl_node *node , TYPE key);

static avl_node *delete_avlnode(avl_node *tree , avl_node *node);

static avl_node *avltree_maxnode(avl_node *node);

static avl_node *avltree_minnode(avl_node *node);

static avl_node *find_node(avl_node *node , TYPE key);

#endif
