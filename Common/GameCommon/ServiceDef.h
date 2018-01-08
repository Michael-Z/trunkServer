#ifndef __SERVICE_DEF_H__
#define __SERVICE_DEF_H__

#include "KxPlatform.h"
#include <vector>
#if KX_TARGET_PLATFORM == KX_PLATFORM_WIN32
#include <ctime>
#else
#include<time.h>
#include<stdlib.h>
#endif

// �����������,���صڼ�����(0��ʼ)
inline int randRateIndex(const std::vector<int> &vec)
{
    int nRegion = -1;
    if (!vec.empty())
    {
        int nTotal = 0;
        // [a, b, c...]->[a, a+b, a+b+c,...]
        std::vector<int> vecPro;
        std::vector<int>::const_iterator iter = vec.begin();
        for (; iter != vec.end(); ++iter)
        {
            nTotal += *iter;
            vecPro.push_back(nTotal);
        }

        if (nTotal > 0)
        {
            int nRand = rand() % nTotal;

            for (nRegion = 0; nRegion < int(vecPro.size()); ++nRegion)
            {
                if (nRand < vecPro[nRegion])
                {
                    break;
                }
            }
        }
    }

    return nRegion;
}

// ���ص����ڼ�����ʱ��ʱ���, ����0������һ�ζ���ʱ��ʱ���
inline int calcWDayTimeStamp(int hour, int wday = 0)
{
    if (wday < 0 || hour < 0)
    {
        return 0;
    }

    time_t cur = time(NULL);
    tm curTm;
    
    #if KX_TARGET_PLATFORM == KX_PLATFORM_WIN32
    gmtime_s(&curTm, &cur);
		#else
    gmtime_r(&cur,&curTm);
    #endif

    if (wday > 7)
        wday %= 7;

    if (hour > 24)
        hour %= 24;

    int nVal = (int)cur;
    int nHour = (hour - (curTm.tm_hour + 8) % 24) * 3600 - (curTm.tm_min * 60 + curTm.tm_sec);
    if (0 == wday)
    {
        nVal += (nHour > 0 ? nHour : nHour + 24 * 3600);
    }
    else if (wday >= curTm.tm_wday)
    {
        int nday = (wday - curTm.tm_wday) * 24 * 3600 + nHour;
        nVal += (nday < 0 ? nday + 7 * 24 * 3600 : nday);
    }
    else
    {
        nVal += (wday - curTm.tm_wday + 7) * 24 * 3600 + nHour;
    }

    return nVal;
}

enum ESkillIndex
{
    SKILL_0,                              // 
    SKILL_1,                              // ����1
    SKILL_2,                              // ����2
    SKILL_3,                              // ����3
    SKILL_4,                              // ����4
    SKILL_5,                              // ����5
    SKILL_6,                              // ����6
    SKILL_7,                              // ����7
    SKILL_8,                              // ����8
};

enum EItemType
{
    ITEM_NON,                             // 
    ITEM_EQUIP,                           // װ��
    ITEM_EQUIPMATERIAL,                   // װ������
    ITEM_HERO,                            // Ӣ�ۿ�
    ITEM_SUMMONER,                        // �ٻ�ʦ
    ITEM_EXPCARD,                         // ���鿨
    ITEM_SKILLBOOK,                       // ������
    ITEM_GOLDBAG,                         // ��Ҵ���
    ITEM_ENERGYGBA,                       // ��������
    ITEM_EXPBAG,                          // �������
    ITEM_DIAMONDBAG,                      // ��ʯ����
    ITEM_BOX,                             // ����
    ITEM_UPSTARITEM,                      // ���ǵ���

    ITEM_CONSUMABLE,                      // ����Ʒ
};

enum EItemEffectType
{
    ITEM_EFFECT_NON,                      // 
    ITEM_EFFECT_ADDGOLD,                  // �ӽ��
    ITEM_EFFECT_ADDRANDGOLD,              // ����ӽ��
    ITEM_EFFECT_ADDEXP,                   // �Ӿ���
    ITEM_EFFECT_ADDENERGY,                // ������
    ITEM_EFFECT_ADDDIAMOND,               // ����ʯ
    ITEM_EFFECT_ADDITEM,                  // �ӵ���
    ITEM_EFFECT_ADDHEROCARD,              // ����ٻ�ʦ��Ƭ,ע:Ӣ������ʱΪ���ĵĵ���
    ITEM_EFFECT_ADDSOLDIERCARD,           // ���Ӣ�ۿ�Ƭ,ע:Ӣ������ʱΪ���ĵĵ���
    ITEM_EFFECT_ADDEXPTOCARD = 100,       // ����Ƭ�Ӿ���
    ITEM_EFFECT_SKILLLVUP,                // ��������
};

enum EItemUseType
{
    ITEM_USE_NON,                         // ����ʹ��
    ITEM_USE_TOBAG,                       // ����ʹ��
    ITEM_USE_IMMEDIDATE,                  // ����ʹ��
    ITEM_USE_TOCARD,                      // ��Ƭ����ʹ��
    ITEM_USE_TOEQUIP,                     // װ���ϳ�ʹ��
};

