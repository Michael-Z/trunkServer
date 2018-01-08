/*
 * ��ɫ�Ļ���
 * ��װ��Ӣ�ۺ�ʿ���Ĺ������Ժͷ���
 * �����������Եĳ�ʼ��
 *
 * 2014-12-23 by ��ү
 */
#ifndef __ROLE_H__
#define __ROLE_H__

#include "GameObject.h"
#include "Skill.h"

#include "AIComponent.h"
#include "RoleComponent.h"
#include "BuffComponent.h"
#include "SkillComponent.h"
#include "CallComponent.h"
#include "RoleComm.h"

class CRole;

//�ڰ壬������¼����
class CBlackBoard : public ISerializable
{
public:
	CBlackBoard();
    ~CBlackBoard();

	void setBattleHelper(CBattleHelper* battleHelper) { m_BattleHelper = battleHelper; }
    void clearTargetList();
    void clearBlackBoard();

    // ��Role��ӵ�TargetList
    void addToTargetList(CRole* role);
    // ��TargetList��ղ���Role��ӵ�TargetList
    void clearAndAddToTargetList(CRole* role);
    void removeFormTargetList(CRole* role);

    inline void setTargetPoint(const Vec2& pt) { m_TargetPoint = pt; }
    inline Vec2& getTargetPoint() { return m_TargetPoint; }
    inline std::vector<CRole*>& getTargetList() { return m_TargetList; }

	virtual bool serialize(CBufferData& data);
	virtual bool unserialize(CBufferData& data);
private:
    Vec2                m_TargetPoint;  // Ŀ���
    std::vector<CRole*> m_TargetList;   // ��ʱĿ���ɫ
	CBattleHelper*		m_BattleHelper;	
};

class CRole : public CGameObject
{
public:
    CRole();
    virtual ~CRole();

    // �����������ID��������ID������ΨһID��ս������
    virtual bool init(int typeId, int ownerId, int objId, CBattleHelper* battle);
	virtual void onExit();

    // ��ʼ����ɫ��ͨ����������
    void initRoleAttribute(CRoleModel *model);
	// ��ʼ����ɫͨ�õ��߼����
    bool initRoleComponents(const Role* role);
	// ��ʼ����ʾ��ص������������Ϊ��ʵ�֣�
    bool initDisplayComponents(const Role* role);

	//////////////////////////////////SERIALIZE///////////////////////////////////
	// ����Ϸ���������data��
	virtual bool serialize(CBufferData& data);
	// ��data�н�ѹ����Ϸ����
	virtual bool unserialize(CBufferData& data);

    /////////////////////////////////BUFF////////////////////////////////////////
    //�Ƿ�ӵ��ָ��buff
    bool haveBuff(int buffId);
    //���buff
    bool addBuff(CRole *maker, int buffId, int addstack);
	//ɾ��buff
	void delBuff(int buffId);

    //////////////////////////////////����////////////////////////////////////////
    CSkill *getSkillWithID(int skillId);
    //�ܷ�ִ�еڼ�������
    CSkill *getSkillWithIndex(int skillIndex);
    bool canExecuteSkillIndex(int skillIndex);
    bool executeSkillIndex(int skillIndex, Vec2 &pos);

    //ʹ��ָ������
    bool canExecuteSkill(int skillId);
    bool executeSkill(int skillId, Vec2 &pos);
    bool executeSkill(int skillId, std::vector<CRole*> &targets, Vec2 &pos); 

    //////////////////////////////////״̬////////////////////////////////////////
    CState* changeState(int stateId);
    int currentState();

