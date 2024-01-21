#include "gameclient.hpp"

GameClient::GameClient(const char* ip_address, uint16_t port) {
    //Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    socklen_t clientAddrLen = sizeof(serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &(serverAddr.sin_addr)); 
}

ssize_t GameClient::send_position(float x, float y) {
    //Send rectangle position over UDP
    char buffer[100];
    sprintf(buffer, "%f,%f", x, y);
    ssize_t len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    return len;
}

void GameClient::close_socket() {
    close(sockfd);
}