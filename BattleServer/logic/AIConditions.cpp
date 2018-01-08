#include "AIConditions.h"
#include "ConfManager.h"
#include "AIComponent.h"
#include "RoleComponent.h"
#include "Role.h"
#include "Soldier.h"
#include "Hero.h"
#include "BattleHelper.h"
#include "ConfAnalytic.h"

using namespace std;

CAIConditions::CAIConditions()
{
    m_pAICom = NULL;
    m_pOwner = NULL;
}

CAIConditions::~CAIConditions()
{
    m_pAICom = NULL;
    m_pOwner = NULL;
}

bool CAIConditions::DoAICondition(CAIComponent* com, const CDataFunction* func)
{
	CHECK_RETURN(NULL != com);
	m_pAICom = com;
    m_pOwner = dynamic_cast<CRole*>(com->getOwner());
    CHECK_RETURN(NULL != m_pOwner);
    m_pOwnerId = m_pOwner->getOwnerId();

	KXLOGBATTLE("ObjId %d DoAICondition %d", m_pOwner->getObjectId(), func->NameID);

	bool b_rev = itob(func->ParamInt[0]);
    switch (func->NameID)
    {
        // ������
    case AICondition_Is_None:
        return AIConditionIsNone(b_rev);

        // ��ǰĿ���Ƿ�Ϸ�
    case AICondition_Is_FindTarget:
        return AIConditionIsFindTarget(b_rev, func->ParamInt[1]);

        // ��Χ���Ƿ���Ŀ��
    case AICondition_Is_HasTarget:
        return AIConditionIsHasTarget(b_rev, func->ParamInt[1]);

        // �Լ�Ѫ���Ƿ���������
    case AICondition_Is_MyHpLessThan:
        return AIConditionIsMyHpLessThan(b_rev, func->ParamFloat[0]);

        // �����Ƿ����
    case AICondition_Is_SkillReady:
        return AIConditionIsSkillReady(b_rev, func->ParamInt[1]);

        // �����Ŀ����Ƿ�Ϊ��
    case AICondition_Is_HasTargetPoint:
        return AIConditionIsHasTargetPoint(b_rev);

        // �Լ��Ƿ���BUFF
    case AICondition_Is_MyBuff:
        return AIConditionIsMyBuff(b_rev, func->ParamInt[1]);

        // �ҵ�������Ӫ�ж�
	case AICondition_Is_RedOrBlue:
		return AIConditionIsRedOrBlue(b_rev, func->ParamInt[1]);

		//�����Ƿ�����
	case AICondition_Is_Random:
		return AIConditionIsRandom(b_rev, func->ParamInt[1]);

    case AICondition_Is_MyStarForAI:
        return AIConditionIsMyStarForAI(b_rev, func->ParamInt[1]);

    case AICondition_Is_MyAttribute:
        return AIConditionIsMyAttribute(b_rev, func->ParamInt[1], func->ParamInt[2], func->ParamFloat[0]);

    case AICondition_Is_EnemySoldierNumber:
        return AIConditionIsEnemySoldierNumber(b_rev, func->ParamInt[1]);

    case AICondition_Is_MySoldierNumber:
        return AIConditionIsMySoldierNumber(b_rev, func->ParamInt[1]);

    default:
        break;
    }
	return false;
}

bool CAIConditions::AIConditionIsNone(bool rev)
{
	RETURN_REV(rev, true);
}

// �Ƿ��ҵ�Ŀ��(�������д��д���ڰ�)
bool CAIConditions::AIConditionIsFindTarget(bool rev, int searchId)
{
    m_pOwner->getBlackBoard().clearTargetList();
    
    bool ret = m_pOwner->getBattleHelper()->getTargetsWithId(searchId, m_pOwner,
        &m_pOwner->getBlackBoard().getTargetList());

    vector<CRole*>& targets = m_pOwner->getBlackBoard().getTargetList();
    for (vector<CRole*>::iterator iter = targets.begin();
        iter != targets.end(); ++iter)
    {
        SAFE_RETAIN((*iter));
    }
    // ������ͨ��
    RETURN_REV(rev, ret);
}

