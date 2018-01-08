#ifndef __MATCHMANAGER_H__
#define __MATCHMANAGER_H__

#include <list>
#include <set>

#include "KxCommInterfaces.h"
#include "KxTimerManager.h"
#include "PvpModel.h"

// ע��: �ͻ��˵�ͬ��������,
// ���Դ�����Ҫ�뷿�����Ͷ�Ӧ
enum MatchType
{
	MATCH_NONE,				//�� 
	MATCH_FAIRPVP,			//��ƽ����
	MATCH_CPN,				//������
};

struct NewMatch
{
	int				uid;		// uid
    int             level;      // ��ҵȼ�
	int				integral;	// ����ֵ
	int				nStartTime;	// ƥ�俪ʼʱ���

	bool operator == (NewMatch compare)
	{
		return this->uid == compare.uid;
	}
};

class CMatchManager : public KxServer::KxTimerObject
{
private:
	CMatchManager();
	~CMatchManager();

public:

	static CMatchManager *getInstance();
	static void destroy();

public:
	//��ӵ�ƥ���б�
	bool addToMatch(int matchType, int uid, KxServer::IKxComm *target);
	//ȡ��ƥ��
	bool cancelMatch(int uid);
	//��ʼƥ��
	void matchUpdate();
	//��ʱ��ʼ
	void onTimer(const KxServer::kxTimeVal& now);
	//����������
	void clearMatchers();
	//�˳�ƥ��ֵ�������
	void removeFromMatchServer(int uid);

private:
	// �������
	bool insert(int matchType, int uid, int integral, int level);
	// �Ƴ�����
	bool remove(int uid);
	// ���Ҳ���
	bool find(int uid);
	//���battleId
	int makeBattleId();
	//ƥ��ɹ�
	void matchSuccess(NewMatch &matcher1, NewMatch &matcher2);
	//ƥ�������
	void matchRobotSuccess(NewMatch &matcher);
	
private:

	static CMatchManager *				m_pInstance;		//ʵ��
	bool								m_bFairInvert;		//��ƽ��������
	bool								m_bCPNIntert;		//����������
	int									m_nBattleId;		//battleId
	unsigned int						m_uTotalSeconds;	//���ۼ�ʱ��
	unsigned int						m_uFairSeconds;		//��ƽ������ʱ
	unsigned int						m_uCPNSeconds;		//��������ʱ
	
	KxServer::KxTimerManager *			m_pTimerManager;	//��ʱ��
	std::set<int>						m_setMatcherUids;	//ƥ����������
	std::list<NewMatch>					m_listFairPvp;		//��ƽ����ƥ���б�
	std::list<NewMatch>					m_listNewGuy;		//ƥ�����С��5�ε����
	std::list<NewMatch>					m_listCpn;			//������ƥ���б�
};

#endif //__MATCHMANAGER_H__
