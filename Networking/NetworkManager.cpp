#include "NetworkManager.hpp"
#include <assert.h>

namespace ArrND::Networking
{
    NetworkManager *NetworkManager::s_pCallbackInstance = nullptr;
    bool NetworkManager::g_bQuit = false;
    SteamNetworkingMicroseconds NetworkManager::g_logTimeZero;
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

    void NetworkManager::Start()
    {
        this->m_pInterface = SteamNetworkingSockets();
        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void *)SteamNetConnectionStatusChangedCallback);
        SteamNetworkingIPAddr serverAddr;

        // connect to port 27015 on IP 0.0.0.0 (localhost)
        serverAddr.Clear();
        serverAddr.SetIPv6LocalHost(27015);
        m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 1, &opt);
        if (m_hConnection == k_HSteamNetConnection_Invalid)
            FatalError("Failed to create connection");
    }

    void NetworkManager::Update(float deltaFloat)
    {
        this->OnUpdate(deltaFloat);
    }

    void NetworkManager::OnUpdate(float deltaFloat)
    {
    }

    void NetworkManager::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *pInfo)
    {
        assert(pInfo->m_hConn == m_hConnection || m_hConnection == k_HSteamNetConnection_Invalid);

        // What's the state of the connection?
        switch (pInfo->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            g_bQuit = true;

            // Print an appropriate message
            if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
            {
                // Note: we could distinguish between a timeout, a rejected connection,
                // or some other transport problem.
                printf_s("We sought the remote host, yet our efforts were met with defeat.  (%s)", pInfo->m_info.m_szEndDebug);
            }
            else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
            {
                printf_s("Alas, troubles beset us; we have lost contact with the host.  (%s)", pInfo->m_info.m_szEndDebug);
            }
            else
            {
                // NOTE: We could check the reason code for a normal disconnection
                printf_s("The host hath bidden us farewell.  (%s)", pInfo->m_info.m_szEndDebug);
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0's.
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            m_hConnection = k_HSteamNetConnection_Invalid;
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
            // We will get this callback when we start connecting.
            // We can ignore this.
            break;

        case k_ESteamNetworkingConnectionState_Connected:
            printf_s("Connected to server OK");
            break;

        default:
            // Silences -Wswitch
            break;
        }
    }
}