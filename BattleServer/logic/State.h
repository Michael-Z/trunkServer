#ifndef __STATE_H__
#define __STATE_H__

#include "RoleComponent.h"
#include "BufferData.h"
#include "ConfRole.h"

enum StateActionType
{
    StateAction_None,
    StateAction_Move,
    StateAction_HitPower,
    StateAction_Dead,
    StateAction_AddBuffForMeOnce,
    StateAction_DoCount,
};

//״̬����ID
enum StateTypeID
{
    State_None, 		        // ��Ч
    State_Death,		        // ����
    State_Born,			        // ����
    State_Stand,		        // ����
    State_Move,			        // �ƶ�
    State_Fight,		        // ս��
    State_FightStand,	        // ս������
    State_Charging,             // ����
    State_Win,                  // ʤ��
    State_Lose,                 // ʧ��

    State_Hit = 10,		        // ����
    State_HitVerigo,	        // ����
    State_HitBack,		        // ����
    State_HitFly,		        // ����
    State_HitDown,		        // ����
    State_GetUp,		        // ����
    State_Frozen,		        // ����
    State_Petrified,	        // ʯ��
    State_Negative_Begin = State_Hit,             // ��һ������״̬
    State_Negative_End   = State_Petrified,       // ���һ������״̬

    State_SkillBase = 30,       // �����ͷ�״̬��+n��ʾ��n���ܣ�n��1��ʼ��
};

inline bool isNegativeState(int state) { return (state >= State_Negative_Begin) && (state <= State_Negative_End); }

class CRoleComponent;
class CRole;

// һ�������״̬
class CState : public ISerializable
{
public:
	CState();
	virtual ~CState();

    // ֻ��ʼ��һ�Σ��������ü��ص�
    virtual bool init(int stateId, const StatusConfItem* confItem, CRoleComponent* component);

    // ִ��״̬����
    virtual void update(float dt);

    // �����״̬ʱ�ص������ڳ�ʼ��״̬
    virtual void onEnter(bool isFouce = false);

    // �뿪��״̬ʱ�ص�����������״̬
    virtual void onExit(bool isFouce = false);

	//���л�,�����л� ״̬
	bool serialize(CBufferData& data);
	bool unserialize(CBufferData& data);

    // ��ѯ״̬�Ƿ����
    inline bool isDone()
    {
        return m_bIsDone;
    }

    inline bool isLock()
    {
        return m_pStateConf->Lock;
    }

    // ״̬���ȼ�
    inline int getPriority()
    {
        return m_pStateConf->CanBreakParam;
    }

    // ״̬ID
    inline int getStateId()
    {
        return m_nStateId;
    }

    // ��ȡ��һ��״̬
    inline int getNextStateId()
    {
        return m_nNextStateId;
    }

    // �ı���һ��״̬
    inline void setNextStateId(int stateId)
    {
        m_nNextStateId = stateId;
    }

    inline const StatusConfItem* getStateConf()
    {
        return m_pStateConf;
    }

    inline float getDuration()
    {
        return m_fDuration;
    }

    inline void setDuration(float duration)
    {
        m_fDuration = duration;
    }

protected:
    bool m_bIsDone;                         // ״̬�Ƿ������
    float m_fDuration;                      // ʣ��ʱ��
    int m_nNextStateId;                     // �Զ��л�״̬
    int m_nStateId;                         // ״̬Id
    CRoleComponent* m_pComponent;           // ״̬���
    CRole* m_pOwner;                        // ӵ����
    const StatusConfItem* m_pStateConf;     // ״̬����
};

// �ƶ�״̬
class CStateMove : public CState
{
    // ִ��״̬����
    virtual void update(float dt);

	//���л�,�����л� ״̬
	bool serialize(CBufferData& data);
	bool unserialize(CBufferData& data);

private:
    float m_fSpeed;                         // ��ǰ�ƶ��ٶ�
    float m_fAcceleration;                  // ��ǰ�ƶ����ٶ�
};

// ������״̬
class CStateHitPower : public CState        
{
    // ִ��״̬����
    virtual void update(float dt);

    // �����״̬ʱ�ص������ڳ�ʼ��״̬ 
    virtual void onEnter(bool isFouce = false);

	//���л�,�����л� ״̬
	bool serialize(CBufferData& data);
	bool unserialize(CBufferData& data);

private:
    float m_fPowerY;
    float m_fBaseY;                         // ��ƽYֵ
    float m_fA;                             // Ħ��ϵ��
    float m_fG;                             // �������ٶ�
    float m_fVx;                            // X�����ٶ�
    float m_fVy;                            // Y�����ٶ�
};

class CStateDeath : public CState
{
    virtual void onEnter(bool isFouce = false);
	// ִ��״̬����
	virtual void update(float dt);
};

#endif 
