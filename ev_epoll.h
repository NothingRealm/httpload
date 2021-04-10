#ifndef EV_EPOLL_H
#define EV_EPOLL_H

#include "ev.h"
#include "ssl_conn.h"
#include <sys/epoll.h>

#define READ_EVENT EPOLLIN
#define WRITE_EVENT EPOLLOUT
#define CLEAR_EVENT EPOLLET

#define ERR_EPOLL_CREATE -7
#define ERR_EPOLL_WAIT -8
#define ERR_EPOLL_CTL -9

int add_new_connection(SSL *ssl, int epollfd);
int wait_for_event(int epollfd);

#endif
