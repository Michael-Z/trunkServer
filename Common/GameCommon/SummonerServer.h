#ifndef __SUMMONER_SERVER_H__
#define __SUMMONER_SERVER_H__

#include <string>
#include "KxBaseServer.h"

// session��������
struct SummonerServerConf
{
	int ipVer;
	int group;
	int serverGroup;
	int serverId;
	int port;
	std::string host;
};

class CSummonerServer : public KxServer::KxBaseServer
{
public:
	CSummonerServer();
	virtual ~CSummonerServer();

public:
	// �ṩ��ͳ��ʹ��
	static CSummonerServer *getNewInstance();
	// ��ʼ����������Ϣ
	virtual bool initServerInfo(int argc, char **argv);
	// ���÷�����id
	virtual void setServerId(int serverId) { m_ServerInfo.serverId = serverId; }
	virtual int getServerId() { return m_ServerInfo.serverId; }
	// ���÷���������
	virtual void setGroup(int group) { m_ServerInfo.group = group; }
	virtual int getGroup() { return m_ServerInfo.group; }
	// ���ö˿ں�
	virtual void setPort(int port) { m_ServerInfo.port = port; }
	virtual int getPort() { return m_ServerInfo.port; }
	// ����ip
	virtual void setHost(std::string &ip) { m_ServerInfo.host = ip; }
	virtual std::string getHost() { return m_ServerInfo.host; }
	
protected:

	SummonerServerConf      m_ServerInfo;	 //��������Ϣ
	static CSummonerServer *m_pNewInstance;	 //�������
};

#endif //__SUMMONER_SERVER_H__
