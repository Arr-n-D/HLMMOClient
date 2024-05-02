#include "Game.hpp"

#include <chrono>

using namespace std::chrono;

namespace ArrND::Core
{
    Game::Game(AActor *player)
    {
        this->player = player;
        this->Start();
    }

    Game::~Game()
    {
    }

    void Game::Start()
    {
        auto previousTime = high_resolution_clock::now();
        double deltaTime = 0.0;

        // Game loop
        while (true) {
            auto currentTime = high_resolution_clock::now();
            auto elapsedTime = duration_cast<duration<double>>(currentTime - previousTime).count();
            previousTime = currentTime;

            this->Update(elapsedTime);
            deltaTime = elapsedTime;
        }
    }

    void Game::Update(float deltaFloat)
    {
        this->OnUpdate(deltaFloat);
        // Call OnUpdate
    }
    void Game::OnUpdate(float deltaFloat)
    {
    }
}