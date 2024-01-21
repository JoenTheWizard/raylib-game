#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "gameobject.hpp"
#include "gameclient.hpp"

#define PLAYER_SPEED 300.f
#define PLAYER_JUMP_SPEED 250.f
#define GRAVITY 500.f

class Player : public GameObject {
public:
    //Player properties
    bool canJump = false, onSurface = false;
    //Game client for connection to server
    GameClient client;
    Player(float x, float y, float width, float height, Color color)
        : GameObject(x, y, width, height, color), client("127.0.0.1", 5656) {
        //Send this player's position information to the server
        client.send_position(x, y);
    }

    ~Player() {
        //Close the socket
        client.close_socket();
    }

    //Player resolving collision after test collision (resolves for jumping as well)
    void resolveCollision(const GameObject& other) override;
    //Update the player by applying controls and gravity. This will be done in the render loop
    void UpdatePlayer(std::vector<GameObject>& tiles, float dt);
};

#endif