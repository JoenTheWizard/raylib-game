#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

//Game client will send the position of the player and maybe other information using UDP (SOCK_DGRAM).
//Things this is currently lacking are: tickrate and prevention from MiTM attacks.
class GameClient {
public:
    //Initialize socket
    int sockfd; struct sockaddr_in serverAddr;
    //Player is assigned an ID
    int playerID = -1;
    //Pass in server IP and port number
    GameClient(const char* ip_address, uint16_t port);
    
    //Send position (x, y) coordinates of the client to server
    ssize_t send_position(float x, float y);

    //Send connection and disconnect requests to the server
    ssize_t send_connect();
    ssize_t send_disconnect();

    //Assign ID from the server (serves as player index). Currently using recvfrom() which is blocking
    //the current thread
    int assign_id();

    //Close socket
    void close_socket();
};

#endif