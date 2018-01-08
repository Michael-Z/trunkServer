#include "ConfRole.h"

#ifndef RunningInServer
#include "Game.h"
#endif

#define MSTOS(ms)       (ms) / 1000.0f       //����ת����

bool StatusConfMap::LoadCSV(const std::string& str)
{
    CCsvLoader csvLoader;
    CHECK_RETURN(loadCsv(csvLoader, str.c_str(), 3));

    //��������� 
    while(csvLoader.NextLine())
    {
        StatusConfItem* cfg = new StatusConfItem();
        cfg->CanBreakParam = csvLoader.NextInt();
        csvLoader.NextStr();	//��һ�в���Ҫ����
        cfg->StateId = CConfManager::getInstance()->getStateID(csvLoader.NextStr());
        cfg->LifeTime = csvLoader.NextInt() * 1.0f / 1000;
        cfg->LifeTimePrarm = csvLoader.NextInt();
        cfg->Lock = csvLoader.NextInt() == 1;
        cfg->NextStateId = CConfManager::getInstance()->getStateID(csvLoader.NextStr());
        CConfAnalytic::ToFunction(csvLoader.NextStr(), &cfg->Action);
        cfg->AnimationTag = csvLoader.NextStr();
        cfg->AnimationLoop = csvLoader.NextInt();
        cfg->AnimationSpeed = csvLoader.NextFloat();
        cfg->SpeedAffect = static_cast<StatusSpeedAffect>(csvLoader.NextInt());
        cfg->AnimationAsh = csvLoader.NextInt() == 1;
        cfg->AnimationTransparency = csvLoader.NextFloat();
        cfg->AnimationFadeOut = csvLoader.NextInt() * 1.0f / 1000;
		cfg->hue = csvLoader.NextFloat();

        CConfAnalytic::ToJsonInt(csvLoader.NextStr(), cfg->EffectIds);
        cfg->EffectPlayType = static_cast<StatusEffPlayType>(csvLoader.NextInt());
        cfg->IsFollow = CConfAnalytic::ToBool(csvLoader.NextStr());
        cfg->UIEffectID = csvLoader.NextInt();
        CConfAnalytic::ToVecMusicInfos(csvLoader.NextStr(), cfg->MusicInfos);

        m_Datas[cfg->StateId] = cfg;
    }
    return true;
}

const StatusConfItem* StatusConfMap::getStateItem(int stateId) const
{
    std::map<int, void*>::const_iterator iter = m_Datas.find(stateId);
    if(iter == m_Datas.end())
    {
        return NULL;
    }
    return reinterpret_cast<const StatusConfItem*>(iter->second);
}

bool CConfStatus::LoadCSV(const std::string& str)
{
    CCsvLoader csvLoader;
    CHECK_RETURN(loadCsv(csvLoader, str.c_str(), 3));

    while (csvLoader.NextLine())
    {
        int stateId = csvLoader.NextInt();
        csvLoader.NextStr();
#ifdef RunningInServer
        std::string stateConf = "../config/Role/Status/" + csvLoader.NextStr();
		if (!LoadCSV(stateId, stateConf))
		{
			printf("load csv stateConf=%s error!\n", stateConf.c_str());
			return false;
		}
#else
        m_StatusFileMap[stateId] = csvLoader.NextStr();
#endif

    }

    return true;
}

bool CConfStatus::LoadCSV(int roleId, const std::string& str)
{
    StatusConfMap* map = new StatusConfMap();
    if (!map->LoadCSV(str))
    {
#ifndef RunningInServer
        cocos2d::MessageBox(("Load " + str + " Error!!").c_str(), "Load CSV Error");
#endif 
        delete map;
        return false;
    }

    m_Datas[roleId] = map;
    return true;
}

const StatusConfItem* CConfStatus::getStateItem(int roleId, int stateId)
{
    const StatusConfMap* map = getStateMap(roleId);
    if(!map)
    {
        return NULL; 
    }
    return map->getStateItem(stateId);
}

