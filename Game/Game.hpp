#pragma once

#include <Unreal/AActor.hpp>
#include "Networking/NetworkManager.hpp"

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND::Networking;
namespace ArrND::Game
{
    class Game
    {
        public:
            Game(AActor* player);
            ~Game();
            void Start();
            void Update(float deltaFloat);
            void OnUpdate(float deltaFloat);

        private: 
        NetworkManager* networkManager = nullptr;
        AActor* player = nullptr;
            // networking class right here
    };

}