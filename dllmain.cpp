#include <stdio.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Core/Core.hpp>

using namespace RC;
using namespace ArrND;

class HLMMOClient : public RC::CppUserModBase
{
public:
   Core::Core *coreInstance = new Core::Core();

    HLMMOClient() : CppUserModBase()
    {
        ModName = STR("HLMMOClient");
        ModVersion = STR("1.0");
        ModDescription = STR("This is my awesome mod");
        ModAuthors = STR("UE4SS Team");
        
    }

    ~HLMMOClient() override
    {
        delete this->coreInstance;
    }

    auto on_update() -> void override
    {
       
    }

    auto on_unreal_init() -> void override
    {
        this->coreInstance->OnUnrealInit();
    }

};

#define MY_AWESOME_MOD_API __declspec(dllexport)
extern "C"
{
    MY_AWESOME_MOD_API RC::CppUserModBase* start_mod()
    {
        return new HLMMOClient();
    }

    MY_AWESOME_MOD_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}
