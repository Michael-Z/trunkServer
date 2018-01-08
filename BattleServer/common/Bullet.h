/*
* �ӵ�, �ӵ��켣, �ӵ�����, �ӵ�����
*
* 2014-12-23 by ��ү
*/
#ifndef __BULLET_H__
#define __BULLET_H__

#include "GameObject.h"
#include <vector>

enum BulletPathType
{
    BPT_NONE = 0,               // ��
    BPT_PARABOLA,               // ������
    BPT_TRACK,                  // ׷��
    BPT_DURATION,               // ��Χ����
    BPT_PIERCE,                 // ֱ�ߴ�͸
};

enum BulletJudgeType
{
    BJT_NONE = 0,               // ��
    BJT_POINT,                  // �������ж�
    BJT_RECT,                   // ���������ж�
};

enum BulletHitType
{
    BHT_NONE = 0,               // ��
    BHT_FLYEND,                 // ���н���
    BHT_FLYING,                 // ������
    BHT_FLYCOLLISION,           // ������ײ
};

enum BulletStateType
{
    BST_NONE,
    BST_FLYING,                 // �ӵ�����״̬��
    BST_FLYEND,                 // �ӵ����н���
};

enum BulletEndType
{
    BET_NONE,                   // ��
    BET_ATTACKERHITPOINT,       // �����ߵı�����
    BET_ATTACKERMOVEPOINT,      // �����ߵ��ƶ����ĵ�
    BET_TARGETHITPOINT,         // Ŀ�걻����
    BET_TARGETMOVEPOINT,        // Ŀ���ƶ���
    BET_POINT,                  // �ӵ�����Ŀ���
    BET_FRIENDHERO,             // ����Ӣ��
    BET_ENEMYHERO,              // �з�Ӣ��
    BET_ORIGIN,                 // ԭ��(0,0)��, ���ڶ���Ļ��ָ��λ���ͷ��ӵ�
};

class BulletConfItem;
class CBulletComponent;
class CBullet : 
    public CGameObject
{
public:
    CBullet();
    virtual ~CBullet();

public:

    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    virtual bool init(int bulletId, int uid, int objId, CRole *owner, CBattleHelper* battle);
    virtual void onEnter();
    virtual void onExit();

    virtual void setTarget(CRole *target);
    virtual CRole *getTarget();

    virtual CRole *getOwner();
    
	virtual void setTargetPos(const Vec2 &pos);
    virtual Vec2 getTargetPos();

    virtual const BulletConfItem *getBulletConf();
    virtual Vec2& getFiringPoint();
    
    virtual void changeState(int state);
    virtual int getState();

    inline void setPrevPosition(const Vec2& pos) { m_PrevPos = pos; }
    inline Vec2& getPrevPosition() { return m_PrevPos; }

protected:
    int                         m_nState;               //�ӵ�״̬
    Vec2                        m_FiringPoint;          //�����
    Vec2                        m_TargetPos;            //�������ָ��Ŀ��, ��Ҫָ������λ��
    Vec2                        m_TargetPosExtend;      //Ŀ����ֵ
    Vec2                        m_PrevPos;              //�ӵ���һ֡��λ��
    CRole*                      m_pTarget;              //Ŀ��
    CRole*                      m_pOwner;               //�ӵ�������
    CBulletComponent*           m_pComponent;           //�߼����
    const BulletConfItem*       m_pBulletConf;          //�ӵ�����
    std::vector<CRole *>        m_Targets;              //Ŀ���б�, �߼�������֮����ڴ˴�, �ṩ�����ֲ�
};


#endif 
