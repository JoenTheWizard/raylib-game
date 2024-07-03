#include <thread>
#include <atomic>
#include "includes/gameenvironment.hpp"
#include "includes/playerclient.hpp" //For client->server
#include "includes/gamelistener.hpp" //For server->client

//TODO (Client/Server)
//[*] Try implementing interpolation using tickrate
//[*] The client on the %d:%f,%f packet is constantly setting the ID which shouldn't be neccessary

int main(int argc, char* argv[]) {
    //Screen width and height
    const int screenWidth  = 650;
    const int screenHeight = 750;

    //Initialize the Raylib (GLFW) with resizable window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Block Game");

    //Player
    PlayerClient player(0, -150, 50, 50, RED, "127.0.0.1", 5656);
    
    //Atomic flag to signal the packet listener thread to stop
    std::atomic<bool> stopListening(false);
    //Start a new thread for listening to UDP packets
    std::thread listener(listener_thread, player.client.serverAddr, player.client.sockfd, std::ref(stopListening));

    //All clients ID initialize to ID = -1
    initialize_clients();

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
            
            //Render other clients connected to server
            render_clients_pos(player);

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
