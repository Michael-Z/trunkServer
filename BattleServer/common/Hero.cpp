#include "Hero.h"
#include "HeroComponent.h"
#include "ConfManager.h"
#include "Soldier.h"
#include "ComponentCache.h"

#ifndef RunningInServer
#include "ArmatureComponent.h"
#endif 
using namespace std;

CHero::CHero() 
	:m_SoldierCards(NULL)
{
    m_RoleType = RT_HERO;
    m_nObjectType = EObjectHero;

#ifndef RunningInServer
    m_nWithholdValue = 0;
    m_bStartSkillExecute = false;
    m_fCurSkillExecutingTime = 0;
    m_fMaxSKillExecutingTime = 0;
#endif
}

CHero::~CHero()
{
    for (map<int, CRole*>::iterator iter = m_SingleMap.begin();
        iter != m_SingleMap.end(); ++iter)
    {
        SAFE_RELEASE(iter->second);
    }
}

bool CHero::init(int typeId, int ownerId, int objId, CBattleHelper* battle)
{
    CHECK_RETURN(CRole::init(typeId, ownerId, objId, battle));

    CPlayerModel* userModel = dynamic_cast<CPlayerModel*>(battle->getUserModel(ownerId));
    CHECK_RETURN(NULL != userModel);
	
    const HeroConfItem* confItem = queryConfHero(typeId);
    CHECK_RETURN(NULL != confItem);

    // ��ȡӢ������
    initRoleAttribute(userModel->getRoleModel());
    initAttribute(EAttributeCamp, userModel->getCamp());
    initAttribute(EHeroCrystalSpeedParam, confItem->CrystalSpeedPrarm);
	
	// ����ģ�ͳ�ʼ��ʿ����Ƭ�б�
    m_SoldierCards = &userModel->getSoldierCards();

    // ���BUFF��� 
    m_BuffComponent = createComponent<CBuffComponent>(EBuffComponent);
    CHECK_RETURN(m_BuffComponent);
    if (!m_BuffComponent->init(this))
    {
        CComponentCache::getInstance()->freeComponent(m_BuffComponent);
        KXLOGDEBUG("CHero buffComponent init fail typeId %d", typeId);
        return false;
    }
    addLogicComponent(m_BuffComponent);

	// ��ʼ����ɫ���
    m_RoleComponent = createComponent<CHeroComponent>(EHeroComponent);
    CHECK_RETURN(m_RoleComponent);
    if (!m_RoleComponent->init(confItem->Common.StatusID))
    {
        CComponentCache::getInstance()->freeComponent(m_RoleComponent);
        KXLOGDEBUG("CHero roleComponent init fail typeId %d, statusID", typeId, confItem->Common.StatusID);
        return false;
    }
	addLogicComponent(m_RoleComponent);
	// ���Ӣ�۵�UI���ܣ�PlayerSkill��, ��Ҫ�����, ��ֹ���ܴ�λ
	if (confItem->PlayerSkill.size() > 0)
	{
		if (NULL == m_SkillComponent)
        {            
            m_SkillComponent = createComponent<CSkillComponent>(ESkillComponent);
            CHECK_RETURN(m_SkillComponent);
            if (!m_SkillComponent->init(this, confItem->PlayerSkill))
            {
                CComponentCache::getInstance()->freeComponent(m_SkillComponent);
                KXLOGDEBUG("CHero skillComponent init fail typeId %d", typeId);
                return false;
            }
			addLogicComponent(m_SkillComponent);
		}
	}
	// ��ʼ�������߼����
	CHECK_RETURN(initRoleComponents(&confItem->Common));
	// ��ʼ����ʾ���
	CHECK_RETURN(initDisplayComponents(&confItem->Common));

	CConfBase* conf = CConfManager::getInstance()->getConf(CONF_CRYSTAL);
	const CrystalConfItem* crystalItem = queryConfCrystal(1);
	CHECK_RETURN(NULL != crystalItem);
	// ��ʼ�ȼ�
	int crystalLevel = 1;
	// ˮ����ǰ����	
	float crystalNum = 150.0f;
	// ���ˮ����
	int crystalMax = crystalItem->Max;
	// �ָ��ٶ�
	float crystalSpeed = crystalItem->Speed;
	
    if (m_pBattle->getBattleType() == EBATTLE_TOWERTEST)
    {
		// ��������û��ˮ���ȼ��ĸ������Ҫд��ˮ��״̬��ˮ���ȼ�Ϊ����
		// ��������Ext1Ϊ��ʼ����ˮ������
        crystalLevel = conf->getDatas().size();
		crystalNum = static_cast<float>(m_pBattle->getRoom()->getExt1());
		crystalMax = 0;
		crystalSpeed = 0.0f;
    }
	else if (m_pBattle->getBattleType() == EBATTLE_GOLDTEST 
		|| m_pBattle->getBattleType() == EBATTLE_GUIDE)
	{
		// �������Ĭ������
		crystalLevel = conf->getDatas().size();
		const CrystalConfItem* crystalItem = queryConfCrystal(crystalLevel);
		CHECK_RETURN(NULL != crystalItem);
		
		crystalNum = crystalItem->Max * 1.0f;
		crystalMax = crystalItem->Max;
		crystalSpeed = crystalItem->Speed;
	}

	initAttribute(EHeroCrystalLevel, crystalLevel);
	initAttribute(EHeroCrystal, crystalNum);
	initAttribute(EHeroMaxCrystal, crystalMax);
	initAttribute(EHeroCrystalSpeed, crystalSpeed);
	// ������Ϣ����
	// �Ƿ���buff��Ҫ��ӵ��Լ�����
	if (m_pBattle->getBattleType() != EBATTLE_PVP)
	{
		const std::vector<BuffData> &buffs = m_pBattle->getRoom()->getStageBuffs();
		for (std::vector<BuffData>::const_iterator iter = buffs.begin(); 
			iter != buffs.end(); ++iter)
		{
			if (iter->Target == 0 || iter->Target == 1)
			{
				addBuff(this, iter->BuffId, iter->BuffLv);
			}
		}
	}
	
    return true;
}

