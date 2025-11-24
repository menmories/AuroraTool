#include "EventClient.h"
#include <cassert>
#include <cstring>
#include "AuroraPackage.h"

#ifdef PLATFORM_LINUX
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include "AuroraLog.h"

#define BUFFER_SIZE 1024

class EventClientParams
{
public:
    EventClientParams()
        : ServerThis(nullptr)
        , Bev(nullptr)
        , IsRecving(false)
    {
        Sendbuffer = new unsigned char[BUFFER_SIZE];
        RecvBuffer = new unsigned char[BUFFER_SIZE];
    }

    ~EventClientParams()
    {
        delete[] RecvBuffer;
        delete[] Sendbuffer;
        bufferevent_free(Bev);
    }

    EventClient* ServerThis;
    bufferevent* Bev;
    unsigned char* RecvBuffer;
    unsigned char* Sendbuffer;
    bool IsRecving;

    AuroraPackage Package;
};



EventClient::EventClient()
    : m_base(nullptr)
    , m_bev(nullptr)
{

}

int EventClient::Run(const char* addr, unsigned short port)
{
    // printf("PLAFRORM_LINUX=%d\n", PLAFRORM_LINUX);
    // printf("PLAFRORM_WINDOWS=%d\n", PLAFRORM_WINDOWS);
    // #define PLAFRORM_LINUX
#ifdef PLATFORM_LINUX
    m_base = event_base_new();
#else
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    event_config* config = event_config_new();
    evthread_use_windows_threads();
    event_config_set_flag(config, EVENT_BASE_FLAG_STARTUP_IOCP);
    event_config_set_num_cpus_hint(config, sysInfo.dwNumberOfProcessors * 2);
    m_base = event_base_new_with_config(config);
    event_config_free(config);
#endif

    sockaddr_in sockAddr = { 0 };
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    //sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    evutil_inet_pton(AF_INET, addr, &sockAddr);
    
    m_bev = NewBufferevent();
    bufferevent_socket_connect(m_bev, (const sockaddr*)&sockAddr, sizeof(sockAddr));

    //bufferevent_socket_connect_hostname(m_bev, );

    return event_base_dispatch(m_base);
}

void EventClient::Read_Cb(struct bufferevent* bev, void* ctx)
{
    EventClientParams* params = (EventClientParams*)ctx;
    u32 size = (u32)bufferevent_read(bev, params->RecvBuffer, BUFFER_SIZE);
    if (size > 0)
    {
        if (params->Package.RecvData(params->RecvBuffer, size))
        {
            //接收完毕

        }
    }
}

void EventClient::Write_Cb(struct bufferevent* bev, void* ctx)
{
    EventClientParams* params = (EventClientParams*)ctx;
}

void EventClient::Event_Cb(struct bufferevent* bev, short what, void* ctx)
{
    EventClientParams* params = (EventClientParams*)ctx;
    assert(params);
    if (what & BEV_EVENT_EOF || what & BEV_EVENT_ERROR)
    {
        delete params;
    }
}

bufferevent* EventClient::NewBufferevent()
{
    assert(m_base);
    bufferevent* bev = bufferevent_socket_new(m_base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
    EventClientParams* params = new EventClientParams();
    params->ServerThis = this;
    params->Bev = bev;

    bufferevent_setcb(bev, EventClient::Read_Cb, EventClient::Write_Cb, EventClient::Event_Cb, params);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    return bev;
}

