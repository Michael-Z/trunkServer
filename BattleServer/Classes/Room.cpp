#include "Room.h"
#include "Player.h"
#include "BattleScene.h"
#include "Director.h"
#include "Protocol.h"
#include "GameDef.h"
#include "EventManager.h"
#include "BattleServer.h"
#include "CommStructs.h"
#include "BufferData.h"
#include "RoleComm.h"
#include "KxMemPool.h"
#include "RoomManager.h"
#include "BattleServer.h"
#include "CommonHelper.h"
#include "ModelHelper.h"
#include "PvpProtocol.h"
#include "GameUserManager.h"
#include "ActiveHelper.h"
#include "KxCommManager.h"
#include "PvpSettleResult.h"
#include "ConfGameSetting.h"
#include "ConfArena.h"
#include "Role.h"
#include "BattleDataCache.h"

using namespace KxServer;
using namespace std;

IRoom::IRoom() 
	: m_nRoomType(0)
{
}

IRoom::~IRoom() 
{
}

CPvpRoom::CPvpRoom(void)
    :m_bClose(false)
    ,m_nStartStamp(0)
    ,m_nCurStamp(0)
	,m_nPrevStamp(0)
	,m_nFPS(0)
    ,m_nStageId(0)
	,m_nBattleId(0)
    ,m_nRoomState(0)
    ,m_nRoomCapacity(2)
	, m_nRoomTick(0)
	,m_fPrepareFightTick(0.0f)
	,m_nWinnerId(0)
	,m_nLoserId(0)
	,m_nChallengeResult(0)
    ,m_pDirector(NULL)
	,m_pBattleScene(NULL)
    ,m_pMaster(NULL)
	,m_pTimerManager(NULL)
    ,m_pEventManager(NULL)
{
}

CPvpRoom::~CPvpRoom(void)
{
}

bool CPvpRoom::initRoom(int battleId, int stageId, int pvpType)
{
	//��ʼ������״̬Ϊ����ǰ
	m_nRoomState = ROOMSTATE_PREPARELOAD;
    // ��ʼ��״̬ ֻ��ʼ�������Ͳ���
	m_nRoomType = pvpType;
	m_nBattleId = battleId;
    m_nStageId = stageId;
	m_nRoomCapacity = 2;
	m_nFPS = 10;
	//5��ս��Ԥ��
	m_fPrepareFightTick = 5.0f;
    // ��ʼ���¼�����
    m_pEventManager = new CEventManager<int>;
	// ��ʱ��
	m_pTimerManager = CBattleServer::getInstance()->getTimerManager();
    // ��ʼ��director
    m_pDirector = new Director;
    m_pDirector->init();
	// ǰ�����ټ�ʱ, ÿ��1s
	m_nRoomTick = PVPROOM_PREPARE_EXPIRE;
	m_pTimerManager->addTimer(this, 1, KXREPEAT_FOREVER);
    return true;
}

void CPvpRoom::loadingRoom()
{
	if (m_nRoomState == ROOMSTATE_FIGHTING)
	{
		KXLOGERROR("Start PVP Room but m_nRoomState == ROOMSTATE_FIGHTING !!!");
		return;
	}
	m_nRoomState = ROOMSTATE_LOADING;
	//������Ҷ�λ��ùؿ�id
	setStageId();
	//�loadingʱ���ʱ
	m_nRoomTick = PVPROOM_LOADING_EXPIRE;
}

bool CPvpRoom::startRoom()
{
    if (m_nRoomState >= ROOMSTATE_IDLETIME)
	{
        KXLOGERROR("start pvp twice pvp battleId=%d!!!", m_nBattleId);
		return false;
	}

	//��ʼ��Ϸ, ����Ԥ��״̬
	m_nRoomState = ROOMSTATE_IDLETIME;
    // ��������, ������initroom��, ��Ϊ������Ҫ�����ķ�����Ϣ
    m_pBattleScene = new CBattleLayer();
	if (!m_pBattleScene->init(this, m_pEventManager))
    {
		KXLOGERROR("Start PVP Room  m_pBattleScene init error !!!");
        return false;
    }

    //����ʱ������ó�ʼֵ
	m_nPrevStamp = m_nCurStamp = m_nStartStamp = m_pTimerManager->getTimestamp();
    m_LastTime = m_pTimerManager->getNow();
    //ִ��ս���߼�
    m_pDirector->runWithScene(m_pBattleScene);

	// Ҫ��ȷʱ�䣬ÿ��0�� + 0.1��ִ��һ��
	stop();
	setDelay(0, MILLION / m_nFPS);
	setRepeat(KXREPEAT_FOREVER);
	m_pTimerManager->addTimer(this);

	// ֪ͨ�����Ϸ��ʼ
	PvpStartSC startSC;
	startSC.startStamp = static_cast<int>(time(NULL));
	sendRoom(CMD_PVP, CMD_PVP_IDLETIME_SC,
		reinterpret_cast<char*>(&startSC), sizeof(startSC));
    return true;
}

