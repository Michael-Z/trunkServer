#include "TargetSearcher.h"
#include "Role.h"
#include "Hero.h"
#include "Bullet.h"

using namespace std;

bool CTargetSearcher::isTargetInRange(CRole *target, float minX, float maxX)
{
	return (target->getRealPositionX() < maxX && target->getRealPositionX() > minX);
    //bool result = (target->getRealPositionX() < maxX && target->getRealPositionX() > minX);
    //if (result)
    //{
    //    KXLOGBATTLE("isTargetInRange true targetObjId %d minx %f maxX %f", target->getObjectId(), minX, maxX);
    //}
    //return result;
}

bool CTargetSearcher::searchTargets(std::vector<CRole*> &objList, float minX, float maxX, int &minIdx, int &maxIdx, int dir)
{
    minIdx = -1;
    maxIdx = -1;

    if (Positive_Sequence == dir)
    {
        int size = objList.size();
        for (int i = 0; i < size; ++i)
		{
            //KXLOGBATTLE("searchTargets_1 realX %f minX %f maxX %f ObjId %d",
            //    objList[i]->getRealPositionX(), minX, maxX, objList[i]->getObjectId());
            if (objList[i]->getRealPositionX() > maxX)
            {
                return maxIdx != -1;
            }
            else
            {
                if (objList[i]->getRealPositionX() >= minX)
                {
                    if (minIdx == -1)
                    {
                        minIdx = i;
                    }
                    maxIdx = i;
                }
            }
        }
    }
    else
    {
        int i = objList.size() - 1;
        for (; i >=0; --i)
		{
            //KXLOGBATTLE("searchTargets_2 realX %f minX %f maxX %f ObjId %d",
            //    objList[i]->getRealPositionX(), minX, maxX, objList[i]->getObjectId());
            if (objList[i]->getRealPositionX() > maxX)
            {
                return minIdx != -1;
            }
            else
            {
                if (objList[i]->getRealPositionX() >= minX)
                {
                    if (maxIdx == -1)
                    {
                        maxIdx = i;
                    }
                    minIdx = i;
                }
            }
        }
    }

    //KXLOGBATTLE("searchTargets_3 listSize %d minX %f maxX %f minIdx %d maxIdx %d dir %d",
    //    objList.size(), minX, maxX, minIdx, maxIdx, dir);

    return minIdx != -1 && maxIdx != -1;
}

CRole *CTargetSearcher::getClosestTarget(std::vector<CRole*> &objList, CRole *origin)
{
    float dist = 0.0f;
    float minDist = -1.0f;
    CRole *closestTarget = NULL;

    for (std::vector<CRole*>::iterator iter = objList.begin();
        iter != objList.end(); ++iter)
    {
        if ((*iter) == origin)
        {
            continue;
        }

        dist = fabs(origin->getRealPositionX() - (*iter)->getRealPositionX());
        if (minDist <= 0.0f || minDist > dist)
        {
            closestTarget = *iter;
            minDist = dist;
        }
        else
        {
            return closestTarget;
        }
    }

    return NULL;
}

