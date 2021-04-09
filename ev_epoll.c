#include "ev_epoll.h"

int loop() {
    int err, epollfd, nfds, n, bytes_recieved;
    struct sockaddr_in peer_address;
    struct epoll_event ev, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    char* host = "localhost";
    char* port = "8000";
    SOCKET sock; 
    SSL *ssl;

    init_ssl();

    domain_lookup(host, port, &peer_address);
    printf("%s:%d\n", inet_ntoa(peer_address.sin_addr), ntohs(peer_address.sin_port));

    err = socket_connect_to_host(&peer_address, &sock);

    if (err < 0)
        exit(err);

    ///err = create_ssl_connection(ssl, &sock);
    if (err < 0)
        exit(err);

    epollfd = epoll_create1(0);
    if(epollfd < 0) {
        ERROR("epoll_create1() failed");
        exit(ERR_EPOLL_CREATE);
    }

    ev.events = READ_EVENT | WRITE_EVENT;
    ev.data.ptr = &sock;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);


    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            ERROR("epoll_wait() failed");
            exit(ERR_EPOLL_WAIT);
        }
        printf("%d\n", nfds);
        for (n = 0; n < nfds; n++) {
            sock = *(SOCKET*) events[n].data.ptr;
            if (events[n].events || READ_EVENT) {
                //bytes_recieved = SSL_read((SSL *) events[n].data.ptr, buffer, BUFFER_SIZE);
                printf("reading\n");
                read(sock, buffer, BUFFER_SIZE);
                if (bytes_recieved < 0)
                    continue;
                printf("reading finished\n");
                printf("%s\n", buffer);
            }
            if (events[n].events || WRITE_EVENT) {
                printf("writing\n");
                sprintf(buffer, "Hello There");
                write(sock, buffer, BUFFER_SIZE);
                //SSL_write((SSL *) events[n].data.ptr, buffer, strlen(buffer));
            }
        }
    }

}
