/*
 * Starter code for proxy lab.
 * Feel free to modify this code in whatever way you wish.
 */

/* Some useful includes to help you get started */

#include "csapp.h"

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>

/*
 * Debug macros, which can be enabled by adding -DDEBUG in the Makefile
 * Use these if you find them useful, or delete them if not
 */
#ifdef DEBUG
#define dbg_assert(...) assert(__VA_ARGS__)
#define dbg_printf(...) fprintf(stderr, __VA_ARGS__)
#else
#define dbg_assert(...)
#define dbg_printf(...)
#endif

/*
 * Max cache and object sizes
 * You might want to move these to the file containing your cache implementation
 */
#define MAX_CACHE_SIZE (1024 * 1024)
#define MAX_OBJECT_SIZE (100 * 1024)
#define HOSTLEN 256
#define PORTLEN 8

typedef struct sockaddr SA;

/*
 * String to use for the User-Agent header.
 * Don't forget to terminate with \r\n
 */
static const char *header_user_agent = "Mozilla/5.0"
                                       " (X11; Linux x86_64; rv:3.10.0)"
                                       " Gecko/20221116 Firefox/63.0.1";

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <poart>\n", argv[0]);
        exit(1);
    }

    int connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_hostname[HOSTLEN];
    char client_port[PORTLEN];
    int listenfd = open_listenfd(argv[1]);
    if (listenfd < 0) {
        fprintf(stderr, "Failed to listen on port: %s\n", argv[1]);
        exit(1);
    }
    while (true) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        if (connfd < 0) {
            perror("accept");
            continue;
        }
        int gni = getnameinfo((SA *)&clientaddr, clientlen, client_hostname,
                              HOSTLEN, client_port, PORTLEN, 0);
        if (gni == 0) {
            printf("Accepted connection from: %s:%s\n", client_hostname,
                   client_port);
        } else {
            fprintf(stderr, "getnameinfo failed: %s\n", gai_strerror(gni));
        }
    }

    return 0;
}
