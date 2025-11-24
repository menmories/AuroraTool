#ifndef __AURORAPROXY_H__
#define __AURORAPROXY_H__


#include "PlatformDef.h"
#include "EventServer.h"
#include "AuroraServerConfig.h"

class AuroraProxy
{
public:
	AuroraProxy();

	~AuroraProxy();

	void SetConfig(const AuroraServerConfig& config);

	int Run();
public:
	EventServer* Server;
	AuroraServerConfig Config;
};



#endif // !__AURORAPROXY_H__
