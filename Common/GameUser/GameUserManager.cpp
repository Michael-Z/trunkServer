#include "GameUserManager.h"
#include "StorageManager.h"
#include "RedisStorer.h"
#include "ConfGameSetting.h"
#include "ConfStage.h"
#include "RankModel.h"
#include "PvpRankModel.h"
#include "ModelHelper.h"
#include "TaskAchieveHelper.h"
#include "ActiveHelper.h"
#include "GlobalMailModel.h"

CGameUserManager::CGameUserManager()
:m_pPassTeamModel(NULL)
{
	m_VectServerModel.clear();
	m_DelUserList.clear();
	m_MapDelUserList.clear();
}

CGameUserManager::~CGameUserManager()
{
	for (std::map<int, CGameUser *>::iterator iter = m_GameUsers.begin();
		iter != m_GameUsers.end(); ++iter)
	{
		delete iter->second;
	}

	m_VectServerModel.clear();
	m_DelUserList.clear();
	m_MapDelUserList.clear();
}

CGameUserManager * CGameUserManager::m_pInstance = NULL;
CGameUserManager *CGameUserManager::getInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CGameUserManager;
		m_pInstance->m_pPassTeamModel = new CPassTeamModel;
		m_pInstance->m_pPassTeamModel->init();
	}
	return m_pInstance;
}

void CGameUserManager::destroy()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
	}
}

bool CGameUserManager::init(KxServer::KxTimerManager *pTimerManager)
{
	m_TimeManager = pTimerManager;

	if (m_TimeManager == NULL)
	{
		return false;
	}

	if (!m_TimeManager->addTimer(this, CHECK_TIME_INTERVAL, KXREPEAT_FOREVER))
	{
		return false;
	}

	return true;
}

void CGameUserManager::addModelType(ModelType Type)
{
	m_VectServerModel.push_back(Type);
}

void CGameUserManager::onTimer(const KxServer::kxTimeVal& now)
{
	int nCurTime = (int)time(NULL);

	for (std::list<SDelayDelData>::iterator ator = m_DelUserList.begin();
        ator != m_DelUserList.end();)
	{
		if (nCurTime > ator->nDelayTime)
		{
			RealremoveGameUser(ator->nUid);
			m_MapDelUserList.erase(ator->nUid);
			m_DelUserList.erase(ator++);
		}
		else
		{
			break;
		}
	}
}

CGameUser* CGameUserManager::getGameUser(int uid, bool noNull)
{
	std::map<int, CGameUser *>::iterator iter = m_GameUsers.find(uid);
	if (iter != m_GameUsers.end())
	{
		return iter->second;
	}

	if (noNull)
	{
		return initGameUser(uid);
	}
	return NULL;
}

CGameUser* CGameUserManager::initGameUser(int uid)
{
	std::map<int, CGameUser *>::iterator iter = m_GameUsers.find(uid);
	if (iter == m_GameUsers.end())
	{
		CGameUser *pGameUser = new CGameUser;
		if (!pGameUser->initModels(uid))
		{
			delete pGameUser;
			return NULL;
		}

		m_GameUsers[uid] = pGameUser;
		initUserData(pGameUser);
		return pGameUser;
	}

	return NULL;
}

