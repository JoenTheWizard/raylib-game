#include "player.hpp"

void Player::resolveCollision(const GameObject& other) {
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

void Player::UpdatePlayer(std::vector<GameObject>& tiles, float dt) {
  if (IsKeyDown(KEY_D)) x += PLAYER_SPEED * dt;
  if (IsKeyDown(KEY_A)) x -= PLAYER_SPEED * dt;
  if (IsKeyPressed(KEY_SPACE) && canJump && onSurface) {
    velocityY -= PLAYER_JUMP_SPEED;
    canJump = false;
  }

  applyGravity(GRAVITY, dt);

  //Test Collision
  onSurface = false;        
  for (const auto& tile : tiles) {
    if (testCollision(tile)) {
        resolveCollision(tile);
    }
  }
  //Check if player is on surface
  if (onSurface)
    canJump = true;
}