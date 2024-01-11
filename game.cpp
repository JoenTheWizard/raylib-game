#include <iostream>
#include <vector>
#include <math.h>

#include "includes/perlin.h"
#include "includes/gameobject.hpp"
#include "includes/player.hpp"

//Draw the environment. This should be done in the render loop for now
std::vector<GameObject> drawEnvironment(GameObject& player, Texture2D* texture);

int main(int argc, char* argv[]) {
    //Screen width and height
    const int screenWidth = 650;
    const int screenHeight = 750;

    //Initialize the Raylib (GLFW) with resizable window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Infinite Terrain Game");

    //Player
    Player player(0, -150, 50, 50, RED);
    
    //Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.centerX(), player.centerY() };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    //Block texture
    Texture2D block_texture = LoadTexture("resources/stoneblock.png");

    //Set Target frames
    SetTargetFPS(60);

    //Game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        //Obstacle
        std::vector<GameObject> tiles = drawEnvironment(player, &block_texture);

        //Update
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

    //Deallocate
    UnloadTexture(block_texture);

    //Close window on exit
    CloseWindow();

    return 0;
}

std::vector<GameObject> drawEnvironment(GameObject& player, Texture2D* texture) {
  std::vector<GameObject> gameEnvironment;
  int l = player.x - 350;
  int x = l - (l % 50);

  //std::cout << player.x << " " << x << std::endl;

  for (int i = 0; i < 16; i++) {
    float p2d = perlin2d(x/50, 0, 0.3, 4) * 200;
    for (int j = p2d - fmod(p2d, 50); j < 400; j += 50) {
      gameEnvironment.push_back(GameObject(x, j, 50, 50, DARKGRAY, texture));
    }
    x += 50;
  }

  return gameEnvironment;
}
