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

//Clients struct - store the address, ID and position of each client
typedef struct {
    struct sockaddr_in addr; uint16_t port;
    int id;
    float x; float y;
} Client;

//This array stores all the clients and their respective IDs. The indexes will change with connections
//and disconnections using the IDs (IDs = index)
Client clients[MAX_CLIENTS];

int main(int argc, char** argv) {

    //Store the number of clients
    int num_of_clients = 0;

    //Program requires to supply port number
    if (argc < 2) {
        std::cerr << "Usage: ./server <port-number>" << std::endl; 
        return -1;
    }

    //All clients ID initialize to ID = -1
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].id = -1;
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

    char clientName[INET_ADDRSTRLEN];

    while (true) {
        //Obtain packet
        ssize_t packet = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr*)&clientAddress, &clientAdr_len);
        buffer[packet] = '\0';

        //Clear terminal screen
        std::cout << "\033c";

        //On received valid packet
        if (packet > 0) {
            //Print the packet received
            inet_ntop(AF_INET, &(clientAddress.sin_addr), clientName, INET_ADDRSTRLEN);
            std::cout << "Received ("  << clientName << ":" << ntohs(clientAddress.sin_port) << ") - " << buffer <<  std::endl;

            //If we get position update packet then update array
            int id; float x, y;
            if (sscanf(buffer, "%d:%f,%f", &id, &x, &y) == 3) {
                clients[id].x = x; 
                clients[id].y = y; 
            }

            //On client Connect
            if (!strcmp("Connect", buffer)) {
                //Send the player ID to the connected user
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].id == -1) {
                        //Send the ID to client
                        char msg[12];
                        sprintf(msg, "playerID:%d", i);
                        sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&clientAddress, clientAdr_len);
                        //Send the positions of all other connected clients when the new client connects
                        for (int j = 0; j < MAX_CLIENTS; j++) {
                            if (clients[j].id == -1) continue;
                            char pos[40];
                            sprintf(pos, "%d:%f,%f", clients[j].id, clients[j].x, clients[j].y);
                            sendto(sockfd, pos, strlen(pos), 0, (struct sockaddr*)&clientAddress, clientAdr_len);
                        }
                        //Assign the new client in the clients array
                        clients[i].id = i;
                        clients[i].port = ntohs(clientAddress.sin_port);
                        memcpy(&clients[i].addr, &clientAddress, sizeof(clientAddress));
                        //Increment number of clients since client has connected
                        num_of_clients++;
                        break;
                    }
                }
            }

            //On client Disconnect
            int disconnectedClientId;
            if (sscanf(buffer, "Disconnect:%d", &disconnectedClientId)) {
                //When a client disconnects, only the id is set to -1 and num_of_clients is decremented
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].id == disconnectedClientId) {
                        clients[i].id = -1;
                        num_of_clients--;
                        break;
                    }
                }
            }

            //Number of clients connected
            std::cout << "Clients: " << num_of_clients << std::endl;

            //Print the clients and broadcast (using unicast method)
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].id == -1) continue;
                std::cout << "Player" << clients[i].id << ": (" << clients[i].x << "," << clients[i].y << ")" << std::endl; 
                //Broadcast here
                if (memcmp(&clients[i].addr, &clientAddress, clientAdr_len) != 0)
                    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
            }
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