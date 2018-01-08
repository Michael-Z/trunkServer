#include "Role.h"
#include "RoleComponent.h"
#include "ComponentCache.h"

#ifndef RunningInServer

#include "ArmatureComponent.h"
#include "RoleDisplayComponent.h"
#include "SpineComponent.h"
#include "ConfOther.h"
#include "HPBar.h"
#include "BlinkAction.h"
#include "Game.h"

USING_NS_CC;

#endif

using namespace std;

void dumpIntMap(std::map<int, int>& m)
{
    for (map<int, int>::iterator iter = m.begin();
        iter != m.end(); ++iter)
    {
        KXLOGBATTLE("int key %d value %d", iter->first, iter->second);
    }
}

void dumpFloatMap(std::map<int, float>& m)
{
    for (map<int, float>::iterator iter = m.begin();
        iter != m.end(); ++iter)
    {
        KXLOGBATTLE("float key %d value %f", iter->first, iter->second);
    }
}

CBlackBoard::CBlackBoard()
	:m_BattleHelper(NULL)
{
}

CBlackBoard::~CBlackBoard()
{
    clearBlackBoard();
}

void CBlackBoard::clearTargetList()
{
    releaseAndClearVec<CRole*>(m_TargetList);
}

void CBlackBoard::clearBlackBoard()
{
    releaseAndClearVec<CRole*>(m_TargetList);
    m_TargetPoint = Vec2::ZERO;
}

void CBlackBoard::addToTargetList(CRole* role)
{
    if (NULL != role)
    {
        SAFE_RETAIN(role);
        m_TargetList.push_back(role);
    }
}

void CBlackBoard::clearAndAddToTargetList(CRole* role)
{
    releaseAndClearVec<CRole*>(m_TargetList);
    if (NULL != role)
    {
		SAFE_RETAIN(role);
        m_TargetList.push_back(role);
    }
}

void CBlackBoard::removeFormTargetList(CRole* role)
{
    if (NULL != role)
    {
        for (vector<CRole*>::iterator iter = m_TargetList.begin();
            iter != m_TargetList.end(); ++iter)
        {
            if ((*iter)->getObjectId() == role->getObjectId())
            {
                SAFE_RELEASE((*iter));
                m_TargetList.erase(iter);
                break;
            }
        }
    }
}

bool CBlackBoard::serialize(CBufferData& data)
{
	SERIALIZE_MATCH(data);

	data.writeData(m_TargetPoint.x);
    data.writeData(m_TargetPoint.y);

	int length = m_TargetList.size();
	data.writeData(length);	
	std::vector<CRole*>::iterator iter = m_TargetList.begin();
	for (; iter != m_TargetList.end(); ++iter)
	{
		data.writeData((*iter)->getObjectId());
	}

	SERIALIZE_MATCH(data);

	return true;
}

bool CBlackBoard::unserialize(CBufferData& data)
{
	UN_SERIALIZE_MATCH(data);

	if (NULL == m_BattleHelper)
	{
		return false;
	}
    
    float x = 0.0f, y = 0.0f;
    data.readData(x);
    data.readData(y);
    m_TargetPoint = Vec2(x, y);
	
	int nTargetCount = 0;
	data.readData(nTargetCount);
    
	// ����Ŀ���б�
	releaseAndClearVec(m_TargetList);

	for (int i = 0; i < nTargetCount; i++)
	{
		int nObjectID = 0;
		data.readData(nObjectID);
		// ����objectID��ȡCROLE
		CRole *pRole = m_BattleHelper->getRoleByObjectId(nObjectID);
		if (NULL == pRole)
		{
			continue;
			LOG("error about role NULL!");
		}
		else
		{
			SAFE_RETAIN(pRole);
		}

		m_TargetList.push_back(pRole);
	}

	UN_SERIALIZE_MATCH(data);

	return true;
}

CRole::CRole()
: m_AIComponent(NULL),
m_BuffComponent(NULL),
m_SkillComponent(NULL),
m_RoleComponent(NULL),
m_DisplayNode(NULL)
{
}


CRole::~CRole()
{
}

