///////////////////////////////////////////////////////
//���������ö�ȡ
//ServerConfig.h
//�Ƴ�
///////////////////////////////////////////////////////

#ifndef _SERVER_CONFIG_H__
#define _SERVER_CONFIG_H__

#include <map>
#include <vector>
#include <set>
#include "ServerData.h"

class CServerConfig
{
	typedef std::map<int, std::vector<ServerConfigData*> >	ServerMap;

public:
	CServerConfig();
	~CServerConfig();

	bool create();
	void clean();

	bool updateConfig();

	ServerConfigData* getLocalServerInfo() { return &m_LocalServer; }
	std::map<int, ServerConfigData>* getConnServer() { return &m_MapConnSByServerID; }
	ServerRouteConfigData* getRouteConfigData(int nMainCmd, int nSubCmd);
	std::set<int>* getClosingWhiteCmd(){ return &m_ServerCloseWhiteCmd; }

protected:
	bool loadLocalServer();
	bool loadConnServer();
	bool loadRouteServer();
	bool loadClosingWhiteCmd();
	bool loadNewConnServer();
	bool loadNewRouteServer();

private:
	ServerConfigData                        m_LocalServer;
    ServerMap                               m_MapConnServer;			// ���������ӷ������б�
    std::map<int, ServerConfigData>         m_MapConnSByServerID;		// ��������ID���������б�
    std::map<int, ServerRouteConfigData>    m_MapRouteMain;				// ���������ȡ·�ɹ���
    std::map<int, ServerRouteConfigData>    m_MapRouteCmd;				// ��������+�������ȡ·�ɹ���
	std::set<int>							m_ServerCloseWhiteCmd;		// ������ƽ���ر�ʱ����ͨ��������
};

#endif //_SERVER_CONFIG_H__
