#ifndef __SESSIONSERVER_H__
#define __SESSIONSERVER_H__

#include "KXServer.h"
#include "ServerConfig.h"
#include "GameInterface.h"

// ���ӷ���������
struct ConnectServerConf
{
	int serverGroup;
	int port;
	std::string host;
};

// session��������
struct SessionServerConf
{
	int ipVer;
	int group;
	int serverGroup;
	int serverId;
	int port;
	std::string host;
	std::vector<ConnectServerConf> connectors;
};

class CSessionServer : public KxServer::KxBaseServer
{
private:
    CSessionServer(void);
    virtual ~CSessionServer(void);

public:
    static CSessionServer* getInstance();
    static void destroy();

	//��̬���·�����
	bool dynamicUpdate();											
	//��������Ϣ��ʼ��
	virtual bool initServerInfo(int argc, char ** argv);
	//������ģ���ʼ��
    virtual bool onServerInit();
	//���÷��������̹ر� 
	void setSessionServerClose();							
	//���÷�����ƽ����ʼ��־
	void setSessionServerClosing();							
	//�õ�ƽ���رձ�־λ
	bool getClosingFlag(){ return m_IsClosing; }			
	CServerConfig * getServerCoinfig() { return &m_ServerConfig; }
	IGameEvent*	getGameEvent(){ return m_GameEvent; }

private:

	SessionServerConf       m_ServerInfo;	//��������Ϣ
	bool					m_IsClosing;	//����ƽ���ر�ʱ�ı�־
	IGameEvent*				m_GameEvent;	//SessionServe�����¼�����ص��ӿ�
	CServerConfig			m_ServerConfig;	//����������ȡ���ñ�
	static CSessionServer*	m_Instance;		//����
};

#endif //__SESSIONSERVER_H__
