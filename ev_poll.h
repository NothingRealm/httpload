#ifndef EV_POLL_H
#define EV_POLL_H

struct pollfd_ssl {
    struct pollfd pfd;
    SSL* ssl;
};

#include "ev.h"
#include "ssl_conn.h"
#include <poll.h>

#define READ_EVENT POLLIN
#define WRITE_EVENT POLLOUT
#define CLEAR_EVENT 0

#define ERR_POLL_CREATE -7
#define ERR_POLL_WAIT -8
#define ERR_POLL_CTL -9

#endif
