#include "ssl_conn.h"
#include "logging.h"


void init_ssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithm();
    SSL_load_error_strings();
}


void create_ssl_connection(SSL *ssl, SOCKET *sock) {
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
