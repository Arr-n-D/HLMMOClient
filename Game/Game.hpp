#pragma once

#include <Unreal/AActor.hpp>

using namespace RC;
using namespace RC::Unreal;
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
        AActor* player = nullptr;
            // networking class right here
    };

}