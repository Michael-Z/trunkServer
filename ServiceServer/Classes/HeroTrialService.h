#ifndef __HEROTRIAL_SERVICE_H__
#define __HEROTRIAL_SERVICE_H__

#include "KxCommInterfaces.h"

class CHeroTrialService
{
public:

	static void ProcessService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��Ҹ�����ʼ
	static void ProcessChanllege(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��Ҹ�������
	static void ProcessFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif
