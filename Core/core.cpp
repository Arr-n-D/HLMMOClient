// include core.h header
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Hooks.hpp>
#include <Unreal/World.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UFunction.hpp>
#include "core.hpp"

using namespace RC;
using namespace RC::Unreal;
namespace ArrND::Core
{
    int foundPlayerHowManyTimes = 0;

    Core::Core()
    {
        Output::send<LogLevel::Verbose>(STR("Core initialized"));

    }

    Core::~Core()
    {
    }

    AActor* Core::GetPlayer()
    {
        return this->player;
    }

    void Core::GetPlayerInstanceFromGame()
    {
        auto prehook = [](Unreal::UnrealScriptFunctionCallableContext &Context, void *CustomData)
        {
            // do nothing
        };

        auto posthook = [this](Unreal::UnrealScriptFunctionCallableContext &Context, void *CustomData)
        {
            auto tempPlayer = UObjectGlobals::FindFirstOf(STR("Biped_Player"));

            // if not null ptr
            if (tempPlayer != nullptr && !tempPlayer->IsUnreachable())
            {
                // increase the number of times we found the player
                foundPlayerHowManyTimes++;

                if (foundPlayerHowManyTimes == 2)
                {
                    Output::send<LogLevel::Verbose>(STR("Player found"));
                    this->SetPlayer((AActor*) tempPlayer);
                    foundPlayerHowManyTimes = 0;
                }
            }
        };

        Unreal::UObjectGlobals::RegisterHook(STR("/Script/Engine.PlayerController:ClientRestart"), prehook, posthook, nullptr);
    }

    void Core::OnUnrealInit()
    {
        this->GetPlayerInstanceFromGame();
        this->bIsInitialized = true;

    }

    void Core::SetPlayer(AActor* player)
    {
		this->player = player;
	}

    bool Core::isInitialized()
    {
		return this->bIsInitialized;
	}
}