    //////////////////////////////////�̶���//////////////////////////////////////
    inline Vec2& getFireOffset() { return m_FireOffset; }
    inline Vec2 getBaseFireOffset()     
    {
		Vec2 ret = m_pModel->getRoleComm()->FireOffset;
        ret.x *= m_nDirection;
        return ret;
    }
    inline Vec2 getFirePoint() { return m_Position + m_FireOffset; }
    inline Vec2& getHeadOffset() { return m_HeadOffset; }
    inline Vec2 getBaseHeadOffset() 
    {
		Vec2 ret = m_pModel->getRoleComm()->HeadOffset;
        ret.x *= m_nDirection;
        return ret;
    }
    inline Vec2 getHeadPoint() { return m_Position + m_HeadOffset; }
    inline Vec2& getHitOffset() { return m_HitOffset; }
    inline Vec2 getBaseHitOffset()
    {
		Vec2 ret = m_pModel->getRoleComm()->HitOffset;
        ret.x *= m_nDirection;
        return ret;
    }
    inline Vec2 getHitPoint() { return m_Position + m_HitOffset; }
    inline void initPosition(const Vec2& pos)
    {
        setAllPosition(pos);
        // ��Ҫ������·ID��������
		initAttribute(EAttributeYmin, pos.y);
    }
    inline void updateDirection()
    {
        int nPreDirection = m_nDirection;
        m_nDirection = getIntAttribute(EAttributeCamp) == ECamp_Blue ? 1 : -1;
        if (nPreDirection != m_nDirection)
        {
            m_FireOffset.x *= m_nDirection;
            m_HitOffset.x *= m_nDirection;
            m_HeadOffset.x *= m_nDirection;
        }
    }

    //////////////////////////////////�������////////////////////////////////////////
    inline CAIComponent* getAIComponent() { return m_AIComponent; }
    inline CBuffComponent* getBuffComponent() { return m_BuffComponent; }
    inline CSkillComponent* getSkillComponent() { return m_SkillComponent; }
    inline CRoleComponent* getRoleComponent() { return m_RoleComponent; }
    inline CBlackBoard& getBlackBoard()
    {
        return m_BlackBoard;
    }

	inline Node* getDisplayNode()
	{
		if (NULL != m_DisplayNode)
		{
			return m_DisplayNode;
		}
		else
		{
			return this;
		}
	}

    ///////////////////////////////////�������///////////////////////////////////////
    // ��ȡ��ɫ����
    inline RoleType getRoleType() { return m_RoleType; }
    // ��ȡģ������
    inline const CRoleModel* getRoleModel() { return m_pModel; }
    // ����Stat
    void updateStat(EAttributeTypes stat);

    virtual int getIntAttribute(EAttributeTypes key);
    virtual float getFloatAttribute(EAttributeTypes key);
    virtual bool checkAttribute(EAttributeTypes key, int& outv);
    virtual bool checkAttribute(EAttributeTypes key, float& outv);
    int limitAttribute(EAttributeTypes key, int v);
    float limitAttribute(EAttributeTypes key, float v);
    void setAttribute(EAttributeTypes key, int v);
    void setAttribute(EAttributeTypes key, float v);
    bool updateAttribute(EAttributeTypes key, int v);
    bool updateAttribute(EAttributeTypes key, float v);
	void initAttribute(EAttributeTypes key, int v);
	void initAttribute(EAttributeTypes key, float v);

protected:
    // ����intֵ���Զ����������޹���
    virtual void modifyAttribute(EAttributeTypes key, int v);
    // ����floatֵ���Զ����������޹���
    virtual void modifyAttribute(EAttributeTypes key, float v);
    // ��������Ƿ���ڽ����޸�
    inline bool canModifyAttribute(EAttributeTypes key);
    // �����޸���Ӧ������
    inline void changeOtherAttrbute(EAttributeTypes key);

protected:
    RoleType m_RoleType;                    // ��ɫ����
    Vec2 m_FireOffset;                      // ����ƫ�Ƶ�
    Vec2 m_HeadOffset;                      // ͷ��ƫ�Ƶ�
    Vec2 m_HitOffset;                       // �ܻ�ƫ�Ƶ�
    CBlackBoard m_BlackBoard;               // �ڰ�
	CAIComponent* m_AIComponent;		    // AI���
	CBuffComponent* m_BuffComponent;	    // BUFF���
	CSkillComponent* m_SkillComponent;	    // �������
    CRoleComponent* m_RoleComponent;	    // ��ɫ���
    CRoleModel* m_pModel;

private:
    Node* m_DisplayNode;				    // ������ʾ��صĽڵ�
};

#endif
