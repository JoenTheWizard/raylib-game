#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H

#include <math.h>
#include "gameobject.hpp"
#include "player.hpp"
#include "perlin.h"

class GameEnvironment {
public:
  Texture2D dirt, stone; 
  GameEnvironment();
  ~GameEnvironment();

  //Draw the Perlin noise environment that draws relative to the current player's position. This should be done in the render loop for now
  std::vector<GameObject> draw_perlin_continous(Player& player);

  //Draw the environment based on array index. This can be done outside the render loop as it's a static environment that doesn't rely on player position
  std::vector<GameObject> draw_tile_index();

};

#endif // !DEBUG
