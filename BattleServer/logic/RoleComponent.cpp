#include "RoleComponent.h"

#ifndef RunningInServer
#include "RoleDisplayComponent.h"
#endif

#include "GameObject.h"
#include "Role.h"

using namespace std;

CRoleComponent::CRoleComponent()
: m_fRecoverTick(0.0f)
, m_Role(NULL)
{
    m_eComponentType = ERoleComponent;
}

CRoleComponent::~CRoleComponent()
{
    deleteAndClearMap<int, CState*>(m_StateMap);
}

bool CRoleComponent::init(int roleId)
{
	// init default state
    m_nStateConfId = roleId;
    m_CurState = NULL;
    return true;
}

void CRoleComponent::onEnter()
{
    CLogicComponent::onEnter();
    m_Role = dynamic_cast<CRole*>(m_pOwner);
    m_CurState = getState(State_Born);
    if (NULL != m_CurState)
    {
        m_CurState->onEnter();
    }
}

void CRoleComponent::onExit()
{
    m_fRecoverTick = 0.0f;
    m_Role = NULL;
    deleteAndClearMap<int, CState*>(m_StateMap);
}

bool CRoleComponent::serialize(CBufferData& data)
{	
	SERIALIZE_MATCH(data);

	//���л�����
	CLogicComponent::serialize(data);

	data.writeData(m_fRecoverTick);
	//����״̬���ñ�id
	data.writeData(m_nStateConfId);

	//���浱ǰ״̬
	if (NULL == m_CurState)
	{
		data.writeData(-1);
	}
	else
	{
		data.writeData(m_CurState->getStateId());
		m_CurState->serialize(data);
	}

	SERIALIZE_MATCH(data);

	return true;
}

bool CRoleComponent::unserialize(CBufferData& data)
{
	UN_SERIALIZE_MATCH(data);

	//�����л�����
	CLogicComponent::unserialize(data);

	data.readData(m_fRecoverTick);

	int stateConfId = 0;
	//��ȡ���ñ�id
	data.readData(stateConfId);
	if (m_nStateConfId != stateConfId)
	{
		deleteAndClearMap<int, CState*>(m_StateMap);
		m_nStateConfId = stateConfId;
		m_CurState = NULL;
	}

	int stateId = 0;
	//��ȡ��ǰ״̬
	data.readData(stateId);
	if (-1 == stateId)
	{
		if (m_CurState)
		{
			m_CurState->onExit(true);
		}
		m_CurState = NULL;
	}
	else
	{
		// ״̬����ͬ��Ҫ���г�
		if (m_CurState != NULL 
			&& m_CurState->getStateId() != stateId)
		{
            // ����״̬�Ქ�ŵ�������
            if (m_CurState->getStateId() == State_Death)
            {
#ifndef RunningInServer
                // ������Ӧ��ֹֻͣ����Action
                getOwner()->stopActionByTag(DEATH_ACT_TAG);
                getOwner()->setOpacity(255);
#endif
            }
            m_CurState->onExit(true);
			m_CurState = getState(stateId);
            m_CurState->onEnter(true);
		}
        else if (m_CurState == NULL)
        {
            m_CurState = getState(stateId);
            m_CurState->onEnter(true);
        }

		m_CurState->unserialize(data);		
	}

	UN_SERIALIZE_MATCH(data);
	return true;
}

void CRoleComponent::logicUpdate(float delta)
{
    // �����Լ���״̬
    if (NULL != m_CurState)
    {
        if (m_CurState->isDone())
        {
            // ����֮�����л�״̬
            nextState();
        }
        else
        {
            m_CurState->update(delta);
        }
    }

    // �Ƶ�˵ֻ��������״̬�²Ż�ִ�л����߼���2016-5-25
    if (NULL != m_Role && State_Charging == m_Role->currentState())
    {
        m_Role->updateAttribute(EAttributeMP, floatToInt(m_Role->getIntAttribute(EAttributeMPRecover) * delta));
        updateHpBar();
        /*// ÿ��ָ�һ�������Ͱ��塿
        m_fRecoverTick += delta;
        if (m_fRecoverTick >= 1.0f)
        {
            // �Ƶ�˵���ɵ������ָ���2016-5-25
            //m_Role->updateAttribute(EAttributeStrong, m_Role->getIntAttribute(EAttributeStrongRecover));
            m_Role->updateAttribute(EAttributeMP, m_Role->getIntAttribute(EAttributeMPRecover));
			m_fRecoverTick = 0.0f;

            KXLOGBATTLE("update EAttributeStrong %d, EAttributeStrongRecover %d",
            m_Role->getIntAttribute(EAttributeStrong), m_Role->getIntAttribute(EAttributeStrongRecover));
        }*/
    }
    m_Role->updateAttribute(EAttributeStrong, floatToInt(m_Role->getIntAttribute(EAttributeStrongRecover) * delta));
}