bool CRole::init(int typeId, int ownerId, int objId, CBattleHelper* battle)
{
#ifndef RunningInServer
	setOpacity(255);
	setCascadeOpacityEnabled(true);
#endif 

    for (int i = EAttributeIntVarBegin + 1; i < EAttributeIntVarEnd; ++i)
    {
        m_IntAttributes[i] = 0;
    }
    for (int i = EAttributeFloatVarBegin + 1; i < EAttributeFloatVarEnd; ++i)
    {
        m_FloatAttributes[i] = 0.0f;
    }
	for (int i = EStatBegin + 1; i < EStatEnd; i++)
	{
		m_IntAttributes[i] = 0;
	}
    // ��ʼ���̶��Ķ�̬����
    initAttribute(EAttributeExtraHP, 0);
    initAttribute(EAttributeBeAoeable, 1);
    initAttribute(EAttributeBeAttackable, 1);
    initAttribute(EAttributeBeBuffable, 1);
    initAttribute(EAttributeHitPowerX, 0.0f);
    initAttribute(EAttributeHitPowerY, 0.0f);
    initAttribute(EAttributeX, 0.0f);
    initAttribute(EAttributeY, 0.0f);
    // EAttributeYmin �ɴ�������������
    // Camp�ɾ�����������
    m_FloatAttributes[EAttributeAttackSpeedVar] = 1.0f;
	
    m_BlackBoard.setBattleHelper(battle);
    return CGameObject::init(typeId, ownerId, objId, battle);
}

void CRole::onExit()
{
    CGameObject::onExit();
    m_BlackBoard.clearBlackBoard();
    m_AIComponent = NULL;
    m_BuffComponent = NULL;
    m_SkillComponent = NULL;
    m_RoleComponent = NULL;
#ifndef RunningInServer
    m_DisplayNode = NULL;
#endif

}

void CRole::initRoleAttribute(CRoleModel *model)
{
    CHECK_RETURN_VOID(NULL != model);
    m_pModel = model;

    int offset = EAttributeIntBegin - EClassIntBegin;
    for (int i = EClassIntBegin + 1; i < EClassIntEnd; ++i)
    {
        m_IntAttributes[offset + i] = model->getClassInt(static_cast<EAttributeTypes>(i));
    }
    offset = EAttributeFloatEnd - EClassFloatEnd;
    for (int i = EClassFloatBegin + 1; i < EClassFloatEnd; ++i)
    {
        m_FloatAttributes[offset + i] = model->getClassFloat(static_cast<EAttributeTypes>(i));
    }

    m_IntAttributes[EAttributeHPPosX] = 0;
    m_IntAttributes[EAttributeHPPosY] = 0;
    m_IntAttributes[EAttributeHP] = m_IntAttributes[EAttributeMaxHP];
    // �Ƶ�˵����ʼ����Ϊ0��2016-5-25
    m_IntAttributes[EAttributeMP] = 0; //m_IntAttributes[EAttributeMaxMP];
    m_IntAttributes[EAttributeStrong] = m_IntAttributes[EAttributeMaxStrong];
    m_IntAttributes[EAttributeRage] = m_IntAttributes[EAttributeMaxRage];;
    m_IntAttributes[ECardLevel] = model->getLevel();
 
	float scale = m_FloatAttributes[EAttributeScale];
    modifyAttribute(EAttributeFireRange, floatToInt(getIntAttribute(EClassFireRange) * scale));
	const Role* role = model->getRoleComm();
	m_FireOffset = role->FireOffset * scale;
	m_FireOffset.x *= m_nDirection;
	m_HitOffset = role->HitOffset * scale;
	m_HitOffset.x *= m_nDirection;
	m_HeadOffset = role->HeadOffset * scale;
	m_HeadOffset.x *= m_nDirection;

    //dumpIntMap(m_IntAttributes);
    //dumpFloatMap(m_FloatAttributes);
}

bool CRole::initRoleComponents(const Role* role)
{
	// ���ⲿ�ȴ�����ɫ���
	CHECK_RETURN(NULL != m_RoleComponent);

    // ���AI���
    m_AIComponent = createComponent<CAIComponent>(EAIComponent);
    CHECK_RETURN(m_AIComponent);
    if (!m_AIComponent->init(role->AIID, this))
    {
        CComponentCache::getInstance()->freeComponent(m_AIComponent);
        KXLOGDEBUG("CRole AIComponent init fail roleID %d, AIID %d", role->ClassID, role->AIID);
        return false;
    }
    addLogicComponent(m_AIComponent);

	// ���BUFF���
	//m_BuffComponent = createComponent<CBuffComponent>(EBuffComponent);
    //CHECK_RETURN(m_BuffComponent);
    //if (!m_BuffComponent->init(this))
    //{
    //    CComponentCache::getInstance()->freeComponent(m_BuffComponent);
    //    KXLOGDEBUG("CRole AIComponent init fail roleID %d", role->ClassID);
    //    return false;
    //}
    //addLogicComponent(m_BuffComponent);

    // ����������ò�Ϊ�գ���Ӽ������
	if (role->Skill.size() > 0)
	{
		if (NULL == m_SkillComponent)
        {            
            m_SkillComponent = createComponent<CSkillComponent>(ESkillComponent);
            CHECK_RETURN(m_SkillComponent);
            if (!m_SkillComponent->init(this, role->Skill))
            {
                CComponentCache::getInstance()->freeComponent(m_SkillComponent);
                KXLOGDEBUG("CRole skillComponent init fail roleID %d", role->ClassID);
                return false;
            }
			addLogicComponent(m_SkillComponent);
		}
		else
		{
			CHECK_RETURN(m_SkillComponent->init(this, role->Skill));
            if (!m_SkillComponent->init(this, role->Skill))
            {
                CComponentCache::getInstance()->freeComponent(m_SkillComponent);
                KXLOGDEBUG("CRole skillComponent init fail roleID %d", role->ClassID);
                return false;
            }
		}		
	}
	return true;
}

