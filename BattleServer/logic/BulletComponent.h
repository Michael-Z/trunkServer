/*
*  BulletComponent�����ӵ����ƶ��Լ��ӵ���״̬�л�
*
*/

#ifndef __BULLET_COMPONENT_H__
#define __BULLET_COMPONENT_H__

#include "LogicComponent.h"
#include "BufferData.h"

class CRole;
class CBullet;
class CBulletPathBase;
class CBulletHitJudge; 
class BulletConfItem;
class CBattleHelper;
class CBulletComponent : public CLogicComponent
{
public:
    
    CBulletComponent();
    ~CBulletComponent();

public:
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    virtual bool init(CBullet *bullet);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool isLogicOver();
    virtual bool isFlyToScreenOut();
    virtual void logicUpdate(float delta);
    virtual void addHitTarget(CRole* target);
    virtual std::vector<CRole*>& getTargets();

private:
    // ����������Ч ���� for client
    inline void playHitEffect();

private:
    
    float                     m_fBulletExsitTime;   //����ʱ��
    CBattleHelper*            m_pHelper;
    CBullet*                  m_pOwner;             //�����ӵ�����
    CBulletPathBase*          m_pBulletPath;        //·��
    CBulletHitJudge*          m_pBulletHitJudge;    //�����ж�
    std::vector<CRole*>       m_TargetList;         //Ŀ���б�
};


/************************************************************************
�ӵ��켣
************************************************************************/
class CBulletPathBase : public ISerializable
{
public:
    CBulletPathBase();
    virtual ~CBulletPathBase();

public:

    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    virtual bool init(CBullet* bullet);
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float dt);
    //�켣�Ƿ����
    virtual void setPathOver(bool isOver = true);
    virtual bool isPathOver();

protected:

    bool             m_bIsPathOver;         //����
    float            m_fExsitTime;          //�Ѿ�����ʱ��
    CBullet*         m_pBullet;             //�ӵ�
    const BulletConfItem*  m_pBulletConf;   //�ӵ�����
};

//�����߹켣
class CBulletParabola : public CBulletPathBase
{
public:
    CBulletParabola();
    ~CBulletParabola();

public:
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);
    bool init(CBullet* bullet);
    void onEnter();
    void update(float dt);

private:
    float m_fVx;                            // X����ٶ�
    float m_fVy;                            // Y����ٶ�
    float m_fG;                             // �������ٶ�G
    float m_fFlyTime;                       // �ܵķ�������ʱ��
};

//�����켣
class CBulletDuration : public CBulletPathBase
{
public:
    CBulletDuration();
    ~CBulletDuration();

public:
	bool init(CBullet* bullet);
    void onEnter();
    void update(float dt);
};

//ֱ�ߴ�͸
class CBulletPierce : public CBulletPathBase
{
public:
    CBulletPierce();
    ~CBulletPierce();

public:
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);
    bool init(CBullet* bullet);
    void onEnter();
    void update(float dt);

private:
    float   m_fSpeed;                       // �ӵ����ٶ�
    float   m_fPullBackTime;                // �۷�ʱ��
};

/************************************************************************/
/* �����ж�                                                               */
/************************************************************************/
class CBulletHitJudge : public ISerializable
{
public:
    CBulletHitJudge();
    virtual ~CBulletHitJudge();

public:

    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    virtual bool init(CBullet *bullet, CBulletComponent *btComponent);
    virtual void update(float dt);
    virtual void setJudgeOver();
    virtual bool isJudgeOver();
    virtual bool isNeedCount();
    virtual bool judge();
    //�����ɫ, ��δ�ﵽ�����������Ŀ����н��� 
    void countRole();
    //����������Ŀ������buff
    void roleAddBuff();

private:
    //�����Ƿ�����
    bool judgePointHit();
    //�����Ƿ�����
    bool judgeRectHit();
    //�Ƿ���ӵ�Ŀ����ײ, �ж��Ƿ�����,
    bool isTargetCollide(CRole *target);
    //�Ƿ�ﵽ���������
    bool isMaxCount(CRole *target);

protected:

    int   m_nHitType;                       //��ײ����
    int   m_nJudgeTimes;                    //���жϴ���
    float m_fFirstJudgeTime;                //�״��ж�ʱ��
    float m_fInterval;                      //�������

    float m_fParam;                         //�����ж�����
    bool  m_bNeedCount;                     //�Ƿ���Ҫ����
    bool  m_bJudgeOver;                     //�Ƿ��ж�����

    CBullet *m_pBullet;                     //�ӵ�
    CBulletComponent* m_pBulletComponent;   //�ӵ��߼����
    std::map<int, int> m_TargetCountTimes;  //��λ�������<objid, times>
};

#endif 
