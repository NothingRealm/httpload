#include "ev_epoll.h"

int loop() {
    int err, epollfd, nfds, n, bytes_recieved;
    struct epoll_event ev, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    SOCKET sock; 
    SSL *ssl;

    init_ssl();

    err = socket_connect_to_host("localhost", "8000", &sock);
    if (err < 0)
        exit(err);

    err = create_ssl_connection(ssl, &sock);
    if (err < 0)
        exit(err);

    epollfd = epoll_create1(0);
    if(epollfd < 0) {
        ERROR("epoll_create1() failed");
        exit(ERR_EPOLL_CREATE);
    }

    ev.events = READ_EVENT | WRITE_EVENT;
    ev.data.ptr = ssl;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);


    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            ERROR("epoll_wait() failed");
            exit(ERR_EPOLL_WAIT);
        }
        for (n = 0; n < nfds; n++) {
            printf("here\n");
            if (events[n].events || WRITE_EVENT) {
                sprintf(buffer, "Hello There");
                SSL_write((SSL *) events[n].data.ptr, buffer, strlen(buffer));
            }
            if (events[n].events || READ_EVENT) {
                bytes_recieved = SSL_read((SSL *) events[n].data.ptr, buffer, BUFFER_SIZE);
                if (bytes_recieved < 0)
                    printf();
                printf("%s\n", buffer);
            }
        }
    }
}
