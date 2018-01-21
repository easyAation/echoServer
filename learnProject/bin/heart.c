#include "heart.h"
/*
 * 判断优先级 : 1.时间 2 fd.
 * s1>s2 return 1;
 * s2<s2 return 0;
 * s1==s2 return 2;
 */
static int heart_compare(TYPE *s1 , TYPE *s2) {	
	//printf("比较: %lu" , s2->curr , s1->curr);
	//printf("  %lu\n" , s1->curr);
	if(s1->curr.tv_sec != s2->curr.tv_sec) {
	//	printf("秒不相等\n");
		return s1->curr.tv_sec > s2->curr.tv_sec;
	}
	if(s1->curr.tv_nsec != s2->curr.tv_nsec) {
	//	printf("纳秒不相等\n");
		return s1->curr.tv_nsec > s2->curr.tv_nsec;
	}
//	printf("时间相等了\n");
	if(s1->fd != s2->fd) {
//			printf("可是fd 不相等\n");
		return s1->fd > s2->fd;
	}
	return 2;
}

static int Height(avl_node *node) {
	return (NULL == node ? 0 : node->height);
}

static int max(int a , int b) {
	if(a > b) return a;
	return b;
}

/*
 * avl Left Left型
 */
static avl_node *LL_rotation(avl_node *root) {
	avl_node* node = root->lchild;
	root->lchild = node->rchild;
    node->rchild = root;
	
	root->height = max(Height(root->lchild) , Height(root->rchild)) + 1;
	node->height = max(Height(node->lchild) , Height(node->rchild)) + 1;
	return node;
}

/*
 * avl Right Right型
 */
static avl_node *RR_rotation(avl_node *root) {
	avl_node* node = root->rchild;
	root->rchild = node->lchild;
	node->lchild = root;

	root->height = max(Height(root->lchild) , Height(root->rchild)) + 1;
	node->height = max(Height(node->lchild) , Height(node->rchild)) + 1;
	return node;
}

/*
 * avl Left Right型
 */
static avl_node *LR_rotation(avl_node *node) {
	node->lchild = RR_rotation(node->lchild);
	return LL_rotation(node);
}

/*
 * avl Right Left型
 */
static avl_node *RL_rotation(avl_node *node) {
	node->rchild = LL_rotation(node->rchild);
	return RR_rotation(node);
}

static avl_node *insert_avlnode(avl_node *node , TYPE key) {
	if(NULL == node) 
	{
		node  = (avl_node*)malloc(sizeof(avl_node));
		if(NULL == node) {
			perror("insert_avlnode faild!");
			exit(-1);
		}
		node->key = key;
		printf("avl_node 插入的节点的时间是: %lu\n" , key.curr);
		node->height = 1;
		node->lchild = NULL;
		node->rchild = NULL;
	}
	else if(heart_compare(&key , &node->key) == 0) //插入tree的左孩子节点
	{
		node->lchild = insert_avlnode(node->lchild , key);
		if(Height(node->lchild) - Height(node->rchild) == 2) 
		{
			if(heart_compare(&key , &node->lchild->key) == 0)
				node = LL_rotation(node);
			else
				node = LR_rotation(node);
		}
	}
	else if(heart_compare(&key , &node->key) == 1) //插入node的右孩子节点
	{
		node->rchild = insert_avlnode(node->rchild , key);
		if(Height(node->rchild) - Height(node->lchild) == 2)
		{
			if(heart_compare(&key , &node->rchild->key) == 1)
				node = RR_rotation(node);
			else
				node = RL_rotation(node);
		}
	}
	node->height = max(Height(node->lchild) , Height(node->rchild)) + 1;
	return node;
}

static avl_node *avltree_maxnode(avl_node *node) {
	/*if(NULL == node->rchild)
		return node;
	return avltree_maxnode(node->rchild);
	*/
	if(NULL == node) {
		return NULL;
	}
	avl_node *ptr = node;
	while(NULL != ptr->rchild)
		ptr = ptr->rchild;
	return ptr;
}

static avl_node *avltree_minnode(avl_node *node) {
  /* if(NULL == node->lchild)
		return node;
	return avltree_minnode(node->lchild);*/
	if(NULL == node) {
		printf("minnode : node is NULL\n");
		return NULL;
	}
    avl_node *ptr = node;
	while(NULL != ptr->lchild)
		ptr = ptr->lchild;
	return ptr;	
	
}

