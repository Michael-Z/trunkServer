#include "CallSoldier.h"
#include "ComponentCache.h"

CCallSoldier::CCallSoldier()
: m_pCallModel(NULL)
{
    m_RoleType = RT_SUMMON;
    m_nObjectType = EObjectCall;
}

CCallSoldier::~CCallSoldier()
{
    SAFE_DELETE(m_pCallModel);
}

bool CCallSoldier::init(int callid, int level, int ownerId, int objId, CBattleHelper *battle)
{
    int camp = static_cast<int>(ECamp_Neutral);
    CHECK_RETURN(CRole::init(callid, ownerId, objId, battle));
    //�ٻ�������
    m_pCallConf = queryConfCall(callid);
    CHECK_RETURN(NULL != m_pCallConf);

    // �û��ٻ�
    CBattlePlayerModel* userModel = battle->getUserModel(ownerId);
	if (NULL != userModel)
	{
		camp = userModel->getCamp();
	}
    // BOSS�ٻ�
    else if (EDefaultNpc == ownerId)
    {
        camp = ECamp_Red;
    }

    if (CAMP_ENEMY == m_pCallConf->RoleType)
    {
        camp = battle->getEnmeyCamp(camp);
    }

    // �ƶ�������1 ����, �ƶ�������2 ����, ����ֻ��������������
    if (m_pCallConf->RoleMoveDirection == 1)
    {
        m_nDirection = 1;
    }
    else if (m_pCallConf->RoleMoveDirection == 2)
    {
        m_nDirection = -1;
    }
    else
    {
        m_nDirection = camp == ECamp_Blue ? 1 : -1;
    }

	//�ٻ���ĵȼ�
    level = m_pCallConf->CardCurrentLevel == 0 ? level : m_pCallConf->CardCurrentLevel;

    CRoleModel* m_pCallModel = new CRoleModel();
    CHECK_RETURN(m_pCallModel->init(RT_SUMMON, level, &(m_pCallConf->Common), battle->getBattleType()));

    //��ʼ���ٻ�������
    initRoleAttribute(m_pCallModel);
    // ��Ƭ���Գ�ʼ��
    initAttribute(EAttributeCamp, camp);

    // ���BUFF���
    m_BuffComponent = createComponent<CBuffComponent>(EBuffComponent);
    CHECK_RETURN(m_BuffComponent);
    if (!m_BuffComponent->init(this))
    {
        CComponentCache::getInstance()->freeComponent(m_BuffComponent);
        KXLOGDEBUG("CCallSoldier buffComponent init fail callid %d, callLv %d", callid, level);
        return false;
    }
    addLogicComponent(m_BuffComponent);

    //��ʼ���ٻ����
    CCallComponent* com = createComponent<CCallComponent>(ECallComponent);
    CHECK_RETURN(com);
    if (!com->init(m_pModel->getRoleComm()->StatusID, m_pCallConf))
    {
        CComponentCache::getInstance()->freeComponent(com);
        KXLOGDEBUG("CCallSoldier callComponent init fail callid %d, callLv %d", callid, level);
        return false;
    }
 	m_RoleComponent = com;
	addLogicComponent(com);

    // ��ʼ�������߼����
    CHECK_RETURN(initRoleComponents(m_pModel->getRoleComm()));
    // ��ʼ����ʾ���
    CHECK_RETURN(initDisplayComponents(m_pModel->getRoleComm()));
    return true;
}