void CPvpRoom::fightingRoom()
{
	m_nRoomState = ROOMSTATE_FIGHTING;
}

void CPvpRoom::exitRoom()
{
    if (NULL != m_pDirector)
    {
        m_pDirector->end();
        delete m_pDirector;
        m_pDirector = NULL;
    }

    if (NULL != m_pEventManager)
    {
        delete m_pEventManager;
        m_pEventManager = NULL;
    }

	for (map<int, KxServer::IKxComm *>::iterator ator = m_PlayerCommu.begin(); 
		ator != m_PlayerCommu.end(); ++ator)
	{
		CKxCommManager::getInstance()->delKxComm(ator->first);
	}

	m_PlayerCommu.clear();
	//ֹͣ��ʱ��
	stop();
	//���������������
	CRoomManager::getInstance()->removeWithBattleId(m_nBattleId);
}

void CPvpRoom::onTimer(const kxTimeVal& now)
{
	if (m_nRoomState != ROOMSTATE_IDLETIME && m_nRoomState != ROOMSTATE_FIGHTING)
	{
		m_nRoomTick -= 1;
		if (m_nRoomTick != 0)
		{
			return;
		}
		// ����һֱ�ȴ�2λ��Ҽ���
		if (m_nRoomState == ROOMSTATE_PREPARELOAD)
		{
			KXLOGDEBUG("PVP Room prepare loading time over, send user rematch!! user count %d", m_Players.size());
			//��������ƥ��
			sendRoom(CMD_MATCH, CMD_MATCH_REMATCH_SC, NULL, 0);
			//������
			exitRoom();
		}
		// ��������¼�����ָ��ʱ��, ǿ�п�ʼ��Ϸ(��ֹ���һֱ������, ��һֱ�ȴ���Ҽ���)
		else if (m_nRoomState == ROOMSTATE_LOADING)
		{
			// ���˫��������, �Ƴ�����
			if (isAllDisconnect())
			{
				//������
				exitRoom();
			}
			else
			{
				startRoom();
			}
		}
	}
	else
	{
		// �����ƫ��ʱ��
		kxTimeVal tv = now - m_LastTime;
		m_LastTime = now;
		float dt = tv.getfloat();
		// ս��Ԥ��
		if (m_nRoomState == ROOMSTATE_IDLETIME)
		{
			m_fPrepareFightTick -= dt;
			if (m_fPrepareFightTick <= 0.0f)
			{
				//����ս��״̬
				fightingRoom();
				//֪ͨǰ����ʽ��ʼ��Ϸ
				sendRoom(CMD_PVP, CMD_PVP_START_SC, NULL, 0);
			}
		}
		// ������Ϸ
		else if (m_nRoomState == ROOMSTATE_FIGHTING)
		{
			executeGame(dt);
		}
	}
}

void CPvpRoom::executeGame(float dt)
{
    //LOG("game tick %d", m_pBattleScene->getBattleHelper()->GameTick);
	if (!m_pBattleScene->getBattleHelper()->IsBattleOver)
    {
        // ִ������n֡, �ڲ�����֡������
		m_pDirector->loopDelta(dt);
		// ������һ��ʱ���
		m_nPrevStamp = m_nCurStamp;
    }
	else
	{
		//���������Ϣ, ������Ҹ�����Ϣ
		overGame();
		//������
		exitRoom();
	}
}

void CPvpRoom::onProcessGame(int cmd, int subcmd, void *data, int len)
{
    // �ϲ�ƫ����ͷ����������Ҫ������ͷ������ƫ����
	int respLen = len + sizeof(Head);
    char *respData = reinterpret_cast<char *>(data) - sizeof(Head);
    Head* head = reinterpret_cast<Head*>(respData);
    head->cmd = MakeCommand(cmd, subcmd);
    if (NULL != m_pBattleScene)
    {
		m_pBattleScene->onResponse(respData, respLen);
    }
}

