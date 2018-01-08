#ifndef _UNION_REWARD_SERVICE_H__
#define _UNION_REWARD_SERVICE_H__

#include "KXServer.h"
#include <vector>

class CUnionExpiditionRewardService
{
public:
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Զ���˺����н�����ȡ
	static void processExpiditionRewardGet(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

class CUnionRewardService
{
public:
    static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    //�����ḣ��������ȡ
    static void processWelfareReward(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //_UNION_REWARD_SERVICE_H__
