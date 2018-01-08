#ifndef __AI_CONDITIONS_H__
#define __AI_CONDITIONS_H__

#include "KxCSComm.h"
#include "ConfAnalytic.h"

#define itob(a)				(0 != a ? true : false)
#define RETURN_REV(rev,b)	return rev ? !(b) : (b)

enum AIConditionType
{
	AICondition_Is_None = 1,   						// ������
	AICondition_Is_FindTarget,					    // �Ƿ��ҵ�Ŀ��(�������д��д���ڰ�)
	AICondition_Is_HasTarget,						// ��Χ���Ƿ��з�������Ŀ��(���������д���ڰ�)
	AICondition_Is_MyHpLessThan,                    // �Լ�Ѫ���Ƿ���������
	AICondition_Is_SkillReady,						// �����Ƿ����
	AICondition_Is_HasTargetPoint,	                // �����Ŀ����Ƿ�Ϊ��
	AICondition_Is_MyBuff,						    // �Լ��Ƿ���ָ��Buff
	AICondition_Is_RedOrBlue,						// �ҵ�������Ӫ�ж�
	AICondition_Is_Random,							// �����Ƿ�����
    AICondition_Is_MyStarForAI,                     // �Ǽ��Ƿ���������
    AICondition_Is_MyAttribute,                     // �����Ƿ���������
    AICondition_Is_EnemySoldierNumber,              // ���ϵз�ʿ�������Ƿ���������(���������д���ڰ�)
    AICondition_Is_MySoldierNumber,                 // ���ϼ���ʿ�������Ƿ���������(���������д���ڰ�)
};

enum AICompareType
{
    AICompareLarge,
    AICompareEqual,
    AICompareLess,
};

//AI������Χ
enum AIAttackRange
{
	AIAR_ANYWHERE,									// ȫͼ
	AIAR_NEAR,										// ��ս��Χ
	AIAR_FAR,										// Զս��Χ
};

class CRole;
class CAIComponent;
class CAIConditions
{
public:
	CAIConditions();
	~CAIConditions();
	bool DoAICondition(CAIComponent* com, const CDataFunction* func);

protected:
    // ������
	bool AIConditionIsNone(bool rev);
    // �Ƿ��ҵ�Ŀ��(�������д��д���ڰ�)
    bool AIConditionIsFindTarget(bool rev, int searchId);
    // ��Χ���Ƿ��з�������Ŀ��(���������д���ڰ�)
    bool AIConditionIsHasTarget(bool rev, int searchId);
    // �Լ�Ѫ���Ƿ���������
	bool AIConditionIsMyHpLessThan(bool rev, float hp);
    // �����Ƿ�����������뼼��ID���Ǽ����±꣡������
    bool AIConditionIsSkillReady(bool rev, int skillId);
	// �����Ŀ����Ƿ�Ϊ��
    bool AIConditionIsHasTargetPoint(bool rev);
    // �Լ��Ƿ���ָ��Buff
    bool AIConditionIsMyBuff(bool rev, int buffid);
    // �ҵ�������Ӫ�ж�
    // ��Ϸ��Ĭ����߶���Ϊ�췽���ұ߶���Ϊ������1����췽2����������
    bool AIConditionIsRedOrBlue(bool rev, int camp);
	// �����Ƿ�����
    bool AIConditionIsRandom(bool rev, int ProbabilityValue);
    // �Ǽ��Ƿ�����
    bool AIConditionIsMyStarForAI(bool rev, int star);
    // �жϵ�ǰ�����Ƿ���������
    bool AIConditionIsMyAttribute(bool rev, int attId, int eqType, float var);
    // ���ϵз�ʿ�������Ƿ���������(���������д���ڰ�)
    bool AIConditionIsEnemySoldierNumber(bool rev, int num);
    // ���ϼ���ʿ�������Ƿ���������(���������д���ڰ�)
    bool AIConditionIsMySoldierNumber(bool rev, int num);

private:
	CAIComponent* m_pAICom;
    CRole* m_pOwner;
    int m_pOwnerId;
};

#endif