// ��ʼ����ʾ��ص������������Ϊ��ʵ�֣�
bool CRole::initDisplayComponents(const Role* role)
{
#ifndef RunningInServer

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // �����ڵ�������
    switch (m_RoleType)
    {
    case RT_HERO:
        setName("Summoner" + toolToStr(m_nGameObjectId));
        break;
    case RT_SOLDIER:
        setName("Hero" + toolToStr(m_nGameObjectId));
        break;
    case RT_MONSTER:
        setName("Monster" + toolToStr(m_nGameObjectId));
        break;
    case RT_BOSS:
        setName("Boss" + toolToStr(m_nGameObjectId));
        break;
    case RT_SUMMON:
        setName("Call" + toolToStr(m_nGameObjectId));
        break;

    case RT_NONE:
    default:
        break;
    }
#endif


	m_DisplayNode = Node::create();
	this->addChild(m_DisplayNode);
    getDisplayNode()->setScale(role->Scale);

	const SResPathItem* pResInfo = queryConfSResInfo(role->AnimationID);
    LOG("initDisplayComponents AnimationId %d", role->AnimationID);
    CHECK_RETURN(pResInfo);
	CAnimateComponent *pAnimateCom = NULL;

	// ��������
	if (RT_ARMATURE == pResInfo->ResType)
    {
		// �����ʾ���
		CArmatureComponent* armcom = new CArmatureComponent();
		if (!armcom->initAnimate(pResInfo->ResName, getDisplayNode()))
		{
			armcom->release();
            KXLOGERROR("error resName %s", pResInfo->ResName.c_str());
#ifndef RunningInServer
            if (CGame::getInstance()->isDebug())
            {
                cocos2d::MessageBox(pResInfo->ResName.c_str(), "error resName");
            }
#endif
			return false;
		}
		addComponent(armcom);
		armcom->release();
		pAnimateCom = armcom;
	}
	else if (RT_SPINE == pResInfo->ResType)
	{
		CSpineComponent* spinecom = new CSpineComponent();
        if (!spinecom->initAnimate(pResInfo->Path, getDisplayNode()))
		{
            spinecom->release();
            KXLOGERROR("error Path %s", pResInfo->Path.c_str());
#ifndef RunningInServer
            if (CGame::getInstance()->isDebug())
            {
                cocos2d::MessageBox(pResInfo->Path.c_str(), "error Path");
            }
#endif
			return false;
		}
		if (pResInfo->Skin.empty() == false)
		{
			spinecom->getSkeletonAnimation()->setSkin(pResInfo->Skin);
		}

		addComponent(spinecom);
		spinecom->release();
		pAnimateCom = spinecom;

        //����ɫ��,Ŀǰֻ��spine��Ҫ
        spinecom->setHSV(role->AnimationHSV);
	}
	else
    {
        KXLOGERROR("error type %d", pResInfo->ResType);
#ifndef RunningInServer
        if (CGame::getInstance()->isDebug())
        {
            cocos2d::MessageBox(String::createWithFormat("%d", pResInfo->ResType)->getCString(), "error type");
        }
#endif
		return false;
	}

	// �����ʾ�߼����
	CRoleDisplayComponent* rolecom = new CRoleDisplayComponent();
	if (!rolecom->init(this, pAnimateCom, m_RoleComponent))
	{
		pAnimateCom->release();
		rolecom->release();
        KXLOGERROR("error obj %p, %p", pAnimateCom, m_RoleComponent);
#ifndef RunningInServer
        if (CGame::getInstance()->isDebug())
        {
            cocos2d::MessageBox(String::createWithFormat("%p %p", pAnimateCom, m_RoleComponent)->getCString(), "error obj");
        }
#endif
		return false;
	}
	addComponent(rolecom);
	rolecom->release();

	// ������������
	switch (m_RoleType)
	{
	case RT_BOSS:
	//case RT_MONSTER:
        rolecom->getHPBar()->setName(getBattleHelper()->getComputerModel()->getUserName());
		break;
	case RT_HERO:
	//case RT_SOLDIER:
        rolecom->getHPBar()->setName(getBattleHelper()->getUserModel(getOwnerId())->getUserName());
		break;
	default:
		break;
	}

#endif

	// ������ֱ�ӷ���true
	return true;
}

