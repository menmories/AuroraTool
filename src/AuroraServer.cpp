#include "AuroraServer.h"
#include "AuroraLog.h"
#include "EventServer.h"

AuroraServer::AuroraServer()
{
    m_server = new EventServer();

#ifdef PLATFORM_WINDOWS
    WSAData g_wsaData = { 0 };
    if (WSAStartup(MAKEWORD(2, 2), &g_wsaData))
    {
        AuroraLog::Println("WSAStartup error.");
    }
#endif
}

AuroraServer::~AuroraServer()
{
    delete m_server;
#ifdef PLATFORM_WINDOWS
    WSACleanup();
#endif
}

const char* AuroraServer::GetVersion()
{
    return "1.0.1";
}

void AuroraServer::SetConfig(const AuroraServerConfig& config)
{
    m_config = config;
}

int AuroraServer::Run()
{
    AuroraLog::Println(std::string("Server version:") + AuroraServer::GetVersion());
    AuroraLog::Println("Server start.");
    AuroraLog::PrintToFile("Server start.");
    int ret = m_server->Run(m_config.Addr.c_str(), m_config.Port);
    AuroraLog::Println("Server Stop.");
    return ret;
}

