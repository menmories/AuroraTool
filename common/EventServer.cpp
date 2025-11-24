#include "EventServer.h"
#include <cassert>
#include <cstring>
#include "AuroraPackage.h"

#ifdef PLATFORM_LINUX
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include "AuroraLog.h"

#define BUFFER_SIZE 1024     //每次发送/接收最高1024个字节


class EventServerParams
{
public:
    EventServerParams()
        : ServerThis(nullptr)
        , Bev(nullptr)
        , IsRecving(false)
    {
        Sendbuffer = new unsigned char[BUFFER_SIZE];
        RecvBuffer = new unsigned char[BUFFER_SIZE];
    }

    ~EventServerParams()
    {
        delete[] RecvBuffer;
        delete[] Sendbuffer;
        bufferevent_free(Bev);
    }

    EventServer* ServerThis;
    bufferevent* Bev;
    unsigned char* RecvBuffer;
    unsigned char* Sendbuffer;
    bool IsRecving;

    AuroraPackage Package;
};



EventServer::EventServer()
    : m_base(nullptr)
    , m_listener(nullptr)
{

}

int EventServer::Run(const char* addr, unsigned short port)
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
    m_listener = evconnlistener_new_bind(m_base, Listener_Cb, this, LEV_OPT_CLOSE_ON_FREE, 533, (const struct sockaddr*)&sockAddr, sizeof(sockAddr));
    return event_base_dispatch(m_base);
}

void EventServer::RecvData(bufferevent* bev, AuroraPackage* package)
{

}

void EventServer::Listener_Cb(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *, int socklen, void *args)
{
    EventServer* pThis = (EventServer*)args;
    bufferevent* bev = bufferevent_socket_new(pThis->m_base, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
    EventServerParams* eventServerparams = new EventServerParams();
    eventServerparams->ServerThis = pThis;
    eventServerparams->Bev = bev;

    bufferevent_setcb(bev, EventServer::Read_Cb, EventServer::Write_Cb, EventServer::Event_Cb, eventServerparams);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    /*timeval timeout_read;
    timeout_read.tv_sec = 60;
    timeout_read.tv_usec = 0;
    bufferevent_set_timeouts(bev, &timeout_read, nullptr);*/
}

size_t ReadLastData(AuroraPackage& package, u8* data, size_t size)
{
    return 0;
}

void EventServer::Read_Cb(struct bufferevent *bev, void *ctx)
{
    EventServerParams* eventServerParams = (EventServerParams*)ctx;
    size_t size = bufferevent_read(bev, eventServerParams->RecvBuffer, BUFFER_SIZE);
    if (size > 0) 
    {
        if (eventServerParams->Package.RecvData(eventServerParams->RecvBuffer, size))
        {
            //接收完毕

        }
    }
}

void EventServer::Write_Cb(struct bufferevent *bev, void *ctx)
{
    EventServerParams* eventServerParams = (EventServerParams*)ctx;
}

void EventServer::Event_Cb(struct bufferevent *bev, short what, void *ctx)
{
    EventServerParams* eventServerParams = (EventServerParams*)ctx;
    assert(eventServerParams);
    if(what & BEV_EVENT_EOF || what & BEV_EVENT_ERROR)
    {
        delete eventServerParams;
    }
}

