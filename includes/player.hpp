#ifndef PLAYER_H
#define PLAYER_H
#include "gameobject.hpp"

class Player : public GameObject {
public:
    //Player properties
    bool canJump = false, onSurface = false;
    Player(float x, float y, float width, float height, Color color)
       : GameObject(x, y, width, height, color) {}

    void resolveCollision(const GameObject& other) override;
};

#endif