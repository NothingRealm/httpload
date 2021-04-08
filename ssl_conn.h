#ifndef SSL_CONN_H
#define SSL_CONN_H

#define SOCKET int

#define SUCCESS 0
#define ERR_SSL_CTX -1
#define ERR_SSL -2
#define ERR_SSL_CONN -3

#include <openssl/ssl.h>


void init_ssl();
int create_ssl_connection(SSL *ssl, SOCKET *sock);

#endif
