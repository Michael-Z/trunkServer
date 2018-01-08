#ifndef __CHALLENGE_ROOM_MANAGER_H__
#define __CHALLENGE_ROOM_MANAGER_H__

#include <map>
#include <vector>
#include "ChallengeRoom.h"
#include "GameDef.h"
#include "KXServer.h"
/*
 ��ս���������, ÿ����ҽ�����սʱ��Ҫ����ط��Ǽ�
*/

struct SDelayDelData;
class CChallengeRoomManager : public KxServer::KxTimerObject
{
private:
	CChallengeRoomManager();
	~CChallengeRoomManager();

public:

	static CChallengeRoomManager *getInstance();
	static void destroy();

public:

	bool init(KxServer::KxTimerManager *pTimerManager);

	virtual void onTimer(const KxServer::kxTimeVal& now);

	// ����Ƿ������սָ���ؿ�
	bool isStageEnough(int uid, int chapterId, int stageId, int times = 1);
	// ����Ƿ������սָ������
	bool isInstanceEnough(int uid, int activityId, int times = 1);
	// ����Ƿ������ս�������
	bool isGoldTestEnough(int uid, int wday);
	// ����Ƿ������սӢ�۸���
	bool isHeroTestEnough(int uid, int nInstanceId, int nDiff);
	// ����Ƿ��������
	bool isTowerTestEnough(int uid);
	// ��������ս����
	CChallengeRoom *getRoom(int uid);
	// ������ս����
	CChallengeRoom *createRoom(int uid, int battleType);
	// �Ƿ��з���
	bool haveRoom(int uid);
	// �����Ƴ�����
	void removeRoom(int uid);
	//�ӳ�ɾ������
	void PushDelRoom(int uid);
	//ȥ���ӳ�ɾ������
	void ReSetDelRoom(int uid);
private:

	static CChallengeRoomManager *	m_pInstance;
	std::map<int, CChallengeRoom *>	m_mapRooms;
	std::map<int, std::list<SDelayDelData>::iterator>	m_MapDelUserList;					//��Ҷ�Ӧ������������
	std::list<SDelayDelData>							m_DelUserList;						//�ӳ���������б�
	KxServer::KxTimerManager *		m_TimeManager;
};

#endif //__CHALLENGE_ROOM_MANAGER_H__