void CPvpRoom::overGame()
{
    if ( m_nWinnerId == 0
        || m_nLoserId == 0
        || m_nChallengeResult == CHALLENGE_CANCEL)
    {
        return;
    }

    CBattleHelper *pBattleHepler = m_pBattleScene->getBattleHelper();
    if (pBattleHepler == NULL)
    {
        return;
    }

    CPlayer* winner = getRoomPlayer(m_nWinnerId);
    CPlayer* loser = getRoomPlayer(m_nLoserId);
    if (winner == NULL || loser == NULL)
    {
        return;
    }

    int nWinCrycalLv = 0;
    int nLoseCryCalLv = 0;
    int nTime = 0;

    CRole *pRole = pBattleHepler->getMainRole(pBattleHepler->getCampWithUid(m_nWinnerId));
    nWinCrycalLv = pRole->getIntAttribute(EHeroCrystalLevel);
    pRole = pBattleHepler->getMainRole(pBattleHepler->getCampWithUid(m_nLoserId));
    nLoseCryCalLv = pRole->getIntAttribute(EHeroCrystalLevel);
    nTime = pBattleHepler->GameTick / 10;
	// pvp setting
	const ArenaSettingItem *pArenaConf = queryArenaSetting();

    //��������ֵ��ֲ�
    int nWinnerGNum = 0, nWinnerGDval = 0;
    //���Ե�MMR
    int nWinnerMMR = winner->getMMR();
    int nLoserMMR = loser->getMMR();
    //����
    int nWinnerIntegral = winner->getIntegral();
    int nLoserIntegral = loser->getIntegral();
    //�仯ϵ��(��ʤ����)
    int nKW = winner->getCoutinueWin();
    int nKL = loser->getCoutinueWin();

    //����MMR
    int nWChangeMMR = 0;
    int nLChangeMMR = 0;
    int nWChangeIntegral = 0;
    int nLChangeIntegral = 0;
    if (m_nChallengeResult == CHALLENGE_DRAW)
    {
        nWChangeMMR = CPvpSettleResult::calcMMR(nWinnerMMR, nLoserMMR, nKW, -1);
        nLChangeMMR = CPvpSettleResult::calcMMR(nLoserMMR, nWinnerMMR, nKL, -1);
    }
    else
    {
        //����MMR
        nWChangeMMR = CPvpSettleResult::calcMMR(nWinnerMMR, nLoserMMR, nKW, 1);
        nLChangeMMR = CPvpSettleResult::calcMMR(nLoserMMR, nWinnerMMR, nKL, 0);
        //�������
        nWChangeIntegral = CPvpSettleResult::calcIntegral(nWinnerIntegral, nLoserIntegral, 1);
        //���ִ���һ�����ֲſ۳�
		if (nLoserIntegral > pArenaConf->CriticalPoint)
        {
            nLChangeIntegral = CPvpSettleResult::calcIntegral(nLoserIntegral, nWinnerIntegral, 0);
        }
        //ʤ����ս���������ж��ٻ���
        nWinnerIntegral += nWChangeIntegral;
        // ʧ���߽�����ж��ٻ���
        nLoserIntegral += nLChangeIntegral;
    }

    processPlayer(m_nChallengeResult == CHALLENGE_DRAW ? CHALLENGE_DRAW : CHALLENGE_WON,
		winner, loser->getMainRoleId(), nWChangeMMR, nWChangeIntegral, nWinnerIntegral, nWinCrycalLv, nTime);
    processPlayer(m_nChallengeResult == CHALLENGE_DRAW ? CHALLENGE_DRAW : CHALLENGE_FAILED,
        loser,winner->getMainRoleId(), nLChangeMMR, nLChangeIntegral, nLoserIntegral, nLoseCryCalLv, nTime);
}

