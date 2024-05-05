#pragma once

#include <Unreal/AActor.hpp>
#include "Game\Game.hpp"


using namespace RC;
using namespace RC::Unreal;
using namespace ArrND::Game;
namespace ArrND::Core {
    class Core {
        public:
            Core();
            ~Core();
            AActor* GetPlayer();

            void GetPlayerInstanceFromGame();
            void SetPlayer(AActor* player);
            
            void OnUnrealInit();
            bool isInitialized();

            void SetGame(Game::Game* game);
            Game::Game* GetGame();
        private:
            AActor* player = nullptr;
            Game::Game* game = nullptr;
            bool bIsInitialized = false;
    };
}