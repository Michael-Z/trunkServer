/*
* �ʼ�����
* 1.��ȡ�ʼ�(������ͨ��web�ʼ�)
*/

#ifndef __MAIL_SERVICE_H__
#define __MAIL_SERVICE_H__

#include "KxCommInterfaces.h"
#include "BagModel.h"
#include "ServiceDef.h"

#include <list>

class CMailService
{
public:
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��ȡ�ʼ�����
	static void processGetMailGoodsService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//��ȡ�ʼ�����
	static void processReadMailContextService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���web�ʼ�
    static void processWebMailService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    //�ʼ�ģ��ˢ��
    static void processUpdateMailService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__MAIL_SERVICE_H__
