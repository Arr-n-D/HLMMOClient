#include <stdio.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Core/Core.hpp>
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#ifdef _WIN32
	#include <windows.h> // Ug, for NukeProcess -- see below
#else
	#include <unistd.h>
	#include <signal.h>
#endif

using namespace RC;
using namespace ArrND;

SteamNetworkingMicroseconds g_logTimeZero;

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

#ifdef STEAMNETWORKINGSOCKETS_OPENSOURCE
        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            FatalError("GameNetworkingSockets_Init failed.  %s", errMsg);
#else
        SteamDatagram_SetAppID(570); // Just set something, doesn't matter what
        SteamDatagram_SetUniverse(false, k_EUniverseDev);

        SteamDatagramErrMsg errMsg;
        if (!SteamDatagramClient_Init(errMsg))
            FatalError("SteamDatagramClient_Init failed.  %s", errMsg);

        // Disable authentication when running with Steam, for this
        // example, since we're not a real app.
        //
        // Authentication is disabled automatically in the open-source
        // version since we don't have a trusted third party to issue
        // certs.
        SteamNetworkingUtils()->SetGlobalConfigValueInt32(k_ESteamNetworkingConfig_IP_AllowWithoutAuth, 1);
#endif

        g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

        SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);
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

    static void NukeProcess(int rc)
    {
#ifdef _WIN32
        ExitProcess(rc);
#else
        (void)rc; // Unused formal parameter
        kill(getpid(), SIGKILL);
#endif
    }

    static void DebugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char *pszMsg)
    {
        SteamNetworkingMicroseconds time = SteamNetworkingUtils()->GetLocalTimestamp() - g_logTimeZero;
        printf("%10.6f %s\n", time * 1e-6, pszMsg);
        fflush(stdout);
        if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
        {
            fflush(stdout);
            fflush(stderr);
            NukeProcess(1);
        }
    }

    static void FatalError(const char *fmt, ...)
    {
        char text[2048];
        va_list ap;
        va_start(ap, fmt);
        vsprintf_s(text, fmt, ap);
        va_end(ap);
        char *nl = strchr(text, '\0') - 1;
        if (nl >= text && *nl == '\n')
            *nl = '\0';
        DebugOutput(k_ESteamNetworkingSocketsDebugOutputType_Bug, text);
    }
};

#define MY_AWESOME_MOD_API __declspec(dllexport)
extern "C"
{
    MY_AWESOME_MOD_API RC::CppUserModBase *start_mod()
    {
        return new HLMMOClient();
    }

    MY_AWESOME_MOD_API void uninstall_mod(RC::CppUserModBase *mod)
    {
        delete mod;
    }
}