void CPvpRoom::processPlayer(int result, CPlayer* player, int nEnemySummerID, int mmr,
    int integral, int newintegral, int crylv, int dt)
{
	int nMySummonerID = player->getMainRoleId();
	std::vector<CSoldierModel*> vectHero = player->getSoldierCards();

    CBufferData* bufferData = new CBufferData();
    bufferData->init(256);
    bufferData->writeData(m_nRoomType);
    bufferData->writeData(result);
    bufferData->writeData(newintegral);
    bufferData->writeData(integral);

    // ת����Service�����������н����Ĵ���
    CBufferData finishSSBuffer;
    finishSSBuffer.init(256);
    finishSSBuffer.writeData(m_nRoomType);  // ��������
    finishSSBuffer.writeData(result);       // ���
    finishSSBuffer.writeData(newintegral);  // �����Ļ���
    finishSSBuffer.writeData(integral);     // �ı�Ļ���
    finishSSBuffer.writeData(mmr);          // �ı��MMR
    finishSSBuffer.writeData(crylv);        // ˮ���ȼ�
    finishSSBuffer.writeData(dt);           // ��ʱ���룩
	finishSSBuffer.writeData(nMySummonerID);  // �ٻ�ʦID
	for (unsigned int i = 0; i < MAX_HERO_COUNT; i++)
	{
		if (i < vectHero.size())
		{
			finishSSBuffer.writeData(vectHero[i]->getSoldId());
		}
		else
		{
			finishSSBuffer.writeData((int)0);
		}
	}
	finishSSBuffer.writeData(nEnemySummerID);

    if (CHALLENGE_WON == result)
    {
        // ʤ������
        std::vector<DropItemInfo> vecDropItems;
        int winnerDropId = CCommonHelper::getArenaDrop(player->getUserLv(), player->getDayWinTimes());
        if (winnerDropId > 0)
        {
            CItemDrop::Drop(winnerDropId, vecDropItems);
        }

        bufferData->writeData(static_cast<int>(vecDropItems.size()));
        finishSSBuffer.writeData(static_cast<int>(vecDropItems.size()));
        for (std::vector<DropItemInfo>::iterator iter = vecDropItems.begin();
            iter != vecDropItems.end(); ++iter)
        {
            bufferData->writeData(*iter);
            finishSSBuffer.writeData(*iter);
        }
    }
    else
    {
        bufferData->writeData(0);
        finishSSBuffer.writeData(0);
    }

    // ����������ݣ�ȷ�����ڶ��߶�ʧ�˸����ݵ���ҿ��Եõ�������棬CBattleDataCache���Զ����� delete
    CBattleDataCache::getInstance()->pushBackBattleData(player->getUserId(), bufferData);
    // ���߿ͻ��˽��
    CKxCommManager::getInstance()->sendData(player->getUserId(),
        CMD_PVP, CMD_PVP_RESULT_SC, bufferData->getBuffer(), bufferData->getDataLength());
    // ����Service�����ͳ�ơ��������а񡢸������ݵȵ�
    CKxCommManager::getInstance()->forwardData(player->getUserId(),
        CMD_PVP, CMD_PVP_FINISH_SS, finishSSBuffer.getBuffer(), finishSSBuffer.getDataLength());
}

int CPvpRoom::getRoomState()
{
    return m_nRoomState;
}

bool CPvpRoom::isFull()
{
    return static_cast<int>(m_Players.size()) >= m_nRoomCapacity;
}

bool CPvpRoom::isAllReady()
{
	if ((int)m_PlayerProgress.size() < m_nRoomCapacity)
	{
		return false;
	}

    for (std::map<int, int>::iterator iter = m_PlayerProgress.begin(); 
        iter != m_PlayerProgress.end(); ++iter)
    {
        if (iter->second < 100)
        {
            return false;
        }
    }

    return true;
}

void CPvpRoom::playerLoading(int uid, int progress)
{
    if (m_PlayerProgress.empty())
    {
        for (std::map<int, CPlayer*>::iterator iter = m_Players.begin(); 
            iter != m_Players.end(); ++iter)
        {
            m_PlayerProgress[iter->first] = 0;
        }
    }

    m_PlayerProgress[uid] = progress;
}

int CPvpRoom::getPlayerProgress(int uid)
{
	if (m_PlayerProgress.find(uid) != m_PlayerProgress.end())
	{
		return m_PlayerProgress[uid];
	}
	return 0;
}

