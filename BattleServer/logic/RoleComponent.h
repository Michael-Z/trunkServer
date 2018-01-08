/*
* ��ɫ���
* ִ��״̬�߼�
* ʵ��״̬�л�
* ��ɫ���Իָ����̡���ŭ������
*
* 2014-12-24 by ��ү
*/
#ifndef __STATE_COMPONENT_H__
#define __STATE_COMPONENT_H__

#include "LogicComponent.h"
#include "State.h"
#include "ConfManager.h"
#include "ConfRole.h"

class CState;
class CRole;
class CRoleComponent : public CLogicComponent
{
public:
    CRoleComponent();
    virtual ~CRoleComponent();

    // ����״̬���ã���ʼ��״̬����ڲ�
    virtual bool init(int roleId);

    virtual void onEnter();
    virtual void onExit();

	//���л�,�����л� ��ɫ�������
	bool serialize(CBufferData& data);
	bool unserialize(CBufferData& data);

    // ִ��״̬������߼�����
    virtual void logicUpdate(float delta);

    // ����״̬�л����л��ɹ�����true��ʧ�ܷ���false
    virtual CState* changeState(int stateId);

    // ���¼���
    bool reload(int roleId);

    int getStateId();

    const StatusConfItem* getStateConf();

    inline int getStateConfId() { return m_nStateConfId; }

    // ����Ѫ��
    void updateHpBar();

protected:
    // �л�����һ��״̬
    bool nextState();

    // ����״̬ID����̬����
    CState* getState(int stateId);


protected:
    float m_fRecoverTick;
    int m_nStateConfId;                         // ״̬���ñ��ID
    std::map<int, CState*> m_StateMap;          // ״̬����
    CState* m_CurState;                         // ��ǰ״̬
    CRole* m_Role;
};

#endif
