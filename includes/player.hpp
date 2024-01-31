#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "gameobject.hpp"

#define PLAYER_SPEED 300.f
#define PLAYER_JUMP_SPEED 250.f
#define GRAVITY 500.f

class Player : public GameObject {
public:
    //Player properties
    bool canJump = false, onSurface = false;
    Player(float x, float y, float width, float height, Color color)
        : GameObject(x, y, width, height, color) {}

    //Player resolving collision after test collision (resolves for jumping as well)
    void resolveCollision(const GameObject& other) override;
    //Update the player by applying controls and gravity. This will be done in the render loop
    virtual void UpdatePlayer(std::vector<GameObject>& tiles, float dt);
};

#endif