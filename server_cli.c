#include "common.h"
#include "httpd.h"
#include "logging.h"
#include "cli.h"

#include <stdio.h>
#include <argp.h>
#include <stdlib.h>

#define SERVER_DEFAULT_PORT     8080
#define SERVER_DEFAULT_FORKS    1

static struct {
    uint16_t port;
    uint8_t forks;
} settings = {
    SERVER_DEFAULT_PORT,
    SERVER_DEFAULT_FORKS,
};

static char doc[] = "HTTP echo server using Linux epoll.";
static char args_doc[] = "";

/* Options definition */
static struct argp_option options[] = {
    ARG_VERBOSITY,
    ARG_CONCURRENCY,
    { "port", 'p', "PORT", 0,
     "TCP Port to bind, default: " STR(SERVER_DEFAULT_PORT) "." },
    { 0 }
};

/* Parse a single option. */
static int
parse_opt(int key, char *arg, struct argp_state *state) {
    switch (key) {
        case 'c':
            settings.forks = atoi(arg);
            break;

        case 'p':
            settings.port = atoi(arg);
            break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 0) {
                /* Too many arguments. */
                return ARGP_ERR_UNKNOWN;
            }
            break;

        default:
            return parse_common_opts(key, arg, state);
    }
    return EXIT_SUCCESS;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int
servercli_run(int argc, char **argv) {
    int err = argp_parse(&argp, argc, argv, 0, 0, NULL);

    if (err) {
        return err;
    }

    struct httpd m = {.port = settings.port,.forks = settings.forks };
    err = httpd_fork(&m);
    if (err) {
        FATAL("Cannot start http mock server");
    }
    INFO("Listening on port: %d", m.port);

    err = httpd_join(&m);
    if (err) {
        ERROR("Cannot kill chilren!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