bool CRole::haveBuff(int buffId)
{
    if (NULL != m_BuffComponent)
    {
        return m_BuffComponent->hasBuff(buffId);
    }

    return false;
}

bool CRole::addBuff(CRole *maker, int buffId, int addstack)
{
    if (NULL != m_BuffComponent)
    {
        m_BuffComponent->addBuff(maker, buffId, addstack);
        return true;
    }

    return false;
}

void CRole::delBuff(int buffId)
{
	if (NULL != m_BuffComponent)
	{
		m_BuffComponent->removeBuff(buffId);
	}
}

CSkill *CRole::getSkillWithID(int skillId)
{
    if (NULL != m_SkillComponent)
    {
        return m_SkillComponent->getSkillWithID(skillId);
    }

    return NULL;
}

CSkill *CRole::getSkillWithIndex(int skillIndex)
{
    if (NULL != m_SkillComponent)
    {
        return m_SkillComponent->getSkillWithIndex(skillIndex);
    }

    return NULL;
}

bool CRole::canExecuteSkillIndex(int skillIndex)
{
    if (NULL != m_SkillComponent)
    {
        return m_SkillComponent->canExecuteSkillIndex(skillIndex);
    } 

    return false;
}

bool CRole::executeSkillIndex(int skillIndex, Vec2 &pos)
{
    if (NULL != m_SkillComponent)
    {
        if (m_SkillComponent->canExecuteSkillIndex(skillIndex))
        {
            m_SkillComponent->executeSkillIndex(skillIndex, pos);
        }
        return true;
    }

    return false;
}

bool CRole::canExecuteSkill(int skillId)
{
    if (NULL != m_SkillComponent)
    {
        return m_SkillComponent->canExecuteSkill(skillId);
    }

    return false;
}

bool CRole::executeSkill(int skillId, Vec2 &pos)
{
    if (NULL != m_SkillComponent)
    {
        //ע��ָ��λ��
        if (m_SkillComponent->canExecuteSkill(skillId))
        {
            m_SkillComponent->executeSkill(skillId, pos);
        }
        return true;
    }

    return false;
}

bool CRole::executeSkill(int skillId, std::vector<CRole*> &targets, Vec2 &pos)
{
    if (NULL != m_SkillComponent)
    {
        //ע��ָ��λ��
        if (m_SkillComponent->canExecuteSkill(skillId))
        {
            m_SkillComponent->executeSkill(skillId, targets, pos);
        }
        return true;
    }

    return false;
}

CState* CRole::changeState(int stateId)
{
    if (NULL != m_RoleComponent)
    {
        return m_RoleComponent->changeState(stateId);
    }

    return NULL;
}

int CRole::currentState()
{
    if (NULL != m_RoleComponent)
    {
        return m_RoleComponent->getStateId();
    }

    return State_None;
}

void CRole::updateStat(EAttributeTypes stat)
{
    if (stat > EStatBegin && stat < EStatEnd)
    {
        m_IntAttributes[stat] += 1;
    }
}

bool CRole::updateAttribute(EAttributeTypes key, int v)
{
    std::map<int, int>::iterator iter = m_IntAttributes.find(key);
    if (iter == m_IntAttributes.end())
    {
        return false;
    }
    else
    {
        iter->second = limitAttribute(key, iter->second + v);
        return true;
    }
}

bool CRole::updateAttribute(EAttributeTypes key, float v)
{
    std::map<int, float>::iterator iter = m_FloatAttributes.find(key);
    if (iter == m_FloatAttributes.end())
    {
        return false;
    }
    else
    {
        iter->second = limitAttribute(key, iter->second + v);
        return true;
    }
}

bool CRole::checkAttribute(EAttributeTypes key, int& outv)
{
    std::map<int, int>::iterator iter = m_IntAttributes.find(key);
    if (iter != m_IntAttributes.end())
    {
        outv = iter->second;
        return true;
    }
    if (m_pModel->classIntKeyExist(key))
    {
        outv = m_pModel->getClassInt(key);
        return true;
    }
    return false;
}

bool CRole::checkAttribute(EAttributeTypes key, float& outv)
{
    std::map<int, float>::iterator iter = m_FloatAttributes.find(key);
    if (iter != m_FloatAttributes.end())
    {
        outv = iter->second;
        return true;
    }
    if (m_pModel->classFloatKeyExist(key))
    {
        outv = m_pModel->getClassFloat(key);
        return true;
    }
    return false;
}

