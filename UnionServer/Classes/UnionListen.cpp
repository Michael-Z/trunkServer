#include "UnionListen.h"
#include "UnionServer.h"
#include "GateManager.h"

using namespace KxServer;


CUnionListen::CUnionListen()
{
}


CUnionListen::~CUnionListen()
{
}

// Acceptʱ�ص�������KXCOMMID����KxTCPClienter
KxTCPClienter* CUnionListen::onAccept(KXCOMMID client)
{
    KxTCPClienter* tcpClient = new KxTCPClienter();
    if (tcpClient->init(client))
    {

        CUnionServer::getInstance()->getMainPoller(
            )->addCommObject(tcpClient, tcpClient->getPollType());

        //Ŀǰ����ֻ����һ��
        CGateManager::getInstance()->SetGate(tcpClient);

        return tcpClient;
    }
    else
    {
        tcpClient->release();
        return NULL;
    }
}