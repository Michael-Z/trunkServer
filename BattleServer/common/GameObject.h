/*
 * GameObject��Ϸ���󣬶Դ�������Ϸ�����е����ж���Ķ���
 *
 * 1.��װEventManager��BattleHelper�ȶ���ָ�루ǰ��˲����װ��
 * 2.��װ��������map���ṩ�޸ĺͻ�ȡ����
 *
 * 2014-12-19 by ��ү
 */
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "KxCSComm.h"
#include "BattleHelper.h"
#include "LogicComponent.h"
#include "BufferData.h"
#include "EventManager.h"

class CBattleHelper;

enum EObjectType
{
    EObjectHero,        // Ӣ��
    EObjectSoldier,     // ʿ��
    EObjectCall,        // �ٻ���
	EObjectBoss,		//BOSS
	EObjectMonster,		//����
    EObjectBullet,      // �ӵ�
};

class CGameObject : public Node, public ISerializable
{
public:
    CGameObject();
    virtual ~CGameObject();

    // �����������ID��������ID������ΨһID��ս������
    virtual bool init(int typeId, int ownerId, int objId, CBattleHelper* battle);

    virtual void onExit();

    // �������
    virtual void reset();

    ///////////////////////////////���л��ͷ����л�/////////////////////////////////////

    // ����Ϸ���������data��
    virtual bool serialize(CBufferData& data);
    // ��data�н�ѹ����Ϸ����
    virtual bool unserialize(CBufferData& data);

    //////////////////////////////////////////////////////////////////////////

    void addLogicComponent(CLogicComponent* com);

    void clearLogicComponents();

    void updateLogic(float dt);

    void remove();

    //////////////////////////////////////////////////////////////////////////

    // �����߼�λ������ʾλ��
    inline void setAllPosition(const Vec2& pos)
    {
        m_Position = pos;
		setPosition(pos);
    }

    inline const Vec2& getRealPosition()
    {
        return m_Position;
    }

    inline void setRealPosition(const Vec2& pos)
    {
        m_Position = pos;
    }

    inline float getRealPositionX() const
    {
        return m_Position.x;
    }

    inline float getRealPositionY() const
    {
        return m_Position.y;
    }

    inline void setRealPositionX(float x)
    {
        m_Position.x = x;
    }

    inline void setRealPositionY(float y)
    {
        m_Position.y = y;
    }

    inline void addRealPosition(const Vec2& v)
    {
        m_Position += v;
    }

    inline float addRealPositionX(float x)
    {
        return m_Position.x += x;
    }

    inline float addRealPositionY(float y)
    {
        return m_Position.y += y;
    }

    // ��ȡEventManager
    CEventManager<int>* getEventManager() { return m_pEventManager; }
    // ��ȡBattleHelper
    CBattleHelper* getBattleHelper() { return m_pBattle; }
    // ��ȡ���������ID���ӵ���Ӣ�ۣ�ʿ�����ٻ�������ٻ���
    inline EObjectType getObjectType() { return m_nObjectType; }
    // ��ȡ�����ΨһID
    inline int getObjectId() { return m_nGameObjectId; }
    // ��ȡ����ID�������ʿ����Monster�����ｫ����Index������������ID
    inline int getTypeId() { return m_nTypeId; }
    // ��ȡ�����ߵ�ID
    inline int getOwnerId() { return m_nOwnerId; }
    // ��ȡ���� 1Ϊ���ң�-1Ϊ����
    inline int getDirection() { return m_nDirection; }
    // �Ƿ�ɱ�ɾ��
    inline bool canRemove() { return m_bSafeRemove; }
    // ɾ������
    inline void safeRemove() { m_bSafeRemove = true; }

    // ��������
    inline void realDead(){ m_bRealDead = true; }
    // �Ƿ���������
    inline bool isRealDead(){ return m_bRealDead; }

protected:
    bool m_bRealDead;                               // �Ƿ��Ѿ����������
    bool m_bSafeRemove;                             // �Ƿ�ɱ���ȫ���Ƴ�
    int m_nDirection;                               // ����    
    int m_nGameObjectId;                            // ����ΨһID
    int m_nOwnerId;                                 // ���˵�UserID
    int m_nTypeId;                                  // ����ID��Ӣ��ID��ʿ��ID���ӵ�ID��
    EObjectType m_nObjectType;                      // �������ͣ��ӵ���Ӣ�ۣ�ʿ�����ٻ�������ٻ��
    Vec2 m_Position;                                // �߼�λ��

    std::list<CLogicComponent*> m_LogicComponents;  // �߼�����б�
    std::map<int, int> m_IntAttributes;             // Int����
    std::map<int, float> m_FloatAttributes;         // Float����
    CEventManager<int>* m_pEventManager;            // �¼�����
    CBattleHelper* m_pBattle;                       // ս��������
};

#endif