bool CHero::serialize(CBufferData& data)
{
	SERIALIZE_MATCH(data);

	//���л���������
	CRole::serialize(data);

	//���л�Ψһ�ɷ�
	int length1 = m_SingleMap.size();
	data.writeData(length1);
	for (std::map<int, CRole*>::iterator iter = m_SingleMap.begin(); iter != m_SingleMap.end(); ++iter)
	{
		data.writeData(iter->second->getObjectId());
	}

	//���л�ʿ����Ƭģ��
	int length2 = m_SoldierCards->size();
	data.writeData(length2);
	for (std::vector<CSoldierModel*>::iterator iter = m_SoldierCards->begin(); iter != m_SoldierCards->end(); ++iter)
	{
		(*iter)->serialize(data);
	}

	SERIALIZE_MATCH(data);

	return true;
}

bool CHero::unserialize(CBufferData& data)
{
	UN_SERIALIZE_MATCH(data);

	//�����л���������
	CRole::unserialize(data);

	//�����л�Ψһ�ɷ�
	int soldierCounts;
	int objID;
	CRole* targetTemp = NULL;

	releaseAndClearMap(m_SingleMap);
	data.readData(soldierCounts);
	for (int i = 0; i < soldierCounts; ++i)
	{
		data.readData(objID);
		//����objID��ȡĿ��ʵ��
		targetTemp = m_pBattle->getRoleByObjectId(objID);
		targetTemp->retain();
		m_SingleMap[targetTemp->getTypeId()] = targetTemp;
	}
	
	//�����л�ʿ����Ƭģ��
	int solderCardCount;
	data.readData(solderCardCount);
	CHECK_RETURN(m_SoldierCards->size()==solderCardCount);
	for (int i = 0; i < solderCardCount; ++i)
	{
		(*m_SoldierCards)[i]->unserialize(data);
	}

	UN_SERIALIZE_MATCH(data);

#ifndef RunningInServer
    m_nWithholdValue = 0;
    m_bStartSkillExecute = false;
    m_fCurSkillExecutingTime = 0;
    m_fMaxSKillExecutingTime = 0;
#endif

	return true;
}

