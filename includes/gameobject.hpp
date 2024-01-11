#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "raylib.h"

class GameObject {
public:
    float x, y, width, height;
    float velocityX = 0, velocityY = 0;
    Color color;
    Texture2D* texture;
    //bool canJump = false, onSurface = false;

    GameObject(float x, float y, float width, float height, Color color, Texture2D* texture = nullptr)
        : x(x), y(y), width(width), height(height), color(color), texture(texture) {}

    void draw() const;

    float centerX() const { return x + width * 0.5f; }
    float centerY() const { return y + height * 0.5f; }
    float bottom() const { return y + height; }
    float left() const { return x; }
    float right() const { return x + width; }
    float top() const { return y; }

    void applyGravity(float gravityConst, float delta);

    bool testCollision(const GameObject& other) const;

    virtual void resolveCollision(const GameObject& other);
};

#endif