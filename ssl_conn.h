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
#include <netinet/in.h>
#include <arpa/inet.h>

void init_ssl();
int create_ssl_connection(SSL *ssl, SOCKET *sock);
int socket_connect_to_host(struct sockaddr_in* peer_address, SOCKET *sock);
int domain_lookup(char* host_name, char* port, struct sockaddr_in* addr_in);

#endif
