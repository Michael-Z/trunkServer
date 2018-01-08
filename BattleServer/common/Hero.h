/*
* Ӣ��
*
* 2014-12-23 by ��ү
*/
#ifndef __HERO_H__
#define __HERO_H__

#include "Role.h"

class CHero : public CRole
{
public:
	CHero();
	virtual ~CHero();

	virtual bool init(int typeId, int ownerId, int objId, CBattleHelper* battle);

	//���л�,�����л� Ӣ��
	bool serialize(CBufferData& data);
	bool unserialize(CBufferData& data);

	virtual bool executeSkillIndex(int skillIndex, Vec2 pos = Vec2::ZERO);
    virtual bool executeSkill(int skillId, Vec2 pos = Vec2::ZERO);

    // �Ƿ��������ˮ��
    bool canUpgradeCrystal();
    // ����ˮ�� - �Զ��۳�����
    bool upgradeCrystal();
    // �Ƿ���ʹ����ô��ˮ��
    bool canUseCrystal(int crystal);
    // ����ˮ��
    bool useCrystal(int crystal);
	// ˮ���Ƿ���󼶱�
	bool isCrystalMaxLevel();
    // ����ָ�������ˮ����������
	int getCrystalCost(int level);

    // �Ƿ����ٻ�ʿ��
    bool canUseSoldierCard(int soldierId);
    // �ٻ�ʿ��
    bool createSoldier(int soldierId, int line = -1);

	bool isSoldierCardLock(int id);
	bool isSoldierCardBuff(int id);
	bool isSoldierCardDebuff(int id);
	bool isSoldierCardBinding(int id);
	float getSoldierCardCDPercent(int id);

    // ��ǰ��Ƭ�Ƿ�Ψһ
    bool isSoldierCardSingle(int soldierId);
    // ��ǰ�Ƿ���Ψһʿ���ڳ���
    bool isSoldierSingle(int soldierId);
    // ÿ֡���ô˷������ж�Ψһ��Ƭ
    void checkSingo();

    CSoldierModel* getSoldierCard(int index)
    {
        if (NULL != m_SoldierCards
            && index >= 0 && index < static_cast<int>(m_SoldierCards->size()))
        {
            return m_SoldierCards->at(index);
        }
        return NULL;
    }

    std::vector<CSoldierModel*>* getSoldierCards()
    {
        return m_SoldierCards; 
    }

#ifndef RunningInServer
    // ����ˮ����Ԥ��ֵ
    void setWithholdValue(int value = 0);
    inline int getWithholdValue(){ return m_nWithholdValue; }

    // ���ù�������ִ��ʱ��
    void startSkillExecute(bool flag);
    void setCurSkillExecutingTime(float time);
    void setMaxSkillExecutingTime(float time);
    float getCommonCDPercent();
    bool isCommonCD(){ return m_bStartSkillExecute; }
#endif

private:
    std::map<int, CRole*> m_SingleMap; // Ψһ�ɷ�
    std::vector<CSoldierModel*>* m_SoldierCards;

#ifndef RunningInServer
    int m_nWithholdValue;                               // ˮ��Ԥ��ֵ
    bool m_bStartSkillExecute;                          // ��ʼִ�еı�־
    float m_fCurSkillExecutingTime;                     // ��ǰִ��ʱ��
    float m_fMaxSKillExecutingTime;                     // ���ִ��ʱ��
#endif
};

#endif
