#ifndef __TOWERTRIAL_SERVICE_H__
#define __TOWERTRIAL_SERVICE_H__

#include "KxCommInterfaces.h"

class CTowerTrialService
{
public:
	// ҵ����
    static void processService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ӭս����
	static void processFighting(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ��Ϸ����
	static void processFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ѡ��ս����buff
	static void processChoseOuterBonus(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// �򿪱���
	static void processOpenTreasure(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// һ������
	static void processOneKeyFighting(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__TOWERTRIAL_SERVICE_H__
