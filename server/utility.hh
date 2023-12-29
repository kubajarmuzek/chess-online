#pragma once 

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



