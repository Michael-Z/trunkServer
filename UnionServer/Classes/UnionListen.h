#ifndef _UNION_LISTEN_H__
#define _UNION_LISTEN_H__

#include "KXServer.h"

class CUnionListen: public KxServer::KxTCPListener
{
public:
    CUnionListen();
    ~CUnionListen();

    // Acceptʱ�ص�������KXCOMMID����KxTCPClienter
    virtual KxServer::KxTCPClienter* onAccept(KXCOMMID client);
};


#endif //_UNION_LISTEN_H__
