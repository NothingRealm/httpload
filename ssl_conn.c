#include "ssl_conn.h"
#include "logging.h"


void init_ssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithm();
    SSL_load_error_strings();
}


int create_ssl_connection(SSL *ssl, SOCKET *sock) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        ERROR("Could not create SSL context!!!");
        return ERR_SSL_CTX;
    }

    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        ERROR("Could not SSL!!!");
        return ERR_SSL;
    }

    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) == -1) {
        ERROR("Could not make SSL Connection!!!");
        return ERR_SSL_CONN;
    }
    return SUCCESS;
}


int socket_connect_to_host(char* hostname, char* port, SOCKET *sock) {
    struct addrinfo hints;
    struct addrinfo *peer_address;
    int err;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(hostname, port, &hints, &peer_address);

    *sock = socket(peer_address->ai_family, peer_address->ai_socktype,
            peer_address->ai_protocol);
    if (*sock == -1) {
        ERROR("socket() failed!!!");
        return ERR_SOCKET_CREATE;
    }
    err = connect(*sock, peer_address->ai_addr, peer_address->ai_addrlen);
    if (err == -1) {
        ERROR("connect() failed!!!");
        return ERR_SOCKET_CONNECT;
    }
    freeaddrinfo(peer_address);
    return SUCCESS;
}




