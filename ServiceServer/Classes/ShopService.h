#ifndef _SHOP_SERVICE_H__
#define _SHOP_SERVICE_H__

#include "KXServer.h"


class CShopService
{
public:
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//�����̵깺��
	static void ProcessShopGoodsBuy(int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//�������̵깺��
	static void processUnionShopBuy(int uid, char *buffer, int len, KxServer::IKxComm *commun);

	//�����̵�ˢ��
	static void ProcessShopFresh(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};


#endif //_SHOP_SERVICE_H__
