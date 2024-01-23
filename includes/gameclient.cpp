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
    sprintf(buffer, "%d:%f,%f", playerID, x, y);
    ssize_t len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    return len;
}

ssize_t GameClient::send_connect() {
    //Send connect over UDP
    char buffer[] = "Connect";
    ssize_t len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    return len;
}

ssize_t GameClient::send_disconnect() {
    //Send connect over UDP
    char buffer[] = "Disconnect";
    ssize_t len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    return len;
}

int GameClient::assign_id() {
    char msg[12];
    socklen_t serverAddrLen = sizeof(serverAddr);
    ssize_t len = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
    msg[len] = '\0';

    if (len != -1) {
        if (sscanf(msg, "playerID:%d", &playerID) == 1) {
            std::cout << "Assigned process ID:"  << playerID << std::endl;
        }
    }
    return 0;
}

void GameClient::close_socket() {
    close(sockfd);
}