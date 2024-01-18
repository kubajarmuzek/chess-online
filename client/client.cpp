#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_addr.s_addr = inet_addr(server_ip);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(server_port);

    if (connect(socketFD, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    {
        perror("connect failed");
        close(socketFD);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t n;

    // Read and print the initial greeting message and socket descriptor
    n = read(socketFD, buffer, sizeof(buffer) - 1);
    if (n > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    // Infinite loop for reading and sending messages
    while (1)
    {
        n = read(socketFD, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer);

            // If the message contains the player's own socket descriptor, send a move
            if (strstr(buffer, "It's your turn"))
            {
                char move[256];
                printf("Enter your move: ");
                fgets(move, sizeof(move), stdin);
                write(socketFD, move, strlen(move));
            }
        }
    }

    close(socketFD);
    return EXIT_SUCCESS;
}
