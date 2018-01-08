#ifndef _MATCH_SERVER_H__
#define _MATCH_SERVER_H__

#include "SummonerServer.h"

class CMatchServer : public CSummonerServer
{
private:

	CMatchServer();
	~CMatchServer();

public:

	static CMatchServer * getInstance();
	static void destroy();
	// ��������ʼ��
	virtual bool onServerInit();
	// ��������ӵ�е�ģ��
	void AddModelType();

private:

	static CMatchServer * m_pInstance;
};

#endif //_MATCH_SERVER_H__

