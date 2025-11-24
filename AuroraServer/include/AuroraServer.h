#ifndef __AURORASERVER_H__
#define __AURORASERVER_H__

#include "PlatformDef.h"
#include "AuroraServerConfig.h"

class AuroraServer
{
public:
    AuroraServer();
    ~AuroraServer();

    static const char* GetVersion();

    void SetConfig(const AuroraServerConfig& config);

    int Run();

private:
    AuroraServerConfig m_config;
    class EventServer* m_server;
};

#endif  //__AURORASERVER_H__

