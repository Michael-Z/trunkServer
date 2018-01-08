#include "ChallengeRoomManager.h"
#include "ChallengeRoom.h"
#include "ConfHall.h"
#include "ConfStage.h"
#include "ConfGameSetting.h"
#include "StageProtocol.h"

#include "GameUserManager.h"
#include "ModelHelper.h"
#include "GameDef.h"
#include "CommStructs.h"
#include "TimeCalcTool.h"
#include "KxCommManager.h"
#include "Protocol.h"
#include "ErrorCodeProtocol.h"

CChallengeRoomManager::CChallengeRoomManager()
{
}

CChallengeRoomManager::~CChallengeRoomManager()
{
	for (std::map<int, CChallengeRoom*>::iterator iter = m_mapRooms.begin();
		iter != m_mapRooms.end(); ++iter)
	{
		delete iter->second;
	}
	m_mapRooms.clear();
}

CChallengeRoomManager *CChallengeRoomManager::m_pInstance = NULL;
CChallengeRoomManager *CChallengeRoomManager::getInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CChallengeRoomManager;
	}
	return m_pInstance;
}

void CChallengeRoomManager::destroy()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool CChallengeRoomManager::init(KxServer::KxTimerManager *pTimerManager)
{
	m_TimeManager = pTimerManager;

	if (m_TimeManager == NULL)
	{
		return false;
	}

	//���޴���
    if (!m_TimeManager->addTimer(this, CHECK_TIME_INTERVAL, KXREPEAT_FOREVER))
	{
		return false;
	}

	return true;
}

void CChallengeRoomManager::onTimer(const KxServer::kxTimeVal& now)
{
	int nCurTime = (int)time(NULL);

	for (std::list<SDelayDelData>::iterator ator = m_DelUserList.begin(); ator != m_DelUserList.end();)
	{
		if (nCurTime > ator->nDelayTime)
		{
			removeRoom(ator->nUid);
			m_MapDelUserList.erase(ator->nUid);
			m_DelUserList.erase(ator++);
		}
		else
		{
			break;
		}
	}
}

bool CChallengeRoomManager::isStageEnough(int uid, int chapterId, int stageId, int times)
{
	//�ڽ���������ս��, ������
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		KXLOGDEBUG("%s m_mapRooms.find(uid) != m_mapRooms.end()", __FUNCTION__);
		//�����ʾ����Ѿ����뵽������ս���У����Է���Tips
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_EXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}

	return CModelHelper::canChallengeStage(uid, chapterId, stageId, times);
}

bool CChallengeRoomManager::isInstanceEnough(int uid, int activityId, int times)
{
	//�ڽ���������ս��, ������
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		KXLOGDEBUG("%s m_mapRooms.find(uid) != m_mapRooms.end()", __FUNCTION__);
		//�����ʾ����Ѿ����뵽������ս���У����Է���Tips
		//�����ʾ����Ѿ����뵽������ս���У����Է���Tips
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_EXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}

	return true;

	//int needEnergy = 3 * times;
	//int haveEnergy = 0;
	////���������
	//CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	//haveEnergy = CModelHelper::getEnergy(uid);
	//return haveEnergy >= needEnergy;
}

bool CChallengeRoomManager::isGoldTestEnough(int uid, int wday)
{
	//�ڽ���������ս��, ������
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		KXLOGDEBUG("%s m_mapRooms.find(uid) != m_mapRooms.end()", __FUNCTION__);
		//�����ʾ����Ѿ����뵽������ս���У����Է���Tips
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_EXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}
	//��ˢ��һ�½��������Ϣ
	CModelHelper::updateGoldTestInfo(uid);
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CGoldTestModel *pGoldTestModel = dynamic_cast<CGoldTestModel*>(pGameUser->getModel(MODELTYPE_GOLDTEST));
	//�����������
	const GoldTestConfItem *pConf = queryConfGoldTest(wday);
	CHECK_RETURN(NULL != pConf);
	//�����ж�
	int nUseTimes = 0;
	CHECK_RETURN(pGoldTestModel->GetFieldNum(GT_FD_USETIMES, nUseTimes));

	if (nUseTimes >= pConf->Frequency)
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_SHILIANTIMES_NOTENOUGH;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}

	return true;
}

