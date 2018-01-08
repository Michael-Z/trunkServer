#include "AIComponent.h"
#include "RoleComponent.h"
#include "Role.h"
#include "AIConditions.h"
#include "AIActions.h"

using namespace std;

CAIComponent::CAIComponent()
: m_CurAIID(0)
{
    m_eComponentType = EAIComponent;
	m_pOwner = NULL;
	m_pRoleCom = NULL;
	m_pAIConf = NULL;
}

CAIComponent::~CAIComponent()
{
	m_pOwner = NULL;
	m_pRoleCom = NULL;
	m_pAIConf = NULL;
}

bool CAIComponent::init(int AIId, CRole* pOwner)
{
	//AI����
	m_pAIConf = const_cast<AIConfMap*>(queryConfAIMap(AIId));
	CHECK_RETURN(m_pAIConf);

	m_CurAIID = AIId;

	//�����ɫ
	CRole* pRole = dynamic_cast<CRole*>(pOwner);
	CHECK_RETURN(pRole);
	m_pOwner = pRole;
	
	//�����ɫ���
	CRoleComponent* comRole = m_pOwner->getRoleComponent();
	CHECK_RETURN(comRole);
    m_pRoleCom = comRole;	

	return true;
}

void CAIComponent::logicUpdate(float delta)
{
	// �����ǰ״̬û��ƥ���ID����ֱ�ӷ���
	int stateID = m_pRoleCom->getStateId();
	const std::vector<AIConfItem*>* items = m_pAIConf->getAIItems(stateID);
	CHECK_RETURN_VOID(items);

	// ����AI������
	for(std::vector<AIConfItem*>::const_iterator iter = items->begin();
		iter != items->end(); ++iter)
	{
		// ������ǰAI���������������
		if(DoAIConditions(*iter))
		{
			// ����������ִ��ƥ��Ķ����飬������AI
			DoAIActions(*iter);
			break;
		}
	}
}

void CAIComponent::onExit()
{
    CLogicComponent::onExit();
}

bool CAIComponent::reload(int AIId)
{
	if (m_CurAIID == AIId)
	{
		return false;
	}

    return init(AIId, m_pOwner);
}

bool CAIComponent::serialize(CBufferData& data)
{
	SERIALIZE_MATCH(data);

	if (!CLogicComponent::serialize(data))
	{
		return false;
	}

	data.writeData(m_CurAIID);

	SERIALIZE_MATCH(data);

	return true;
}

bool CAIComponent::unserialize(CBufferData& data)
{
	UN_SERIALIZE_MATCH(data);

	if (!CLogicComponent::unserialize(data))
	{
		return false;
	}

	int nAIID = 0;
	data.readData(nAIID);

	// ״̬����
	reload(nAIID);

	UN_SERIALIZE_MATCH(data);

	return true;
}

bool CAIComponent::DoAIConditions(const AIConfItem* item)
{
	CAIConditions condition;
	for(std::vector<CDataFunction*>::const_iterator iter = item->Condition.begin();
		iter != item->Condition.end(); ++iter)
	{
		//����հף�ֱ������0
        if (!condition.DoAICondition(this, *iter))
        {
            return false;
        }
	}
	return true;
}

void CAIComponent::DoAIActions(const AIConfItem* item)
{
	CAIActions action;
	for(std::vector<CDataFunction*>::const_iterator iter = item->Action.begin();
        iter != item->Action.end(); ++iter)
	{
		action.DoAIAction(this, *iter);
	}
}
