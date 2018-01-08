/*
* Ӣ�����
*
* ʿ����ƬCD
* ˮ���ָ�
*
* 2015-1-12 by ��ү
*/
#ifndef __HERO_COMPONENT_H__
#define __HERO_COMPONENT_H__

#include "RoleComponent.h"
#include "Hero.h"

class CHeroComponent : public CRoleComponent
{
public:
    CHeroComponent();
    virtual ~CHeroComponent();

    virtual void onEnter();

    virtual void onExit();

    virtual void logicUpdate(float delta);

private:
    std::vector<CSoldierModel*>* m_pSodlierCards;
    CHero* m_pHero;
};

#endif