int CRole::getIntAttribute(EAttributeTypes key)
{
    std::map<int, int>::iterator iter = m_IntAttributes.find(key);
    if (iter != m_IntAttributes.end())
    {
        return iter->second;
    }
    return m_pModel->getClassInt(key);
}

float CRole::getFloatAttribute(EAttributeTypes key)
{
    std::map<int, float>::iterator iter = m_FloatAttributes.find(key);
    if (iter != m_FloatAttributes.end())
    {
        return iter->second;
    }
    return m_pModel->getClassFloat(key);
}

int CRole::limitAttribute(EAttributeTypes key, int v)
{
    // ���������
    switch (key)
    {
    case EAttributeExtraHP:
        limitValue(v, 0, 10000);
        break;
    case EAttributeSpeed:
        limitValue(v, 33, 400);
        break;
    /*case EAttributeSpeedVar:
        //if (v > 200) v = 200;
        break;
    case EAttributePPenetrateVar:
    case EAttributeMPenetrateVar:
        break;*/
    case EAttributeRageVar:
    case EAttributeRageRecoverVar:
    case EAttributeMPVar:
    case EAttributeMPRecoverVar:
        limitValue(v, -20000, 10000);
        break;
    case EAttributeMaxRage:
    case EAttributeMaxMP:
    case EAttributeMPRecover:
        limitValue(v, 0, 10000);
        break;
    case EAttributeMaxStrong:
    case EAttributeStrongRecover:
        limitValue(v, 0, 10000);
    case EAttributeStrongVar:
        limitValue(v, -20000, 10000);
        break;
    case EAttributeHP:
        limitValue(v, 0, m_IntAttributes[EAttributeMaxHP]);
        break;
    case EAttributeMP:
        limitValue(v, 0, m_IntAttributes[EAttributeMaxMP]);
        break;
    case EAttributeRage:
        limitValue(v, 0, m_IntAttributes[EAttributeMaxRage]);
        break;
    case EAttributeStrong:
        limitValue(v, 0, m_IntAttributes[EAttributeMaxStrong]);
        break;
    case EAttributeHaterd:
        limitValue(v, 0, 100);
        break;
    case EAttributeBeAoeable:
    case EAttributeBeAttackable:
    case EAttributeBeBuffable:
        limitValue(v, 0, 1);
        break;

    case EAttributePGuardVar:
    case EAttributeMGuardVar:
    case EAttributeDefendVar:
    case EAttributePAttackVar:
    case EAttributeMAttackVar:
        break;
    case EAttributeResustanceVar:
        limitValue(v, -2, 1);
        break;
    default:
        if (v < 0) { v = 0; }
        break;
    }
    return v;
}

float CRole::limitAttribute(EAttributeTypes key, float v)
{
    // ���������
    switch (key)
    {
    case EAttributeX:
        m_Position.x = v;
        setPositionX(v);
        break;
    case EAttributeY:
        m_Position.y = v;
        setPositionY(v);
        break;
    /*case EAttributeScale:
    case EAttributeEffectScale:
    case EAttributeCritDamageVar:
        break;
    case EAttributeDefendVar:
    case EAttributeResustanceVar:
        //limitValue(v, -1.75f, 0.75f);
        break;
    case EAttributeCritVar:
        //limitValue(v, -200.0f, 100.0f);
        break;
    case EAttributeReboundVar:
    case EAttributeVampireVar:
        //limitValue(v, -150.0f, 50.0f);
        break;
    case EAttributeMissVar:
        //limitValue(v, -175.0f, 75.0f);
        break;*/
    case EAttributeDefend:
    case EAttributeResustance:
        limitValue(v, -1.0f, 1.0f);
        break;
    case EAttributeVampire:
    case EAttributeRebound:
    case EAttributeMiss:
    case EAttributeCrit:
        limitValue(v, 0.0f, 100.0f);
        break;
    default:
        if (v < 0.0f) { v = 0.0f; }
        break;
    }
    return v;
}

void CRole::initAttribute(EAttributeTypes key, int v)
{
    m_IntAttributes[key] = v;
}

void CRole::initAttribute(EAttributeTypes key, float v)
{
    m_FloatAttributes[key] = v;
}

bool CRole::canModifyAttribute(EAttributeTypes key)
{
    // ���˲����޸�
    switch (key)
    {
        // int ����
        case EAttributePAttack:
        case EAttributeMAttack:
        case EAttributePGuard:
        case EAttributeMGuard:
        case EAttributePPenetrate:
        case EAttributeMPenetrate:
        case EAttributeAttackSpeed:
        case EAttributeMaxRage:
        case EAttributeRageRecover:
        //case EAttributeMaxMP:
        case EAttributeMaxStrong:
        case EAttributeStrongRecover:
        case EAttributeYmin:
        // float ����
        case EAttributeDefend:
        case EAttributeResustance:
        case EAttributeVampire:
        case EAttributeRebound:
        case EAttributeMiss:
        case EAttributeCrit:
        case EAttributeCritDamage:
            LOG("Error You Can Not Modify Attribute %d At Count", key);
            return false;
        default:
            return true;
    }
}

