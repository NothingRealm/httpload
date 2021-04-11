#ifndef EV_POLL_H
#define EV_POLL_H

#include "ev.h"
#include "ssl_conn.h"
#include <poll.h>

#define READ_EVENT POLLIN
#define WRITE_EVENT POLLOUT
#define CLEAR_EVENT 0

#define ERR_POLL_WAIT -8
#define ERR_POLL_ADD -9

struct pollfd_ssl {
    struct pollfd pfd;
    SSL* ssl;
};

#endif
