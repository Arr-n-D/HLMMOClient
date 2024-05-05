#pragma once

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>

SteamNetworkingMicroseconds g_logTimeZero;

namespace ArrND::Networking
{
    class NetworkManager
    {
    public:
        NetworkManager();

        ~NetworkManager();

        void Init();

        void Update(float deltaFloat);

        void OnUpdate(float deltaFloat);

        static void DebugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char *pszMsg)
        {
            SteamNetworkingMicroseconds time = SteamNetworkingUtils()->GetLocalTimestamp() - g_logTimeZero;
            printf("%10.6f %s\n", time * 1e-6, pszMsg);
            fflush(stdout);
            if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
            {
                fflush(stdout);
                fflush(stderr);
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

        private:
            ISteamNetworkingSockets *m_pInterface = nullptr;
    };
}
