// include core.h header
#include "core.hpp"

namespace ArrND::Core
{
    int foundPlayerHowManyTimes = 0;

    Core::Core() {
        Output::send<LogLevel::Verbose>(STR("Core initialized"));
    }

    Core::~Core() {
    }

    void Core::GetPlayerInstanceFromGame() {
        auto prehook = [](Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            //do nothing
        };

        auto posthook = [this](Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            auto player = UObjectGlobals::FindFirstOf(STR("Biped_Player"));

            //if not null ptr 
            if (player != nullptr && !player->IsUnreachable()) {
                //increase the number of times we found the player
                foundPlayerHowManyTimes++;

                if (foundPlayerHowManyTimes == 2) {
                    this->SetPlayer((AActor*)Player);
                    foundPlayerHowManyTimes = 0;
                }

            }
        };

        Unreal::UObjectGlobals::RegisterHook(STR("/Script/Engine.PlayerController:ClientRestart"), prehook, posthook, nullptr);
    }
}