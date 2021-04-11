#include "ev_poll.h"

int ev_init(struct pollfd_ssl* pfds) {
    int n;
    for (n = 0; n < MAX_EVENTS; n++) {
       pfds[n].pfd.fd = -1;
       pfds[n].pfd.events = WRITE_EVENT | READ_EVENT;
    }
    return SUCCESS;
}


int ev_add(struct pollfd_ssl* pfds, SSL* ssl, int sock) {
    int err, n;
    for (n = 0; n < MAX_EVENTS; n++) {
        if (pfds[n].pfd.fd = -1){
            pfds[n].pfd.events = WRITE_EVENT | READ_EVENT;
            pfds[n].pfd.fd = sock;
            pfds[n].ssl = ssl;
            return SUCCESS;
        }
    }
    return ERR_POLL_ADD;
}


int ev_wait(struct pollfd_ssl* pfds) {
    return poll((struct pollfd*)pfds, MAX_EVENTS, -1);
}


int ev_del(struct pollfd_ssl* pfds, int index) {
    pfds[index].pfd.fd = -1;
}


int loop() {
    int err, nfds, n, bytes_recieved, count;
    struct sockaddr_in peer_address;
    struct pollfd_ssl pfds[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    char* host = "google.com";
    char* port = "443";
    SOCKET sock; 
    SSL *ssl;

    domain_lookup(host, port, &peer_address);

    init_ssl();

    err = socket_connect_to_host(&peer_address, &sock);
    if (err < 0)
        exit(err);

    ssl = create_ssl_connection(&sock);
    if (err < 0)
        exit(err);

    for (n = 0; n < MAX_EVENTS; n++) {
       pfds[n].pfd.fd = -1;
       pfds[n].pfd.events = WRITE_EVENT | READ_EVENT | POLLHUP;
    }

    err = ev_add(pfds, ssl, sock);
    if (err < 0)
        exit(err);

    while (1) {
        nfds = ev_wait(pfds);
        if (nfds == -1) {
            ERROR("epoll_wait() failed");
            exit(ERR_POLL_WAIT);
        }
        for (n = 0; n < MAX_EVENTS; n++) {
            if (pfds[n].pfd.revents & WRITE_EVENT) {
                count = sprintf(buffer, "Hello There\n");
                SSL_write(ssl, "hello", strlen(buffer));
                printf("writing\n");
            } else if (pfds[n].pfd.revents & READ_EVENT) {
                bytes_recieved = SSL_read(ssl, buffer,
                        BUFFER_SIZE);
                if (bytes_recieved < 0)
                    continue;
                printf("Read: %s\n", buffer);
            } else if (pfds[n].pfd.revents & POLLHUP) {
                ev_del(pfds, n);
                printf("deleting");
            }
        }
    }
}
