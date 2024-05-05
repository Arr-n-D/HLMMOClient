#include "NetworkManager.hpp"

namespace ArrND::Networking
{
    NetworkManager::NetworkManager()
    {
    }

    NetworkManager::~NetworkManager()
    {
    }

    void NetworkManager::Init()
    {
        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            FatalError("GameNetworkingSockets_Init failed.  %s", errMsg);

        g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

        SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Everything, DebugOutput);
    }

    void NetworkManager::Update(float deltaFloat)
    {
        this->OnUpdate(deltaFloat);
    }

    void NetworkManager::OnUpdate(float deltaFloat)
    {
    }
}