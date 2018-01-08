#include "Dispatch.h"
#include "ConfManager.h"
#include "Hero.h"

CDispatch::CDispatch() :m_nPlayerCamp(0)
, m_nEnemyCamp(0)
, m_pBattleHelper(NULL)
, m_ConfStageItem(NULL)
{
	m_listDispatchClose.clear();
	m_listDispatchOpen.clear();
	m_listDispatchCache.clear();
	m_listDispatchInfo.clear();
}


CDispatch::~CDispatch()
{
}

bool CDispatch::init(int stageID, int playerID, int enemyID, CBattleHelper *battleHelper)
{
    // �ؿ�����
    const StageConfItem *pStageConf = queryConfStage(stageID);
	CHECK_RETURN(NULL != pStageConf);

	// �ؿ���������
    CConfDispatch *pDispatchConf = queryConfDispatch(stageID);
	CHECK_RETURN(NULL != pDispatchConf);

	m_ConfStageItem = pStageConf;
    m_nPlayerCamp = battleHelper->getCampWithUid(playerID);
    // enemyID->camp
    m_nEnemyCamp = enemyID;//battleHelper->getCampWithUid(enemyID);
	m_pBattleHelper = battleHelper;

    // �ؿ�����
	const std::map<int, void*> &mapDispatchData = pDispatchConf->getDatas();
	const std::vector<int>& vecGroup = pDispatchConf->getGroup();

	// �����ӵķ���
	int nRandGroup = 0;
	if (!vecGroup.empty())
	{
		nRandGroup = vecGroup[m_pBattleHelper->RandNum.random() % vecGroup.size()];
	}

	// ������Ϊ0�ļ���
	std::map<int, void*>::const_iterator iter = mapDispatchData.begin();
	for (; iter != mapDispatchData.end(); ++iter)
	{
		CDispatchItem *pDispatchItem = static_cast<CDispatchItem *>(iter->second);
		if (NULL != pDispatchItem)
		{
            // ����Ϊ0������ķ���ȫ������
            if (0 == pDispatchItem->Group || nRandGroup == pDispatchItem->Group)
            {
                if (pDispatchItem->IsOpen)	// Ĭ�Ͽ���
                {
                    m_listDispatchOpen.push_back(pDispatchItem);
                    continue;
                }
                // ��Ҫ���ص��ǹر�״̬
                m_listDispatchClose.push_back(pDispatchItem);
            }
		}
	}

	return true;
}

void CDispatch::logicUpdate(float dt)
{
    if (!m_listDispatchInfo.empty())
    {
        std::list<int> lsCloseCondition;

        // ������������
        std::list<SDispatchInfo>::iterator iterDispatchInfo = m_listDispatchInfo.begin();
        for (; iterDispatchInfo != m_listDispatchInfo.end();)
        {
            iterDispatchInfo->Delay -= dt;
            if (iterDispatchInfo->Delay <= 0)
            {
                // �ӳ�ʱ�䵽,����
                execute(*iterDispatchInfo);
                // �����¿���������
                VecInt::iterator iterConditionOpen = iterDispatchInfo->ConditionOpenID.begin();
                for (; iterConditionOpen != iterDispatchInfo->ConditionOpenID.end(); ++iterConditionOpen)
                {
                    // �ɹر��б�תΪ�����б�
                    processCloseList(*iterConditionOpen);
                }

                // �رտ���������
                VecInt::iterator iterConditionClose = iterDispatchInfo->ConditionCloseID.begin();
                for (; iterConditionClose != iterDispatchInfo->ConditionCloseID.end(); ++iterConditionClose)
                {
                    // �ɿ����򻺴��б�תΪ�ر��б�
                    processOpenList(*iterConditionClose);
                    lsCloseCondition.push_back(*iterConditionClose);
                }

                iterDispatchInfo = m_listDispatchInfo.erase(iterDispatchInfo);
            }
            else
            {
                ++iterDispatchInfo;
            }
        }

        // �������б����йرյ��Ŀ�������ȥ��
        if (!lsCloseCondition.empty())
        {
            std::list<int>::iterator iterClose = lsCloseCondition.begin();
            for (; iterClose != lsCloseCondition.end(); ++iterClose)
            {
                processDispatchList(*iterClose);
            }
        }
    }

    // �������б�����ݲ��뿪���б��ĩβ
    if (!m_listDispatchCache.empty())
    {
        m_listDispatchOpen.splice(m_listDispatchOpen.end(), m_listDispatchCache);
        // ��������б�
        //m_listDispatchCache.clear();
    }
	
	// �����б��ж��Ƿ�����������
	std::list<CDispatchItem*>::iterator iter = m_listDispatchOpen.begin();
	for (; iter != m_listDispatchOpen.end();)
	{
		CDispatchItem *pDispatchItem = *iter;
		CHECK_RETURN_VOID(NULL != pDispatchItem);

		// �ж�����
		if (checkCondition(pDispatchItem->ConditionType, pDispatchItem->ConditionParam))
		{
			// ֻ��Ҫ�ӳ�ʱ����б�
			m_listDispatchInfo.push_back(pDispatchItem->DispatchInfo);
			// ����ر��б�
			m_listDispatchClose.push_back(pDispatchItem);
			// �ӿ����б�ȥ��
			iter = m_listDispatchOpen.erase(iter);
 		}
 		else
 		{
 			++iter;
 		}
	}
}

