#ifndef _CHAT_LISTEN_H__
#define _CHAT_LISTEN_H__

#include "KXServer.h"

class CChatListen: public KxServer::KxTCPListener
{
public:
    CChatListen();
    ~CChatListen();

    // Acceptʱ�ص�������KXCOMMID����KxTCPClienter
    virtual KxServer::KxTCPClienter* onAccept(KXCOMMID client);
};


#endif //_CHAT_LISTEN_H__