void CRole::modifyAttribute(EAttributeTypes key, int v)
{
    m_IntAttributes[key] = limitAttribute(key, v);
}

void CRole::modifyAttribute(EAttributeTypes key, float v)
{
    m_FloatAttributes[key] = limitAttribute(key, v);
}

void CRole::changeOtherAttrbute(EAttributeTypes key)
{
    // ��������
    switch (key)
    {
        // �޸����ŵ��µ�����
        case EAttributeScale:
        {
            float scale = m_FloatAttributes[key];
#ifndef RunningInServer
            getDisplayNode()->setScale(scale);
#endif    
            modifyAttribute(EAttributeFireRange, (int)(getIntAttribute(EClassFireRange) * scale));
            //modifyAttribute(EAttributeHPLine, getIntAttribute(EClassHPLine) * scale);
            const Role* role = m_pModel->getRoleComm();
            m_FireOffset = role->FireOffset * scale;
            m_FireOffset.x *= m_nDirection;
            m_HitOffset = role->HitOffset * scale;
            m_HitOffset.x *= m_nDirection;
            m_HeadOffset = role->HeadOffset * scale;
            m_HeadOffset.x *= m_nDirection;
            break;
        }
        // ��ɫ�ƶ��ٶȱ仯ֵ
		case EAttributeSpeedVar:
		{	
			int var = m_IntAttributes[EAttributeSpeedVar];
			limitValue(var, -200, 200);
			modifyAttribute(EAttributeSpeed, getIntAttribute(EClassSpeed) + var);
			break;
		}
        // ��ɫ���������仯ֵ
        case EAttributePAttackVar:
            modifyAttribute(EAttributePAttack, getIntAttribute(EClassPAttack) + m_IntAttributes[EAttributePAttackVar]);
            break;
        // ��ɫħ���������仯ֵ
        case EAttributeMAttackVar:
            modifyAttribute(EAttributeMAttack, getIntAttribute(EClassMAttack) + m_IntAttributes[EAttributeMAttackVar]);
            break;
        // ��ɫ�����ױ仯ֵ
        case EAttributePGuardVar:
            modifyAttribute(EAttributePGuard, getIntAttribute(EClassPGuard) + m_IntAttributes[EAttributePGuardVar]);
            break;
        // ��ɫħ�����ױ仯ֵ
        case EAttributeMGuardVar:
            modifyAttribute(EAttributeMGuard, getIntAttribute(EClassMGuard) + m_IntAttributes[EAttributeMGuardVar]);
            break;
        // ��ɫ����͸�仯ֵ
        case EAttributePPenetrateVar:
            modifyAttribute(EAttributePPenetrate, getIntAttribute(EClassPPenetrate) + m_IntAttributes[EAttributePPenetrateVar]);
            break;
        // ��ɫħ����͸�仯ֵ
        case EAttributeMPenetrateVar:
            modifyAttribute(EAttributeMPenetrate, getIntAttribute(EClassMPenetrate) + m_IntAttributes[EAttributeMPenetrateVar]);
            break;
        // ��ɫ��������
        case EAttributeAttackSpeedVar:
			modifyAttribute(EAttributeAttackSpeed, getIntAttribute(EClassAttackSpeed) / m_FloatAttributes[EAttributeAttackSpeedVar]);
            break;
        // ��ɫŭ������ֵ�仯ֵ
        case EAttributeRageVar:
            modifyAttribute(EAttributeMaxRage, getIntAttribute(EClassRage) + m_IntAttributes[EAttributeRageVar]);
            break;
        // ��ɫŭ���ظ�ֵ�仯ֵ
        case EAttributeRageRecoverVar:
            modifyAttribute(EAttributeRageRecover, getIntAttribute(EClassRageRecover) + m_IntAttributes[EAttributeRageRecoverVar]);
            break;
        // ��ɫħ��ֵ����ֵ�仯ֵ
        case EAttributeMPVar:
            modifyAttribute(EAttributeMaxMP, getIntAttribute(EClassMP) + m_IntAttributes[EAttributeMPVar]);
            break;
        // ��ɫħ���ظ��仯ֵ
        case EAttributeMPRecoverVar:
            modifyAttribute(EAttributeMPRecover, getIntAttribute(EClassMPRecover) + m_IntAttributes[EAttributeMPRecoverVar]);
            break;
        // ��ɫ��������ֵ�仯ֵ
        case EAttributeStrongVar:
            modifyAttribute(EAttributeMaxStrong, getIntAttribute(EClassStrong) + m_IntAttributes[EAttributeStrongVar]);
            break;
        // ��ɫ����ֵ�ָ�����
        case EAttributeStrongRecoverVar:
            modifyAttribute(EAttributeStrongRecover, getIntAttribute(EClassStrongRecover) * m_IntAttributes[EAttributeStrongRecoverVar]);
            break;

        // Float Attributes ...
        // ��ɫ����ֵ�仯ֵ
        case EAttributeCritVar:
            modifyAttribute(EAttributeCrit, getFloatAttribute(EClassCrit) + m_FloatAttributes[EAttributeCritVar]);
            break;
        //��ɫ�����˺��仯ֵ
		case EAttributeCritDamageVar:
		{
			float var = m_FloatAttributes[EAttributeCritDamageVar];
			limitValue(var, -200.0f, 100.0f);
			modifyAttribute(EAttributeCritDamage, getFloatAttribute(EClassCritDamage) + var);
			break;
		}
        //��ɫ����ֵ�仯ֵ
		case EAttributeMissVar:
		{
			float var = m_FloatAttributes[EAttributeMissVar];
			limitValue(var, -200.0f, 100.0f);
			modifyAttribute(EAttributeMiss, getFloatAttribute(EClassMiss) + var);
			break;
		}
        //��ɫ����ֵ�仯ֵ
		case EAttributeReboundVar:
		{
			float var = m_FloatAttributes[EAttributeReboundVar];
			limitValue(var, -200.0f, 100.0f);
			modifyAttribute(EAttributeRebound, getFloatAttribute(EClassRebound) + var);
			break;
		}
        //��ɫ��Ѫֵ�仯ֵ
		case EAttributeVampireVar:
		{
			float var = m_FloatAttributes[EAttributeVampireVar];
			limitValue(var, -200.0f, 100.0f);
			modifyAttribute(EAttributeVampire, getFloatAttribute(EClassVampire) + var);
			break;
		} 
        // ��ɫħ�����˱仯ֵ
		case EAttributeResustanceVar:
		{
			float var = m_FloatAttributes[EAttributeResustanceVar];
			limitValue(var, -2.0f, 1.0f);
			modifyAttribute(EAttributeResustance, getFloatAttribute(EClassResustance) + var);
			break;
		}
        // ��ɫ������˱仯ֵ
		case EAttributeDefendVar:
		{
			float var = m_FloatAttributes[EAttributeDefendVar];
			limitValue(var, -2.0f, 1.0f);
			modifyAttribute(EAttributeDefend, getFloatAttribute(EClassDefend) + var);
			break;
		}
        default:
            break;
    }
}

