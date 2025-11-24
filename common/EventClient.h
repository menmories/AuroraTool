#ifndef __EVENTCLIENT_H__
#define __EVENTCLIENT_H__

extern "C"
{
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/dns.h>
#include <event2/listener.h>
#include <event2/thread.h>
}

#include "PlatformDef.h"
#include "AuroraPackage.h"
class EventClient
{
public:
    EventClient();

    int Run(const char* addr, unsigned short port);
protected:
    
private:
    static void Read_Cb(struct bufferevent* bev, void* ctx);
    static void Write_Cb(struct bufferevent* bev, void* ctx);
    static void Event_Cb(struct bufferevent* bev, short what, void* ctx);

    struct bufferevent* NewBufferevent();
private:
    struct event_base* m_base;
    struct bufferevent* m_bev;
};

#endif  // __EVENTCLIENT_H__