const StatusConfMap* CConfStatus::getStateMap(int roleId)
{
    std::map<int,void*>::iterator iter = m_Datas.find(roleId);
    if(iter == m_Datas.end())
    {
#ifndef RunningInServer
		std::string stateConf = "config/Role/Status/" + m_StatusFileMap[roleId];
#else
		std::string stateConf = "../config/Role/Status/" + m_StatusFileMap[roleId];
#endif
		// std::string stateConf = "config/Role/Status/" + toolToStr(roleId) + "_Status.csv";
		if (LoadCSV(roleId, stateConf))
		{
			return reinterpret_cast<StatusConfMap*>(m_Datas[roleId]);
		}
		return NULL;
    }
    return reinterpret_cast<StatusConfMap*>(iter->second);
}

AIConfMap::~AIConfMap()
{
    for (std::map<int, std::vector<AIConfItem*> >::iterator iter = m_AIDatas.begin();
        iter != m_AIDatas.end(); ++iter)
    {
        deleteAndClearVec<AIConfItem*>(iter->second);
    }
}

bool AIConfMap::LoadCSV(const std::string& str)
{
    CCsvLoader csvLoader;
    CHECK_RETURN(loadCsv(csvLoader, str.c_str(), 3));

    //��������� 
    while(csvLoader.NextLine())
    {
        AIConfItem* cfg = new AIConfItem();
        //cfg->Order = csvLoader.NextInt();
        csvLoader.NextStr();	//��һ�в���Ҫ����
        csvLoader.NextStr();	//��һ�в���Ҫ����
        cfg->NeedRoleStatus = CConfManager::getInstance()->getStateID(csvLoader.NextStr());

        for (int i = 0; i < 4; ++i)
        {
            CDataFunction* fun = NULL;
            CConfAnalytic::ToFunction(csvLoader.NextStr(), &fun);
            if (NULL != fun)
            {
                cfg->Condition.push_back(fun);
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            CDataFunction* fun = NULL;
            CConfAnalytic::ToFunction(csvLoader.NextStr(), &fun);
            if (NULL != fun)
            {
                cfg->Action.push_back(fun);
            }
        }

        m_AIDatas[cfg->NeedRoleStatus].push_back(cfg);	
    }
    return true;
}

const std::vector<AIConfItem*>* AIConfMap::getAIItems(int stateid)
{
    std::map<int, std::vector<AIConfItem*> >::iterator iter = m_AIDatas.find(stateid);
    if (iter == m_AIDatas.end())
    {
        return NULL;
    }
    return &(m_AIDatas[stateid]);
}

const std::map<int, std::vector<AIConfItem*> >& AIConfMap::getAIMap()
{
    return m_AIDatas;
}

bool CConfAI::LoadCSV(const std::string& str)
{
    CCsvLoader csvLoader;
    CHECK_RETURN(loadCsv(csvLoader, str.c_str(), 3));

    while (csvLoader.NextLine())
    {
        int aiId = csvLoader.NextInt();
#ifdef RunningInServer
        std::string aiConf = "../config/Role/AI/" + csvLoader.NextStr();
		if (!LoadCSV(aiId, aiConf))
		{
			return false;
		}
#else
        m_AIFileMap[aiId] = csvLoader.NextStr();
        //std::string aiConf = "config/Role/AI/" + csvLoader.NextStr();
#endif
    }

    return true;
}

bool CConfAI::LoadCSV(int roleid, const std::string& str)
{
    AIConfMap* map = new AIConfMap();
    if (!map->LoadCSV(str))
    {
#ifndef RunningInServer
        cocos2d::MessageBox(("Load " + str + " Error!!").c_str(), "Load CSV Error");
#endif 
        delete map;
        return false;
    }

    m_Datas[roleid] = map;
    return true;
}

const std::vector<AIConfItem*>* CConfAI::getAIItems(int roleId, int stateid)
{
    AIConfMap* map = (AIConfMap*)getAIMap(roleId);
    if(!map)
    {
        return NULL; 
    }

    return map->getAIItems(stateid);
}

const AIConfMap* CConfAI::getAIMap(int roleId)
{
    std::map<int,void*>::iterator iter = m_Datas.find(roleId);
    if (iter == m_Datas.end())
    {
#ifndef RunningInServer
		std::string aiConf = "config/Role/AI/" + m_AIFileMap[roleId];
#else
		std::string aiConf = "../config/Role/AI/" + m_AIFileMap[roleId];
#endif
        if (LoadCSV(roleId, aiConf))
        {
            return reinterpret_cast<AIConfMap*>(m_Datas[roleId]);
        }
        return NULL;
    }
    return reinterpret_cast<AIConfMap*>(iter->second);
}

bool CConfStrToID::LoadCSV(const std::string& str)
{
    CCsvLoader csvLoader;
    if(!csvLoader.LoadCSV(str.c_str()))
    {
        return false;
    }

    //��������� 
    while(csvLoader.NextLine())
    {
        std::string key = csvLoader.NextStr();
        int val = csvLoader.NextInt();
        m_mapConvert[key] = val;
    }
    return true;
}

int CConfStrToID::getIDByName(const std::string& str)
{
    std::map<std::string,int>::iterator iter = m_mapConvert.find(str);
    if (iter != m_mapConvert.end())
    {
        return iter->second;
    }
    else
    {
        if (!str.empty() && str != "0")
        {
            LOG("can't not find %s in NameToIdMap", str.c_str());
        }
        return -1;
    }
}

void CConfRole::LoadRoleLogic(CCsvLoader& csvLoader, Role &common, bool noCall)
{
    // ��ɫ����ID
    common.AnimationID = csvLoader.NextInt();
	// ����HSV
	CConfAnalytic::ToJsonFloat(csvLoader.NextStr(), common.AnimationHSV);
    // ��ɫ��С����
    common.Scale = csvLoader.NextFloat();
    // ��ɫ��Ч��С����ֵ
    common.EffectScale = csvLoader.NextFloat();
    // ��ɫ�ӵ������
    CConfAnalytic::ToVec2(csvLoader.NextStr(), common.FireOffset);
    // ��ɫͷ����Ч���ŵ�
    CConfAnalytic::ToVec2(csvLoader.NextStr(), common.HeadOffset);
    // ��ɫ������Ч���ŵ�
    CConfAnalytic::ToVec2(csvLoader.NextStr(), common.HitOffset);
    // ��ɫѪ�����ͺ�λ��
    CConfAnalytic::ToJsonInt(csvLoader.NextStr(), common.HPLine);
    for (int i = (int)common.HPLine.size(); i < 3; ++i)
    {
        common.HPLine.push_back(1);
    }
    // ��ɫ״̬��ID
    common.StatusID = csvLoader.NextInt();
    // ��ɫAI��ID
    common.AIID = csvLoader.NextInt();
    // ����8�� (ǰ���ĸ�������Ҫ��ʾ, ���Լ�ʹ�ǿ�ֵҲҪ����, ����ȷ���Ǽ�������)
    for (int i = 0; i < 4; ++i)
    {
        common.Skill.push_back(csvLoader.NextInt());
    }
    for (int i = 4; i < 12; ++i)
    {
        int value = csvLoader.NextInt();
        if (value != 0)
        {
            common.Skill.push_back(value);
        }
    }

    // ��Ƭ����
    common.Name = csvLoader.NextInt();
    // ��Ƭ����
    common.Desc = csvLoader.NextInt();

    if (noCall)
    {
        // ��Ƭ����
        common.Picture = csvLoader.NextStr();
        // ��Ƭͷ��
        common.HeadIcon = csvLoader.NextStr();

    }
    // ����
    common.Race = csvLoader.NextInt();
    // ְҵ
    common.Vocation = csvLoader.NextInt();
    // �Ա�
    common.Sex = csvLoader.NextInt();
    // ������ʽ
    common.AttackType = csvLoader.NextInt();
    if (noCall)
    {
        // ��ɫ��������
        common.AttackDistance = csvLoader.NextInt();
    }
}

void CConfRole::LoadRoleNumerial(CCsvLoader& csvLoader, Role &common)
{
    // ��ɫ��ս������Χ
    common.FireRange = csvLoader.NextInt();
    // ��ɫԶ�̹�����Χ
    common.FarFireRange = csvLoader.NextInt();
    // ��ɫ�ƶ��ٶ�
    common.Speed = csvLoader.NextInt();
    // ��ɫ����ֵ
    common.HP = csvLoader.NextInt();
    // ��ɫ�����ɳ�ֵ
    common.HPGrowUp = csvLoader.NextInt();
    // ��ɫ��������
    common.PAttack = csvLoader.NextInt();
    // ��ɫ���������ɳ�ֵ
    common.PAttackGrowUp = csvLoader.NextInt();
    // ħ��������
    common.MAttack = csvLoader.NextInt();
    // ħ���������ɳ�ֵ
    common.MAttackGrowUp = csvLoader.NextInt();
    // ��ɫ�������
    common.Defend = csvLoader.NextFloat();
    // ��ɫ������
    common.PGuard = csvLoader.NextInt();
    // ��ɫ�����׳ɳ�
    common.PGuardGrowUp = csvLoader.NextInt();
    // ��ɫħ������
    common.Resustance = csvLoader.NextFloat();
    // ħ������
    common.MGuard = csvLoader.NextInt();
    // ħ�����׳ɳ�
    common.MGuardGrowUp = csvLoader.NextInt();
    // ��ɫ����͸
    common.PPenetrate = csvLoader.NextInt();
    // ��ɫħ����͸
    common.MPenetrate = csvLoader.NextInt();
    // ��ɫ�����ٶ�
    common.AttackSpeed = csvLoader.NextInt();
    // ��ɫ��Ѫֵ
    common.Vampire = csvLoader.NextFloat();
    // ��ɫ����ֵ
    common.Rebound = csvLoader.NextFloat();
    // ��ɫ����ֵ
    common.Miss = csvLoader.NextFloat();
    // ��ɫ����ֵ
    common.Crit = csvLoader.NextFloat();
    // ��ɫ�����ɳ�
    common.CritGrowUp = csvLoader.NextFloat();
    // ��ɫ�����˺�
    common.CritDamage = csvLoader.NextFloat();
    // ��ɫŭ������ֵ
    common.Rage = csvLoader.NextInt();
    // ��ɫŭ���ظ�ֵ
    common.RageRecover = csvLoader.NextInt();
    // ��ɫħ������ֵ
    common.MP = csvLoader.NextInt();
    // ��ɫħ���ظ�
    common.MPRecover = csvLoader.NextInt();
    // ��ɫ����
    common.Mass = csvLoader.NextFloat();
    // ��ɫ����ֵ����
    common.Strong = csvLoader.NextInt();
    // ��ɫ����ֵ�ָ��ٶ�
    common.StrongRecover = csvLoader.NextInt();
    // ��ɫ���ֵ
    common.Haterd = csvLoader.NextInt();
}

bool CConfHero::LoadCSV(const std::string& str)
{
    std::string strLogicCsv = str;
    std::string strNumerialCsv = str;
    strLogicCsv.append(".csv");
    strNumerialCsv.append("_Num.csv");

    CCsvLoader Loader;
    CHECK_RETURN(loadCsv(Loader, strLogicCsv.c_str(), 3));

    //��������� 
    while (Loader.NextLine())
    {
        HeroConfItem* hero = new HeroConfItem;
        // ��ɫID
        hero->Common.ClassID = Loader.NextInt();
        Loader.NextStr();//��ע
        LoadRoleLogic(Loader, hero->Common);
        // ��Ҽ���3��
        hero->PlayerSkill.reserve(3);
        fillIntVec(3, Loader, hero->PlayerSkill);
        // Ӣ��ˮ����������
        hero->CrystalSpeedPrarm = Loader.NextFloat();
        // ��Ƭ�����ǿ
        hero->RaceEnhance.EnhanceType = Loader.NextInt();
        hero->RaceEnhance.ConsumeParam = Loader.NextFloat();
        hero->RaceEnhance.CDParam = CConfAnalytic::ToPercent(Loader.NextStr());
        // ְҵ��ǿ
        hero->VocationEnhance.EnhanceType = Loader.NextInt();
        hero->VocationEnhance.ConsumeParam = Loader.NextFloat();
        hero->VocationEnhance.CDParam = CConfAnalytic::ToPercent(Loader.NextStr());
        // �Ա��ǿ
        hero->SexEnhance.EnhanceType = Loader.NextInt();
        hero->SexEnhance.ConsumeParam = Loader.NextFloat();
        hero->SexEnhance.CDParam = CConfAnalytic::ToPercent(Loader.NextStr());
        // �������ͼ�ǿ
        hero->AttackTypeEnhance.EnhanceType = Loader.NextInt();
        hero->AttackTypeEnhance.ConsumeParam = Loader.NextFloat();
        hero->AttackTypeEnhance.CDParam = CConfAnalytic::ToPercent(Loader.NextStr());
        m_Datas[hero->Common.ClassID] = hero;
    }

    // �������
    Loader.Release();
    // �����ֵ����
    CHECK_RETURN(loadCsv(Loader, strNumerialCsv.c_str(), 3));

    while (Loader.NextLine())
    {
        int nRoleID = Loader.NextInt();
        Loader.NextStr();//��ע
        std::map<int, void*>::iterator iter = m_Datas.find(nRoleID);
        if (iter == m_Datas.end())
        {
            return false;
        }

        HeroConfItem* pHeroConfigItem = static_cast<HeroConfItem*>(iter->second);
        LoadRoleNumerial(Loader, pHeroConfigItem->Common);
    }

    return true;
}

bool CConfBoss::LoadCSV(const std::string& str)
{
    std::string strLogicCsv = str;
    std::string strNumerialCsv = str;
    strLogicCsv.append(".csv");
    strNumerialCsv.append("_Num.csv");

    CCsvLoader Loader;
    CHECK_RETURN(loadCsv(Loader, strLogicCsv.c_str(), 3));

    //��������� 
    while (Loader.NextLine())
    {
        BossConfItem* boss = new BossConfItem;
        boss->Common.ClassID = Loader.NextInt();
        Loader.NextStr();//��ע
        LoadRoleLogic(Loader, boss->Common);

        m_Datas[boss->Common.ClassID] = boss;
    }

    // �������
    Loader.Release();
    // �����ֵ����
    CHECK_RETURN(loadCsv(Loader, strNumerialCsv.c_str(), 3));

    while (Loader.NextLine())
    {
        int nRoleID = Loader.NextInt();
        Loader.NextStr();//��ע
        std::map<int, void*>::iterator iter = m_Datas.find(nRoleID);
        if (iter == m_Datas.end())
        {
            return false;
        }

        BossConfItem* pBossConfigItem = static_cast<BossConfItem*>(iter->second);
        LoadRoleNumerial(Loader, pBossConfigItem->Common);
    }

    return true;
}

bool CConfMonster::LoadCSV(const std::string& str)
{
    std::string strLogicCsv = str;
    std::string strNumerialCsv = str;
    strLogicCsv.append(".csv");
    strNumerialCsv.append("_Num.csv");

    CCsvLoader Loader;
    CHECK_RETURN(loadCsv(Loader, strLogicCsv.c_str(), 3));

    //��������� 
    while (Loader.NextLine())
    {
        MonsterConfItem* monster = new MonsterConfItem;
        monster->Common.ClassID = Loader.NextInt();
        Loader.NextStr();//��ע
        LoadRoleLogic(Loader, monster->Common);

        m_Datas[monster->Common.ClassID] = monster;
    }

    // �������
    Loader.Release();
    // �����ֵ����
    if (!loadCsv(Loader, strNumerialCsv.c_str(), 3))
    {
        return false;
    }

    while (Loader.NextLine())
    {
        int nRoleID = Loader.NextInt();
        Loader.NextStr();//��ע
        std::map<int, void*>::iterator iter = m_Datas.find(nRoleID);
        if (iter == m_Datas.end())
        {
            return false;
        }

        MonsterConfItem* pMonsterConfigItem = static_cast<MonsterConfItem*>(iter->second);
        LoadRoleNumerial(Loader, pMonsterConfigItem->Common);
    }

    return true;
}

bool CConfSoldier::LoadCSV(const std::string& str)
{
    std::string strLogicCsv = str;
    std::string strNumerialCsv = str;
    strLogicCsv.append(".csv");
    strNumerialCsv.append("_Num.csv");

    CCsvLoader Loader;
    CHECK_RETURN(loadCsv(Loader, strLogicCsv.c_str(), 3));

    //��������� 
    while (Loader.NextLine())
    {
        SoldierConfItem* soldier = new SoldierConfItem;
        soldier->Common.ClassID = Loader.NextInt();
        Loader.NextStr();//��ע
        soldier->Star = Loader.NextInt();
		soldier->Rare = Loader.NextInt();
        LoadRoleLogic(Loader, soldier->Common);
        // �Ƿ�Ψһ�ɷ�
        soldier->IsSingo = Loader.NextInt();

        // SoilderID + Star
        m_mapSodierItem[soldier->Common.ClassID][soldier->Star] = soldier;
    }

    // �������
    Loader.Release();
    // �����ֵ����
    CHECK_RETURN(loadCsv(Loader, strNumerialCsv.c_str(), 3));

    while (Loader.NextLine())
    {
        int nRoleID = Loader.NextInt();
        Loader.NextStr();//��ע
        int nRoleStar = Loader.NextInt();
        std::map<int, std::map<int, SoldierConfItem*> >::iterator iter = m_mapSodierItem.find(nRoleID);
        // 
        if (iter == m_mapSodierItem.end())
        {
            return false;
        }

        std::map<int, SoldierConfItem*>::iterator iterItem = iter->second.find(nRoleStar);
        if (iterItem == iter->second.end())
        {
            return false;
        }

        SoldierConfItem* pSoldierConfigItem = iterItem->second;
        LoadRoleNumerial(Loader, pSoldierConfigItem->Common);
        // �ɷ�ˮ������
        pSoldierConfigItem->Cost = Loader.NextInt();
        // �ɷ�CD
        pSoldierConfigItem->CD = MSTOS(Loader.NextInt());
    }

    return true;
}

bool CConfCall::LoadCSV(const std::string& str)
{
    std::string strLogicCsv = str;
    std::string strNumerialCsv = str;
    strLogicCsv.append(".csv");
    strNumerialCsv.append("_Num.csv");

    CCsvLoader Loader;
    CHECK_RETURN(loadCsv(Loader, strLogicCsv.c_str(), 3));

    //��������� 
    while (Loader.NextLine())
    {
        CallConfItem* call = new CallConfItem;
        call->Common.ClassID = Loader.NextInt();
        Loader.NextStr();//��ע
        LoadRoleLogic(Loader, call->Common, false);

        call->CardCurrentLevel = Loader.NextInt();
        call->RoleLifeTime = Loader.NextInt() * 1.0f / 1000.0f;
        call->RoleType = Loader.NextInt();
        call->RoleIdentity = Loader.NextInt();
        call->RoleMoveType = Loader.NextInt();
        call->RoleMoveDirection = Loader.NextInt();

        m_Datas[call->Common.ClassID] = call;
    }

    // �������
    Loader.Release();
    // �����ֵ����
    CHECK_RETURN(loadCsv(Loader, strNumerialCsv.c_str(), 3));

    while (Loader.NextLine())
    {
        int nRoleID = Loader.NextInt();
        Loader.NextStr();//��ע
        std::map<int, void*>::iterator iter = m_Datas.find(nRoleID);
        if (iter == m_Datas.end())
        {
            return false;
        }

        CallConfItem* pCallConfigItem = static_cast<CallConfItem*>(iter->second);
        LoadRoleNumerial(Loader, pCallConfigItem->Common);
    }

    return true;
}

bool CConfZoom::LoadCSV(const std::string& str)
{
    CCsvLoader Loader;
    CHECK_RETURN(loadCsv(Loader, str.c_str(), 3));

    while (Loader.NextLine())
    {
        ZoomItem *item = new ZoomItem;
        item->RoleID = Loader.NextInt();
        Loader.NextStr(); // ��ע

        item->ZoomNumber = Loader.NextFloat();
        CConfAnalytic::ToVec2(Loader.NextStr(), item->StandOffSet);
		item->HallZoom = Loader.NextFloat();
        item->Priority = Loader.NextInt();

        m_Datas[item->RoleID] = item;
    }

    return true;
}