bool CChallengeRoomManager::isHeroTestEnough(int uid, int nInstanceId, int nDiff)
{
	//�ڽ���������ս��, ������
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		KXLOGDEBUG("%s m_mapRooms.find(uid) != m_mapRooms.end()", __FUNCTION__);
		//�����ʾ����Ѿ����뵽������ս���У����Է���Tips
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_EXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}
	//ˢ��Ӣ������
	CModelHelper::updateHeroTestInfo(uid);

	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN(NULL != pGameUser);
	CHeroTestModel *pHeroTestModel = dynamic_cast<CHeroTestModel*>(pGameUser->getModel(MODELTYPE_HEROTEST));
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
	CHECK_RETURN(NULL != pHeroTestModel && NULL != pUserModel);

	const TimeRecoverItem *pRecoverConf = queryConfTimeRecoverSetting();
    const HeroTestItem* pHeroTestItem = queryConfHeroTest(nInstanceId);
    CHECK_RETURN(NULL != pHeroTestItem && NULL != pRecoverConf);

	// ָ����Ӣ�����������Ƿ���
	bool bOpening = false;
	time_t nowTime = time(NULL);
	tm *pCurTm = localtime(&nowTime);
	int nCurWDay = pCurTm->tm_wday == 0 ? 7 : pCurTm->tm_wday;

	int hour = pRecoverConf->AllTimeReset / 60;
	int min = pRecoverConf->AllTimeReset % 60;
  
    for (unsigned int i = 0; i < pHeroTestItem->Time.size(); ++i)
    {
		//����5�㿪��, ���ڽ���5�㲢С�������5��, 24Сʱ��
		int nConfWDay = pHeroTestItem->Time[i];
		int nConfNextWDay = pHeroTestItem->Time[i] == 7 ? 1 : pHeroTestItem->Time[i] + 1;
		
		if ((nCurWDay == nConfWDay && pCurTm->tm_hour >= hour && pCurTm->tm_min >= min)
			|| (nCurWDay == nConfNextWDay && pCurTm->tm_hour < hour && pCurTm->tm_min < min))
        {
            bOpening = true;
            break;
        }
    }

    CHECK_RETURN(bOpening);
	//�ȼ�����
    CHECK_RETURN(nDiff > 0 && nDiff <= (int)pHeroTestItem->Diff.size());
    int nUserLv = 0;
    CHECK_RETURN(pUserModel->GetUserFieldVal(USR_FD_USERLV, nUserLv));
    CHECK_RETURN(nUserLv >= pHeroTestItem->Diff[nDiff - 1].UnlockLevel);

	if (pHeroTestItem->Times <= pHeroTestModel->GetChallengeTimes(nInstanceId))
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_SHILIANTIMES_NOTENOUGH;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}

	return true;
}

bool CChallengeRoomManager::isTowerTestEnough(int uid)
{
	//�ڽ���������ս��, ������
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		KXLOGDEBUG("%s m_mapRooms.find(uid) != m_mapRooms.end()", __FUNCTION__);
		//�����ʾ����Ѿ����뵽������ս���У����Է���Tips
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_EXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}

	CGameUser * pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	if (pGameUser == NULL)
	{
		KXLOGDEBUG("%s pGameUser == NULL", __FUNCTION__);
		return false;
	}
	//�����������
	//�����������
	CModelHelper::updateTowerTestInfo(uid);
	CTowerTestModel *pTowerTestModel = dynamic_cast<CTowerTestModel*>(pGameUser->getModel(MODELTYPE_TOWERTEST));
	if (pTowerTestModel->GetTowerTestField(TOWER_FD_TIMES) <= 0)
	{
		KXLOGDEBUG("%s pTowerTestModel->GetTowerTestField(TOWER_FD_TIMES) <= 0", __FUNCTION__);
		//���ʹ��������Tips
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_TIMESNOTENOUGH;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}

	int floor = pTowerTestModel->GetTowerTestField(TOWER_FD_FLOOR);
	int state = pTowerTestModel->GetTowerTestField(TOWER_FD_FLOORSTATE);
	int maxFloor = queryMaxTowerFloor();

	// �����������ս״̬��������ս
	if (state != FLOORSTATE_FIGHTING)
	{
		KXLOGDEBUG("%s state != FLOORSTATE_FIGHTING", __FUNCTION__);
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_STATEERROR;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}
	// ��ǰ¥�����
	if (floor > maxFloor)
	{
		KXLOGDEBUG("%s floor > maxFloor", __FUNCTION__);
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_BATTLE_MAXTOWER;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return false;
	}
	return true;
}

CChallengeRoom *CChallengeRoomManager::getRoom(int uid)
{
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		return m_mapRooms[uid];
	}
	return NULL;
}

CChallengeRoom *CChallengeRoomManager::createRoom(int uid, int battleType)
{
	CChallengeRoom *pRoom = NULL;
	if (m_mapRooms.find(uid) != m_mapRooms.end())
	{
		removeRoom(uid);
	}

	switch (battleType)
	{
	case EBATTLE_INSTANCE:
		pRoom = new CInstanceRoom;
		break;
	default:
		return NULL;
		break;
	}

	m_mapRooms[uid] = pRoom;
	return pRoom;
}

bool CChallengeRoomManager::haveRoom(int uid)
{
	return m_mapRooms.find(uid) != m_mapRooms.end();
}

void CChallengeRoomManager::removeRoom(int uid)
{
	CChallengeRoom *pRoom = NULL;
	std::map<int, CChallengeRoom*>::iterator iter = m_mapRooms.find(uid);
	if (iter != m_mapRooms.end())
	{
		pRoom = iter->second;
		delete pRoom;
		m_mapRooms.erase(iter);
	}
}

void CChallengeRoomManager::PushDelRoom(int uid)
{
	if (m_TimeManager == NULL)
	{
		return;
	}

	std::map<int, CChallengeRoom*>::iterator iter = m_mapRooms.find(uid);
	if (iter == m_mapRooms.end())
	{
		return;
	}

	std::list<SDelayDelData>::iterator ator;
	SDelayDelData DelData;

	DelData.nDelayTime = (int)time(NULL) + DELAY_TIME_NUM;
	DelData.nUid = uid;
	ator = m_DelUserList.insert(m_DelUserList.end(), DelData);
	m_MapDelUserList[uid] = ator;
}

//ȥ���ӳ�ɾ������
void CChallengeRoomManager::ReSetDelRoom(int uid)
{
	std::map<int, std::list<SDelayDelData>::iterator>::iterator ator = m_MapDelUserList.find(uid);

	if (ator != m_MapDelUserList.end())
	{
		std::list<SDelayDelData>::iterator iter = ator->second;
		m_DelUserList.erase(iter);
		m_MapDelUserList.erase(ator);
	}
}
