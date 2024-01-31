#ifndef PLAYER_CLIENT_H
#define PLAYER_CLIENT_H

#include "player.hpp"
#include "gameclient.hpp"

class PlayerClient : public Player {
public:
    //Initialize the game client
    GameClient client;
    //Constructor and destructor
    PlayerClient(float x, float y, float width, float height, Color color, const char* server_ip, uint16_t server_port);
    ~PlayerClient();

    //The player client will send position of the playeer to the server
    void UpdatePlayer(std::vector<GameObject>& tiles, float dt) override;
};

#endif