CGameUser *CGameUserManager::newGameUser(int uid)
{
	// ���û�
	CGameUser *pGameUser = new CGameUser;
	int nCreateTime = KxServer::KxBaseServer::getInstance()->getTimerManager()->getTimestamp();
	// ����ÿ��ģ��
	CUserModel *pUserModel = new CUserModel;
	CBagModel *pBagModel = new CBagModel;
	CEquipModel *pEquipModel = new CEquipModel;
	CStageModel *pStageModel = new CStageModel;
	CSummonModel *pSummonModel = new CSummonModel;
	CHeroModel *pHeroModel = new CHeroModel;
	CTeamModel *pTeamModel = new CTeamModel;
	CTaskModel *pTaskModel = new CTaskModel;
	CAchievementModel *pAchieveModel = new CAchievementModel;
	CGuideModel *pGuideModel = new CGuideModel;
	CInstanceModel*pInstanceModel = new CInstanceModel;
	CMailModel *pMailModel = new CMailModel;
	CGoldTestModel * pGoldTestModel = new CGoldTestModel;
	CHeroTestModel * pHeroTestModel = new CHeroTestModel;
	CTowerTestModel *pTowerTestModel = new CTowerTestModel;
	CPvpModel *pPvpModel = new CPvpModel;
	CShopModel *pShopModel = new CShopModel;
	CUserActiveModel *pActiveModel = new CUserActiveModel();
	CHeadModel *pHeadModel = new CHeadModel();
	CUserUnionModel *pUserUnionModel = new CUserUnionModel;
	CPersonMercenaryModel *pPersonMercenaryModel = new CPersonMercenaryModel;
	CBlueDiamondModel *pBDModel = new CBlueDiamondModel;
    CPvpChestModel *pPvpChestModel = new CPvpChestModel;

	pGameUser->setModel(MODELTYPE_USER, pUserModel);
	pGameUser->setModel(MODELTYPE_BAG, pBagModel);
	pGameUser->setModel(MODELTYPE_EQUIP, pEquipModel);
	pGameUser->setModel(MODELTYPE_STAGE, pStageModel);
	pGameUser->setModel(MODELTYPE_SUMMONER, pSummonModel);
	pGameUser->setModel(MODELTYPE_HERO, pHeroModel);
	pGameUser->setModel(MODELTYPE_TEAM, pTeamModel);
	pGameUser->setModel(MODELTYPE_TASK, pTaskModel);
	pGameUser->setModel(MODELTYPE_ACHIEVEMENT, pAchieveModel);
	pGameUser->setModel(MODELTYPE_GUIDE, pGuideModel);
	pGameUser->setModel(MODELTYPE_INSTANCE, pInstanceModel);
	pGameUser->setModel(MODELTYPE_MAIL, pMailModel);
	pGameUser->setModel(MODELTYPE_GOLDTEST, pGoldTestModel);
	pGameUser->setModel(MODELTYPE_HEROTEST, pHeroTestModel);
	pGameUser->setModel(MODELTYPE_TOWERTEST, pTowerTestModel);
	pGameUser->setModel(MODELTYPE_PVP, pPvpModel);
	pGameUser->setModel(MODELTYPE_SHOP, pShopModel);
	pGameUser->setModel(MODELTYPE_ACTIVE, pActiveModel);
	pGameUser->setModel(MODELTYPE_HEAD, pHeadModel);
	pGameUser->setModel(MODELTYPE_USERUNION, pUserUnionModel);
	pGameUser->setModel(MODELTYPE_MERCENARY, pPersonMercenaryModel);
	pGameUser->setModel(MODELTYPE_BDACTIVE, pBDModel);
    pGameUser->setModel(MODELTYPE_PVPCHEST, pPvpChestModel);
	pGameUser->setUid(uid);

	// ��ӵ���ҹ�����
	addGameUser(uid, pGameUser);

	// ��ȡ���, ����������Ʒ����������	
	const NewPlayerItem *pNewConf = queryConfNewPlayerItem(1);
	// �û���Ϣģ�ͳ�ʼ��
	char name[32] = {};
	sprintf(name, "%s", pNewConf->UserName.c_str());
	// �û�������Ϣ
	std::map<int, int> attrs;
	for (int i = USR_FD_USERID; i < USR_FD_END; i++)
	{
		// ��������Ĭ��Ϊ0
		attrs[i] = 0;
	}

	attrs[USR_FD_USERID] = uid;
	attrs[USR_FD_USERLV] = pNewConf->UserLv;
	attrs[USR_FD_EXP] = pNewConf->UserExp;
	attrs[USR_FD_GOLD] = pNewConf->Gold;
	attrs[USR_FD_DIAMOND] = pNewConf->Diamond;
	attrs[USR_FD_FREEHEROTIMES] = 1;
	attrs[USR_FD_BAGCAPACITY] = pNewConf->InitBagCapacity;
	attrs[USR_FD_EQUIPREF] = DEFAULT_EQUIPREF;
	attrs[USR_FD_RESETTIMESTAMP] = 0;
	attrs[USR_FD_CREATETIME] = nCreateTime;

	if (!pNewConf->Heads.empty())
	{
		if (!pHeadModel->init(uid))
		{
			return NULL;
		}

		for (size_t i = 0; i < pNewConf->Heads.size(); i++)
		{
			const std::map<int, SSystemHeadIconItem>& mapAllHeadIcon = queryConfHeadIcon();

			if (mapAllHeadIcon.find(pNewConf->Heads[i]) != mapAllHeadIcon.end())
			{
				pHeadModel->AddHeadID(pNewConf->Heads[i]);

				if (attrs[USR_FD_HEADICON] == 0)
				{
					attrs[USR_FD_HEADICON] = pNewConf->Heads[i];
				}
			}
		}
	}

	if (!pUserModel->NewUser(uid, name, attrs))
	{
		// �û����ݳ�ʼ��ʧ��
		return NULL;
	}

	//����ɫ�ȼ����а���
	CRankModel::getInstance()->AddRankData(LEVEL_RANK_TYPE, uid, pNewConf->UserLv);

	// ����ģ������ & װ������
	if (pBagModel->init(uid) && pEquipModel->init(uid))
	{
		// ����Ʒ
		for (std::map<int, int>::const_iterator iter = pNewConf->Items.begin();
			iter != pNewConf->Items.end(); ++iter)
		{
			const PropItem* pPropItem = queryConfProp(iter->first);
			if (pPropItem)
			{
				pBagModel->AddItem(iter->first, iter->second);
			}
			else
			{
				LOG("Execute False On File %s Line %d : %d", __FILE__, __LINE__, iter->first);
			}
		}
		// װ��
		if (pNewConf->Equips.size() > 0)
		{
			std::map<int, int>::const_iterator iterEquip = pNewConf->Equips.begin();
			for (; iterEquip != pNewConf->Equips.end(); ++iterEquip)
			{
				for (int i = 0; i < iterEquip->second; i++)
				{
					SEquipInfo EquipInfo;
					if (!CModelHelper::addEquip(uid, iterEquip->first, EquipInfo))
					{
						continue;
					}
				}
			}
		}
	}
	// �ٻ�ʦģ������
	if (pSummonModel->init(uid))
	{
		// �ٻ�ʦ
		for (std::vector<int>::const_iterator iter = pNewConf->SummonerIds.begin();
			iter != pNewConf->SummonerIds.end(); ++iter)
		{
			pSummonModel->AddSummon(*iter);
		}
	}
	// Ӣ��ģ������
	if (pHeroModel->init(uid))
	{
		// Ӣ��
		if (pNewConf->Heros.size() > 0)
		{
			for (std::vector<NewPlayerHeroInfo>::const_iterator iter = pNewConf->Heros.begin();
				iter != pNewConf->Heros.end(); ++iter)
			{
				const NewPlayerHeroInfo &heroInfo = *iter;
				CModelHelper::addHero(uid, heroInfo.heroId, heroInfo.heroStarLv, 1);
			}
		}
	}
	// �ؿ�ģ��
	if (pStageModel->init(uid))
	{
		if (pNewConf->Chapters.size() > 0)
		{
			for (std::vector<int>::const_iterator iter = pNewConf->Chapters.begin();
				iter != pNewConf->Chapters.end(); ++iter)
			{
				int chapterId = *iter;
				const ChapterConfItem *pChapterConf = queryConfChapter(chapterId);
				if (NULL != pChapterConf)
				{
					if (pChapterConf->Type == CHAPTERTYPE_NORMAL)
					{
						//������ͨ�½�
						pStageModel->SetChapterStatus(chapterId, CHAPTERSTATUS_UNLOCK);
						pStageModel->SetCurStage(pChapterConf->ExInfo.FirstStageID);
					}
					else
					{
						//������Ӣ�½�
						pStageModel->SetEliteChapterStatus(chapterId, CHAPTERSTATUS_UNLOCK);
						pStageModel->SetCurElite(pChapterConf->ExInfo.FirstStageID);
					}
				}
			}
		}
	}
	// ����ģ��
	if (pTeamModel->init(uid))
	{
		if (pNewConf->SummonerInGroup > 0)
		{
			TeamInfo teamInfo;
			memset(&teamInfo, 0, sizeof(teamInfo));
			teamInfo.summonerId = pNewConf->SummonerInGroup;
			if (!pTeamModel->SetTeamInfo(ETT_PASE, teamInfo))
			{
				KXLOGDEBUG("userRegister SetTeamInfo failed");
			}
		}
	}
	// ����ģ��
	if (pTaskModel->init(uid))
	{
	}
	// �ɾ�ģ��
	if (pAchieveModel->init(uid))
	{
	}
	// ����ģ��
	if (pGuideModel->init(uid))
	{
		if (!pNewConf->Guides.empty())
		{
			for (size_t i = 0; i < pNewConf->Guides.size(); i++)
			{
				pGuideModel->AddGuideID(pNewConf->Guides[i]);
			}
		}
	}
	// �����ģ��
	if (!pInstanceModel->init(uid))
	{
		KXLOGERROR("CInstanceModel init fail!!!");
	}
	// �ʼ�ģ��
	if (!pMailModel->init(uid))
	{
		KXLOGERROR("CMailModel init fail!!!");
	}
	// �������ģ��
	if (!pGoldTestModel->init(uid))
	{
		KXLOGERROR("CGoldTestModel init fail!!!");
	}
	// Ӣ������ģ��
	if (!pHeroTestModel->init(uid))
	{
		KXLOGERROR("CHeroTestModel init fail!!!");
	}
	// ��������
	if (pTowerTestModel->init(uid))
	{
		//��ʼ����������
		pTowerTestModel->ResetTowerTest();
		CModelHelper::updateTowerTestInfo(uid);
	}
	// pvpģ��
	if (pPvpModel->init(uid))
	{
		std::map<int, int> mapValues;
		for (int i = PVP_FD_MMR; i < PVP_FD_MAX; ++i)
		{
			mapValues[i] = 0;
		}
		
		mapValues[PVP_FD_MMR] = 750;
		
		if (!pPvpModel->SetPvpField(mapValues))
		{
			KXLOGERROR("Init New Player Pvp Model Data Error!");
		}
	}
	// pvp������
	if (pPvpChestModel->init(uid))
	{
		int maxChest = 5;
		int index = 0;
		for (std::vector<int>::const_iterator iter = pNewConf->ChestIds.begin();
			iter != pNewConf->ChestIds.end(); ++iter)
		{
			if (++index <= maxChest)
			{
				CModelHelper::addPvpChest(uid, *iter);
			}
		}
		// ���䲻��, ��¼�������ʱ��
		if (maxChest > index)
		{
			pPvpModel->SetPvpField(PVPCOMM_FD_LASTCHESTGENTIME, nCreateTime);
		}
	}

	// �̵�ģ��
	if (!pShopModel->init(uid))
	{
		KXLOGERROR("CShopModel init fail!!!");
	}

	// ���˻����ģ��
	if (!pActiveModel->init(uid))
	{
		KXLOGERROR("CActiveModel init fail!!!");
	}

	if (!pUserUnionModel->init(uid))
	{
		KXLOGERROR("CUserUnionModel init fail!!!");
	}

	if (!pPersonMercenaryModel->init(uid))
	{
		KXLOGERROR("CPersonMecenaryModel init fail!!!");
	}

	// ����ɾ����Ŵ���,������Ҫ�õ�����ģ��
	if (!pNewConf->Tasks.empty())
	{
		for (size_t i = 0; i < pNewConf->Tasks.size(); ++i)
		{
			CTaskAchieveHelper::addTask(uid, pNewConf->Tasks[i]);
		}
	}

	if (!pNewConf->Achieves.empty())
	{
		for (size_t i = 0; i < pNewConf->Achieves.size(); ++i)
		{
			CTaskAchieveHelper::addAchieve(uid, pNewConf->Achieves[i]);
		}
	}

	if (pBDModel->init(uid))
	{

	}

    //pvp����ģ��


	initUserData(pGameUser);
	return pGameUser;
}

