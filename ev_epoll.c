#include "ev_epoll.h"


int ev_init(int* epollfd) {
    *epollfd = epoll_create1(0);
    if(epollfd < 0) {
        ERROR("epoll_create1() failed");
        return ERR_EPOLL_CREATE;
    }
    return SUCCESS;
}


int ev_add(int epollfd, SSL* ssl, int sock) {
    int err;
    struct epoll_event ev;
    ev.events = WRITE_EVENT | READ_EVENT;
    ev.data.ptr = ssl;
    err = epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);
    if (err == -1)
        return ERR_EPOLL_CTL;
    return SUCCESS;
}

int ev_wait(int epollfd, struct epoll_event *events) {
    return epoll_wait(epollfd, events, MAX_EVENTS, -1);
}


int loop() {
    int err, epollfd, nfds, n, bytes_recieved, count;
    struct sockaddr_in peer_address;
    struct epoll_event events[MAX_EVENTS];
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

    err = ev_init(&epollfd);
    if (err < 0)
        exit(err);

    err = ev_add(epollfd, ssl, sock);
    if (err < 0)
        exit(err);

    while (1) {
        nfds = ev_wait(epollfd, events);
        if (nfds == -1) {
            ERROR("epoll_wait() failed");
            exit(ERR_EPOLL_WAIT);
        }
        for (n = 0; n < nfds; n++) {
            if (events[n].events & WRITE_EVENT) {
                count = sprintf(buffer, "Hello There\n");
                SSL_write((SSL *) events[n].data.ptr, "hello", strlen(buffer));
                printf("writing\n");
            }
            if (events[n].events & READ_EVENT) {
                bytes_recieved = SSL_read((SSL *) events[n].data.ptr, buffer,
                        BUFFER_SIZE);
                if (bytes_recieved < 0)
                    continue;
                printf("Read: %s\n", buffer);
            }
        }
    }

}
