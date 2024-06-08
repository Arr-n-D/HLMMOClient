#include "NetworkManager.hpp"

#include <assert.h>
#include <shellapi.h>

#include <networking/network_types.hpp>

namespace ArrND::Networking {
NetworkManager *NetworkManager::s_pCallbackInstance = nullptr;
bool NetworkManager::g_bQuit = false;
SteamNetworkingMicroseconds NetworkManager::g_logTimeZero;
NetworkManager::NetworkManager() {
}

NetworkManager::~NetworkManager() {
}

void NetworkManager::Init() {
    SteamDatagramErrMsg errMsg;
    if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
        FatalError( "GameNetworkingSockets_Init failed.  %s", errMsg );

    g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

    SteamNetworkingUtils()->SetDebugOutputFunction( k_ESteamNetworkingSocketsDebugOutputType_Everything, DebugOutput );
}

void NetworkManager::Start() {
    this->m_pInterface = SteamNetworkingSockets();
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void *)SteamNetConnectionStatusChangedCallback );
    SteamNetworkingIPAddr serverAddr;

    // connect to port 27015 on IP 0.0.0.0 (localhost)
    serverAddr.Clear();
    // 127.0.0.1 in host byte order uint32
    serverAddr.SetIPv4(0x7F000001, 27015);
    m_hConnection = m_pInterface->ConnectByIPAddress( serverAddr, 1, &opt );
    if ( m_hConnection == k_HSteamNetConnection_Invalid )
        FatalError( "Failed to create connection" );
}

void NetworkManager::Update( float deltaFloat ) {
    this->OnUpdate( deltaFloat );
}

void NetworkManager::PollIncomingMessages() {
    while ( true ) {
        ISteamNetworkingMessage *pIncomingMsg = nullptr;
        int numMsgs = m_pInterface->ReceiveMessagesOnConnection( m_hConnection, &pIncomingMsg, 1 );
        if ( numMsgs == 0 )
            break;
        if ( numMsgs < 0 )
            FatalError( "Error checking for messages" );

        // Process the incoming message
        this->OnMessageReceived( pIncomingMsg );
    }
}

void NetworkManager::OnUpdate( float deltaFloat ) {
    this->PollIncomingMessages();
}

void NetworkManager::OnMessageReceived( ISteamNetworkingMessage *pMsg ) {
    //
    msgpack::unpacked h = msgpack::unpack( (const char *)pMsg->m_pData, (size_t)pMsg->m_cbSize );
    msgpack::object const &obj = h.get();

    Packet packet = obj.as<Packet>();
    // print packet data type and game message
    printf_s( "Packet Type: %d\n", packet.packetType );
    printf_s( "Game Message: %d\n", packet.gameMessage );

    // unserialize packet data to GameMessageDiscordAuthRequest

    msgpack::unpacked h2 = msgpack::unpack( (const char *)packet.data.data(), packet.size );
    msgpack::object const &obj2 = h2.get();

    GameMessageDiscordAuthRequest gameMessageDiscordAuthRequest = obj2.as<GameMessageDiscordAuthRequest>();

    // print redirection url
    printf_s( "Redirection URL: %s\n", gameMessageDiscordAuthRequest.redirectionUrl.c_str() );
    std::string url = gameMessageDiscordAuthRequest.redirectionUrl;

    // ShellExecuteA with the URL
    ShellExecuteA( 0, 0, url.c_str(), 0, 0, SW_SHOW );

    pMsg->Release();
}

void NetworkManager::OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo ) {
    assert( pInfo->m_hConn == m_hConnection || m_hConnection == k_HSteamNetConnection_Invalid );

    // What's the state of the connection?
    switch ( pInfo->m_info.m_eState ) {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: {
            g_bQuit = true;

            // Print an appropriate message
            if ( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting ) {
                // Note: we could distinguish between a timeout, a rejected connection,
                // or some other transport problem.
                printf_s( "We sought the remote host, yet our efforts were met with defeat.  (%s)", pInfo->m_info.m_szEndDebug );
            } else if ( pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally ) {
                printf_s( "Alas, troubles beset us; we have lost contact with the host.  (%s)", pInfo->m_info.m_szEndDebug );
            } else {
                // NOTE: We could check the reason code for a normal disconnection
                printf_s( "The host hath bidden us farewell.  (%s)", pInfo->m_info.m_szEndDebug );
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0's.
            m_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
            m_hConnection = k_HSteamNetConnection_Invalid;
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
            // We will get this callback when we start connecting.
            // We can ignore this.
            break;

        case k_ESteamNetworkingConnectionState_Connected:
            printf_s( "Connected to server OK" );
            break;

        default:
            // Silences -Wswitch
            break;
    }
}
}  // namespace ArrND::Networking