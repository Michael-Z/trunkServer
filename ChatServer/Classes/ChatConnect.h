#ifndef _CHAT_CONNECT_H__
#define _CHAT_CONNECT_H__

#include "KXServer.h"
#include <map>

#define RECONNECT_TIME_INTERVAL		5			// �������5��
#define MAX_RECONECT_TIME			0xFFFFFFFF	// ����������

class CChatConnector : public KxServer::KxTCPConnector
{
public:
	CChatConnector();
	virtual ~CChatConnector();

	// ����ָ����IP�Ͷ˿�
	virtual bool connect(const char* addr, int port, bool nonblock = true);
	// ���ӳɹ�ʧ�ܻص�
	virtual void onConnected(bool success);
	// ��������ʱ�ص�����IKxCommPoller����
	virtual int onError();

	// ���ö�ʱ����
	virtual void setTimer(int nSec);
	// ��ʱ����
	virtual void onTimer();

private:
	// ����
	bool reconnect();

private:
	int m_nServerID;	    // ������ID
	int m_Port;			    // �������˿�
	char m_strIP[16];	    // ������IP
	KxServer::KxTimerCallback<CChatConnector>* m_TimerCallBack;
};


#endif //SESSION_CONNECT_H__
