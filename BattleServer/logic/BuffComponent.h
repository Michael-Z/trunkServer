#ifndef __BUFF_COMPONENT_H__
#define __BUFF_COMPONENT_H__

#include "LogicComponent.h"
#include "BufferData.h"

class CRole;
class CBuff;

struct NewBuffData
{
    CRole *maker;           // buff�Ĳ�����, ��Я�������, ���ܲ����߾���Я���߱�����
    int buffid;             // ������buffid
    int addstack;           // �仯�Ĳ���, ����Ϊ����, ��Ϊ������
};

class CBuffComponent : public CLogicComponent
{
public:
    CBuffComponent();
    ~CBuffComponent();

public:

    // ���л������л�
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    // �����ʼ��
    bool init(CRole *owner);
    virtual void onExit();
    // �߼���ѯ
    void logicUpdate(float delta);
    // ���һ��buff ������û��Ĭ��һ��, addstackΪ����, ���������ĳbuffid�伸�о�����༸��.
    // n��������n��, -n����n��
    bool addBuff(CRole *maker, int buffid, int addstack = 1);
    // �Ƿ�ӵ��buff
    bool hasBuff(int buffid);
    // �Ƴ�buff
    void removeBuff(int buffid);
    // ��ɢ �ɵ�����buff
    void disperse();
    // ���� �ɵ�����buff(debuff)
    void purification();
    // �������buff
    void clear();
    // ����buff
    CBuff* createBuff(CBuffComponent *cpnt, CRole *owner, CRole *maker, int buffid, int stack);
    // ����buff
    void destoryBuff(CBuff* buff);

private:

    void addBuffToRole(CRole *maker, int buffid, int addstack);

private:

    CRole *                 m_pOwner;
    std::map<int, CBuff*>   m_BuffList;
    std::vector<CBuff*>     m_ExitBuffList;
    std::vector<NewBuffData> m_AddBuffList;
};

#endif //__BUFF_COMPONENT_H__
