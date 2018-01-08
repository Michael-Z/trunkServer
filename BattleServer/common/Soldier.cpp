#include "Soldier.h"
#include "ConfManager.h"
#include "ComponentCache.h"

using namespace std;

static int soldier = 0;

CSoldier::CSoldier()
{
    m_RoleType = RT_SOLDIER;
    m_nObjectType = EObjectSoldier;
	LOG("CSoldier()=============================%d", ++soldier);
}


CSoldier::~CSoldier()
{
	LOG("~CSoldier()=============================%d", --soldier);
}

bool CSoldier::init(int index, int ownerId, int objId, CBattleHelper* battle)
{
    CHECK_RETURN(CRole::init(index, ownerId, objId, battle));

    CPlayerModel* userModel = dynamic_cast<CPlayerModel*>(battle->getUserModel(ownerId));
    CHECK_RETURN(NULL != userModel);
    // ��ȡʿ����Ƭģ��
    CSoldierModel* soldierCard = userModel->getSoldierCard(index);
    CHECK_RETURN(NULL != soldierCard);

    // ��ȡʿ������ ���� ��ʼ��ʿ������
    const SoldierConfItem* confItem = queryConfSoldier(soldierCard->getSoldId(), soldierCard->getStar());
    CHECK_RETURN(NULL != confItem);
	initRoleAttribute(soldierCard);
    initAttribute(EAttributeCamp, userModel->getCamp());

    // ���BUFF���
    m_BuffComponent = createComponent<CBuffComponent>(EBuffComponent);
    CHECK_RETURN(m_BuffComponent);
    if (!m_BuffComponent->init(this))
    {
        CComponentCache::getInstance()->freeComponent(m_BuffComponent);
        KXLOGDEBUG("CSoldier buffComponent init fail soldierID %d, soldierStar %d",
            soldierCard->getSoldId(), soldierCard->getStar());
        return false;
    }
    addLogicComponent(m_BuffComponent);

	// ��ʼ����ɫ���
    m_RoleComponent = createComponent<CRoleComponent>(ERoleComponent);
    CHECK_RETURN(m_RoleComponent);
    if (!m_RoleComponent->init(m_pModel->getRoleComm()->StatusID))
    {
        CComponentCache::getInstance()->freeComponent(m_RoleComponent);
        KXLOGDEBUG("CSoldier roleComponent init fail soldierID %d, soldierStar %d, statusID %d",
            soldierCard->getSoldId(), soldierCard->getStar(), m_pModel->getRoleComm()->StatusID);
        return false;
    }
    addLogicComponent(m_RoleComponent);

	// ��ʼ�������߼����
    // ���AI���
    m_AIComponent = createComponent<CAIComponent>(EAIComponent);
    CHECK_RETURN(m_AIComponent);
    if (!m_AIComponent->init(soldierCard->getAI(), this))
    {
        CComponentCache::getInstance()->freeComponent(m_AIComponent);
        KXLOGDEBUG("CSoldier AIComponent init fail soldierID %d, soldierStar %d, AIID %d",
            soldierCard->getSoldId(), soldierCard->getStar(), soldierCard->getAI());
        return false;
    }
    addLogicComponent(m_AIComponent);

    // ����������ò�Ϊ�գ���Ӽ������
    if (soldierCard->getSkillIds().size() > 0)
    {        
        m_SkillComponent = createComponent<CSkillComponent>(ESkillComponent);
        CHECK_RETURN(m_SkillComponent);
        if (!m_SkillComponent->init(this, soldierCard->getSkillIds()))
        {
            CComponentCache::getInstance()->freeComponent(m_SkillComponent);
            KXLOGDEBUG("CSoldier skillComponent init fail soldierID %d, soldierStar %d",
                soldierCard->getSoldId(), soldierCard->getStar());
            return false;
        }
        addLogicComponent(m_SkillComponent);
    }

	// ��ʼ����ʾ���
    CHECK_RETURN(initDisplayComponents(m_pModel->getRoleComm()));

    // ���buff
    map<int, int>::iterator iterBuff = soldierCard->getAddBuff().begin();
    for (; iterBuff != soldierCard->getAddBuff().end(); iterBuff++)
    {
        addBuff(this, iterBuff->first, iterBuff->second);
    }
    return true;
}

int CSoldier::getStar()
{
    if (m_pModel)
    {
        return ((CSoldierModel*)m_pModel)->getStar();
    }
    return 0;
}