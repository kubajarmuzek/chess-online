#pragma once 

#define TRUE   1  
#define FALSE  0
#define TIMEOUT_SEC 1

const int max_pending=32;


int make_reusable(int fd) {
    const int one = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
}

int make_nonblocking(int fd) {
    const int one = 1;
    return ioctl(fd, FIONBIO, &one);
}

inline void error(const char *message) {
	fprintf(stderr, "[error]: %s\n", message);
	exit(1);
}

void send_to_client(int client_socket, const char *message) {
    send(client_socket, message, strlen(message), 0);
}

