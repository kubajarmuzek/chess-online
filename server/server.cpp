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
#include <sys/time.h>
#include <string.h>

#include "utility.hh"
#include "game.hh"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        error("No port provided\n");
    }

    ChessGame chessGame;
    int port = atoi(argv[1]);
    int valread, activity, sd, max_sd;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0)
    {
        error("Could not create socket/n");
    }

    if (make_reusable(server) < 0)
    {
        error("Could not make socket reusable/n");
    }

    // if (make_nonblocking(server) < 0){
    // error("Could not make socket non-blocking/n");
    //}

    sockaddr_in addr{AF_INET, htons(port), {INADDR_ANY}};

    if (bind(server, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        error("Could not bind socket/n");
    }

    if (listen(server, 3) < 0)
    {
        error("could not listen on socket");
    }

    printf("[info]: listening on port %d\n", port);

    int client_socket[2], new_socket;
    int max_clients = 2;
    int connected_clients = 0; // New variable to track connected players

    for (int i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    int addrlen = sizeof(addr);
    fd_set readfds;

    char *message = "You successfully connected to the chess game\n";
    char *turn_message = "It's your turn\n";
    char *wait_message = "Wait for your turn\n";

    int current_turn = 0;
    char buffer[1025];
    while (TRUE)
    {
        // clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(server, &readfds);
        max_sd = server;

        // add child sockets to set
        for (int i = 0; i < max_clients; i++)
        {
            // socket descriptor
            sd = client_socket[i];

            // if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            // highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        if (connected_clients > 1)
        {
            // else its some IO operation on some other socket
            for (int i = 0; i < max_clients; i++)
            {
                sd = client_socket[i];
                if (i == current_turn && connected_clients % 2 == 0)
                {
                    // Process the move and switch to the other player's turn
                    // Send messages to both players
                    std::string messageWithBoard = chessGame.getBoardState() + "\n" + "Enter your move (startRow startCol destRow destCol): ";
                    send_to_client(sd, messageWithBoard.c_str());

                    int other_player = (i == 0) ? 1 : 0;
                    send_to_client(client_socket[other_player], wait_message);
                    valread = read(sd, buffer, 1024);
                    buffer[valread] = '\0';
                    printf("%s\n", buffer);

                    int startRow, startCol, destRow, destCol;
                    if (sscanf(buffer, "%d %d %d %d", &startRow, &startCol, &destRow, &destCol) == 4)
                    {
                        char piece = chessGame.getPieceAt(startRow, startCol);
                        if ((current_turn == 0 && islower(piece)) || (current_turn == 1 && isupper(piece)))
                        {
                            if (chessGame.makeMove(startRow, startCol, destRow, destCol))
                            {
                                current_turn = 1 - current_turn; // Toggle turn (0 to 1, 1 to 0)
                                send_to_client(sd, "Move accepted. It's your opponent's turn.\n");
                                int other_player = (i == 0) ? 1 : 0;
                                send_to_client(client_socket[other_player], "It's your turn.\n");

                                if (chessGame.isGameOver())
                                {
                                    send_to_client(client_socket[0], "Game over.\n");
                                    send_to_client(client_socket[1], "Game over.\n");
                                    close(server);
                                    return EXIT_SUCCESS;
                                }
                            }
                            else
                            {
                                // Inform the player about the invalid move
                                send_to_client(sd, "Invalid move. Try again.\n");
                            }
                        }
                        else
                        {
                            // Inform the player about the invalid move
                            send_to_client(sd, "Invalid move. Try again.\n");
                        }
                    }
                    else
                    {
                        // Inform the player about the invalid input format
                        send_to_client(sd, "Invalid input format. Enter your move in the format 'startRow startCol destRow destCol'.\n");
                    }
                }
                else
                {
                    // It's not the player's turn, send a message indicating that they should wait
                    send_to_client(sd, wait_message);
                }
            }
        }

        // wait for an activity on one of the sockets , timeout is NULL ,
        // so wait indefinitely
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;

        // wait for an activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);

        if ((activity < 0) && (errno != EINTR))
        {
            error("select error\n");
        }

        // If something happened on the master socket ,
        // then its an incoming connection
        if (FD_ISSET(server, &readfds))
        {
            if ((new_socket = accept(server,
                                     (struct sockaddr *)&addr, (socklen_t *)&addrlen)) < 0)
            {
                error("accept error\n");
            }

            // inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n", new_socket, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

            // send new connection greeting message
            if (send(new_socket, message, strlen(message), 0) != strlen(message))
            {
                error("send error\n");
            }

            // add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                // if position is empty
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    connected_clients++;

                    if (connected_clients == max_clients)
                    {
                        // Both players connected, notify them
                        send_to_client(client_socket[0], "Both players connected. Let the game begin!\n");
                        send_to_client(client_socket[1], "Both players connected. Let the game begin!\n");
                    }

                    break;
                }
            }
        }

        // else its some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                // Check if it was for closing , and also read the
                // incoming message
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr *)&addr,
                                (socklen_t *)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n",
                           inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

                    // Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                    connected_clients--;

                    // Notify the other player about disconnection
                    int other_player = (i == 0) ? 1 : 0;
                    send_to_client(client_socket[other_player], "Your opponent has disconnected. The game is over.\n");
                }
            }
        }
    }
    close(server);
    return EXIT_SUCCESS;
}
