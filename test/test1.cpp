#include "raylib.h"
#include <iostream>
#include <vector>

#define PLAYER_SPEED 300.f
#define PLAYER_JUMP_SPEED 250.f
#define GRAVITY 500.f

class GameObject {
public:
    float x, y, width, height;
    float velocityX = 0, velocityY = 0;
    Color color;
    bool canJump = false, onSurface = false;

    GameObject(float x, float y, float width, float height, Color color)
        : x(x), y(y), width(width), height(height), color(color) {}

    void draw() const {
        Rectangle playerRect = (Rectangle){x, y, width, height};
        DrawRectangleRec(playerRect, color);
    }

    float centerX() const { return x + width * 0.5f; }
    float centerY() const { return y + height * 0.5f; }
    float bottom() const { return y + height; }
    float left() const { return x; }
    float right() const { return x + width; }
    float top() const { return y; }

    void applyGravity(float gravityConst, float delta) {
        y += velocityY * delta;
        velocityY += gravityConst * delta;
    }

    bool testCollision(const GameObject& other) const {
        return !(top() > other.bottom() || right() < other.left() || bottom() < other.top() || left() > other.right());
    }

    void resolveCollision(const GameObject& other) {
        float vector_x = centerX() - other.centerX();
        float vector_y = centerY() - other.centerY();

        if (vector_y * vector_y > vector_x * vector_x) {
            if (vector_y > 0) {
                y = other.bottom();
                canJump = false;
                //velocityY = -velocityY;
            } else {
                y = other.top() - height;
                velocityY = 0;
                canJump = true; onSurface = true;
            }
        } else {
            if (vector_x > 0) {
                x = other.right();
                velocityX = 0;
                canJump = false;
            } else {
                x = other.left() - width;
                velocityX = 0;
                canJump = false;
            }
        }
    }
};

//Update the player. This will be done in the render loop
void UpdatePlayer(GameObject& player, std::vector<GameObject>& tiles, float dt);
//Draw the environment. This should be done before the render loop
std::vector<GameObject> drawEnvironment();

int main(int argc, char* argv[]) {
    //Screen width and height
    const int screenWidth = 650;
    const int screenHeight = 750;

    //Initialize the Raylib (GLFW) window
    InitWindow(screenWidth, screenHeight, "Small Game");

    //Player
    GameObject player(350, 0, 50, 50, RED);
    
    //Obstacle
    std::vector<GameObject> tiles = drawEnvironment();

    //Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.centerX(), player.centerY() };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    //Set Target frames
    SetTargetFPS(60);

    //Game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        //Update
        UpdatePlayer(player, tiles, dt);
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

void UpdatePlayer(GameObject& player, std::vector<GameObject>& tiles, float dt) {
  if (IsKeyDown(KEY_D)) player.x += PLAYER_SPEED * dt;
  if (IsKeyDown(KEY_A)) player.x -= PLAYER_SPEED * dt;
  if (IsKeyPressed(KEY_SPACE) && player.canJump && player.onSurface) {
    player.velocityY -= PLAYER_JUMP_SPEED;
    player.canJump = false;
  }

  player.applyGravity(GRAVITY, dt);

  //Test Collision
  player.onSurface = false;        
  for (const auto& tile : tiles) {
    if (player.testCollision(tile)) {
        player.resolveCollision(tile);
    }
  }
  //Check if player is on surface
  if (player.onSurface)
    player.canJump = true;
}

std::vector<GameObject> drawEnvironment() {
  int env[16][13] = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
   {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}
  };
 
  std::vector<GameObject> gameEnvironment;
  int x = 0, y = 0;

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 13; j++) {
      if (env[i][j])
        gameEnvironment.push_back(GameObject(x, y, 50, 50, GRAY));
      x += 50;
    }
    y += 50;
    x = 0;
  }
 
  return gameEnvironment;
}