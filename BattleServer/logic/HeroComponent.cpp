#include "HeroComponent.h"

using namespace std;

CHeroComponent::CHeroComponent()
{
    m_eComponentType = EHeroComponent;
}


CHeroComponent::~CHeroComponent()
{
}

void CHeroComponent::onEnter()
{
    CRoleComponent::onEnter();

    m_pHero = dynamic_cast<CHero*>(getOwner());
    m_pSodlierCards = m_pHero->getSoldierCards();
}

void CHeroComponent::onExit()
{
    CRoleComponent::onExit();
    m_pHero = NULL;
}

void CHeroComponent::logicUpdate(float delta)
{
    CRoleComponent::logicUpdate(delta);

    // ��ƬCD����
    for (vector<CSoldierModel*>::iterator iter = m_pSodlierCards->begin();
        iter != m_pSodlierCards->end(); ++iter)
    {
        CSoldierModel* card = *iter;
        // ��Ƭδ����ס
        if (!card->IsLock && card->MaxCD > card->CurCD)
        {
            card->CurCD += delta;
            if (card->CurCD > card->MaxCD)
            {
                card->CurCD = card->MaxCD;
            }
        }
    }

    // ����Ψһ
    m_pHero->checkSingo();

    // ��������������ˮ���ָ�
    if (m_pHero->getBattleHelper()->getBattleType() != EBATTLE_TOWERTEST)
    {
        // ˮ���ָ�����
        float curCrystal = m_pHero->getFloatAttribute(EHeroCrystal);
        int maxCrystal = m_pHero->getIntAttribute(EHeroMaxCrystal);
        if (curCrystal < maxCrystal)
        {
            float curSpeedParam = m_pHero->getFloatAttribute(EHeroCrystalSpeedParam);
            float curSpeed = m_pHero->getFloatAttribute(EHeroCrystalSpeed);
            curCrystal += curSpeed * curSpeedParam * delta;
            if (curCrystal > maxCrystal)
            {
                curCrystal = static_cast<float>(maxCrystal);
            }
            m_pHero->setAttribute(EHeroCrystal, curCrystal);
			//LOG("crystal recover curCrystal : %f, curSpeedParam: %f, curSpeed: %f ", curCrystal, curSpeedParam, curSpeed);
        }
    }

#ifndef RunningInServer
    m_pHero->setCurSkillExecutingTime(delta);
#endif
}
