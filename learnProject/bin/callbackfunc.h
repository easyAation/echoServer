#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include "epoll.h"
#include "heart.h"

void* OnMessageCall_heart(struct echo_request *this , void *root);

void* OnMessageCall_handle(struct echo_request *this , void *root);