bool positiveSort(CRole* obj1, CRole* obj2)
{
    if (obj1->getRealPositionX() < obj2->getRealPositionX())
    {
        return true;
    }
    else if (obj1->getRealPositionX() == obj2->getRealPositionX() && 
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

bool invertedSort(CRole* obj1, CRole* obj2)
{
    if (obj1->getRealPositionX() > obj2->getRealPositionX())
    {
        return true;
    }
    else if (obj1->getRealPositionX() == obj2->getRealPositionX() &&
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

void CTargetSearcher::sortList(std::vector<CRole*> &objList, int sortorder)
{
    if (sortorder == Positive_Sequence)
    {
        std::sort(objList.begin(), objList.end(), positiveSort);
    }
    else
    {
        std::sort(objList.begin(), objList.end(), invertedSort);
    }
}

void makeMinMax(CGameObject* searcher, const SearchConfItem *conf, float &minX, float &maxX)
{
    CBattleHelper *helper = searcher->getBattleHelper();
    int camp = helper->getCampWithUid(searcher->getOwnerId());

    // ��Χ��ֵ
    float d = 0.0f;
    CRole *role = dynamic_cast<CRole *>(searcher);
    if (conf->RangeParam > 0)
    {
        d = static_cast<float>(conf->RangeParam);
    }
    else if (NULL != role)
    {
        camp = role->getIntAttribute(EAttributeCamp);
        if (conf->RangeParam == SearchRange_Far)
        {
            d = static_cast<float>(role->getIntAttribute(EAttributeFarFireRange));
        }
        else if(conf->RangeParam == SearchRange_Close)
        {
            d = static_cast<float>(role->getIntAttribute(EAttributeFireRange));
        }
    }

    if (camp != ECamp_Red)
    {
        maxX = searcher->getRealPositionX() + d;
        //  ��˫��
        if (conf->RangeType == SearchDirection_OneWay)
        {
            minX = searcher->getRealPositionX();
        }
        else
        {
            minX = searcher->getRealPositionX() - d;
        }
    }
    else
    {
        minX = searcher->getRealPositionX() - d;
        //  ��˫��
        if (conf->RangeType == SearchDirection_OneWay)
        {
            maxX = searcher->getRealPositionX();
        }
        else
        {
            maxX = searcher->getRealPositionX() + d;
        }
    }

    CBullet* bullet = dynamic_cast<CBullet*>(searcher);
    if (NULL != bullet)
    {
        // �ӵ��ƶ��ٶ�����
        float xOffset = bullet->getRealPositionX() - bullet->getPrevPosition().x;
        float xExtra = fabs(xOffset) - d;
        // ����ƶ��ļ������������Χ
        if (xExtra > 0.0f)
        {
            // �����ƶ�����0�������ƶ�С��0
            xOffset > 0.0f ? minX -= xExtra : maxX += xExtra;
        }
    }
}

// ����һ���б��У���ָ����Χ�ڣ������������������/�ҵĽ�ɫ
inline CRole* searchListFirstByXInRange(CGameObject* searcher, vector<CRole*>& soldiers, const SearchConfItem *conf, float& minX, float& maxX, int dir, bool byleft)
{
    int minIdx = 0, maxIdx = 0, step = 1;
    if (!CTargetSearcher::searchTargets(soldiers, minX, maxX, minIdx, maxIdx, dir))
    {
        return NULL;
    }

    // �ܹ�Ҫ�������ٸ�����Ϊ���±꣬����Ҫ +1
    int count = maxIdx - minIdx + 1;
    if (count < 1)
    {
        return NULL;
    }
    else if (count > 1)
    {
        // ������Ҫȷ��Inverted_Sequence��Positive_Sequence��Ӧ��˳��
        // ���Ҫ��X��С�ģ��������ǰ��մӴ�С����ģ�Ӧ�ôӺ�������
        if ((byleft && dir == Inverted_Sequence)
            // ���Ҫ��X���ģ��������ǰ��մ�С��������ģ�Ӧ�ôӺ�������
            || (!byleft && dir == Positive_Sequence))
        {
            // ������ң����������ֻ�õ���minIdx���Ǵ�minIdx����stepƫ�ƽ�������
            // ���������������maxIdx��
            minIdx = maxIdx;
            step = -1;
        }
    }

    // ����count��
    for (int i = 0; i < count; ++i, minIdx += step)
    {
        CRole* role = soldiers[minIdx];
        if (CSearchCondition::checkConditionAll(role, conf))
        {
            // �������������һ���ж��Ƿ���Ҫȥ���Լ�
            if (conf->Self && searcher == role)
                continue;
            // �ҵ���һ��Ŀ�꣬����������Χ����С��
            byleft ? maxX = role->getRealPositionX() :
                minX = role->getRealPositionX();
            return role;
        }
    }
    return NULL;
}

// ��ȡ�����ߵ���Ӫ
inline int getCamp(CGameObject* searcher)
{
    CRole* role = dynamic_cast<CRole*>(searcher);
    if (NULL != role)
    {
        return role->getIntAttribute(EAttributeCamp);
    }

    CBullet* bullet = dynamic_cast<CBullet*>(searcher);
    if (NULL != bullet)
    {
        return bullet->getOwner()->getIntAttribute(EAttributeCamp);
    }

    if (searcher->getOwnerId() == searcher->getBattleHelper()->getMasterId())
    {
        return ECamp_Blue;
    }

    return ECamp_Red;
}

inline int getSequence(int camp)
{
    switch (camp)
    {
    case ECamp_Neutral:
        return Inverted_Sequence;
    case ECamp_Blue:
        return Positive_Sequence;
    case ECamp_Red:
        return Inverted_Sequence;;
    default:
        break;
    }
    return Positive_Sequence;
}

static bool s_isSearchCampInit = false;
static vector<int> s_allCamp;
static vector<int> s_emptyCamp;
static vector<int> s_blueFriend;
static vector<int> s_blueEnemy;
static vector<int> s_redFriend;
static vector<int> s_redEnemy;
static vector<int> s_neutralFriend;
static vector<int> s_neutralEnemy;

inline const vector<int>& getCamps(CGameObject* searcher, int searchType)
{
    if (!s_isSearchCampInit)
    {
        s_isSearchCampInit = true;
		s_allCamp.push_back(ECamp_Blue);
		s_allCamp.push_back(ECamp_Red);
		s_allCamp.push_back(ECamp_Neutral);
		s_blueFriend.push_back(ECamp_Blue);
		s_blueEnemy.push_back(ECamp_Red);
		s_blueEnemy.push_back(ECamp_Neutral);
		s_redFriend.push_back(ECamp_Red);
		s_redEnemy.push_back(ECamp_Blue);
		s_redEnemy.push_back(ECamp_Neutral);
		s_neutralFriend.push_back(ECamp_Neutral);
		s_neutralEnemy.push_back(ECamp_Blue);
		s_neutralEnemy.push_back(ECamp_Red);
    }

    switch (searchType)
    {
    case SearchListType_Friend:
    case SearchListType_NeedTreastFriend:
        switch (getCamp(searcher))
        {
        case ECamp_Blue:
            return s_blueFriend;
        case ECamp_Red:
            return s_redFriend;
        case ECamp_Neutral:
            return s_neutralFriend;
        }
    case SearchListType_Enemy:
    case SearchListType_FirstEnemy:
        switch (getCamp(searcher))
        {
        case ECamp_Blue:
            return s_blueEnemy;
        case ECamp_Red:
            return s_redEnemy;
        case ECamp_Neutral:
            return s_neutralEnemy;
        }
    case SearchListType_All:
    case SearchListType_NeedTreastAll:
        return s_allCamp;
    }
    return s_emptyCamp;
}

// ����ȫ����Χ�ڣ����������ĵ�һ����ɫ
inline CRole* searchFirstByXAllRange(CGameObject* searcher, const SearchConfItem *conf, std::vector<CRole *> *targets)
{        
    CBattleHelper *helper = searcher->getBattleHelper();
    CRole* target = NULL;
    CRole* role = NULL;
    float x = 0.0f;
    int camp = getCamp(searcher);
    bool byleft = camp != ECamp_Red;
    bool returnOnce = targets == NULL;
    if (byleft)
    {
        x = helper->MaxX;
    }

    const vector<int>& camps = getCamps(searcher, conf->ListType);
    for (unsigned int i = 0; i < camps.size(); ++i)
    {
        // ������Ӫ���ٻ�ʦ
        role = helper->getMainRole(camps[i]);
        // ����ȵ�ǰ��λ���Ľ�ɫ�����������һ���ж������Ƿ�����
        if (role != NULL
            && (byleft && role->getRealPositionX() < x
            || !byleft && role->getRealPositionX() > x))
        {
            if (CSearchCondition::checkConditionAll(role, conf)
                // �������������һ���ж��Ƿ���Ҫȥ���Լ�
                && !(conf->Self && searcher == role))
            {
                // �ҵ���һ��Ŀ�꣬����������Χ����С��
                if (returnOnce) return role;
                targets->push_back(role);
                x = role->getRealPositionX();
                target = role;
            }
        }

        // ������Ӫ��ʿ���б�
        vector<CRole*> soliders = helper->getRoleWithCamp(camps[i]);
        int beginIdx = 0, step = 1;
        // ����Ƿ���ģ�Ӧ�ô����һ������
        if (getSequence(camp) == Inverted_Sequence)
        {
            beginIdx = soliders.size() - 1;
            step = -1;
        }

        for (unsigned int j = 0; j < soliders.size(); ++j)
        {
            role = soliders[beginIdx];
            // �ж����Ե�ķ���������
            if ((byleft && role->getRealPositionX() < x
                || !byleft && role->getRealPositionX() > x))
            {
                if (CSearchCondition::checkConditionAll(role, conf)
                    // �������������һ���ж��Ƿ���Ҫȥ���Լ�
                    && !(conf->Self && searcher == role))
                {
                    if (returnOnce) return role;
                    targets->push_back(role);
                    // �ҵ���һ��Ŀ�꣬����������Χ����С��������Ĳ��ÿ���
                    x = role->getRealPositionX();
                    target = role;
                    break;
                }
                // ���һ����һ���Ƿ��������
                beginIdx += step;
            }
            // ���Ե�Ķ������ϣ������Ĳ��ÿ���
            else
            {
                break;
            }
        }
    }
    return target;
}

// ����ָ����Χ�ڣ����������ĵ�һ����ɫ
inline CRole* searchFirstByXInRange(CGameObject* searcher, const SearchConfItem *conf, std::vector<CRole *> *targets, float minX, float maxX)
{
    CBattleHelper *helper = searcher->getBattleHelper();
    CRole* target = NULL;
    CRole* role = NULL;
    int camp = getCamp(searcher);
    bool byleft = camp != ECamp_Red;
    bool returnOnce = targets == NULL;

    const vector<int>& camps = getCamps(searcher, conf->ListType);
    for (unsigned int i = 0; i < camps.size(); ++i)
    {
        // ������Ӫ���ٻ�ʦ
        role = helper->getMainRole(camps[i]);
        if (role != NULL
            // �����ж��Ƿ��ڷ�Χ��
            && CTargetSearcher::isTargetInRange(role, minX, maxX)
            // ����ڷ�Χ�����ж��Ƿ��������
            && CSearchCondition::checkConditionAll(role, conf)
            // �������������һ���ж��Ƿ���Ҫȥ���Լ�
            && !(conf->Self && searcher == role))
        {
            if (returnOnce) return role;
            targets->push_back(role);
            // �ҵ���һ��Ŀ�꣬����������Χ����С��
            // �����������ߵģ�����maxX����������minX
            byleft ? maxX = role->getRealPositionX() 
                : minX = role->getRealPositionX();
            target = role;
        }

        // ������Ӫ��ʿ���б�
        vector<CRole*> soliders = helper->getRoleWithCamp(camps[i]);
        // �ҵ��б������ָ����Ե��һ����ɫ��ͬʱ����������Χ
        role = searchListFirstByXInRange(searcher, soliders, conf, minX, maxX, getSequence(camps[i]), byleft);
        if (role != NULL)
        {
            if (returnOnce) return role;
            targets->push_back(role);
            target = role;
        }
    }
    return target;
}

// �������������Ľ�ɫ
inline bool searchAll(CGameObject* searcher, const SearchConfItem *conf, std::vector<CRole *> *targets)
{
    CBattleHelper *helper = searcher->getBattleHelper();
    CRole* role = NULL;
    int minIdx = 0;
    int maxIdx = 0;
    float minX = 0.0f;
    float maxX = 0.0f;
    bool allRange = conf->RangeParam == SearchRange_All;
    bool returnOnce = targets == NULL;

    if (!allRange)
    {
        makeMinMax(searcher, conf, minX, maxX);
    }

    const vector<int>& camps = getCamps(searcher, conf->ListType);
    for (unsigned int i = 0; i < camps.size(); ++i)
    {
        role = helper->getMainRole(camps[i]);
        if (role != NULL
            && (allRange || CTargetSearcher::isTargetInRange(role, minX, maxX))
            && (!conf->Self || role != searcher)
            && CSearchCondition::checkConditionAllAndPush(role, conf, targets)
            && returnOnce)
        {
            return true;
        }

        // ������Ӫ��ʿ���б�
        vector<CRole*> soliders = helper->getRoleWithCamp(camps[i]);
        if (allRange)
        {
            minIdx = 0;
            maxIdx = soliders.size() - 1;
        }
        else if (!CTargetSearcher::searchTargets(soliders, minX, maxX, minIdx, maxIdx, getSequence(camps[i])))
        {
            //KXLOGBATTLE("searchTargetsWithId_4 searchid %d searcherObjId %d bRetAtOnce %d soldiersSize %d",
            //    searchid, searcher->getObjectId(), bRetAtOnce, soldiers.size());
            continue;
        }

        for (int i = minIdx; i <= maxIdx; ++i)
        {
            if ((!conf->Self || soliders[i] != searcher)
                && CSearchCondition::checkConditionAllAndPush(soliders[i], conf, targets)
                && returnOnce)
            {
                //KXLOGBATTLE("searchTargetsWithId_5 searchid %d searcherObjId %d soldierObjId %d",
                //    searchid, searcher->getObjectId(), soldiers[i]->getObjectId());
                return true;
            }
        }
    }

    if (NULL != targets && targets->size() > 0)
    {
        // 4. ������Ƹ���, ����б�Ϊ0��Ҳ����false
        return CSearchCondition::checkLimitMaxAndSort(*targets, searcher, conf->Num, conf->Reorder);
    }

    return false;
}

bool CTargetSearcher::searchTargetsWithId(int searchid, CGameObject* searcher, std::vector<CRole *> *targets)
{
    if (searcher == NULL)
    {
        return false;
    }
    CBattleHelper* helper = searcher->getBattleHelper();
    int camp = getCamp(searcher);
    const SearchConfItem *conf = queryConfSearch(searchid);
    if (NULL == conf)
    {
        KXLOGBATTLE("searchTargetsWithId_1 not find seachId %d", searchid);
        return false;
    }

    switch (conf->ListType)
    {
    case SearchListType_Self:
        if (dynamic_cast<CRole*>(searcher) != NULL)
        {
            return CSearchCondition::checkConditionAllAndPush(
                dynamic_cast<CRole*>(searcher), conf, targets);
        }
    case SearchListType_EnemySummoner:
        return CSearchCondition::checkConditionAllAndPush(
            helper->getMainRole(helper->getEnmeyCamp(camp)), conf, targets);
    case SearchListType_FriendSummoner:
        return CSearchCondition::checkConditionAllAndPush(
            helper->getMainRole(camp), conf, targets);
    case SearchListType_FirstEnemy:
        if (conf->RangeParam == SearchRange_All)
        {
            return searchFirstByXAllRange(searcher, conf, targets) != NULL;
        }
        else
        {
            float minX = 0.0f;
            float maxX = 0.0f;
            makeMinMax(searcher, conf, minX, maxX);
            return searchFirstByXInRange(searcher, conf, targets, minX, maxX) != NULL;
        }
    case SearchListType_NeedTreastAll:
        if (helper->getTreatCount(-1) == 0)
        {
            return false;
        }
    case SearchListType_NeedTreastFriend:
        if (helper->getTreatCount(getCamp(searcher)) == 0)
        {
            return false;
        }
    }

    // ǰ�������һЩ����ԵĿ���������ʣ�µĵ��浶��ǹ����
    return searchAll(searcher, conf, targets);
}

bool CSearchCondition::checkConditionAllAndPush(CRole *target, const SearchConfItem *conf, std::vector<CRole *> *targets)
{
    if (checkConditionAll(target, conf))
    {
        if (targets != NULL)
        {
            targets->push_back(target);
        }
        return true;
    }
    return false;
}

bool CSearchCondition::checkConditionAll(CRole *target, const SearchConfItem *conf)
{
    do
    {
        // ture, falseд�����������׿�
        // ���ò������������������
        if (!checkDeath(target, conf->Death))
            break;

        for (unsigned int i = 0; i < conf->Conditions.size(); ++i)
        {
            switch (conf->Conditions[i])
            {
            case ConditionIdentity:
                if (!checkIdentify(target, conf->Identity))
                {
                    return false;
                }
                break;
            case ConditionHP:
                if (!checkBlood(target, conf->HP[0] > 0.0000f ? true : false, conf->HP[1]))
                {
                    return false;
                }
                break;
            case ConditionCareer:
                if (!checkVocation(target, conf->Career[0] > 0 ? true : false, conf->Career))
                {
                    return false;
                }
                break;
            case ConditionSex:
                if (!checkSex(target, conf->Sex[0] > 0 ? true : false, conf->Sex[1]))
                {
                    return false;
                }
                break;
            case ConditionAttackType:
                if (!checkFireRange(target, conf->AttackType))
                {
                    return false;
                }
                break;
            case ConditionRace:
                if (!checkRace(target, conf->Race[0] > 0 ? true : false, conf->Race[1]))
                {
                    return false;
                }
                break;
            case ConditionStar:
                if (!checkStarLevel(target, conf->Star[0] > 0 ? true : false, conf->Star[1]))
                {
                    return false;
                }
                break;
            case ConditionBuff:
                if (!checkBuff(target, conf->Buff[0] > 0 ? true : false, conf->Buff))
                {
                    return false;
                }
                break;
            case ConditionType:
                if (!checkType(target, conf->Type))
                {
                    return false;
                }
                break;
            case ConditionRoleId:
                if (!checkRoleId(target, conf->RoleID[0] > 0 ? true : false, conf->RoleID))
                {
                    return false;
                }
                break;
            case ConditionState:
                if (!checkIsInState(target, conf->State[0] > 0 ? true : false, conf->State[1]))
                {
                    return false;
                }
                break;
            default:
                break;
            }
        }
        return true;
    } while (false);
    return false;
}

bool CSearchCondition::checkDeath(CRole *target, bool canSearch)
{
    if (target->currentState() == State_Death 
        || target->currentState() == State_None)
    {
        return canSearch;
    }

    return true;
}

bool CSearchCondition::checkIdentify(CRole *target, const std::vector<int> &identify)
{
	if (identify[0] == 0)
	{
		for (unsigned int i = 1; i < identify.size(); ++i)
		{
			// ���Ϊ����һ��
			if (target->getRoleType() == identify[i])
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for (unsigned int i = 1; i < identify.size(); ++i)
		{
			// ������Ϊ����һ��
			if (target->getRoleType() == identify[i])
			{
				return false;
			}
		}
		return true;
	}
    return true;
}

bool CSearchCondition::checkBlood(CRole *target, bool ng, float percent)
{
    float bloodPerc = target->getIntAttribute(EAttributeHP) * 1.0f / target->getIntAttribute(EAttributeMaxHP);
    bool ret = ((bloodPerc * 100) < percent);
    return ng ? !ret : ret;
}

bool CSearchCondition::checkVocation(CRole *target, bool ng, const std::vector<int> &vocations)
{
    bool ret = false;
    for (unsigned int i = 1; i < vocations.size(); ++i)
    {
        if (target->getIntAttribute(ECardVocation) == vocations[i])
        {
            ret = true;
            break;
        }
    }
    return ng ? !ret : ret;
}

bool CSearchCondition::checkSex(CRole *target, bool ng, int sex)
{
    bool ret = (target->getIntAttribute(ECardSex) == sex);
    return ng ? !ret : ret;
}

bool CSearchCondition::checkFireRange(CRole * target, int rangeType)
{
    return target->getIntAttribute(ECardAttackType) == rangeType;
}

bool CSearchCondition::checkRace(CRole *target, bool ng, int race)
{
    bool ret = (target->getIntAttribute(ECardRace) == race);
    return ng ? !ret : ret;
}

bool CSearchCondition::checkStarLevel(CRole *target, bool ng, int starLevel)
{
    bool ret = (target->getIntAttribute(ECardLevel) > starLevel);
    return ng ? !ret : ret;
}

bool CSearchCondition::checkType(CRole *target, int type)
{
    bool ret = true;
    switch (type)
    {
    case SearchType_Aoeable:
        ret = target->getIntAttribute(EAttributeBeAoeable) > 0;
        break;
    case SearchType_Buffable:
        ret = target->getIntAttribute(EAttributeBeBuffable) > 0;
        break;
    case SearchType_Attackable:
        ret = target->getIntAttribute(EAttributeBeAttackable) > 0;
        break;
    default:
        break;
    }
    return ret;
}

bool CSearchCondition::checkBuff(CRole *target, bool ng, const std::vector<int> &buffIds)
{
    bool ret = true;
    for (unsigned int i = 1; i < buffIds.size(); ++i)
    {
        if (!target->getBuffComponent()->hasBuff(buffIds[i]))
        {
            ret = false;
            break;
        }
    }
    return ng ? !ret : ret;
}

bool CSearchCondition::checkRoleId(CRole *target, bool ng, const std::vector<int> &roleIds)
{
    bool ret = false;
    const CRoleModel* model = target->getRoleModel();
    if (model != NULL)
    {
        int roleId = model->getRoleComm()->ClassID;
        for (unsigned int i = 1; i < roleIds.size(); ++i)
        {
            if (roleId == roleIds[i])
            {
                ret = true;
                break;
            }
        }
    }

    return ng ? !ret : ret;
}

bool CSearchCondition::checkIsInState(CRole *target, bool ng, int state)
{
    bool ret = (target->currentState() == state);
    return ng ? !ret : ret;
}

bool CSearchCondition::checkCutMe(std::vector<CRole *> &targets, CGameObject *searcher)
{
    for (std::vector<CRole *>::iterator iter = targets.begin();
        iter != targets.end(); ++iter)
    {
        if (*iter == searcher)
        {
            targets.erase(iter);
            return true;
        }
    }
    return true;
}

bool sortBlood(CRole * obj1, CRole * obj2)
{
    if (obj1->getIntAttribute(EAttributeHP) < obj2->getIntAttribute(EAttributeHP))
    {
        return true;
    }
    else if (obj1->getIntAttribute(EAttributeHP) == obj2->getIntAttribute(EAttributeHP) &&
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

bool sortLeftX(CRole * obj1, CRole * obj2)
{
    if (obj1->getRealPositionX() < obj2->getRealPositionX())
    {
        return true;
    }
    else if (obj1->getRealPositionX() == obj2->getRealPositionX() &&
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

bool sortRightX(CRole * obj1, CRole * obj2)
{
    if (obj1->getRealPositionX() > obj2->getRealPositionX())
    {
        return true;
    }
    else if (obj1->getRealPositionX() == obj2->getRealPositionX() &&
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

bool sortBigHatred(CRole * obj1, CRole * obj2)
{
    if (obj1->getIntAttribute(EAttributeHaterd) > obj2->getIntAttribute(EAttributeHaterd))
    {
        return true;
    }
    else if (obj1->getIntAttribute(EAttributeHaterd) == obj2->getIntAttribute(EAttributeHaterd) &&
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

bool sortSmallHatred(CRole * obj1, CRole * obj2)
{
    if (obj1->getIntAttribute(EAttributeHaterd) < obj2->getIntAttribute(EAttributeHaterd))
    {
        return true;
    }
    else if (obj1->getIntAttribute(EAttributeHaterd) == obj2->getIntAttribute(EAttributeHaterd) &&
        obj1->getObjectId() < obj2->getObjectId())
    {
        return true;
    }
    return false;
}

bool CSearchCondition::checkLimitMaxAndSort(std::vector<CRole *> &targets, CGameObject *searcher, int max, int sortOrder)
{     
    if (targets.size() == 0)
    {
        return false;
    }

    // ��Ŀ���б�С�������������ʱ������Ҫ����������Ϊ���ᷢ���޳�
    if (targets.size() > 1 && static_cast<int>(targets.size()) < max)
    {
        switch (sortOrder)
        {
        case SortOrder_Blood:
            std::sort(targets.begin(), targets.end(), sortBlood);
            break;
        case SortOrder_X:
            if (searcher->getOwnerId() == searcher->getBattleHelper()->getMasterId())
            {
                // ����ߵ�
                std::sort(targets.begin(), targets.end(), sortLeftX);
            }
            else
            {
                // ���ұߵ�
                std::sort(targets.begin(), targets.end(), sortRightX);
            }
            break;
        case SortOrder_BigHatred:
            std::sort(targets.begin(), targets.end(), sortBigHatred);
            break;
        case SortOrder_SmallHatred:
            std::sort(targets.begin(), targets.end(), sortSmallHatred);
            break;
        case SortOrder_RND:
            {
                // ���ȥ��Ŀ��
                CRandomNumber &rnd = targets[0]->getBattleHelper()->RandNum;
                while ((int)targets.size() > max)
                {
                    int i = rnd.random(0, targets.size());
                    targets.erase(targets.begin() + i);
				}
                //KXLOGBATTLE("checkLimitMaxAndSort_1 targetsSize %d searcherObjId %d, max %d, sortOrder %d",
                //    targets.size(), searcher->getObjectId(), max, sortOrder);
                return true;
            }
        default:
            break;
        }
    }

	// ��������, �����ں���Ķ����ȥ��
    while ((int)targets.size() > max)
    {
        targets.erase(--targets.end());
	}
    //KXLOGBATTLE("checkLimitMaxAndSort_2 targetsSize %d searcherObjId %d, max %d, sortOrder %d",
    //    targets.size(), searcher->getObjectId(), max, sortOrder);
    return true;
}