void CPvpRoom::playerReady(int uid)
{
	KXLOGDEBUG("PVP Room user %d is ready!!!", uid);
	// �������
	playerLoading(uid, 100);

	PvpReadySC pvpReadySC;
	pvpReadySC.uid = uid;
	sendRoom(CMD_PVP, CMD_PVP_READY_SC, reinterpret_cast<char*>(&pvpReadySC), sizeof(pvpReadySC));
    // ���仹δ��ʼ
    if (m_nRoomState <= ROOMSTATE_LOADING)
    {
        if (isAllReady())
        {
            //��ʼ��Ϸ
            startRoom();
        }
        else
        {
            //������ֶ���, �ȴ�n�������Ϸ
            if (isPlayerDisconnect(getOpponentUid(uid)))
            {
                m_nRoomTick = 10;
            }
        }
    }
    else if (m_nRoomState == ROOMSTATE_IDLETIME)
    {
        // ����յ�Readyʱ���䴦��׼����ʼ״̬��Ҫ������л���ս������
        PvpStartSC startSC;
        startSC.startStamp = m_nStartStamp;
        CKxCommManager::getInstance()->sendData(uid, CMD_PVP, CMD_PVP_IDLETIME_SC,
            reinterpret_cast<char*>(&startSC), sizeof(startSC));
    }
    else
    {
        // ����յ�Readyʱ���䴦��ս��״̬��Ҫ�����ˢ�����ݣ����л���ս������
        PvpReconnectSC reconnectSC;
        reconnectSC.roomState = m_nRoomState;
        reconnectSC.roomType = getRoomType();
        CKxCommManager::getInstance()->sendData(uid, CMD_PVP, CMD_PVP_RECONNECT_SC,
            reinterpret_cast<char *>(&reconnectSC), sizeof (reconnectSC));

        PvpStartSC startSC;
        startSC.startStamp = m_nStartStamp;
        CKxCommManager::getInstance()->sendData(uid, CMD_PVP, CMD_PVP_IDLETIME_SC,
            reinterpret_cast<char*>(&startSC), sizeof(startSC));
    }
}

void CPvpRoom::playerReconnect(int uid)
{
	CPlayer *pPlayer = getRoomPlayer(uid);
	if (NULL == pPlayer)
	{
		KXLOGERROR("PVP room user %d is NULL!", uid);
		return;
	}
	pPlayer->setIsConnecting(true);

	int opponentUid = getOpponentUid(uid);
	sendPlayer(opponentUid, CMD_PVP, CMD_PVP_OPPRECONNECT_SC, NULL, 0);
}

void CPvpRoom::playerDisconnect(int uid)
{
	CPlayer *pPlayer = getRoomPlayer(uid);
	if (NULL == pPlayer)
	{
		KXLOGERROR("playerDisconnect PVP room user %d is NULL!", uid);
		return;
	}
	pPlayer->setIsConnecting(false);
	KXLOGINFO("PVP room user %d is disconnect!");

    // �����ж��߲����, ��ȴ��������
    // 	if (m_nRoomState == ROOMSTATE_LOADING)
    // 	{
    // 		PvpLoadingSC loadingSC;
    // 		loadingSC.uid = uid;
    // 		loadingSC.progress = 100;
    // 
    // 		sendRoom(CMD_PVP, CMD_PVP_LOADING_SC,
    // 			reinterpret_cast<char*>(&loadingSC), sizeof(loadingSC));
    // 		// �������׼������
    // 		playerReady(uid);
    // 	}

	// ֪ͨ������Ҷ���
	int opponentUid = getOpponentUid(uid);
	sendPlayer(opponentUid, CMD_PVP, CMD_PVP_OPPDISCONNECT_SC, NULL, 0);
}

int CPvpRoom::getOpponentUid(int uid)
{
	for (std::map<int, CPlayer*>::iterator iter = m_Players.begin();
		iter != m_Players.end(); ++iter)
	{
		if (iter->first != uid)
		{
			return iter->first;
		}
	}
	return 0;
}

void CPvpRoom::setRoomTick(int tick)
{
	m_nRoomTick = tick;
}

int CPvpRoom::setStageId()
{
	m_nStageId = 1;
	if (static_cast<int>(m_Players.size()) >= m_nRoomCapacity)
	{
		int finalDan = 0;
		for (std::map<int, CPlayer*>::iterator iter = m_Players.begin(); 
			iter != m_Players.end(); ++iter)
		{
            // ��ȡ���MMR��Ӧ�Ķ�λ
            int dan = CCommonHelper::getUserMMRDan(iter->second->getMMR());
			if (dan > finalDan)
			{
				finalDan = dan;
			}
		}

		m_nStageId = qureyRandomPvpStage(finalDan);
	}
	return m_nStageId;
}

int CPvpRoom::getStageId()
{
    return m_nStageId;
}

int CPvpRoom::getBattleId()
{
	return m_nBattleId;
}

int CPvpRoom::getCurrentTick()
{
    if (m_pBattleScene != NULL)
    {
        return m_pBattleScene->getBattleHelper()->GameTick;
    }
    return 0;
}

int CPvpRoom::getRoomTick()
{
	return m_nRoomTick;
}

float CPvpRoom::getPrepareTick()
{
	return m_fPrepareFightTick;
}

CPlayer *CPvpRoom::getRoomPlayer(int uid)
{
    std::map<int, CPlayer*>::iterator iter = m_Players.find(uid);
    if (iter != m_Players.end())
    {
        return iter->second;
    }

    return NULL;
}