static avl_node *delete_avlnode(avl_node *tree , avl_node *node) {
	if(NULL == tree || NULL == node) {
		return NULL;
	}
	if(heart_compare(&node->key , &tree->key) == 0)	//待删除的结点在左子树中
	{
		tree->lchild = delete_avlnode(tree->lchild , node);
		if(Height(tree->rchild) - Height(tree->lchild) == 2)
		{
			avl_node *r = tree->rchild;
			if(Height(r->lchild) > Height(r->rchild))
				tree = RL_rotation(tree);
			else
				tree = RR_rotation(tree);
		}
	}
	else if(heart_compare(&node->key , &tree->key) == 1) //待删除的结点在右子树中
	{
		tree->rchild = delete_avlnode(tree->rchild , node);
		if(Height(tree->lchild) - Height(tree->rchild) == 2)
		{
			avl_node *l = tree->lchild;
			if(Height(l->rchild) > Height(l->lchild))
				tree = LR_rotation(tree);
			else
				tree = LL_rotation(tree);
		}
	}
	else	//待删除的结点
	{
		if(tree->lchild && tree->rchild)
		{
			if(Height(tree->lchild) > Height(tree->rchild))
			{
				avl_node *max = avltree_maxnode(tree->lchild);
				tree->key = max->key;
				tree->lchild = delete_avlnode(tree->lchild , max);
			}
			else
			{
				avl_node *min = avltree_minnode(tree->rchild);
				tree->key = min->key;
				tree->rchild = delete_avlnode(tree->rchild , min);
			}
		}
		else
		{
			avl_node *tmp = tree;
			tree = tree->lchild ? tree->lchild : tree->rchild;
			free(tmp);
		}
	}
	if(tree)
		tree->height = max(Height(tree->lchild) , Height(tree->rchild)) + 1;
	return tree;
}
avl_node *delete_avl_by_key(avl_node *root , TYPE key) {
	avl_node *node = find_node(root , key);
	if(NULL != node) {
		root = delete_avlnode(root , node);
	}
	else {
		printf("删除的时候没有找到该节点\n");
	}
	return root;
}
static avl_node *find_node(avl_node *node , TYPE key) {
	if(NULL == node)
		return NULL;
	if(heart_compare(&key , &node->key) == 0)
		return find_node(node->lchild , key);
	if(heart_compare(&key , &node->key) == 1)
		return find_node(node->rchild , key);
	return node;
}

avl_node *pop(avl_node *root) {
	struct timespec curr;
	if(clock_gettime(CLOCK_MONOTONIC , &curr) == -1) {
		perror("ERROR: pop()->clock_gettime faild");
		exit(-1);
	}
	while(1) //将超过max interval time 没有与服务端通信的链接断掉。
	{
		avl_node *min = avltree_minnode(root);
		if(NULL == min) {
			printf("---min is NULL\n");
			return root;
		}
		//如果最远的也还没有超出时间，就退出
		if(curr.tv_sec - min->key.curr.tv_sec < conf.max_interval_time.tv_sec) {
			return root;
		}
		if(min->key.fd != -1) {
			printf("执行epoll_del 函数了！\n");
			printf("epfd : %d , fd : %d\n" , min->key.epfd , min->key.fd);
			epoll_del(min->key.epfd , min->key.fd , 1);
		
		}
		if(NULL == root)
			printf("root is NULL\n");
		root = delete_avlnode(root , min);
		if(NULL == root) {
			printf("function() pop-> root is NULL\n");
			return NULL;
		}
	}
	return root;
}

avl_node *push_back(avl_node *root , struct echo_request *this) {
	/*
	*若这个key存在，就从avl tree中删除它。
	*/
	//printf("进入push_back函数,要删除的节点的fd和时间是:%d , %lu\n",this->fd, this->curr);
	
TYPE key = create_avl_key(this->fd , this->epfd , &this->curr);
	avl_node *is_exist = find_node(root , key);
	if(NULL != is_exist) {
		root = delete_avlnode(root , is_exist);
		printf("找到了原先的节点 ， 并已经将它删除\n");
	}
	else {
		printf("没有找到原先的节点\n");
	}

	struct timespec curr;
	if(clock_gettime(CLOCK_MONOTONIC , &curr) == -1) {
		perror("ERROR: pop()->clock_gettime faild");
		exit(-1);
	}
	TYPE key1 = create_avl_key(this->fd , this->epfd , &curr);
	this->curr = curr;

	root = insert_avlnode(root , key1);
	return root;
}
TYPE create_avl_key(int fd , int epfd , struct timespec *curr) {
	TYPE package;
	package.fd = fd;
	package.epfd = epfd;
	if(NULL == curr) {
   		 if(clock_gettime(CLOCK_MONOTONIC , &package.curr) == -1)
	   	 {
			perror("ERROR: create_heart_key->clocl_gettime failed");
			exit(-1);
		}
	}
    else {
		package.curr = *curr;
	}
	return package;
}
avl_node *create_heart_tree(TYPE key) {
	avl_node *root = (avl_node *)malloc(sizeof(avl_node));
	root->key = key;
	root->height = 1;
	root->lchild = NULL;
	root->rchild = NULL;
	
	return root;
}
TYPE create_heart_package(int fd , int epfd) {
	TYPE package;
	package.fd = fd;
	package.epfd = epfd;
    if(clock_gettime(CLOCK_MONOTONIC , &package.curr) == -1)
	{
		perror("ERROR: create_heart_package->clocl_gettime failed");
		exit(-1);
	}
	return package;
}