bool CHero::executeSkillIndex(int skillIndex, Vec2 pos)
{
    m_pBattle->pEventManager->raiseEvent(0xfff2, NULL);
    return CRole::executeSkillIndex(skillIndex, pos);
}

bool CHero::executeSkill(int skillId, Vec2 pos)
{
    m_pBattle->pEventManager->raiseEvent(0xfff2, NULL);
    return CRole::executeSkill(skillId, pos);
}

bool CHero::canUpgradeCrystal()
{
    // ��ȡ��ǰ���������
    int cur = getIntAttribute(EHeroCrystalLevel);
    const CrystalConfItem* cf = queryConfCrystal(cur);
    CHECK_RETURN(cf);

    // ��ȡ��һ��������ã���ȡ����˵���������ˣ�
    int level = getIntAttribute(EHeroCrystalLevel) + 1;
    const CrystalConfItem* conf = queryConfCrystal(level);
    CHECK_RETURN(conf);

    return canUseCrystal(cf->Price);
}

bool CHero::upgradeCrystal()
{
    // ��ȡ��ǰ���������
    int cur = getIntAttribute(EHeroCrystalLevel);
    const CrystalConfItem* cf = queryConfCrystal(cur);
    CHECK_RETURN(cf);

    // ��ȡ��һ��������ã���ȡ����˵���������ˣ�
    int level = getIntAttribute(EHeroCrystalLevel) + 1;
    const CrystalConfItem* conf = queryConfCrystal(level);
    CHECK_RETURN(conf);

    // �������������ˮ��
    CHECK_RETURN(useCrystal(cf->Price));

    // ����ˮ��������ˮ�����ޣ�ˮ���ָ��ٶ�
    setAttribute(EHeroCrystalLevel, level);
    setAttribute(EHeroMaxCrystal, conf->Max);
    setAttribute(EHeroCrystalSpeed, conf->Speed);
    return true;
}

bool CHero::canUseCrystal(int crystal)
{
#ifndef RunningInServer
    return (floatToInt(getFloatAttribute(EHeroCrystal) - m_nWithholdValue) >= crystal);
#else
    return (floatToInt(getFloatAttribute(EHeroCrystal)) >= crystal);
#endif
}

bool CHero::useCrystal(int crystal)
{
    // �ж϶���ˮ������ͨˮ���������Ƿ�����
    int curCrystal = floatToInt(getFloatAttribute(EHeroCrystal));
    if (curCrystal < crystal)
    {
        return false;
    }
    curCrystal -= crystal;
    setAttribute(EHeroCrystal, curCrystal * 1.0f);
    // ����������Ҫͳ��ˮ������
    //if (m_pBattle->getBattleType() == EBATTLE_TOWERTEST)
    //{
        m_pBattle->getSettleAccountModel()->addCostCrystal(crystal);
    //}
    return true;
}

bool CHero::isCrystalMaxLevel()
{
    int level = getIntAttribute(EHeroCrystalLevel) + 1;
    const CrystalConfItem* conf = queryConfCrystal(level);
    return NULL == conf;
}

int CHero::getCrystalCost(int level)
{
    const CrystalConfItem* conf = queryConfCrystal(level);
    if (NULL == conf)
    {
        return 0;
    }
    else
    {
        return conf->Price;
    }
}

bool CHero::canUseSoldierCard(int soldierId)
{
    CSoldierModel* soldierCard = getSoldierCard(soldierId);
    CHECK_RETURN(soldierCard);

    // �ж���
    CHECK_RETURN(!soldierCard->IsLock);
    // �ж�Ψһ
    CHECK_RETURN(!soldierCard->IsSingo || !isSoldierSingle(soldierId))
    // �ж�CD
    CHECK_RETURN(soldierCard->CurCD >= soldierCard->MaxCD);
    // �ж�����
    CHECK_RETURN(canUseCrystal(soldierCard->CurCost));

    return true;
}