void CRole::setAttribute(EAttributeTypes key, int v)
{
    if (!canModifyAttribute(key))
    {
        return;
    }
    modifyAttribute(key, v);
    changeOtherAttrbute(key);
}

void CRole::setAttribute(EAttributeTypes key, float v)
{
    if (!canModifyAttribute(key))
    {
        return;
    }
    modifyAttribute(key, v);
    changeOtherAttrbute(key);
}

bool CRole::serialize(CBufferData& data)
{

	SERIALIZE_MATCH(data);

	if (!CGameObject::serialize(data))
	{
		return false;
	}

	// Attribute - ��̬����(int)
	for (int h = EAttributeBegin + 1; h <= EAttributeBeBuffable; ++h)
	{
		data.writeData(getIntAttribute((EAttributeTypes)h));
	}

	// Attribute - ��̬����(float)
	for (int i = EAttributeYmin; i < EAttributeEnd; ++i)
	{
		data.writeData(getFloatAttribute((EAttributeTypes)i));
	}

	// Attribute - int ����
	for (int j = EAttributeIntBegin + 1; j < EAttributeIntEnd; ++j)
	{
		data.writeData(getIntAttribute((EAttributeTypes)j));
	}

	// Attribute - float ����
	for (int k = EAttributeFloatBegin + 1; k < EAttributeFloatEnd; ++k)
	{
		data.writeData(getFloatAttribute((EAttributeTypes)k));
	}

	// Var ���� - int ����
	for (int m = EAttributeIntVarBegin + 1; m < EAttributeIntVarEnd; ++m)
	{
		data.writeData(getIntAttribute((EAttributeTypes)m));
	}

	// Var ���� - float ����
	for (int n = EAttributeFloatVarBegin + 1; n < EAttributeFloatVarEnd; ++n)
	{
		data.writeData(getFloatAttribute((EAttributeTypes)n));
	}

	// Stat���� (int)
	for (int x = EStatBegin + 1; x <= EStatAttack; ++x)
	{
		data.writeData(getIntAttribute((EAttributeTypes)x));
	}

	data.writeData(m_RoleType);
	if (RT_HERO == m_RoleType)
	{
		for (int y = EHeroCrystalLevel; y <= EHeroMaxCrystal; ++y)
		{
			data.writeData(getIntAttribute((EAttributeTypes)y));
		}
		for (int z = EHeroCrystal; z <= EHeroCrystalSpeed; ++z)
		{
			data.writeData(getFloatAttribute((EAttributeTypes)z));
		}
	}

	//BOSS����������ܲ�����
	if (!m_bSafeRemove)
	{
		// �ڰ�
		m_BlackBoard.serialize(data);
		// AI���
		m_AIComponent->serialize(data);
		// BUFF���
		m_BuffComponent->serialize(data);
		if (RT_BOSS != m_RoleType)
		{
			// �������
			m_SkillComponent->serialize(data);
		}
		// ��ɫ���
		m_RoleComponent->serialize(data);
	}

	SERIALIZE_MATCH(data);

	return true;
}

