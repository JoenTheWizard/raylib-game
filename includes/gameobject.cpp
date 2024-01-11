#include "gameobject.hpp"

void GameObject::draw() const {
    if (texture != nullptr) {
        //This doesn't check the width and height of the input image
        Vector2 position = {x, y};
        DrawTextureV(*texture, position, WHITE);
    } else {
        Rectangle playerRect = (Rectangle){x, y, width, height};
        DrawRectangleRec(playerRect, color);
    }
}

void GameObject::applyGravity(float gravityConst, float delta) {
    y += velocityY * delta;
    velocityY += gravityConst * delta;
}

bool GameObject::testCollision(const GameObject& other) const {
    return !(top() > other.bottom() || right() < other.left() || bottom() < other.top() || left() > other.right());
}

void GameObject::resolveCollision(const GameObject& other) {
    float vector_x = centerX() - other.centerX();
    float vector_y = centerY() - other.centerY();

    if (vector_y * vector_y > vector_x * vector_x) {
        if (vector_y > 0) {
            y = other.bottom();
            //canJump = false;
            //velocityY = -velocityY;
        } else {
            y = other.top() - height;
            //velocityY = 0;
            //canJump = true; onSurface = true;
        }
    } else {
        if (vector_x > 0) {
            x = other.right();
            //velocityX = 0;
            //canJump = false;
        } else {
            x = other.left() - width;
            //velocityX = 0;
            //canJump = false;
        }
    }
}