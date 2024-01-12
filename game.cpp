#include <iostream>
#include "includes/gameenvironment.hpp"

int main(int argc, char* argv[]) {
    //Screen width and height
    const int screenWidth = 650;
    const int screenHeight = 750;

    //Initialize the Raylib (GLFW) with resizable window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Block Game");

    //Player
    Player player(0, -150, 50, 50, RED);
    
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

        //Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);

            //Initialize the camera
            BeginMode2D(camera);
            
            //Draw player
            player.draw();
            
            //Draw all the obstacles
            for (const auto& tile : tiles) {
                tile.draw();
            }
        EndDrawing();
    }

    //Close window on exit
    CloseWindow();

    return 0;
}
