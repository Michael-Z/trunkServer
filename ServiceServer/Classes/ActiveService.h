#ifndef _ACTIVE_SERVICE_H__
#define _ACTIVE_SERVICE_H__

#include "KxCommInterfaces.h"

class CActiveService
{
public:
	CActiveService();
	~CActiveService();

	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//������ȡҵ��
	static void ProcessActiveGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//����7����ȡ
	static void processSevenActiveGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//����QQ������ȡ
	static void processQQActiveGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//����һ����ȡ
	static void processExChangeGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

protected:
	//�����̵��������
	static void ProcessActiveShopBuy(int uid, int nActiveID, int nGiftID,int nActiveType,KxServer::IKxComm *commun);

	//����������������ȡ
	static void ProcessActiveTaskGet(int uid, int nActiveID, int nActiveTaskID,int nActiveType, KxServer::IKxComm *commun);

	//�����¿���е��¿�������ȡ
	static void processActiveMonthGet(int uid, int nActiveID, int nMonthCardID, KxServer::IKxComm *commun);

	//����������ʱ��������ȡ
	static void processTimesActiveTaskGet(int uid, int nActiveID, int nActiveTaskID, int nActiveType, KxServer::IKxComm *commun);

	//��������ʱ���Ļ����
	static void processOLTimeActiveTaskGet(int uid, int nActiveID, int nActiveTaskID, int nActiveType, KxServer::IKxComm *commun);

};


#endif //_ACTIVE_SERVICE_H__