//�����û���Ҫ���õ�����
void CGameUserManager::reSetGameUserData(int uid, bool bLogin)
{
	CGameUser *pGameUser = getGameUser(uid);
	if (pGameUser == NULL)
	{
		return;
	}
    initUserData(pGameUser);
    updateGameUserData(pGameUser, bLogin);
}

void CGameUserManager::updateGameUserData(CGameUser* gameUsr, bool bLogin)
{
    if (gameUsr == NULL)
    {
        return;
    }
    int uid = gameUsr->getUid();
    //���½�ҹ������
    CModelHelper::updateUserModelReset(uid, bLogin);
    //���ý�Ҹ�����Ϣ
    CModelHelper::updateGoldTestInfo(uid);
    //����Ӣ��������Ϣ
    CModelHelper::updateHeroTestInfo(uid);
    //��¼ʱ�ȸ�����������������
    CModelHelper::updateTowerTestInfo(uid);
    //����PVP��Ϣ
    CModelHelper::updatePvpInfo(uid);
    //�����̵���Ϣ
    CModelHelper::updateShopData(uid);
    CModelHelper::checkShopFreshTimes(uid);
    // �ȴ�������ڵ��ʼ�
    CModelHelper::removeExpireGlobalMail();
    CModelHelper::removeExpireMail(uid);
    // ������������
    CModelHelper::updateTaskInfo(uid);
    // ���˹�����Ϣ
    CModelHelper::updateUserUnionInfo(uid, false);
}

