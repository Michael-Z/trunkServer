#ifndef __PVPBATTLE_SERVICE_H__
#define __PVPBATTLE_SERVICE_H__

#include "KxCommInterfaces.h"

class CPvpBattleService
{
public:
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �������PVP����
    static void processPvpEnterRoomSSServices(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ����PVP����
    static void processPvpFinishSSServices(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ������������PVP����
    static void processEnterRobotRoom(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��������˽���
    static void processFinishRobotRoom(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__PVPINFO_SERVICE_H__

