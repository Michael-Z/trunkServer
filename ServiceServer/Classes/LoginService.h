#ifndef __LOGIN_SERVICE_H__
#define __LOGIN_SERVICE_H__

#include "KxCommInterfaces.h"

class CLoginService
{
public:

	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);

	static void processUserLogin(int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//ǰ��ս��������С������
	static void processUserReconect(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�����û���¼���ύ��־��Ϣ
    static void processUserLogService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    //�����û���¼���ύ������־��Ϣ
    static void processGuideLogService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�����û�����������ʱ���ˢ����������
	static void processLoginFreshService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

private:
	//static void sendUserModel(int uid, CUserModel *model);
	//void sendBagModel(int uid, int capacity, CBagModel *model);
	//void sendEquipModel(int uid, CEquipModel *model);
	//void sendSummonModel(int uid, CSummonModel *model);
	//void sendHeroModel(int uid, int heroCapacity, CHeroModel *model);
	//void sendStageModel(int uid, CStageModel *model);
	//void sendTeamModel(int uid, CTeamModel *model);
	//void sendTaskModel(int uid, CTaskModel *model);

};

#endif //__LOGIN_SERVICE_H__
