#include "ssl_conn.h"
#include "logging.h"


void init_ssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithm();
    SSL_load_error_strings();
}