void CGameUserManager::addGameUser(int uid, CGameUser* gameUsr)
{
	std::map<int, CGameUser *>::iterator iter = m_GameUsers.find(uid);
	if (iter == m_GameUsers.end())
	{
		m_GameUsers[uid] = gameUsr;
	}
}

//����û��ڸ÷������Ƿ����
bool CGameUserManager::checkUserIsExist(int uid)
{
	std::map<int, CGameUser *>::iterator iter = m_GameUsers.find(uid);
	if (iter == m_GameUsers.end())
	{
		return false;
	}
	return true;
}

void CGameUserManager::removeGameUser(int uid)
{
	if (m_TimeManager == NULL)
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

//ɾ���Ƴ��û�����
void CGameUserManager::donotDeleteUser(int uid)
{
	std::map<int, std::list<SDelayDelData>::iterator>::iterator ator = m_MapDelUserList.find(uid);
	if (ator != m_MapDelUserList.end())
	{
		std::list<SDelayDelData>::iterator iter = ator->second;
		m_DelUserList.erase(iter);
		m_MapDelUserList.erase(ator);
	}
}

bool CGameUserManager::initUserData(CGameUser* gameUsr)
{
	if (gameUsr == NULL)
	{
		return false;
	}
	//�������
	gameUsr->getUserActionManager()->CleanAction();
	//��Ӽ���
	CTaskAchieveHelper::initTask(gameUsr->getUid());
	CTaskAchieveHelper::initAchieve(gameUsr->getUid());
	CActiveHelper::getInstance()->initActiveTask(gameUsr->getUid());
	return true;
}

//����ɾ���û�
void CGameUserManager::RealremoveGameUser(int uid)
{
	std::map<int, CGameUser *>::iterator iter = m_GameUsers.find(uid);
	if (iter != m_GameUsers.end())
	{
		delete iter->second;
		m_GameUsers.erase(iter);
	}
}
