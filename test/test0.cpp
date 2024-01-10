#include "raylib.h"
#include <iostream>

#define PLAYER_SPEED 300.f
#define GRAVITY 200.f

typedef struct Player {
   Vector2 position;
   float speed;
   bool isJumping;
   //Other properties
   Color color;
   float size;
} Player;

int main() {
   //Initialization
   const int screenWidth = 800;
   const int screenHeight = 450;

   InitWindow(screenWidth, screenHeight, "Simple Raylib game");

   //Define the floor properties
   float floorY = screenHeight - 50; //Position the floor at the bottom of the screen
   float floorX = 0; //Start at the left edge of the screen
   float floorWidth = screenWidth; //Extend across the full width of the screen
   float floorHeight = 50; //Make the floor 50 pixels high
   Color floorColor = LIME; //Lime color floor
  
   //Player properties
   Player player = { 0 };
   player.position = { 50.0f, 50.0f }; //Starting position of the player
   player.speed = 0.f; //Player initial speed
   player.isJumping = false; //Player jumping condition
   player.size = 30.0f; //Size of the player
   player.color = RED; //Color of the player
    
   //Floor properties
   Rectangle floorBounds = { floorX, floorY, floorWidth, floorHeight };

   SetTargetFPS(60); // Set our game to run at 60 frames-per-second

   //Main game loop: Detect window close button or ESC key
   while (!WindowShouldClose()) {
      //Draw
      BeginDrawing();

      //Delta time
      float dt = GetFrameTime();

      //Blue background
      ClearBackground(SKYBLUE);
   
      //Draw floor
      DrawRectangle(floorX, floorY, floorWidth, floorHeight, floorColor);

      Rectangle playerBounds = { player.position.x, player.position.y, player.size, player.size };;
      //Update the player position based on keyboard input
      if (IsKeyDown(KEY_A)) player.position.x -= PLAYER_SPEED * dt; // Move left
      if (IsKeyDown(KEY_D)) player.position.x += PLAYER_SPEED * dt; // Move right
      if (IsKeyDown(KEY_SPACE) && !player.isJumping) {
         player.speed = -200.f;
         player.isJumping = true;
      }
      //Check if the player collided with the floor
      else if (CheckCollisionRecs(playerBounds, floorBounds)) {
         //The player has hit the floor. Stop the player's downward movement and set isJumping to false.
         player.speed = 0.0f;
         player.position.y = floorY - player.size;
         player.isJumping = false;
      }

      player.position.y += player.speed * dt;
      player.speed += GRAVITY * dt;

      //Draw player
      DrawRectangle(player.position.x, player.position.y, player.size, player.size, player.color);

      EndDrawing();
   }

   // De-Initialization
   CloseWindow(); // Close window and OpenGL context

   return 0;
}