bool CDispatch::checkCondition(const int& conditionType, const int& conditionVal)
{
	bool bRlt = false;
	switch (conditionType)
	{
	case DC_Non:
		bRlt = true;
		break;
	case DC_PCHeroDistance:
		bRlt = checkPCHeroDistance(conditionVal);
		break;
	case DC_PCHeroHP:
		bRlt = checkPCHeroHP(conditionVal);
		break;
	case DC_CrystalLv:
		bRlt = checkCrystalLv(conditionVal);
		break;
	case DC_SoldierCount:
		bRlt = checkSoldierCount(conditionVal);
		break;
	case DC_SoldierMaxStar:
		bRlt = checkSoldierMaxStar(conditionVal);
		break;
	case DC_DispatchSoldierCount:
		bRlt = checkDispatchSoldierCount(conditionVal);
		break;
	case DC_DeadSoldierCount:
		bRlt = checkDeadSoldierCount(conditionVal);
		break;
	case DC_PCDeadSoldierCount:
		bRlt = checkPCDeadSoldierCount(conditionVal);
		break;
	case DC_DispatchSoldierType:
		bRlt = checkDispatchSoldierType(conditionVal);
		break;
	case DC_DispatchSoldierID:
		bRlt = checkDispatchSoldierID(conditionVal);
		break;
	default:
		break;
	}

	return bRlt;
}

void CDispatch::execute(const SDispatchInfo &info)
{
	// �ڼ���ʿ��,���ñ��1��ʼ,�����ȡ��0��ʼ
	int nSoldierOrder = info.DispatchSoldierID - 1;

    if (nSoldierOrder < 0 || (int)m_ConfStageItem->Monsters.size() < info.DispatchSoldierID)
	{
		// ���ñ�����
		return;
	}

    // ���ñ���Ϣ
    m_pBattleHelper->createMonster(nSoldierOrder, info.DispatchLine);
}

bool CDispatch::checkPCHeroDistance(const int& conditionVal)
{
	CRole *pPCHero = m_pBattleHelper->getMainRole(m_nEnemyCamp);
	CHECK_RETURN(NULL != pPCHero);

	// ����Ӣ�۵�λ��
	Vec2 vPCHeroPos = pPCHero->getRealPosition();
	
	// ���ʿ���б�
	std::vector<CRole*> &vecPlayerSoldier = m_pBattleHelper->getRoleWithCamp(m_nPlayerCamp);
	std::vector<CRole*>::iterator iter = vecPlayerSoldier.begin();
	for (; iter != vecPlayerSoldier.end(); ++iter)
	{
		CRole *pRole = *iter;
		if (NULL == pRole)
		{
			continue;
		}

		// Ӣ�۵���ָ����Χ���Ƿ�����ҵ�ʿ��
		if (pRole->getRealPosition().distance(vPCHeroPos) <= conditionVal)
		{
			return true;
		}
	}

	return false;
}

bool CDispatch::checkPCHeroHP(const int& conditionVal)
{
	CRole *pPCHero = m_pBattleHelper->getMainRole(m_nEnemyCamp);
	CHECK_RETURN(NULL != pPCHero);

	float fHpPercent = pPCHero->getIntAttribute(EAttributeHP) * 1.0f / pPCHero->getIntAttribute(EAttributeMaxHP);
	if (fHpPercent <= conditionVal / 100.0f)
	{
		return true;
	}

	return false;
}

bool CDispatch::checkCrystalLv(const int& conditionVal)
{
	CRole *pPlayerHero = m_pBattleHelper->getMainRole(m_nPlayerCamp);
	CHECK_RETURN(NULL != pPlayerHero);

	if (pPlayerHero->getIntAttribute(EHeroCrystalLevel) >= conditionVal)
	{
		return true;
	}

	return false;
}

bool CDispatch::checkSoldierCount(const int& conditionVal)
{
	// ���ʿ���б�
	std::vector<CRole*> &vecPlayerSoldier = m_pBattleHelper->getRoleWithCamp(m_nPlayerCamp);
	// �ڳ�����
	if ((int)vecPlayerSoldier.size() >= conditionVal)
	{
		return true;
	}

	return false;
}

