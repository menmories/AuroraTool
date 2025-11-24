
#include "PlatformDef.h"
#include "AuroraServer.h"
#include "AuroraServerConfig.h"
#include "AuroraProxy.h"

int main(int argc, char** argv)
{
    AuroraProxy server;
    AuroraServerConfig config;
    config.Addr = "0.0.0.0";
    config.Port = 8080;
    server.SetConfig(config);
    return server.Run();
}

