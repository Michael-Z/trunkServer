#ifndef __NEW_MATCH_MANAGER_H__
#define __NEW_MATCH_MANAGER_H__

#include <map>
#include <vector>

#include "KxCommInterfaces.h"
#include "KxTimerManager.h"

// ����״̬
enum MatchStatus
{
	MATCHSTATUS_LOW = 0,
	MATCHSTATUS_MID,
	MATCHSTATUS_HIGH,
};

// ƥ�������Ϣ
struct NewMatcher
{
	int uid;			//���id
	int startStamp;		//��ʼ��ƥ��ʱ��
	int curLevelStamp;	//�����ƥ��ʱ��
	int userLv;			//��ҵȼ�
	int integral;		//����
	int matchLevel;		//��ǰƥ���T��
    int totalWin;       //��ʤ��
	NewMatcher *prev;
	NewMatcher *next;
};

// �ȼ��Ӹߵ�������, �ȼ���ͬ���ִӸߵ�������
struct NewMatcherList
{
	bool reverse;		//�Ƿ���
	int prevMatchStamp;	//�ϴ�ƥ��ʱ��
	NewMatcher *head;
	NewMatcher *tail;
};

class CNewMatchManager : public KxServer::KxTimerObject
{
private:
	CNewMatchManager();
	~CNewMatchManager();

public:
	static CNewMatchManager *getInstance(); 
	static void destroy();

public:
	// ��ʼ��
	bool init();
	// ƥ�������
	bool insertRobot(NewMatcher *matcher);
	// ���뵽��Ӧ�ȼ�
	bool insert(int level, NewMatcher *matcher);
	// ����Ƿ����
	bool isExsit(int uid);
	// ��ǰ��ʱ
	int getCurrentStamp();
	// �Ƴ����, delΪtrue���ͷ�NewMatcher����(ƥ��ɹ��ͷ�, �����¸�T�����ͷ�)
	NewMatcher* removeFromMatchList(int uid, bool del = true);
	// �ӻ������б����Ƴ�
	bool removeFromRobotList(int uid);

	//��ʱ��ʼ
	void onTimer(const KxServer::kxTimeVal& now);
	// ����ƥ��
	void matchUpdate();
	// ���һ��NewMatcher����
	NewMatcher *newMatcher();
	// �ͷ�һ��NewMatcher����
	void deleteMatcher(NewMatcher* matcher);
	// ��������ƥ����Ա
	void clearMatchers();

private:

	//T��Ϊ1�Ķ�ƥ�������
	void matchLevel1List(NewMatcherList &list);
	// �Զ�ӦT���б����ƥ��
	void matchUpdateList(int level, NewMatcherList &list);
	// ��������ƥ��ɹ�
	void matchSuccess(NewMatcher *matcher1, NewMatcher *matcher2);
	// ���ƥ������˳ɹ�, robotLevelƥ��ĵ��Ļ����˵ȼ�
    void matchRobotSuccess(NewMatcher *matcher, int robotLevel);
    // ���ƥ���������������˳ɹ�,
    void matchGuideRobotSuccess(NewMatcher *matcher, int robotId);

	// ���ݵ�ǰ������ø���״̬
	int getMatcherStatus();
	// ���ƥ��ʱ��
	int getMatchTime(int matchLevel);
	// ���ƥ��ȴ�ʱ��
	int getWaitingTime(int matchLevel);
	// ���battleId
	int getBattleId();

private:

	static CNewMatchManager*		m_pInstance;			//ʵ��
	int								m_nBattleId;			//ս��id
	int								m_nMatcherCount;		//ƥ��������
	int								m_nTotalStamp;			//��ʱ��

	KxServer::KxTimerManager*		m_pTimerManager;		//��ʱ��
	std::map<int, NewMatcherList>	m_mapMatcherList;		//ƴ���б�
	std::map<int, NewMatcher*>		m_mapMatchers;			//��¼���, �����Ƴ����
	std::map<int, NewMatcher*>		m_mapRobotMatchers;		//ƥ����������
};

#endif //__NEW_MATCH_MANAGER_H__
