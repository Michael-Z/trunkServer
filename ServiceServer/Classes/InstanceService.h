#ifndef __INSTANCE_SERVICE_H__
#define __INSTANCE_SERVICE_H__

#include "KxCommInterfaces.h"

class CInstanceService
{
public:
	//�����ҵ��
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��ս�����
	static void ProcessChanllege(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��ս����
	static void ProcessFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�������
	static void ProcessBuyTimes(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__INSTANCE_SERVICE_H__