CPlayer *CPvpRoom::getMasterPlayer()
{
    return m_pMaster;
}

CPlayer *CPvpRoom::getOtherPlayer()
{
    return m_pOther;
}

void CPvpRoom::getRoomData(CBufferData& bufferData)
{
    RoomData roomData;
    roomData.BattleType = EBATTLE_PVP;
	roomData.StageLevel = 1;
	roomData.StageId = m_nStageId;
    roomData.PlayerCount = m_Players.size();
	roomData.Ext1 = 0;
	roomData.Ext2 = 0;
	roomData.BuffCount = 0;
    bufferData.writeData(&roomData, sizeof(roomData));

    for (std::map<int, CPlayer*>::iterator iter = m_Players.begin(); 
        iter != m_Players.end(); ++iter)
    {
        iter->second->fillBufferData(bufferData);
    }
}

void CPvpRoom::getBattleData(CBufferData &bufferData)
{
    if (NULL != m_pBattleScene)
    {
        m_pBattleScene->getBattleHelper()->serialize(bufferData);
    }
}

std::map<int, CPlayer*> &CPvpRoom::getRoomPlayers()
{
    return m_Players;
}

bool CPvpRoom::isRoomPlayer(int uid)
{
    return m_Players.find(uid) != m_Players.end(); 
}

bool CPvpRoom::isAllDisconnect()
{
	for (std::map<int, CPlayer*>::iterator iter = m_Players.begin();
		iter != m_Players.end(); ++iter)
	{
		if (iter->second->isConnecting())
		{
			return false;
		}
	}
	return true;
}

bool CPvpRoom::isPlayerDisconnect(int uid)
{
	std::map<int, CPlayer*>::iterator iter = m_Players.find(uid);
	return iter == m_Players.end() || !iter->second->isConnecting();
}

bool CPvpRoom::playerJoin(int uid, char* playerData, int len)
{
    if (static_cast<int>(m_Players.size()) >= m_nRoomCapacity
        || m_Players.find(uid) != m_Players.end())
    {
        KXLOGERROR("PVP room user %d! already in room!", uid);
        return false;
    }

    CPlayer *pPlayer = new CPlayer();
    if (!pPlayer->initWithData(playerData, len))
    {
        KXLOGERROR("PVP room user %d! initWithData faile!", uid);
        delete pPlayer;
        return false;
    }

    m_Players[uid] = pPlayer;
    if (m_Players.size() == 1)
    {
        m_pMaster = pPlayer;
        pPlayer->setCamp(ECamp_Blue);
    }
    else
    {
        m_pOther = pPlayer;
        pPlayer->setCamp(ECamp_Red);
    }
    return true;
}

bool CPvpRoom::playerQuit(int uid)
{
	KXLOGDEBUG("PVP room uid=%d quit room!", uid);
    map<int, CPlayer*>::iterator iter = m_Players.find(uid);
    if (iter != m_Players.end())
    {
        CPlayer *pPlayer = iter->second;
        if (NULL != pPlayer)
        {
            delete pPlayer;
        }
        m_Players.erase(iter);
    }

	CKxCommManager::getInstance()->delKxComm(uid);
    return true;
}

void CPvpRoom::setWinnerId(int uid)
{
	m_nWinnerId = uid;
}

void CPvpRoom::setLoserId(int uid)
{
	m_nLoserId = uid;
}

void CPvpRoom::setPlayerCommunication(int uid, KxServer::IKxComm *target)
{
	m_PlayerCommu[uid] = target;
	CKxCommManager::getInstance()->addKxComm(uid, target);
}

int CPvpRoom::sendRoom(int maincmd, int subcmd, char *data, unsigned int len)
{
	for (std::map<int, KxServer::IKxComm *>::iterator iter = m_PlayerCommu.begin();
		iter != m_PlayerCommu.end(); ++iter)
	{
        CKxCommManager::getInstance()->sendData(
            iter->first, maincmd, subcmd, data, len);
	}
	return 0;
}

int CPvpRoom::sendPlayer(int uid, int maincmd, int subcmd, char *data, unsigned int len)
{
	if (m_PlayerCommu.find(uid) == m_PlayerCommu.end())
    {
        //��������ͬ������˷�����Ϣ(�����������ս���޹���Ϣ����)
        return 0;
    }

	return CKxCommManager::getInstance()->sendData(uid,maincmd, subcmd, data,len);
}
