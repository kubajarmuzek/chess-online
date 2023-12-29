#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/ioctl.h>

#include "utility.hh"

int main(int argc, char **argv) {
    int port = atoi(argv[1]);
    int server = socket(AF_INET,SOCK_STREAM,0);

    if(server < 0) {
        error("Could not create socket/n");
    }

    if (make_reusable(server) < 0) {
        error("Could not make socket reusable/n");
    }

    if (make_nonblocking(server) < 0){
        error("Could not make socket non-blocking/n");
    }

    sockaddr_in addr{AF_INET, htons(port), {INADDR_ANY}};

    if (bind(server, (sockaddr*) &addr, sizeof(addr)) < 0) {
        error("Could not bind socket/n");
    }

    if (listen(server, max_pending) < 0) {
        error("could not listen on socket");
    }

    printf("[info]: listening on port %d\n", port);



}
