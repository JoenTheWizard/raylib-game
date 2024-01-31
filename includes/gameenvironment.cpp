#include "gameenvironment.hpp"

GameEnvironment::GameEnvironment() {
  stone = LoadTexture("resources/stoneblock.png");
  dirt = LoadTexture("resources/dirt.png");
  grass = LoadTexture("resources/grass.png");
}

//By default, we will make the environment blocks be 50 x 50 (same size as player)
//For some reason the grass texture can flicker when resizing the height of the window
//Will make it take defined arguments later
std::vector<GameObject> GameEnvironment::draw_perlin_continous(Player& player) {
  std::vector<GameObject> gameEnvironment;
  int l = player.x - 350;
  int x = l - (l % 50);

  //Loop through the side to append blocks
  for (int i = 0; i < 16; i++) {
    float p2d = perlin2d(x/50, 0, 0.3, 4) * 200;
    float height = p2d - fmod(p2d, 50);

    //Place grass on top
    gameEnvironment.push_back(GameObject(x, height, 50, 50, DARKGRAY, &grass));

    //Then dirt underneath the grass
    for (int j = height + 50; j < 400; j += 50) {
      gameEnvironment.push_back(GameObject(x, j, 50, 50, DARKGRAY, &dirt));
    }
    
    //Remember blocks are 50 blocks wide (50x50) so increment x/width by 50
    x += 50;
  }

  return gameEnvironment;
}

//Will make it take defined arguments later
std::vector<GameObject> GameEnvironment::draw_tile_index() {
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
        gameEnvironment.push_back(GameObject(x, y, 50, 50, WHITE, &stone));
      x += 50;
    }
    y += 50;
    x = 0;
  }
 
  return gameEnvironment;
}

void GameEnvironment::clean(void) {
  UnloadTexture(stone);
  UnloadTexture(dirt);
  UnloadTexture(grass);
}
