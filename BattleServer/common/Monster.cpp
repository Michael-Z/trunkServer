#include "Monster.h"
#include "ComponentCache.h"

CMonster::CMonster()
{
	m_RoleType = RT_MONSTER;
	m_nObjectType = EObjectMonster;
}

CMonster::~CMonster()
{
}

bool CMonster::init(int monsterid, int ownerId, int objId, CBattleHelper *battle)
{
	CHECK_RETURN(CRole::init(monsterid, ownerId, objId, battle));
    CComputerModel* computer = battle->getComputerModel();
    CHECK_RETURN(computer);
    CRoleModel* monsterModel = computer->getMonsterModel(monsterid);
    CHECK_RETURN(monsterModel);

    //��ʼ��ͨ������
    initRoleAttribute(monsterModel);
	// ��Ƭ���Գ�ʼ��
    initAttribute(EAttributeCamp, ECamp_Red);

    // ���BUFF���
    m_BuffComponent = createComponent<CBuffComponent>(EBuffComponent);
    CHECK_RETURN(m_BuffComponent);
    if (!m_BuffComponent->init(this))
    {
        CComponentCache::getInstance()->freeComponent(m_BuffComponent);
        KXLOGDEBUG("CMonster buffComponent init fail monsterid %d", monsterid);
        return false;
    }
    addLogicComponent(m_BuffComponent);

	//��ʼ����ɫ���
    m_RoleComponent = createComponent<CRoleComponent>(ERoleComponent);
    CHECK_RETURN(m_RoleComponent);
    if (!m_RoleComponent->init(m_pModel->getRoleComm()->StatusID))
    {
        CComponentCache::getInstance()->freeComponent(m_RoleComponent);
        KXLOGDEBUG("CMonster roleComponent init fail monsterid %d, statusID %d", 
            monsterid, m_pModel->getRoleComm()->StatusID);
        return false;
    }
    addLogicComponent(m_RoleComponent);
	// ��ʼ�������߼����
	CHECK_RETURN(initRoleComponents(m_pModel->getRoleComm()));
	// ��ʼ����ʾ���
    CHECK_RETURN(initDisplayComponents(m_pModel->getRoleComm()));
	return true;
}
