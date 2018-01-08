#ifndef __GOLDTRIAL_SERVICE_H__
#define __GOLDTRIAL_SERVICE_H__

#include "KxCommInterfaces.h"

class CGoldTrialService
{
public:
    static void ProcessService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��Ҹ�����ʼ
	static void ProcessChallenge(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��Ҹ�������
	static void ProcessFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��ȡ����
	static void ProcessRewardChest(int uid, char * buffer, int len, KxServer::IKxComm * commun);
};

#endif
