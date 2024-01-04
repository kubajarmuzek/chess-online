#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/ioctl.h>

#include "utility.hh"
#include "game.hh"

int main(int argc, char **argv) {
    if(argc < 2) {
        error("No port provided\n");
    }

    int port = atoi(argv[1]);
    
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    int server = socket(AF_INET,SOCK_STREAM,0);
    if(server < 0) {
        error("Could not create socket/n");
    }

    if (make_reusable(server) < 0) {
        error("Could not make socket reusable/n");
    }

    //if (make_nonblocking(server) < 0){
       //error("Could not make socket non-blocking/n");
    //}

    sockaddr_in addr{AF_INET, htons(port), {INADDR_ANY}};

    if (bind(server, (sockaddr*) &addr, sizeof(addr)) < 0) {
        error("Could not bind socket/n");
    }

    if (listen(server, 3) < 0) {
        error("could not listen on socket");
    }

    printf("[info]: listening on port %d\n", port);

    int client_socket[2];
    int max_clients=2;

    for(int i=0;i<max_clients;i++) {
        client_socket[i]=0;
    }

    for (int i=0;i<2;i++) {
        int ConnectFD = accept(server, nullptr, nullptr);
        char buff[256];
        int n;
        n= read(ConnectFD,buff,sizeof(buff));

        printf("message is : %s\n",buff);
        printf("message was %i\n",n);
  
        close(ConnectFD);
    }

    close(server);
    return EXIT_SUCCESS; 

}