bool CHero::createSoldier(int soldierId, int line /*= -1*/)
{
    CHECK_RETURN(canUseSoldierCard(soldierId));

    CSoldierModel* soldierCard = getSoldierCard(soldierId);
    CHECK_RETURN(useCrystal(soldierCard->CurCost));
    soldierCard->CurCD = 0;
    CSoldier* soldier = m_pBattle->createSoldier(getOwnerId(), soldierId, line);
    CHECK_RETURN(soldier);

    if (soldierCard->IsSingo)
    {
        SAFE_RETAIN(soldier);
        m_SingleMap[soldierId] = soldier;
    }
    return true;
}

bool CHero::isSoldierCardLock(int id)
{
    CSoldierModel* card = getSoldierCard(id);
	CHECK_RETURN(card);
	return card->IsLock;
}

bool CHero::isSoldierCardBuff(int id)
{
	return false;
}

bool CHero::isSoldierCardDebuff(int id)
{
	return false;
}

bool CHero::isSoldierCardBinding(int id)
{
	return false;
}

float CHero::getSoldierCardCDPercent(int id)
{
    CSoldierModel* card = getSoldierCard(id);
	CHECK_RETURN(card);
	return card->CurCD / card->MaxCD;
}

void CHero::checkSingo()
{
    for (map<int, CRole*>::iterator iter = m_SingleMap.begin();
        iter != m_SingleMap.end();)
    {
        // ��������Ƴ������������������Ƴ�
        if (iter->second->getRoleComponent() == NULL
            || iter->second->getRoleComponent()->getStateId() == State_Death)
        {
            SAFE_RELEASE(iter->second);
            m_SingleMap.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

bool CHero::isSoldierCardSingle(int id)
{
    CSoldierModel* card = getSoldierCard(id);
    CHECK_RETURN(card);
    return card->IsSingo;
}

bool CHero::isSoldierSingle(int soldierId)
{
    map<int, CRole*>::iterator iter = m_SingleMap.find(soldierId);
    if (iter == m_SingleMap.end()
        || iter->second->getRoleComponent() == NULL
        || iter->second->getRoleComponent()->getStateId() == State_Death)
    {
        return false;
    }

    return true;
}

#ifndef RunningInServer
// ����ˮ����Ԥ��ֵ
void CHero::setWithholdValue(int value)
{
    if (value > 0)
    {
        m_nWithholdValue += value;
    }
    else
    {
        m_nWithholdValue = 0;
    }
}

// ���ù�������ִ��ʱ��
void CHero::startSkillExecute(bool flag)
{
    m_bStartSkillExecute = flag;
}

void CHero::setCurSkillExecutingTime(float time)
{
    if (m_bStartSkillExecute)
    {
        if (m_fCurSkillExecutingTime > m_fMaxSKillExecutingTime)
        {
            m_bStartSkillExecute = false;
            m_fCurSkillExecutingTime = 0;
            m_fMaxSKillExecutingTime = 0;
        }
        else
        {
            m_fCurSkillExecutingTime += time;
        }
    }
}

void CHero::setMaxSkillExecutingTime(float time)
{
    if (0 == m_fMaxSKillExecutingTime)
    {
        m_fCurSkillExecutingTime = 0;
        m_fMaxSKillExecutingTime = 1.5f;
        if (time > m_fMaxSKillExecutingTime)
        {
            m_fMaxSKillExecutingTime = time;
        }
    }
}

float CHero::getCommonCDPercent()
{
    if (m_fMaxSKillExecutingTime > 0)
    {
        return m_fCurSkillExecutingTime / m_fMaxSKillExecutingTime;
    }

    return 0.0f;
}

#endif