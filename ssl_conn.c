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
    int err;
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        ERROR("socket() failed!!!");
        return ERR_SOCKET_CREATE;
    }

    err = connect(*sock, (struct sockaddr*)peer_address, sizeof(*peer_address));
    printf("%d\n", err);

    if (err == -1) {
        ERROR("connect() failed!!!");
        return ERR_SOCKET_CONNECT;
    }
    return set_nonblocking(*sock);
}


int domain_lookup(char* hostname, char* port, struct sockaddr_in* addr_in) {
    int err;
    struct addrinfo hints, *peer_address;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    err = getaddrinfo(hostname, port, &hints, &peer_address);

    if (err != 0) {
        ERROR("Could not resolve domain");
        return ERR_LOOKUP;
    }

    *addr_in = *(struct sockaddr_in*) peer_address->ai_addr;

    return SUCCESS;
}

int set_nonblocking(int fd) {
	int options;
    options = fcntl(fd, F_GETFL);
	if (options < 0) {
        ERROR("Get flags failed");
		return ERR_GET_FLAG;
	}
	options = options | O_NONBLOCK;
	if (fcntl(fd, F_SETFL, options) < 0) {
		ERROR("Set flags failed");
		return ERR_SET_FLAG;
	}
	return SUCCESS;

}
