#pragma once 

#include <iostream>
#include "msgpack.hpp"

enum PacketType {
    RPC = 0,
    GameMessage = 1,
};

struct Packet {
    uint8_t packetType; // 
    uint8_t gameMessage; 
    size_t size;
    std::string data;
    MSGPACK_DEFINE(packetType, gameMessage, size, data);
};

struct GameMessageDiscordAuthRequest {
    std::string redirectionUrl;
    MSGPACK_DEFINE(redirectionUrl);
};