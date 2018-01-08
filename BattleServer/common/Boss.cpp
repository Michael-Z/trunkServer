#include "Boss.h"
#include "ComponentCache.h"

CBoss::CBoss()
{
	m_RoleType = RT_BOSS;
	m_nObjectType = EObjectBoss;
}

CBoss::~CBoss()
{
}

bool CBoss::init(int bossid, int ownerId, int objId, CBattleHelper *battle)
{
	CHECK_RETURN(CRole::init(bossid, ownerId, objId, battle));

    CComputerModel* computer = battle->getComputerModel();
    CHECK_RETURN(computer);
    CRoleModel* bossModel = computer->getRoleModel();
    CHECK_RETURN(bossModel);

    //��ʼ��ͨ������
    initRoleAttribute(bossModel);
    // ��Ƭ���Գ�ʼ��
	initAttribute(EAttributeCamp, ECamp_Red);

    // ���BUFF���
    m_BuffComponent = createComponent<CBuffComponent>(EBuffComponent);
    CHECK_RETURN(m_BuffComponent);
    if (!m_BuffComponent->init(this))
    {
        CComponentCache::getInstance()->freeComponent(m_BuffComponent);
        KXLOGDEBUG("CBoss buffComponent init fail bossid %d", bossid);
        return false;
    }
    addLogicComponent(m_BuffComponent);

	// ��ʼ����ɫ���
    m_RoleComponent = createComponent<CRoleComponent>(ERoleComponent);
    CHECK_RETURN(m_RoleComponent);
    if (!m_RoleComponent->init(m_pModel->getRoleComm()->StatusID))
    {
        CComponentCache::getInstance()->freeComponent(m_RoleComponent);
        KXLOGDEBUG("CBoss roleComponent init fail bossid %d", bossid);
        return false;
    }
	addLogicComponent(m_RoleComponent);
	// ��ʼ�������߼����
    CHECK_RETURN(initRoleComponents(m_pModel->getRoleComm()));
	// ��ʼ����ʾ���
    CHECK_RETURN(initDisplayComponents(m_pModel->getRoleComm()));

	// ������Ϣ����
	// �Ƿ���buff��Ҫ��ӵ��Լ�����
	if (m_pBattle->getBattleType() != EBATTLE_PVP)
	{
		const std::vector<BuffData> &buffs = m_pBattle->getRoom()->getStageBuffs();
		for (std::vector<BuffData>::const_iterator iter = buffs.begin();
			iter != buffs.end(); ++iter)
		{
			if (iter->Target == 0 || iter->Target == 2)
			{
				addBuff(this, iter->BuffId, iter->BuffLv);
			}
		}
	}

	return true;
}

void CBoss::modifyAttribute(EAttributeTypes key, int v)
{
    // ���������Ҫͳ��BOSS��Ѫ������
    if (key == EAttributeHP
		&& (m_pBattle->getBattleType() == EBATTLE_GOLDTEST || m_pBattle->getBattleType() == EBATTLE_UNIONEXPEDITION))
    {
        // ֻͳ�ƿ�Ѫ
        int HitHp = m_IntAttributes[key] - v;
        if (HitHp > 0)
        {
            // ��ʾ������SettleAccountModel��ֵˢ����ʾ
            m_pBattle->getSettleAccountModel()->addHitBossHP(HitHp);
        }
    }
    m_IntAttributes[key] = limitAttribute(key, v);

}