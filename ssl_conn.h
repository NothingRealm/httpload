#ifndef SSL_CONN_H
#define SSL_CONN_H

#define SOCKET int

#define SUCCESS 0
#define ERR_SSL_CTX -1
#define ERR_SSL -2
#define ERR_SSL_CONN -3
#define ERR_SOCKET_CREATE -4
#define ERR_SOCKET_CONNECT -5

#include <openssl/ssl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

void init_ssl();
int create_ssl_connection(SSL *ssl, SOCKET *sock);
int socket_connect_to_host(char* hostname, char* port, SOCKET *sock);

#endif