// ��Χ���Ƿ��з�������Ŀ��(���������д���ڰ�)
bool CAIConditions::AIConditionIsHasTarget(bool rev, int searchId)
{
    m_pOwner->getBlackBoard().clearTargetList();
    bool ret = m_pOwner->getBattleHelper()->getTargetsWithId(searchId, m_pOwner, NULL);
    // ������ͨ��
    RETURN_REV(rev, ret);
}

bool CAIConditions::AIConditionIsMyHpLessThan(bool rev, float hp)
{
    // Ѫ���ٷֱ��Ƿ��������
	float hpPercent = m_pOwner->getIntAttribute(EAttributeHP) * 100.0f
		/ m_pOwner->getIntAttribute(EAttributeMaxHP);

    // Ѫ��С��ָ���ٷֱ�Ϊtrue
    RETURN_REV(rev, hpPercent < hp);
}

bool CAIConditions::AIConditionIsSkillReady(bool rev, int skillId)
{
    RETURN_REV(rev, m_pOwner->canExecuteSkill(skillId));
}

bool CAIConditions::AIConditionIsHasTargetPoint(bool rev)
{
	RETURN_REV(rev, m_pOwner->getBlackBoard().getTargetPoint().isZero());
}

bool CAIConditions::AIConditionIsMyBuff(bool rev, int buffId)
{
    RETURN_REV(rev, m_pOwner->haveBuff(buffId));
}

// �ҵ�������Ӫ�ж�
// ��Ϸ��Ĭ����߶���Ϊ�췽���ұ߶���Ϊ������1����췽2����������
bool CAIConditions::AIConditionIsRedOrBlue(bool rev, int flag)
{
    if (1 == flag)
    {
        bool isRed = m_pOwner->getIntAttribute(EAttributeCamp)
            == ECamp_Red;
        RETURN_REV(rev, isRed);
    }
    else
    {
        bool isBlue = m_pOwner->getIntAttribute(EAttributeCamp)
            == ECamp_Blue;
        RETURN_REV(rev, isBlue);
    }
}

bool CAIConditions::AIConditionIsRandom(bool rev, int ProbabilityValue)
{	
	int randValue = m_pOwner->getBattleHelper()->RandNum.random();
	RETURN_REV(rev, randValue < ProbabilityValue);
}

bool CAIConditions::AIConditionIsMyStarForAI(bool rev, int star)
{
    CSoldier* soldier = dynamic_cast<CSoldier*>(m_pOwner);
    if (soldier != NULL)
    {
        RETURN_REV(rev, soldier->getStar() == star);
    }
    RETURN_REV(rev, false);
}

template <typename T>
bool AICompare(T t1, T t2, int compareType)
{
    switch (compareType)
    {
    case AICompareLarge:
        return t1 > t2;
    case AICompareLess:
        return t1 < t2;
    case AICompareEqual:
        return t1 == t2;
    default:
        break;
    }
    return false;
}

bool CAIConditions::AIConditionIsMyAttribute(bool rev, int attId, int eqType, float var)
{
    int outInt = 0;
    float outFloat = 0.0f;
    if (m_pOwner->checkAttribute(static_cast<EAttributeTypes>(attId), outFloat))
    {
        RETURN_REV(rev, AICompare(outFloat, var, eqType));
    }
    else if (m_pOwner->checkAttribute(static_cast<EAttributeTypes>(attId), outInt))
    {
        RETURN_REV(rev, AICompare(outInt, floatToInt(var), eqType));
    }
    RETURN_REV(rev, false);
}

bool CAIConditions::AIConditionIsEnemySoldierNumber(bool rev, int num)
{
    int myCamp = m_pOwner->getIntAttribute(EAttributeCamp);
    int enemyNum = m_pOwner->getBattleHelper()->getEnmeyRoleWithCamp(myCamp).size();
    RETURN_REV(rev, enemyNum >= num);
}

bool CAIConditions::AIConditionIsMySoldierNumber(bool rev, int num)
{
    int myCamp = m_pOwner->getIntAttribute(EAttributeCamp);
    int enemyNum = m_pOwner->getBattleHelper()->getRoleWithCamp(myCamp).size();
    RETURN_REV(rev, enemyNum >= num);
}
