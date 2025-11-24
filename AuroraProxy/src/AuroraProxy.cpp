#include "AuroraProxy.h"

AuroraProxy::AuroraProxy()
{
	Server = new EventServer();
}

AuroraProxy::~AuroraProxy()
{
	delete Server;
}

void AuroraProxy::SetConfig(const AuroraServerConfig& config)
{
	this->Config = config;
}

int AuroraProxy::Run()
{

	return Server->Run(Config.Addr.c_str(), Config.Port);
}