bool CDispatch::checkSoldierMaxStar(const int& conditionVal)
{
	CConfSoldier *pConfSoldier
		= static_cast<CConfSoldier*>(CConfManager::getInstance()->getConf(CONF_SOLDIER));
	CHECK_RETURN(NULL != pConfSoldier);

	// ���ʿ���б�
	std::vector<CRole*> &vecPlayerSoldier = m_pBattleHelper->getRoleWithCamp(m_nPlayerCamp);
	std::vector<CRole*>::iterator iter = vecPlayerSoldier.begin();
	for (; iter != vecPlayerSoldier.end(); ++iter)
	{
        int nConfigID = 0;
        int nStar = 0;
        if (!getSoldierInfo(*iter, nConfigID, nStar))
        {
            continue;
        }

		// ʿ����Ƭ����
		SoldierConfItem *pSoldierItem 
            = static_cast<SoldierConfItem *>(pConfSoldier->getData(nConfigID, nStar));
		// ʿ���Ǽ�
		if (pSoldierItem && pSoldierItem->Star >= conditionVal)
		{
			return true;
		}
	}

	return false;
}

bool CDispatch::checkDispatchSoldierCount(const int& conditionVal)
{
	if (m_pBattleHelper->isEnoughDispatchCount(conditionVal))
	{
		return true;
	}

	return false;
}

bool CDispatch::checkDeadSoldierCount(const int& conditionVal)
{
	if (m_pBattleHelper->isEnoughDeadSoldierCount(m_nPlayerCamp, conditionVal))
	{
		return true;
	}

	return false;
}

bool CDispatch::checkPCDeadSoldierCount(const int& conditionVal)
{
	if (m_pBattleHelper->isEnoughDeadSoldierCount(m_nEnemyCamp, conditionVal))
	{
		return true;
	}

	return false;
}

bool CDispatch::checkDispatchSoldierType(const int& conditionVal)
{
	// ���ʿ���б�
	std::vector<CRole*> &vecPlayerSoldier = m_pBattleHelper->getRoleWithCamp(m_nPlayerCamp);
	std::vector<CRole*>::iterator iter = vecPlayerSoldier.begin();
	for (; iter != vecPlayerSoldier.end(); ++iter)
	{
        int nConfigID = 0;
        int nStar = 0;
        if (!getSoldierInfo(*iter, nConfigID, nStar))
        {
            continue;
        }

        if (isSoldierType(nConfigID, nStar, conditionVal))
		{
            return true;
		}
	}

	return false;
}

bool CDispatch::checkDispatchSoldierID(const int& conditionVal)
{
	// ���ʿ���б�
	std::vector<CRole*> &vecPlayerSoldier = m_pBattleHelper->getRoleWithCamp(m_nPlayerCamp);
	std::vector<CRole*>::iterator iter = vecPlayerSoldier.begin();
	for (; iter != vecPlayerSoldier.end(); ++iter)
	{
        int nConfigID = 0;
        int nStar = 0;
        if (!getSoldierInfo(*iter, nConfigID, nStar))
        {
            continue;
        }

		// ʿ��ID
        if (nConfigID == conditionVal)
		{
			return true;
		}
	}

	return false;
}

void CDispatch::processCloseList(const int &conditionID)
{
	// ˵��:������������ڹر��б���,���ֿ���:
	// 1�����ڼ��ط�Χ��  2���ڿ����б���(��Ч)
	std::list<CDispatchItem*>::iterator iter = m_listDispatchClose.begin();
	for (; iter != m_listDispatchClose.end(); ++iter)
	{
		CDispatchItem *pDispatch = *iter;
		if (NULL == pDispatch)
		{
			continue;
		}

		if (pDispatch->ID == conditionID)
		{
			// �����б���,����뿪���б�
			m_listDispatchCache.push_back(pDispatch);
			// �ӹر��б���ȥ��
			m_listDispatchClose.erase(iter);

			break;
		}
	}
}

void CDispatch::processOpenList(const int &conditionID)
{
	std::list<CDispatchItem*>::iterator iter = m_listDispatchOpen.begin();
	for (; iter != m_listDispatchOpen.end(); ++iter)
	{
		CDispatchItem *pDispatch = *iter;
		if (NULL == pDispatch)
		{
			continue;
		}

		if (pDispatch->ID == conditionID)
		{
			// ����ر��б�
			m_listDispatchClose.push_back(pDispatch);
			// �ӿ����б�ȥ��
			m_listDispatchOpen.erase(iter);

			// ����ֻ��������һ���б�
			return;
		}
	}

	std::list<CDispatchItem*>::iterator iterCache = m_listDispatchCache.begin();
	for (; iterCache != m_listDispatchCache.end(); ++iterCache)
	{
		CDispatchItem *pDispatch = *iterCache;
		if (NULL == pDispatch)
		{
			continue;
		}

		if (pDispatch->ID == conditionID)
		{
			// ����ر��б�
			m_listDispatchClose.push_back(pDispatch);
			// �ӿ����б�ȥ��
			m_listDispatchCache.erase(iterCache);

			// ����ֻ��������һ���б�
			return;
		}
	}
}

