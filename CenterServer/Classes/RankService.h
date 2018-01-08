#ifndef _RANK_SERVICE_H__
#define _RANK_SERVICE_H__

#include "KxCommInterfaces.h"

class CRankService
{
public:
	CRankService();
	~CRankService();

	static void ProcessService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);

protected:
	static void ProcessRankService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//����PVP���а�
	static void ProcessPvpRankService(int uid, KxServer::IKxComm *commun);
	//����ȼ����а�
	static void ProcessLevelRankService(int uid, KxServer::IKxComm *commun);
	//���������а�
	static void ProcessUnionRankService(int uid, KxServer::IKxComm *commun);
	//�����������а�
	static void ProcessTowerRankService(int uid, KxServer::IKxComm *commun);
	//������������а�
	static void processChampionRankService(int uid, KxServer::IKxComm *commun);

	static int m_LevelExpireTime;					//�ȼ����а����ݹ���ʱ��
	static int m_PvpExpireTime;						//PVP���а����ݹ���ʱ��
	static int m_UnionExpireTime;					//�������а����ݹ���ʱ��
	static int m_TowerExpireTime;					//�����������а����ݹ���ʱ��
	static int m_ChampionExpireTime;				//���������а����ݹ���ʱ��

};


#endif //_RANK_SERVICE_H__
