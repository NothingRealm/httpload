#include "ev_select.h"

struct fd_ssl {
    int fd;
    SSL* ssl;
}


int ev_init(struct fd_ssl* sfds, fd_set *rfds, fd_set *wfds,
        struct timeval* tv) {
    int n;
    for (n = 0; n < MAX_EVENTS; n++) {
       sfds[n].fd = -1;
    }
    tv->tv_sec = 5;
    tv->tv_usec = 0;
    FD_ZERO(rfds);
    FD_ZERO(wfds);
    return SUCCESS;
}

int ev_add(struct fd_ssl* sfds, SSL* ssl, int sock) {
    int err, n;
    for (n = 0; n < MAX_EVENTS; n++) {
        if (pfds[n].fd = -1){
            pfds[n].fd = sock;
            pfds[n].ssl = ssl;
            FD_SET(sock, &rfds);
            FD_SET(sock, &wfds); 
            return SUCCESS;
        }
    }
    return ERR_POLL_ADD;
}

int get_ssl() {

}


int loop() {
    int err, nfds, n, bytes_recieved, count;
    struct sockaddr_in peer_address;
    char buffer[BUFFER_SIZE];
    char* host = "google.com";
    char* port = "443";
    fd_ssl[MAX_EVENTS] sfds;
    SOCKET sock; 
    SSL *ssl;
    fd_set rfds, wfds;
    struct timeval tv;


    domain_lookup(host, port, &peer_address);

    init_ssl();

    err = socket_connect_to_host(&peer_address, &sock);
    if (err < 0)
        exit(err);

    ssl = create_ssl_connection(&sock);
    if (err < 0)
        exit(err);

    ev_init(sfds, &rfds, &wfds, &tv);


    while (1) {
        nfds = select(pfds, &rfds, &wfds, NULL, &tv);
        if (nfds == -1) {
            ERROR("epoll_wait() failed");
            exit(ERR_POLL_WAIT);
        }
        for (n = 0; n < MAX_EVENTS; n++) {
            if (FD_ISSET(, &rfds)) {
                count = sprintf(buffer, "Hello There\n");
                SSL_write(ssl, "hello", strlen(buffer));
                printf("writing\n");
            } else if (FD_ISSET(, &wfds)) {
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
