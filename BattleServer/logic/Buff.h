#ifndef __BUFF_H__
#define __BUFF_H__

#include <vector>
#include "KxCSComm.h"
#include "BufferData.h"

#ifndef RunningInServer
#include "EffectFactory.h"
#endif

enum BuffConditionID
{
    Func_IsHasBuffTarget = 1,
    Func_IsHasSkillPoint,
    Func_IsSkillReady,
    Func_IsRandom,
};

enum BuffType
{
    BUFF = 1,
    DEBUFF,
    UNSTOPPABLE,
};

enum BuffTriggerType
{
    TRIGGER_TIME = 1,           // ʱ��
    TRIGGER_DIE,                // BuffЯ����������ʱ�򴥷�
    TRIGGER_SKILL,              // �ͷż���
    TRIGGER_CRIT,               // ����
    TRIGGER_BECRITED,           // ������
    TRIGGER_DODGE,              // ����
    TRIGGER_BEDODGED,           // ������
    TRIGGER_BETREATED,          // ����
    TRIGGER_BROKENSHIELD,       // �ƶ�
    TRIGGER_ADDSHIELD,          // �Ӷ�
    TRIGGER_DISPATCHSOLIDER,    // �ɱ�
    TRIGGER_BEATTACKED,         // �ܻ�
    TRIGEER_ATTACK,             // ����
};

enum BuffTargetType
{
    BUFFTARGET_MAKER = -2,      // ������
    BUFFTARGET_OWNER = -1,      // Я����
    BUFFTARGET_NONE,            // ��
    BUFFTARGET_CUSTOM,          // �Զ���ѡȡ(����0������id)
};

class CRole;
class BuffConfItem;
class CBuffComponent;
class CBuff;

class CBuffCondition : public ISerializable
{
public:
    CBuffCondition(CBuff *parent);
    ~CBuffCondition();

public:

    bool serialize(CBufferData& data);

    bool unserialize(CBufferData& data);

    bool isHasBuffTarget();

    bool isHasSkillPoint(int flag, int x, int y);

    bool isSkillReady(bool ng, int skillid);

    bool isRandom(int rand);

    std::vector<CRole *> &getTargets();

    void clearTargets();

    Vec2 &getTargetPos();

    void clearTargetPos();

    void getCustomTargets(int searchid, std::vector<CRole*> &targets);

private:

    CBuff*                 m_pParent;
    Vec2                   m_TargetPos;
    std::vector<CRole *>   m_Targets;
};

class CBuff 
    : public Ref
    , public ISerializable
{
public:
    CBuff();
    ~CBuff();

public:

    // ���л������л�
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    bool init(CBuffComponent *cpnt, CRole *owner, CRole *maker, int buffid, int stack);

    void onEnter();

    void onExit(bool isFouce = false);

    int getBuffType();

    bool isFinish();

    bool isUnstoppable();

    void finish(bool force = false);

    void update(float dt);

	void reset();

    int getStack();

    CRole *getMaker();

    const BuffConfItem *getBuffConf();

private:
    // ����Ƿ���Ч
    bool checkCondition();
    // ��ⴥ����
    void checkTrigger(float dt);
    // ��Ч����
    void takeEffectCount();
    // ʧЧ����
    void loseEfficacyCount();
    // �ͷ�/���ռ���
    void executeSkill(int skillid);
    void recycleSkill(int skillid);

    // ע�ᴥ����
    void regTrigger(int trigtype);
    // ȡ��ע��
    void unregTigger(int trigtype);
    // ʱ��
    void triggerTime(float dt);
    // ����
    void triggerDie();

    // �¼������ص�
    // ʹ�ü���
    void triggerSkill(void *data);
    // �ɱ�
    void triggerDispatchSoldier(void *data);

    // ��Ч����
    void playEffect();
    // ֹͣ��Ч����
    void stopEffect();

    // ���ݴ������ͻ�ȡ��Ӧ������ֵ
    int getAttributeValue(BuffTriggerType triggerType);
    // �����Ƚϲ�ֵ����
    void triggerDValue(BuffTriggerType triggerType);
    // �����Ƚ��ۼ���ֵ����
    void triggerAccumulative(BuffTriggerType triggerType);
    // ������������
    void resetData();

private:
    bool     m_bFinish;                             // �Ƿ����
    bool     m_bTriggerActive;                      // �Ƿ񴥷�buff
    bool     m_bUnstoppable;                        // �Ƿ񲻿���ɢ
    bool     m_bForce;                              // �Ƿ�Ϊǿ�ƻ���
    int      m_nWorkTimes;                          // �����Ĵ���
    int      m_nRecordState;                        // ��Ӹ�BUFFʱ��ɫ��״̬
    float    m_fBuffLifeTime;                       // buff�������ڼ�ʱ
    float    m_fFirstTime;                          // �״���Чʱ���ʱ

    float    m_fTriggerTime;                        // ��������ʱ
    int      m_nTrigerValue;                        // ����ֵ - ��ͬ���������в�ͬ������
    CRole *                m_pOwner;                // buffЯ����
    CRole *                m_pMaker;                // buff������
    CBuffComponent *       m_pBuffCpnt;             // buff�������
    const BuffConfItem *   m_pBuffConf;             // buff���
    CBuffCondition         m_Condition;             // ��Ч����
    // ��buff����������buff����Ϣ
    friend class CBuffCondition;

#ifndef RunningInServer
    std::vector<cocos2d::Node *> m_EffectNodes;
#endif 
};

#endif //__BUFF_H__