enum EExpMultiple
{
    EXP_MULTIPLE_NORMAL,                   // ��ͨ�ɹ�(1��)
    EXP_MULTIPLE_SUCCESS,                  // �ɹ�(1.2��)
    EXP_MULTIPLE_BIGSUCCESS,               // ��ɹ�(1.5��)
    EXP_MULTIPLE_HUGESUCCESS,              // ����ɹ�(2��)
};

enum ELock
{
    EL_UNLOCK,                             // ����
    EL_LOCK,                               // ����
};

enum ECostType
{
    COST_FREE,                             // ���
    COST_OUTLAY,                           // ֧��
};

// ���ñ��������
enum EParamCount
{
    EPARAM_COUNT_NON,          // �޲���
    EPARAM_COUNT_ONE,          // 1������
    EPARAM_COUNT_TWO,          // 2������
    EPARAM_COUNT_THREE,        // 3������
};

// ������ʾ(�������񡢳ɾ�)
enum ETaskDisplayType
{
    TASK_HIDE,                      // ����
    TASK_DISPLAY,                   // ��ʾ
};

// ����ְλ
enum EUnionPosition
{
    UNION_POS_NON = -1,
    UNION_POS_NORMAL,                // ��ͨ��Ա
    UNION_POS_VICE_CHAIRMAN,         // ���᳤
    UNION_POS_CHAIRMAN,              // �᳤
};

// ���Ṧ������
enum EUnionFuncType
{
    UNION_FUNC_JOIN,                  // ����
    UNION_FUNC_EXIT,                  // �˳�
    UNION_FUNC_KICK,                  // �߳�
    UNION_FUNC_APPOINT,               // ����
    UNION_FUNC_ASSIGN,                // ������Ʒ
    UNION_FUNC_TRANSFER,              // Ȩ���ƽ�
    UNION_FUNC_RELIEVE,               // ����
    UNION_FUNC_RESIGN,                // ��ְ
    UNION_FUNC_DISMISS,               // ��ɢ
};

// ������ش�����
enum EUnionErrorCode
{
    UNION_EC_FAILD,
    UNION_EC_SUCCESS,
    UNION_EC_AUDIT_OVERTIME,           // ���ʱ���ѹ�
    UNION_EC_OWN_UNION,                // �Ѿ�ӵ�й���
    UNION_EC_NOENOUGH_LV,              // �ȼ�����
    UNION_EC_NOENOUGH_MONEY,           // ���ò���
    UNION_EC_MEMBER_FULL,              // ������Ա
    UNION_EC_APPLY_TIMESTAMP,          // ����(����)������ȴʱ��û��
    UNION_EC_NAME_LEGAL,               // ���������Ϸ�(Ϊ��)
    UNION_EC_NAME_REPEAT,              // �������ظ�   
    UNION_EC_UNION_NOEXIST,            // ���᲻����
    UNION_EC_APPLY_SAME,               // �ظ����빫��
    UNION_EC_NOENOUGH_APPLYCOUNT,      // �����������
    UNION_EC_NO_AUDIT_POWER,           // �����Ȩ��
    UNION_EC_AUTO_AUDIT,               // �Զ�ͨ�����
    UNION_EC_PASS_AUDIT,               // ͨ�����
    UNION_EC_REFUSE_AUDIT,             // �ܾ�ͨ�����
    UNION_EC_VICECHAIR_FULL,           // ���᳤����
    UNION_EC_ALREADY_DO,               // �Ѿ�������
};

// �����Ϣ,�Ƿ�ͬ������
enum EUnionAuditApply
{
    UNION_APPLY_REFUSE,                // �ܾ�
    UNION_APPLY_AGREE,                 // ͬ��
};

// ������������
enum ESearchUnionType
{
    SEARCH_UNION_RANK,                 // ��������(�ȼ�>��Ա����>ID)
    SEARCH_UNION_NO_FULL,              // δ��Ա���� 
};

// ���ḣ������
enum EUnionWelfareType
{
    UNION_WELFARE_ACTIVEBOX,         // ��Ծ����
    UNION_WELFARE_SUPTERACTIVEBOX,   // ������Ծ����
};

// ����״̬����
enum EUserLineStatus
{
    STATUS_OFFLINE,                    // ����
    STATUS_ONLINE,                     // ����
};

struct ConciseHeroInfo
{
    int heroDynID;                     // Ӣ��ID(Ψһ)
    int heroConfID;                    // ���ñ��ID
    int heroLv;                        // �ȼ�
    int heroStarLv;                    // �Ǽ�
};

struct EquipInfo
{
    int equipID;                       // װ��ΨһID
    int equipConfID;                   // װ������ID
};

struct ItemInfo
{
    int itemID;                        // ���ñ�ID
    int itemCount;                     // ��������
};

#endif