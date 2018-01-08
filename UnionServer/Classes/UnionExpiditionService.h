#ifndef _UNION_EXPIDITION_SERVICE_H__
#define _UNION_EXPIDITION_SERVICE_H__

#include "KXServer.h"
#include <vector>

class CUnionExpiditionModel;
struct SExpeditonWorldItem;
class CUnionExpiditionService
{
public:
	static void processService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Զ����Ϣ�·�
	static void processExpiditionInfo(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Զ����ͼѡ��
	static void processExpiditionMapChoose(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Զ���ַ���ʼ
	static void processExpiditionFightStart(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Զ���ַ�����
	static void processExpiditionFightFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//�������˺����а�
	static void processExpiditionDamageRank(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	//������Զ���ؿ���Ϣ
	static void processExpiditionStageInfo(int uid, char *buffer, int len, KxServer::IKxComm *commun);

private:
	static int				m_RankFreshTime;			//���а�ˢ��ʱ��

};


#endif //_UNION_EXPIDITION_SERVICE_H__
