#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

const int MAX_CLIENTS = 3;

int main(int argc, char** argv) {

    //Store the number of clients
    int num_of_clients = 0;

    //Program requires to supply port number
    if (argc < 2) {
        std::cerr << "Usage: ./server <port-number>" << std::endl; 
        return -1;
    }

    //Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //Construct server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(std::stoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // TEST

    char buffer[256];
    struct sockaddr_in clientAddress;
    socklen_t clientAdr_len = sizeof(clientAddress);

    while (true) {
        //Obtain packet
        ssize_t packet = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr*)&clientAddress, &clientAdr_len);
        buffer[packet] = '\0';

        //Clear terminal screen
        std::cout << "\033c";

        if (packet > 0) {
            std::cout << "Received: " << buffer << std::endl;
        }
        else {
            std::cerr << "Error receiving message" << std::endl;
        }
    }

    // END TEST

    //Remove socket
    close(sockfd);

    return 0;
}