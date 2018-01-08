#ifndef __SKILL_COMPONENT_H__
#define __SKILL_COMPONENT_H__

#include "LogicComponent.h"
#include "BufferData.h"

struct SkillItem
{
    int SkillID;                 //����ID
    int SKillLv;                 //���ܵȼ�
};

class CRole;
class CSkill;
class CSkillComponent : public CLogicComponent
{
public:
    CSkillComponent();
    ~CSkillComponent();

public:

    // ���л������л�
    virtual bool serialize(CBufferData& data);
    virtual bool unserialize(CBufferData& data);

    // ��ʼ�������߼����
    virtual bool init(CRole *owner, const std::vector<int> &skillids);
    virtual bool init(CRole *owner, const std::vector<int> &skillids, const std::vector<int> &skillidlvs);
    virtual void onExit();
    // �߼�֡�Ļص�
    virtual void logicUpdate(float delta);
    // ����ע�����¼�
    void regSkillBreak(CSkill *skill);
    // �Ƿ�ӵ�м���
    bool hasSkillID(int skillid);
    // ���һ���¼���
    void addNewSkill(int skillid, int skillLv = 1);
    // �Ƴ�ָ������ (Ӧ��ֻ�Ƴ�buff��ӵļ���)
    void removeSkill(int skillid);
    // �Ƴ����м���
    void removeAllSkill();
    // cd�ٷֱ�
    float getCDPercent(int index);

    CSkill *getSkillWithID(int id);
    CSkill *getSkillWithIndex(int index);
    // �Ƿ����ʹ�õ�n������
    bool canExecuteSkillIndex(int index);
    // �Ƿ����ʹ��ָ������
    bool canExecuteSkill(int skillid);
    // ��ָ��Ŀ��ʹ�õ�n������
    void executeSkillIndex(int index, std::vector<CRole*> &targets);
    // ��ָ��λ��ʹ�õ�n������
    void executeSkillIndex(int index, Vec2 &pos);

    // ͬʱ��Ŀ���Ŀ����, ����Ŀ��ʹ��ָ������
    void executeSkill(int skillid, std::vector<CRole*> &targets, Vec2 &pos);
    // ������Ŀ��ʹ��ָ������
    void executeSkill(int skillid, std::vector<CRole*> &targets);
    // ��ָ��λ��ʹ��ָ������
    void executeSkill(int skillid, Vec2 &pos);

private:
    // ��鸺��״̬����
    void checkBreak();
private:

    int                     m_nPrevState;      // ���������ߵ���һ��״̬
    CRole *                 m_pOwner;
    std::map<int, CSkill*> m_Skills;           // <skillid, skill> 
    std::map<int, CSkill*> m_CanBreakSkills;   // ���Դ�ϵļ���
};

#endif //__SKILL_COMPONENT_H__
