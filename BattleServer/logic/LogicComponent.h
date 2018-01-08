/*
 * �߼����
 * 1.�����߼�֡��֡��
 * 2.ͳһ�߼��ص����
 * 3.�����߼�����ĳ�����
 * 
 * 2014-12-19 by ��ү
 */
#ifndef __LOGIC_COMPONENT_H__
#define __LOGIC_COMPONENT_H__

#include "KxCSComm.h"
#include "BufferData.h"

// �������
enum EComponentType
{
    ELogicComponent,
    EAIComponent,
    EBuffComponent,
    EBulletComponent,
    ERoleComponent,
    ESkillComponent,
    ECallComponent,
    EHeroComponent,
};

class CGameObject;

class CLogicComponent : public Ref, public ISerializable
{
public:
    CLogicComponent();
    virtual ~CLogicComponent();

    //�߼�֡�Ļص�
    virtual void logicUpdate(float delta);

    virtual bool init() { return true; }

    virtual void onEnter() {}

    virtual void onExit();

	virtual bool serialize(CBufferData& data);

	virtual bool unserialize(CBufferData& data);

    inline void setOwner(CGameObject* owner)
    {
        m_pOwner = owner;
    }

    inline CGameObject* getOwner()
    {
        return m_pOwner;
    }

    EComponentType getComponentType()
    { 
        return m_eComponentType; 
    }

protected:
    CGameObject* m_pOwner;
    EComponentType m_eComponentType;
};

#endif
