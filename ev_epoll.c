#include "ev_epoll.h"

int loop() {
    int err, epollfd, nfds, n, bytes_recieved, count;
    struct sockaddr_in peer_address;
    struct epoll_event ev, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    char* host = "google.com";
    char* port = "443";
    SOCKET sock, peer_sock; 
    SSL *ssl;

    domain_lookup(host, port, &peer_address);

    init_ssl();


    err = socket_connect_to_host(&peer_address, &sock);

    if (err < 0)
        exit(err);

    ssl = create_ssl_connection(&sock);
    if (err < 0)
        exit(err);

    epollfd = epoll_create1(0);
    if(epollfd < 0) {
        ERROR("epoll_create1() failed");
        exit(ERR_EPOLL_CREATE);
    }

    ev.events = WRITE_EVENT | READ_EVENT;
    ev.data.ptr = ssl;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);

    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
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
                bytes_recieved = SSL_read((SSL *) events[n].data.ptr, buffer, BUFFER_SIZE);
                if (bytes_recieved < 0)
                    continue;
                printf("Read: %s\n", buffer);
            }
        }
    }

}
