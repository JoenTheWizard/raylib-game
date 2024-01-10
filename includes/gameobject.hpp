#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
public:
    float x, y, width, height;
    float velocityX = 0, velocityY = 0;
    Color color;
    Texture2D* texture;
    //bool canJump = false, onSurface = false;

    GameObject(float x, float y, float width, float height, Color color, Texture2D* texture = NULL)
        : x(x), y(y), width(width), height(height), color(color), texture(texture) {}

    void draw() const {
        if (texture != NULL) {
            //This doesn't check the width and height of the input image
            Vector2 position = {x, y};
            DrawTextureV(*texture, position, WHITE);
        } else {
            Rectangle playerRect = (Rectangle){x, y, width, height};
            DrawRectangleRec(playerRect, color);
        }
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

    virtual void resolveCollision(const GameObject& other) {
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
};

#endif