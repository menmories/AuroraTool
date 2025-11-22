#ifndef __EVENTSERVER_H__
#define __EVENTSERVER_H__

extern "C"
{
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/dns.h>
#include <event2/listener.h>
}

#include "PlatformDef.h"

class EventServer
{
public:
    EventServer();

    int Run(const char* addr, unsigned short port);
protected:
    
private:
    static void Listener_Cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
    static void Read_Cb(struct bufferevent *bev, void *ctx);
    static void Write_Cb(struct bufferevent *bev, void *ctx);
    static void Event_Cb(struct bufferevent *bev, short what, void *ctx);
private:
    struct event_base* m_base;
    struct evconnlistener* m_listener;
};

#endif  // __EVENTSERVER_H__

