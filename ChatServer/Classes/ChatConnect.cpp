#include "ChatConnect.h"
#include "ChatServer.h"
#include "KXServer.h"
#include<string.h>

using namespace KxServer;

CChatConnector::CChatConnector()
: m_nServerID(0)
, m_Port(0)
, m_TimerCallBack(NULL)
{
    memset(m_strIP, 0, sizeof(m_strIP));
}

CChatConnector::~CChatConnector()
{
    if (m_TimerCallBack != NULL)
    {
        m_TimerCallBack->stop();
        KXSAFE_RELEASE(m_TimerCallBack);
    }
}

// ����ָ����IP�Ͷ˿�
bool CChatConnector::connect(const char* addr, int port, bool nonblock)
{
    if (addr == NULL)
    {
        return false;
    }

    memcpy(m_strIP, addr, sizeof(m_strIP));
    m_Port = port;
    return KxTCPConnector::connect(m_strIP, m_Port, nonblock);
}

void CChatConnector::onConnected(bool success)
{
    KxTCPConnector::onConnected(success);
    if (success)
    {
        KXLOGDEBUG("CChatConnector::onConnected() success IP:%s,port:%d", m_strIP, m_Port);
    }
    else
    {
        CChatConnector::onError();
    }
}

// ��������ʱ�ص�����IKxCommPoller����
int CChatConnector::onError()
{
    int nError = KxTCPConnector::onError();
    KXLOGDEBUG("CChatConnector::onError() IP:%s,port:%d", m_strIP, m_Port);
    setTimer(RECONNECT_TIME_INTERVAL);
    return nError;
}

// ���ö�ʱ����
void CChatConnector::setTimer(int nSec)
{
    if (m_TimerCallBack == NULL)
    {
        m_TimerCallBack = new KxServer::KxTimerCallback<CChatConnector>();
    }
    m_TimerCallBack->setCallback(this, &CChatConnector::onTimer);
    CChatServer::getInstance()->getTimerManager()->addTimer(
        m_TimerCallBack, nSec, 0);
}

// ��ʱ����
void CChatConnector::onTimer()
{
    reconnect();
    KXSAFE_RELEASE(m_TimerCallBack);
}

// ����
bool CChatConnector::reconnect()
{
    close();
    if (!init() || !connect(m_strIP, m_Port))
    {
        return false;
    }

    IKxCommPoller* pPoller = CChatServer::getInstance()->getMainPoller();
    pPoller->addCommObject(this, getPollType());
    return true;
}
