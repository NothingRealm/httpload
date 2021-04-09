#ifndef EV_EPOLL_H
#define EV_EPOLL_H

#include "ev.h"
#include "ssl_conn.h"
#include <sys/epoll.h>

#define READ_EVENT (EPOLLIN|EPOLLRDHUP)
#define WRITE_EVENT EPOLLOUT
#define CLEAR_EVENT EPOLLET

#define ERR_EPOLL_CREATE -6
#define ERR_EPOLL_WAIT -7

int add_new_connection(SSL *ssl, int epollfd);
int wait_for_event(int epollfd);

#endif
