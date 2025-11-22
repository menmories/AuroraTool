#include "EventServer.h"
#include <cassert>
#include <cstring>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

#ifdef PLATFORM_LINUX
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#define BUFFER_SIZE 1024     //每次发送/接收最高1024个字节
#define START_DATA  0xA5

// 1 + 2 + 4 + 4
#define DATAHEADER_SIZE 11
class DataHeader
{
public:
    enum class MessageType
    {
        MessageType_Msg,
        MessageType_File,
    };

    DataHeader()
    {

    }

    ~DataHeader()
    {

    }

    void MappingData(const unsigned char* buffer)
    {
        Start = buffer[0];
        buffer += 1;
        memcpy(&Type, buffer, 2);
        buffer += 2;
        memcpy(&Size, buffer, 4);
        buffer += 4;
        memcpy(&Version, buffer, 4);
    }

    unsigned char Start;
    MessageType Type;
    unsigned int Size;
    int Version;
};


class EventServerParams
{
public:
    EventServerParams()
        : ServerThis(nullptr)
        , Bev(nullptr)
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

    DataHeader Header;
};



EventServer::EventServer()
    : m_base(nullptr)
    , m_listener(nullptr)
{

}

int EventServer::Run(const char* addr, unsigned short port)
{
    m_base = event_base_new();
    // event_config* config = event_config_new();
    // event_config_set_flag(config, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    // m_base = event_base_new_with_config(config);
    // event_config_free(config);
    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = port;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_listener = evconnlistener_new_bind(m_base, Listener_Cb, this, LEV_OPT_REUSEABLE, 137, (const struct sockaddr*)&sockAddr, sizeof(sockAddr));
    return event_base_dispatch(m_base);
}

void EventServer::Listener_Cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *args)
{
    EventServer* pThis = (EventServer*)args;
    bufferevent* bev = bufferevent_socket_new(pThis->m_base, -1, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
    EventServerParams* eventServerparams = new EventServerParams();
    eventServerparams->ServerThis = pThis;
    eventServerparams->Bev = bev;

    bufferevent_setcb(bev, EventServer::Read_Cb, EventServer::Write_Cb, EventServer::Event_Cb, eventServerparams);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    timeval timeout_read;
    timeout_read.tv_sec = 60;
    timeout_read.tv_usec = 0;
    bufferevent_set_timeouts(bev, &timeout_read, nullptr);
}

void EventServer::Read_Cb(struct bufferevent *bev, void *ctx)
{
    EventServerParams* eventServerParams = (EventServerParams*)ctx;
    size_t size = bufferevent_read(bev, eventServerParams->RecvBuffer, BUFFER_SIZE);
    if (size > 0) 
    {
        if (eventServerParams->IsRecving) 
        {

        }
        else 
        {
            if (eventServerParams->RecvBuffer[0] == START_DATA && size >= DATAHEADER_SIZE)
            {
                //仅在头部为指定字符才是合法的数据
                eventServerParams->IsRecving = true;
                eventServerParams->Header.MappingData(eventServerParams->RecvBuffer);
                if (eventServerParams->Header.Type == DataHeader::MessageType::MessageType_Msg)
                {
                    //如果是普通聊天消息，发送的总数据长度不能超过10240个字节
                    
                }
                else if (eventServerParams->Header.Type == DataHeader::MessageType::MessageType_File) 
                {
                    //发送文件采用分块发送

                }
            }
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