CState* CRoleComponent::changeState(int stateId)
{
    KXLOGBATTLE("changeState stateId %d m_CurState %d", stateId, m_CurState->getStateId());
    // �л�����״̬
    if (State_None == stateId)
    {
        if (NULL != m_CurState)
        {
            m_CurState->onExit();
            m_CurState = NULL;
        }
        return m_CurState;
    }
    // ͬ״̬���л�
    else if (stateId == getStateId())
    {
        return m_CurState;
    }

    // �л���һ�������ڵ�״̬�򲻽����л�
    //�����޷��������Ķ����л�����״̬��ʧ�ܣ�
    CState* state = getState(stateId);
    if (NULL == state)
	{
		KXLOGBATTLE("state is null stateId %d m_nStateConfId %d", stateId, m_nStateConfId);
        return NULL;
    }

    if (NULL == m_CurState)
    {
        m_CurState = state;
        m_CurState->onEnter();
        return m_CurState;
    }
    //���״̬��ͬ������״̬���ȼ����ھ�״̬���ȼ�
    else if (m_CurState->getPriority() <= state->getPriority())
    {
        if (m_CurState->isLock())
		{
            KXLOGBATTLE("curState isLock stateId %d m_CurStateId %d", stateId, m_CurState->getStateId());
            m_CurState->setNextStateId(stateId);
            return state;
        }
        else
        {
            m_CurState->onExit();
            m_CurState = state;
            m_CurState->onEnter();
            return m_CurState;
        }
	}
    KXLOGBATTLE("changeState fail stateId %d", stateId);
    return NULL;
}

bool CRoleComponent::reload(int roleId)
{
    // ƽ���˳���ǰ״̬
    if (NULL != m_CurState)
    {
        m_CurState->onExit();
        m_CurState = NULL;
    }

    // �����״̬
    deleteAndClearMap<int, CState*>(m_StateMap);
    // ������״̬
    m_nStateConfId = roleId;
    m_CurState = getState(State_Born);
    if (NULL != m_CurState)
    {
        m_CurState->onEnter();
        return true;
    }

    return false;
}

int CRoleComponent::getStateId()
{
    if (NULL == m_CurState)
    {
        return 0;
    }

    return m_CurState->getStateId();
}

const StatusConfItem* CRoleComponent::getStateConf()
{
    if (NULL == m_CurState)
    {
        return NULL;
    }

    return m_CurState->getStateConf();
}

bool CRoleComponent::nextState()
{
    if (m_CurState->getNextStateId() == m_CurState->getStateId())
    {
        return true;
    }

    CState* next = getState(m_CurState->getNextStateId());
    if (NULL != next)
    {
        m_CurState->onExit();
        m_CurState = next;
        m_CurState->onEnter();
        return true;
    }
    else if(State_Death == m_CurState->getStateId())
    {
        m_CurState->onExit();
        m_CurState = NULL;
        return true;
    }

    return false;
}

CState* CRoleComponent::getState(int stateId)
{
    map<int, CState*>::iterator iter = m_StateMap.find(stateId);
    if (iter != m_StateMap.end())
    {
        return iter->second;
    }
    else
    {
        // ��ȡ���ñ�
        const StatusConfItem* item = queryConfStateItem(m_nStateConfId, stateId);
        if (NULL == item)
        {
            return NULL;
        }

        CState* state = NULL;
        if (NULL == item->Action)
        {
            state = new CState();
        }
        else
        {
            switch (item->Action->NameID)
            {
            case StateAction_Move:
                state = new CStateMove();
                break;
            case StateAction_HitPower:
                state = new CStateHitPower();
                break;
            case StateAction_Dead:
                state = new CStateDeath();
                break;
            default:
                state = new CState();
                break;
            }
        }

        if (!state->init(item->StateId, item, this))
        {
            delete state;
            return NULL;
        }

        m_StateMap[item->StateId] = state;
        return state;
    }
}

void CRoleComponent::updateHpBar()
{
#ifndef RunningInServer
    CRoleDisplayComponent* roleCom = dynamic_cast<CRoleDisplayComponent*>(
        m_Role->getComponent("CRoleDisplayComponent"));
    if (roleCom)
    {
        roleCom->updateHpBar();
    }
#endif
}
