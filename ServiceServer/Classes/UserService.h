#ifndef __USER_SERVICE_H__
#define __USER_SERVICE_H__

#include "KxCommInterfaces.h"

class CUserService
{
public:

	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);

	static void processBuyService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	
	static void processRenameService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

    static void processHeadIconChange(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�������ǩ��
	static void processUserSignService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//����������Key
	static void processUserGiftKeyService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�׳������ȡ
	static void processUserFistPayService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Ϣ����
	static void processUserFundService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ����ת��
    static void processUserForwardDataService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�޸�����˺Ŷ�Ӧ����
	static void processUsrPasswordModifyService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__USER_SERVICE_H__
