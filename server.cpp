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

int main(int argc, char **argv) {
    if(argc < 2) {
        error("No port provided\n");
    }

    int port = atoi(argv[1]);
    int valread,activity,sd,max_sd;   
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

    int client_socket[2],new_socket;
    int max_clients=2;

    for(int i=0;i<max_clients;i++) {
        client_socket[i]=0;
    }

    int addrlen=sizeof(addr);
    fd_set readfds;   
    char *message = "ECHO Daemon v1.0 \r\n";   
    char buffer[1025];
    while(TRUE)   
    {   
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(server, &readfds);   
        max_sd = server;   
             
        //add child sockets to set  
        for (int i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            error("select error\n");   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(server, &readfds))   
        {   
            if ((new_socket = accept(server,  
                    (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept error\n");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(addr.sin_addr) , ntohs 
                  (addr.sin_port));   
           
            //send new connection greeting message  
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            {   
                error("send error\n");   
            }   
                 
            puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets  
            for (int i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            }   
        }   
             
        //else its some IO operation on some other socket 
        for (int i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {   
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&addr, \ 
                        (socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(addr.sin_addr) , ntohs(addr.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    client_socket[i] = 0;   
                }   
                     
                //Echo back the message that came in  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    buffer[valread] = '\0';
                    printf("Incoming message was %s\n",buffer);   
                    send(sd , buffer , strlen(buffer) , 0 );   
                }   
            }   
        }
    }
    close(server);
    return EXIT_SUCCESS; 

}