void CDispatch::processDispatchList(const int &conditionID)
{
	std::list<SDispatchInfo>::iterator iterDispatch = m_listDispatchInfo.begin();
	for (; iterDispatch != m_listDispatchInfo.end(); ++iterDispatch)
	{
		VecInt::iterator iterOpen = iterDispatch->ConditionOpenID.begin();
		for (; iterOpen != iterDispatch->ConditionOpenID.end(); )
		{
			if (*iterOpen == conditionID)
			{
				iterOpen = iterDispatch->ConditionOpenID.erase(iterOpen);
			}
			else
			{
				++iterOpen;
			}
		}
	}
}

bool CDispatch::getSoldierInfo(CRole *role, int &confID, int &star)
{
    if (NULL == role)
    {
        return false;
    }

    int nOwerID = role->getOwnerId();
    CPlayerModel *pPlayerModel = dynamic_cast<CPlayerModel *>(m_pBattleHelper->getUserModel(nOwerID));
    if (NULL == pPlayerModel)
    {
        return false;
    }

    CSoldierModel* pSoldierModel = pPlayerModel->getSoldierCard(role->getTypeId());
    if (NULL == pSoldierModel)
    {
        return false;
    }

    confID = pSoldierModel->getSoldId();
    star = pSoldierModel->getStar();

    return true;
}

bool CDispatch::isSoldierType(int confid, int star, int type)
{
    const SoldierConfItem* pSoldierItem = queryConfSoldier(confid, star);
    // ʿ����Ƭ����
    CHECK_RETURN(NULL != pSoldierItem);

    bool b = false;
    if (type >= EST_VOCATION_BEGIN) // ְҵ�ж�
    {
        ERaceType raceType = RACE_TYPE_NON;
        if (EST_RACE_HUMAN == type)
        {
            raceType = RACE_TYPE_HUMAN;
        }
        else if (EST_RACE_GHOST == type)
        {
            raceType = RACE_TYPE_GHOST;
        }
        else
        {
            raceType = RACE_TYPE_OTHER;
        }

        if (raceType == pSoldierItem->Common.Vocation)
        {
            b = true;
        }
    }
    else if (type >= EST_FIRE_BEGIN) // ������ʽ
    {
        EFireType fireType = FIRE_TYPE_NON;
        if (EST_FIRE_NEAR == type)
        {
            fireType = FIRE_TYPE_NEAR;
        }
        else
        {
            fireType = FIRE_TYPE_FAR;
        }

        if (fireType == pSoldierItem->Common.AttackType)
        {
            b = true;
        }
    }
    else if (type >= EST_SEX_BEGIN) // �Ա��ж�
    {
        ESexType sexType = SEX_TYPE_NON;
        if (EST_SEX_MALE == type)
        {
            sexType = SEX_TYPE_MALE;
        }
        else if (EST_SEX_SHEMALE == type)
        {
            sexType = SEX_TYPE_SHEMALE;
        }
        else
        {
            sexType = SEX_TYPE_NEUTER;
        }

        if (sexType == pSoldierItem->Common.Sex)
        {
            b = true;
        }
    }
    else if (type >= EST_RACE_BEGIN) // �����ж�
    {
        EVocationType vocationType = VOCATION_TYPE_NON;
        if (EST_VOCATION_SOLDIER == type)
        {
            vocationType = VOCATION_TYPE_SOLDIER;
        }
        else if (EST_VOCATION_ASSASSIN == type)
        {
            vocationType = VOCATION_TYPE_ASSASSIN;
        }
        else if (EST_VOCATION_SHOOTER == type)
        {
            vocationType = VOCATION_TYPE_SHOOTER;
        }
        else if (EST_VOCATION_MAGE == type)
        {
            vocationType = VOCATION_TYPE_MAGE;
        }
        else if (EST_VOCATION_ASSISTANT == type)
        {
            vocationType = VOCATION_TYPE_ASSISTANT;
        }
        else if (EST_VOCATION_TANK == type)
        {
            vocationType = VOCATION_TYPE_BODYGUARD;
        }

        if (vocationType == pSoldierItem->Common.Vocation)
        {
            b = true;
        }
    }

    return b;
}
