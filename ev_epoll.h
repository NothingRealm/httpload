#ifndef EV_EPOLL_H
#define EV_EPOLL_H

#include "ev.h"

#define READ_EVENT (EPOLLIN|EPOLLRDHUP)
#define WRITE_EVENT EPOLLOUT
#define CLEAR_EVENT EPOLLET


#endif
