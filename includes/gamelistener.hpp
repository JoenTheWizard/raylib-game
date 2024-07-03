#ifndef GAMELISTENER_H
#define GAMELISTENER_H

//The game client's struct and array
#define MAX_CLIENTS 3

//TODO
//[*] Clean the code below (put these functions on separate respective header files/classes?)

typedef struct {
    int id;
    float x; float y;
} Client;

Client clients[MAX_CLIENTS];

//Function to draw the text on top of other players connected to server  
inline void DrawPlayerIdText(int playerId, Rectangle playerRect, int fontSize = 20) {
    //Measure the text width
    const char* text = TextFormat("Player %d", playerId);
    int textWidth    = MeasureText(text, fontSize);

    //Calculate the center of the player
    Vector2 textPosition = {
        playerRect.x + (playerRect.width / 2) - (textWidth / 2),
        playerRect.y - fontSize //Set text height position 
    };

    //Draw the text
    DrawText(text, textPosition.x, textPosition.y, fontSize, WHITE);
}

//Initialize the clients array
void initialize_clients(void) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].id = -1;
    }
}

//Render the other clients from clients array
inline void render_clients_pos(Player& player) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].id == -1) continue;
        //Draw player(s) client via Rectangle
        Rectangle playerRect = (Rectangle){clients[i].x, clients[i].y, player.width, player.height};
        DrawRectangleRec(playerRect, RED);
        //Draw text om top of player 
        DrawPlayerIdText(clients[i].id, playerRect); 
    }
}

//Listener thread 
void listener_thread(sockaddr_in serverAddr, int sockfd, std::atomic<bool>& stopListening) {
    char msg[40];
    socklen_t serverAddrLen = sizeof(serverAddr);

    while (!stopListening.load()) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        struct timeval timeout;
        timeout.tv_sec  = 0;
        timeout.tv_usec = 100000;  //Set the timeout to 100 milliseconds

        int ready = select(sockfd + 1, &fds, NULL, NULL, &timeout);

        if (ready > 0) {
            ssize_t len = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
            msg[len] = '\0';

            if (len != -1) {
                //std::cout << msg << std::endl;
                //If we get position update packet then update array
                int id; float x, y;
                if (sscanf(msg, "%d:%f,%f", &id, &x, &y) == 3) {
                    clients[id].id = id; //Constantly sets the id (need to change)
                    clients[id].x  = x; 
                    clients[id].y  = y; 
                }

                //On client Disconnect
                int disconnectedClientId;
                if (sscanf(msg, "Disconnect:%d", &disconnectedClientId)) {
                    //Shift elements to the left starting from the removed client index
                    for (int i = 0; i < MAX_CLIENTS; i++) {
                        if (clients[i].id == disconnectedClientId) {
                            clients[i].id = -1;
                            break;
                        }
                    }
                }

            }
        }
    }
}

#endif // GAMELISTENER_H