bool CRole::unserialize(CBufferData& data)
{
	UN_SERIALIZE_MATCH(data);

	if (!CGameObject::unserialize(data))
	{
		return false;
	}

	int iValue;
	float fValue;

	// Attribute - ��̬����(int)
	for (int h = EAttributeBegin + 1; h <= EAttributeBeBuffable; ++h)
	{
		data.readData(iValue);
		m_IntAttributes[h] = iValue;
	}

	// Attribute - ��̬����(float)
	for (int i = EAttributeYmin; i < EAttributeEnd; ++i)
	{
		data.readData(fValue);
		m_FloatAttributes[i] = fValue;
	}

	// Attribute - int ����
	for (int j = EAttributeIntBegin + 1; j < EAttributeIntEnd; ++j)
	{
		data.readData(iValue);
		m_IntAttributes[j] = iValue;
	}

	// Attribute - float ����
	for (int k = EAttributeFloatBegin + 1; k < EAttributeFloatEnd; ++k)
	{
		data.readData(fValue);
		m_FloatAttributes[k] = fValue;
	}

	float scale = m_FloatAttributes[EAttributeScale];
    modifyAttribute(EAttributeFireRange, floatToInt(getIntAttribute(EClassFireRange) * scale));
	const Role* role = m_pModel->getRoleComm();
	m_FireOffset = role->FireOffset * scale;
	m_FireOffset.x *= m_nDirection;
	m_HitOffset = role->HitOffset * scale;
	m_HitOffset.x *= m_nDirection;
	m_HeadOffset = role->HeadOffset * scale;
	m_HeadOffset.x *= m_nDirection;

	// Var ���� - int ����
	for (int m = EAttributeIntVarBegin + 1; m < EAttributeIntVarEnd; ++m)
	{
		data.readData(iValue);
		m_IntAttributes[m] = iValue;
	}

	// Var ���� - float ����
	for (int n = EAttributeFloatVarBegin + 1; n < EAttributeFloatVarEnd; ++n)
	{
		data.readData(fValue);
		m_FloatAttributes[n] = fValue;
	}

	// Stat���� (int)
	for (int x = EStatBegin + 1; x <= EStatAttack; ++x)
	{
		data.readData(iValue);
		m_IntAttributes[x] = iValue;
	}

	data.readData(m_RoleType);
	if (RT_HERO == m_RoleType)
	{
		for (int y = EHeroCrystalLevel; y <= EHeroMaxCrystal; ++y)
		{
			data.readData(iValue);
			m_IntAttributes[y] = iValue;
		}
		for (int z = EHeroCrystal; z <= EHeroCrystalSpeed; ++z)
		{
			data.readData(fValue);
			m_FloatAttributes[z] = fValue;
		}
	}

	if (!m_bSafeRemove)
	{
		// �ڰ�
		m_BlackBoard.unserialize(data);
		// AI���	
		m_AIComponent->unserialize(data);
		// BUFF���
		m_BuffComponent->unserialize(data);
		if (RT_BOSS != m_RoleType)
		{
			// �������
			m_SkillComponent->unserialize(data);
		}
		// ��ɫ���	
		m_RoleComponent->unserialize(data);
	}

#ifndef RunningInServer
	//Color3B color;
	//color.r = m_IntAttributes[EAttributeColorR];
	//color.g = m_IntAttributes[EAttributeColorG];
	//color.b = m_IntAttributes[EAttributeColorB];
	//getDisplayNode()->setColor(color);
#endif

	UN_SERIALIZE_MATCH(data);


	return true;
}
