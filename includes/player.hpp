#ifndef PLAYER_H
#define PLAYER_H
#include "gameobject.hpp"

class Player : public GameObject {
public:
    //Player properties
    bool canJump = false, onSurface = false;
    Player(float x, float y, float width, float height, Color color)
       : GameObject(x, y, width, height, color) {}

    void resolveCollision(const GameObject& other) override {
        float vector_x = centerX() - other.centerX();
        float vector_y = centerY() - other.centerY();

        if (vector_y * vector_y > vector_x * vector_x) {
            if (vector_y > 0) {
                y = other.bottom();
                canJump = false;
                velocityY = -velocityY;
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

#endif