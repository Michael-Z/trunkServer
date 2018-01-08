/*
���������
1��������session������
2��������֤������
*/

#ifndef _CHAT_SERVER_H__
#define _CHAT_SERVER_H__

#include "KxBaseServer.h"
#include "ServerConfig.h"
#include <vector>
#include <string>

// ���ӷ���������
struct ConnectServerConf
{
    int serverGroup;
    int port;
    std::string host;
};

// chat��������
struct ChatServerConf
{
    int ipVer;
    int group;
    int serverGroup;
    int serverId;
    int port;
    std::string host;
    std::vector<ConnectServerConf> connectors;
};

class CChatServer : public KxServer::KxBaseServer
{
public:
    CChatServer();
    ~CChatServer();

    static CChatServer* getInstance();

	static void destroy();

    virtual bool initServerInfo(int argc, char ** argv);

    virtual void onServerUpdate();

	virtual bool onServerInit();

    CServerConfig * getServerCoinfig() { return &m_ServerConfig; }

private:

    static CChatServer *    m_pInstance;
    CServerConfig			m_ServerConfig;	      //����������ȡ���ñ�
    ChatServerConf          m_ServerInfo;	      //��������Ϣ
};


#endif //_CHAT_SERVER_H__
