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

    void NetworkManager::Start() {
        this->m_pInterface = SteamNetworkingSockets();
        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void *)NetworkManager::OnSteamNetConnectionStatusChanged);
        SteamNetworkingIPAddr serverAddr;

        // connect to port 27015 on IP 0.0.0.0 (localhost)
        serverAddr.Clear();
        serverAddr.SetIPv6LocalHost(27015);
        m_hConnection = m_pInterface->ConnectByIPAddress( serverAddr, 1, &opt );
        if ( m_hConnection == k_HSteamNetConnection_Invalid )
			FatalError( "Failed to create connection" );
        
    }

    void NetworkManager::Update(float deltaFloat)
    {
        this->OnUpdate(deltaFloat);
    }

    void NetworkManager::OnUpdate(float deltaFloat)
    {
    }
}