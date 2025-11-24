
#include "PlatformDef.h"
#include "AuroraServer.h"
#include "AuroraServerConfig.h"

int main(int argc, char** argv)
{
    AuroraServer server;
    AuroraServerConfig config;
    config.Addr = "0.0.0.0";
    config.Port = 8080;
    server.SetConfig(config);
    return server.Run();
}

