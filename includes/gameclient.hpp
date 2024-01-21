#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

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
    //Pass in server IP and port number
    GameClient(const char* ip_address, uint16_t port);
    
    //Send position (x, y) coordinates of the client to server
    ssize_t send_position(float x, float y);

    //Close socket
    void close_socket();
};

#endif