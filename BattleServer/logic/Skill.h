#ifndef __SKILL_H__
#define __SKILL_H__

#include "KxCSComm.h"
#include "BufferData.h"
#include "ConfFight.h"

#define SKILL_INVALID_POINT  Vec2(-99999.0, -99999.0)

enum SkillLockType
{
    SLT_UNLOCK = 0,         //������
    SLT_CRYSTALLEVEL,       //ˮ���ȼ�
};

enum SkillCostType
{
    SCT_NOCOST = 0,         //������    
    SCT_CRYTAL,             //ˮ������
    SCT_BLOOD,              //����Ѫ��
    SCT_RAGE,               //ŭ��
    SCT_MAGIC,              //ħ��ֵ
};

class CRole;
class CSkillComponent;
class CSkill : 
    public ISerializable
{
public:
    CSkill();
    ~CSkill();

public:

    // ���л������л�
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);
    //�ٻ�ʦ��Ӣ�۱������ܵȼ����������
    //Ӣ�ۣ�ʿ���������ܵȼ����������
    //BOSS�����ܵȼ���1
    //������ܵȼ���1
    //�ٻ�����ܵȼ���1
    bool init(CSkillComponent *skillComponent, CRole *owner, int skillid, int skillindex, int skillLv = 1);

    void update(float dt);

    bool isFinish();

    const SkillConfItem * getSkillConf();

    int skillId();

    int skillIndex();

    float getCDPercent();

    float getExecutingPercent();

    bool isLock();

    bool canUnlock();

    void unlock();

    void breakSkill();

    bool changeState();

    bool canExecute();

    void execute(std::vector<CRole*> &targets, Vec2 &pos);

    void execute(std::vector<CRole*> &targets);

    void execute(Vec2 &pos);
    
    //inline int getSkillLevel() { return m_nSkillLevel; }

private:

    void reset();

    void cost();
    //��������ʱ��
    void calcLinkage();
    //�����ӵ��Ƿ������
    bool nextTargetBullet();
    //�����ӵ��Ƿ������
    bool nextPosBullet();

    void fireBullet(std::vector<CRole*> &targets, int bulletid);

    void fireBullet(Vec2 &pos, int bulletid);

    void callSoldier();

    void addBuff(std::vector<CRole*> &targets);

private:

    bool m_bFinish;                         // �����Ƿ��Ѿ������ͷ�
    bool m_bIsLock;                         // �����Ƿ���ס
    bool m_bTargetBulletShoot;              // �Ƿ���Ŀ���ӵ�
    bool m_bPositionBulletShoot;            // �Ƿ���λ���ӵ�

    int m_nIndex;                           // �ü����ǽ�ɫװ���ĵڼ�������
    int m_nLoopTimes;                       // ����ѭ��������ÿ�ζ�������ģ�
    int m_nTargetBulletIndex;               // ����ѭ����Ŀ�������ӵ������±�
    int m_nPositionBulletIndex;             // ����ѭ����λ�������ӵ������±�
    //int m_nSkillLevel;

    float m_fCDTime;                        // ��ǰCDʱ��
    float m_fLoopDelay;                     // ����ѭ��ʣ��ʱ��
    float m_fTargetTypeDelay;               // ����ѭ����Ŀ�������ӵ������ӳ�ʣ��ʱ��
    float m_fPositionTypeDelay;             // ����ѭ����λ�������ӵ������ӳ�ʣ��ʱ��
    float m_fConfLoopDelay;                 // �����е���ѭ��ʱ��
    float m_fConfFirstTargetTypeDelay;      // �������״�Ŀ�������ӵ������ӳ�ʱ��
    float m_fConfFirstPositionTypeDelay;    // �������״�λ�������ӵ������ӳ�ʱ��
    float m_fConfTargetTypeDelay;           // ������Ŀ�������ӵ������ӳ�ʱ��
    float m_fConfPositionTypeDelay;         // ������λ�������ӵ������ӳ�ʱ��

    CRole* m_pOwner;
    Vec2 m_TargetPos;
    std::vector<CRole*> m_TargetList;
    CSkillComponent* m_pSkillComponent;
    const SkillConfItem* m_pSkillConf;

private:
    //float m_fCurExecutingTime;                      // ��ǰִ��ʱ��
    //float m_fMaxExecutingTime;                      // ���ִ��ʱ��
};

#endif //__SKILL_H__
