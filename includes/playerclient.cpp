#include "playerclient.hpp"

//In the constructor the player will connect to the server and send certain packets to the server to
//notify it that it has connected
PlayerClient::PlayerClient(float x, float y, float width, float height, Color color, const char* server_ip, uint16_t server_port) 
: Player(x,y,width,height,color), client(server_ip, server_port) {
    //Connect to server (send "Connect" UDP packet)
    client.send_connect();

    //After connecting assign an id to the client
    client.assign_id();

    //Then send the initialized position of the player
    client.send_position(x,y);
}

void PlayerClient::UpdatePlayer(std::vector<GameObject>& tiles, float dt) {
    float prev_x = x, prev_y = y;
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

    //Send rectangle position over UDP
    if (prev_x != x || prev_y != y) {
        client.send_position(x, y);
    }
}

//The destructor will disconnect the client (send "Disconnect" UDP) and deallocate socket
//Note: That this assumes that the server is still running and or still reachable
PlayerClient::~PlayerClient() {
    //Disconnect the client
    client.send_disconnect();

    //Close socket
    client.close_socket();
}