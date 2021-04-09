#include "ssl_conn.h"
#include "logging.h"


void init_ssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}


int create_ssl_connection(SSL *ssl, SOCKET *sock) {
    SSL_CTX *ctx;

    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        ERROR("Could not create SSL context!!!");
        return ERR_SSL_CTX;
    }

    ssl = SSL_new(ctx);
    if (!ssl) {
        ERROR("Could not SSL!!!");
        return ERR_SSL;
    }

    SSL_set_fd(ssl, *sock);
    if (SSL_connect(ssl) == -1) {
        ERROR("Could not make SSL Connection!!!");
        return ERR_SSL_CONN;
    }
    return SUCCESS;
}


int socket_connect_to_host(struct sockaddr_in* peer_address, SOCKET *sock) {
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        ERROR("socket() failed!!!");
        return ERR_SOCKET_CREATE;
    }
    if (connect(*sock, (struct sockaddr*)peer_address, sizeof(*peer_address))) {
        ERROR("connect() failed!!!");
        return ERR_SOCKET_CONNECT;
    }
    return SUCCESS;
}


int domain_lookup(char* hostname, char* port, struct sockaddr_in* addr_in) {
    int err;
    struct addrinfo hints, *peer_address;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;

    err = getaddrinfo(hostname, port, &hints, &peer_address);

    if (!err)
        return err;

    addr_in = (struct sockaddr_in*) peer_address->ai_addr;

    printf("%s:%d\n", inet_ntoa(addr_in->sin_addr), ntohs(addr_in->sin_port));

    freeaddrinfo(peer_address);

    return SUCCESS;
}
