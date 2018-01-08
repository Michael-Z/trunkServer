#ifndef __AI_COMPONENT_H__
#define __AI_COMPONENT_H__

#include "KxCSComm.h"
#include "LogicComponent.h"
#include "ConfManager.h"
#include "ConfRole.h"

//AI���
class CRole;
class CRoleComponent;

class CAIComponent : public CLogicComponent
{
public:
	CAIComponent();
	virtual ~CAIComponent();

	// ��ʼ��һЩ����
	virtual bool init(int AIId, CRole* pOwner);
	virtual void logicUpdate(float delta);

    virtual void onExit();

    // ���¼���
    bool reload(int AIId);

    inline CRole* getRole()
    {
        return m_pOwner;
    }

    inline CRoleComponent* getRoleCom()
    {
        return m_pRoleCom;
    }

    inline const AIConfMap* getAIConfMap()
    {
        return m_pAIConf;
    }

	virtual bool serialize(CBufferData& data);

	virtual bool unserialize(CBufferData& data);

protected:
	// ִ��������
	bool DoAIConditions(const AIConfItem* item);
	// ִ�ж�����
	void DoAIActions(const AIConfItem* item);

private:
	int						m_CurAIID;				// ��ǰAI ID
    CRole*					m_pOwner;				// �����ɫ
    AIConfMap*				m_pAIConf;				// AI����
    CRoleComponent*			m_pRoleCom;				// ��ɫ���
};

#endif