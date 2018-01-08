#ifndef __AI_ACTIONS_H__
#define __AI_ACTIONS_H__

#include "KxCSComm.h"
#include "ConfAnalytic.h"

//AI��������
enum AIActionType
{
    AIAction_Do_None = 1,				    // �ն���
    AIAction_Do_LockMe,						// �����Լ�
    AIAction_Do_Clear,						// �������Ŀ�궯��
    AIAction_Do_ChangeStatus,				// �ı�״̬����
    AIAction_Do_CastSkill,				    // �ͷż��ܶ���
    AIAction_Do_LockPoints,					// ����Ŀ��㶯��
    AIAction_Do_LockPointsForMe,            // ����Ϊ���ĵ�����Ŀ���
    AIAction_Do_AddBuffForMe,				// ���Լ����Buff
	AIAction_Do_DelBuffForMe,				// ���Լ�ɾ��Buff
    AIAction_Do_Count,                      // ִ��һ������
    AIAction_Do_LuaEvent,                   // ����һ��Lua�¼��������¼�ID
};

class CRole;
class CAIComponent;
class CAIActions
{
public:
	CAIActions();
	~CAIActions();
	void DoAIAction(CAIComponent* com, const CDataFunction* func);

protected:
	void AIActionDoNone();
    // �����Լ�ΪĿ��
    void AIActionDoLockMe();
    // ��պڰ�
    void AIActionDoClear();
    // �л�����״̬
	void AIActionDoChangeStatus(int stateid);
    // �ͷŵڼ�������
	void AIActionDoCastSkill(int skillIndex);
    // ����һϵ��Ŀ��㵽�ڰ���
    void AIActionDoLockPoints(const VecInt& points);
    // ����Ϊ���ĵ�����Ŀ���
    void AIActionDoLockPointsForMe(const VecInt& points);
    // Ϊ�Լ����BUFF
    void AIActionDoAddBuffForMe(const VecVecInt& buffIds);
	// ���Լ�ɾ��ָ��Buff����
	void AIActionDoDelBuffForMe(const VecInt& buffIds);
    // ִ��һ������
    void AIActionDoCount(int countId);
#ifndef RunningInServer
    // ����һ��Lua�¼��������¼�ID
    void AIActionDoLuaEvent(int eventId);
#endif

private:
	CAIComponent* m_pAICom;
    CRole* m_pOwner;
};

#endif