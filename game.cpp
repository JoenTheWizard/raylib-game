#include <thread>
#include <atomic>
#include "includes/gameenvironment.hpp"
#include "includes/playerclient.hpp" //For client->server

//TODO (Client/Server)
//[*] Try implementing interpolation using tickrate
//[*] The client on the %d:%f,%f packet is constantly setting the ID which shouldn't be neccessary
//[*] Clean the code

//The game client's struct and array
const int MAX_CLIENTS = 3;
typedef struct {
    int id;
    float x; float y;
} Client;
Client clients[MAX_CLIENTS];

void listener_thread(sockaddr_in serverAddr, int sockfd, std::atomic<bool>& stopListening) {
    char msg[40];
    socklen_t serverAddrLen = sizeof(serverAddr);

    while (!stopListening.load()) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
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
                    clients[id].x = x; 
                    clients[id].y = y; 
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

int main(int argc, char* argv[]) {
    //Screen width and height
    const int screenWidth = 650;
    const int screenHeight = 750;

    //Initialize the Raylib (GLFW) with resizable window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Block Game");

    //Player
    PlayerClient player(0, -150, 50, 50, RED, "192.168.0.24", 5656);
    
    //Atomic flag to signal the packet listener thread to stop
    std::atomic<bool> stopListening(false);
    //Start a new thread for listening to UDP packets
    std::thread listener(listener_thread, player.client.serverAddr, player.client.sockfd, std::ref(stopListening));

    //All clients ID initialize to ID = -1
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].id = -1;
    }

    //Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.centerX(), player.centerY() };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    //Game environment
    GameEnvironment environment;

    //Set Target frames
    SetTargetFPS(60);

    //Game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        //Obstacle
        std::vector<GameObject> tiles = environment.draw_perlin_continous(player);

        //Player update
        player.UpdatePlayer(tiles, dt);
        //std::cout << player.onSurface << " " << player.canJump << std::endl;

        //Camera update
        camera.target = (Vector2){ player.centerX(), player.centerY() };
        camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        //Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);

            //Initialize the camera
            BeginMode2D(camera);
            
            //Draw player
            player.draw();
            
            //Render other clients
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].id == -1) continue;
                Rectangle playerRect = (Rectangle){clients[i].x, clients[i].y, player.width, player.height};
                DrawRectangleRec(playerRect, RED);
            }

            //Draw all the obstacles
            for (const auto& tile : tiles) {
                tile.draw();
            }
        EndDrawing();
    }

    //Signal the packet listener thread to stop
    stopListening.store(true);

    //Signal the packet listener thread to stop and wait for it to finish
    listener.join();

    //Clean and deallocate environment (remove textures)
    environment.clean();

    //Close window on exit
    CloseWindow();

    return 0;
}
