#ifndef __TEAM_SERVICE_H__
#define __TEAM_SERVICE_H__

#include "KXServer.h"

class CTeamService
{
public:
	// �������
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ���ö���
	static void ProcessSetTeam(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__TEAM_SERVICE_H__
