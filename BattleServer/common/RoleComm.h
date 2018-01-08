#ifndef __ROLE_COMM_H__
#define __ROLE_COMM_H__

//�����������ö��
enum EAttributeTypes
{
    // Class - ��������������
    EClassBegin = 0,
    EClassHPLine,                           // ��ɫѪ������
    EClassStatusID,                         // ��ɫ״̬��ID
    EClassAIID,                             // ��ɫAI��ID
    EClassHPGrowUp,                         // ��ɫ�����ɳ�ֵ
    EClassPAttackGrowUp,                    // ��ɫ���������ɳ�ֵ
    EClassMAttackGrowUp,                    // ��ɫħ���������ɳ�ֵ
    EClassPGuardGrowUp,                     // ��ɫ�����׳ɳ�
    EClassMGuardGrowUp,                     // ��ɫħ�����׳ɳ�
    EClassCritGrowUp,                       // ��ɫ�����ɳ�
    EClassEnd,

    // Class - int ����
    EClassIntBegin = 100,
    EClassAnimationId,                      // ��ɫ����ID
    EClassHPPosX,                           // ��ɫѪ��X����
    EClassHPPosY,                           // ��ɫѪ��Y����
    EClassFireRange,                        // ��ɫ��ս������Χ
    EClassFarFireRange,                     // ��ɫԶ�̹�����Χ
    EClassPAttack,                          // ��ɫ��������
    EClassMAttack,                          // ��ɫħ��������
    EClassPGuard,                           // ��ɫ������
    EClassMGuard,                           // ��ɫħ������
    EClassPPenetrate,                       // ��ɫ����͸
    EClassMPenetrate,                       // ��ɫħ����͸
    EClassAttackSpeed,                      // ��ɫ�����ٶ�
    EClassSpeed,                            // ��ɫ�ƶ��ٶ�
    EClassHP,                               // ��ɫ����ֵ
    EClassRage,                             // ��ɫŭ��ֵ
    EClassRageRecover,                      // ��ɫŭ���ظ�ֵ
    EClassMP,                               // ��ɫħ��ֵ
    EClassMPRecover,                        // ��ɫħ���ظ�
    EClassStrong,                           // ��ɫ����ֵ����
    EClassStrongRecover,                    // ��ɫ����ֵ�ָ��ٶ�
    EClassHaterd,                           // ��ɫ���ֵ
    EClassIntEnd,

    // Class - float ����
    EClassFloatBegin = 200,
    EClassScale,                            // ��ɫ��С����
    EClassEffectScale,                      // ��ɫ��Ч��С����ֵ
    EClassDefend,                           // ��ɫ�������
    EClassResustance,                       // ��ɫħ������
    EClassVampire,                          // ��ɫ��Ѫֵ
    EClassRebound,                          // ��ɫ����ֵ
    EClassMiss,                             // ��ɫ����ֵ
    EClassCrit,                             // ��ɫ����ֵ
    EClassCritDamage,                       // ��ɫ�����˺�
    EClassMass,                             // ��ɫ����
    EClassFloatEnd,

    // Attribute - ��̬����
    EAttributeBegin = 300,
	// int
    EAttributeHP,                           // ��ɫ������ǰֵ
    EAttributeMP,                           // ��ɫħ��ֵ��ǰֵ
    EAttributeRage,                         // ��ɫŭ����ǰֵ
    EAttributeStrong,                       // ��ɫ���嵱ǰֵ
    EAttributeExtraHP,                      // ��ɫ����ֵ
    EAttributeCamp,                         // ��Ӫ
    EAttributeBeAttackable,                 // �Ƿ�ɱ�����
    EAttributeBeAoeable,                    // �Ƿ�ɱ�AOE
    EAttributeBeBuffable,                   // �Ƿ�ɱ����BUFF
	//float
    EAttributeYmin,                         // ��ɫ�����ƽֵ
    EAttributeX,                            // X������float
    EAttributeY,                            // Y������float
    EAttributeHitPowerX,                    // ��ɫ���嵱ǰ����Xֵ
    EAttributeHitPowerY,                    // ��ɫ���嵱ǰ����Yֵ
    EAttributeEnd,

    // Attribute - int ����
    EAttributeIntBegin = 400,
    EAttributeAnimationId,                  // ��ɫ����ID
    EAttributeHPPosX,                       // ��ɫѪ����ǰX����
    EAttributeHPPosY,                       // ��ɫѪ����ǰY����
    EAttributeFireRange,                    // ��ɫ��ս������Χ
    EAttributeFarFireRange,                 // ��ɫԶ�̹�����Χ
    EAttributePAttack,                      // ��ɫ��������
    EAttributeMAttack,                      // ��ɫħ��������
    EAttributePGuard,                       // ��ɫ������
    EAttributeMGuard,                       // ��ɫħ������
    EAttributePPenetrate,                   // ��ɫ����͸
    EAttributeMPenetrate,                   // ��ɫħ����͸
    EAttributeAttackSpeed,                  // ��ɫ�����ٶ�
    EAttributeSpeed,                        // ��ɫ�ƶ��ٶ�
    EAttributeMaxHP,                        // ��ɫ����ֵ
    EAttributeMaxRage,                      // ��ɫŭ��ֵ
    EAttributeRageRecover,                  // ��ɫŭ���ظ�ֵ
    EAttributeMaxMP,                        // ��ɫħ��ֵ
    EAttributeMPRecover,                    // ��ɫħ���ظ�
    EAttributeMaxStrong,                    // ��ɫ����ֵ����
    EAttributeStrongRecover,                // ��ɫ����ֵ�ָ��ٶ�
    EAttributeHaterd,                       // ��ɫ���ֵ
    EAttributeIntEnd,

    // Attribute - float ����
    EAttributeFloatBegin = 500,
    EAttributeScale,                        // ��ɫ��С����
    EAttributeEffectScale,                  // ��ɫ��Ч��С����ֵ
    EAttributeDefend,                       // ��ɫ�������
    EAttributeResustance,                   // ��ɫħ������
    EAttributeVampire,                      // ��ɫ��Ѫֵ
    EAttributeRebound,                      // ��ɫ����ֵ
    EAttributeMiss,                         // ��ɫ����ֵ
    EAttributeCrit,                         // ��ɫ����ֵ
    EAttributeCritDamage,                   // ��ɫ�����˺�
    EAttributeMass,                         // ��ɫ����
    EAttributeFloatEnd,

    // Var ���� - int ����
    EAttributeIntVarBegin = 600,
    EAttributeSpeedVar,                     // ��ɫ�ƶ��ٶȱ仯ֵ
    EAttributePAttackVar,                   // ��ɫ���������仯ֵ
    EAttributeMAttackVar,                   // ��ɫħ���������仯ֵ
    EAttributePGuardVar,                    // ��ɫ�����ױ仯ֵ
    EAttributeMGuardVar,                    // ��ɫħ�����ױ仯ֵ
    EAttributePPenetrateVar,                // ��ɫ����͸�仯ֵ
    EAttributeMPenetrateVar,                // ��ɫħ����͸�仯ֵ
    EAttributeRageVar,                      // ��ɫŭ������ֵ�仯ֵ
    EAttributeRageRecoverVar,               // ��ɫŭ���ظ�ֵ�仯ֵ
    EAttributeMPVar,                        // ��ɫħ��ֵ����ֵ�仯ֵ
    EAttributeMPRecoverVar,                 // ��ɫħ���ظ��仯ֵ
    EAttributeStrongVar,                    // ��ɫ��������ֵ�仯ֵ
    EAttributeStrongRecoverVar,             // ��ɫ����ֵ�ָ�����
    EAttributeIntVarEnd,

    // Var ���� - float ����
    EAttributeFloatVarBegin = 700,
    EAttributeDefendVar,                    // ��ɫ������˱仯ֵ 
    EAttributeResustanceVar,                // ��ɫħ�����˱仯ֵ 
    EAttributeVampireVar,                   // ��ɫ��Ѫֵ�仯ֵ 
    EAttributeReboundVar,                   // ��ɫ����ֵ�仯ֵ 
    EAttributeMissVar,                      // ��ɫ����ֵ�仯ֵ 
    EAttributeCritVar,                      // ��ɫ����ֵ�仯ֵ 
    EAttributeCritDamageVar,                // ��ɫ�����˺��仯ֵ 
    EAttributeAttackSpeedVar,               // ��ɫ�������ʱ仯ֵ
    EAttributeFloatVarEnd,

    // Card ����
    ECardBegin = 800,
    ECardRace,                              // ����
    ECardSex,                               // �Ա�
    ECardAttackType,                        // ������ʽ 1��ս 2Զ��
    ECardVocation,                          // ְҵ
    ECardLevel,                             // ��ǰ�ȼ�
    ECardEnd,

    // Stat����
    EStatBegin = 900,
	// int
    EStatCrit,                              // ����
    EStatBeCrited,                          // ������
    EStatDodge,                             // ����
    EStatBeDodged,                          // ������
    EStatBeTreated,                         // ����
    EStatBrokenShield,                      // �ƶ�
    EStatAddShield,                         // �Ӷ�
    EStatBeAttacked,                        // �ܻ�
    EStatAttack,                            // ����
	//float
    EStatEnd,

    // Hero ����
    EHeroBegin = 1000,
	// int
    EHeroCrystalLevel,                      // Ӣ�۵�ǰˮ���ȼ�
    EHeroMaxCrystal,                        // Ӣ�۵�ǰ���ˮ��ֵ
    // float
	EHeroCrystal,                           // Ӣ�۵�ǰˮ��ֵ
    EHeroCrystalSpeedParam,                 // Ӣ��ˮ����������
    EHeroCrystalSpeed,                      // Ӣ�۵�ǰˮ�������ٶ�
    EHeroEnd,                                 
};

enum EAttributeType
{
    EUnknow,
    EAttributeBase,
    EAttributeInt,
    EAttributeFloat,
    EClassBase,
    EClassInt,
    EClassFloat,
};

// �������㺯��
inline bool isClassInt(EAttributeTypes type){ return type > EClassIntBegin && type < EClassIntEnd; }
inline bool isClassFloat(EAttributeTypes type){ return type > EClassFloatBegin && type < EClassFloatEnd; }
inline bool isClassType(EAttributeTypes type)
{
    return (type > EClassBegin && type < EClassEnd) 
        || isClassInt(type) 
        || isClassFloat(type); 
}
inline bool isAttributeInt(EAttributeTypes type)
{
    return (type > EAttributeIntBegin && type < EAttributeIntEnd) 
        || (type > EAttributeIntVarBegin && type < EAttributeIntVarEnd);
}
inline bool isAttributeFloat(EAttributeTypes type)
{
    return (type > EAttributeFloatBegin && type < EAttributeFloatEnd) 
        || (type > EAttributeFloatVarBegin && type < EAttributeFloatVarEnd);
}
inline bool isAttributeType(EAttributeTypes type)
{
    return (type > EAttributeBegin && type < EAttributeEnd) 
        || isAttributeInt(type) 
        || isAttributeFloat(type);
}
/*inline EAttributeType getAttributeType(EAttributeTypes type)
{
    if (type > EAttributeBegin && type < EAttributeEnd)
    {
        return EAttributeBase;
    }
    else if (isAttributeInt(type))
    {
        return EAttributeInt;
    }
    else
    {
        return EUnknow;
    }
}*/

// Ĭ�ϵ����ID
enum DefaultUserId
{
    EDefaultScene = -2,                     // ������UserID, ��Ҫ���ڹ����Ӫ����
    EDefaultPlayer = -1,                    // Ĭ�ϵ���� UserID
    EDefaultNpc,                            // Ĭ�ϵ�NPC UserID
};

enum ChangeCampType
{                                           
    ENoChange,                              // ��Ӫ���л�
    EChangeToEnmey,                         // �л����з�
    EChangeToNetral,                        // �л�������
    EChangeToBlue,                          // �л�������
    EChangeToRed,                           // �л����췽
};

// ��Ӫ���ͣ���ӦEAttributeCamp����
enum CampType
{
    ECamp_Neutral,                          // ����ʿ��
    ECamp_Blue,                             // ��ɫ����λ�����
    ECamp_Red,                              // ��ɫ����λ���ұ�
};

// ��ɫ��Ӫ
enum RoleCamp
{
	CAMP_NONE = -1,                         // ��
	CAMP_ALL,                               // ����
	CAMP_ENEMY,                             // ����
	CAMP_FRIEND,                            // ����
	CAMP_NEUTRAL                            // ����
};

//��ɫ����
enum RoleType
{
	RT_NONE = -1,                           // ��
	RT_ALL,                                 // ����
	RT_HERO,							    // Ӣ��
	RT_SOLDIER,                             // ʿ��
	RT_MONSTER,								// ����
	RT_BOSS,								// boss
	RT_SUMMON,								// �ٻ���
};

// ʿ����������
enum ESoldierType
{
    EST_RACE_BEGIN = 1,
    EST_RACE_HUMAN = EST_RACE_BEGIN,        // ������
	EST_RACE_BEAST,							// ������
	EST_RACE_SPRITE,						// ���徫��
	EST_RACE_GHOST,							// ��������
	EST_RACE_GIANT,							// �������
	EST_RACE_DRAGON,						// ������

    EST_SEX_BEGIN = 11,
    EST_SEX_MALE = EST_SEX_BEGIN,           // �Ա���
	EST_SEX_SHEMALE,						// �Ա�Ů
	EST_SEX_NEUTER,							// �Ա�����

    EST_FIRE_BEGIN = 21,
    EST_FIRE_NEAR = EST_FIRE_BEGIN,         // ��ս
	EST_FIRE_FAR,							// Զ��

    EST_VOCATION_BEGIN = 31,
    EST_VOCATION_TANK = EST_VOCATION_BEGIN, // ְҵ̹��(��ʿ)
	EST_VOCATION_SOLDIER,					// ְҵսʿ
	EST_VOCATION_ASSASSIN,					// ְҵ�̿�
	EST_VOCATION_SHOOTER,					// ְҵ����
	EST_VOCATION_MAGE,						// ְҵħ��ʦ
	EST_VOCATION_ASSISTANT,					// ְҵ����
};

// ��������
enum ERaceType
{
    RACE_TYPE_NON,
    RACE_TYPE_HUMAN,             // ����
    RACE_TYPE_GHOST,             // ����
    RACE_TYPE_NATURE,            // ��Ȼ
    RACE_TYPE_OTHER,             // ����
};

// �Ա�����
enum ESexType
{
    SEX_TYPE_NON,
    SEX_TYPE_MALE,               // �Ա���
    SEX_TYPE_SHEMALE,            // �Ա�Ů
    SEX_TYPE_NEUTER,             // �Ա�����
};

// ������ʽ
enum EFireType
{
    FIRE_TYPE_NON,
    FIRE_TYPE_NEAR = 1,           // ��ս
    FIRE_TYPE_FAR,                // Զ��
};

// ְҵ
enum EVocationType
{
    VOCATION_TYPE_NON,
    VOCATION_TYPE_SOLDIER,        // սʿ
    VOCATION_TYPE_ASSASSIN,       // �̿�
    VOCATION_TYPE_SHOOTER,        // ����
    VOCATION_TYPE_MAGE,           // ħ��ʦ
    VOCATION_TYPE_ASSISTANT,      // ����
    VOCATION_TYPE_BODYGUARD,      // ��ʿ
};

enum EEquipAddType
{
    EEquipNon,
    EEquipAdd,
    EEquipPercent,
    EEquipAddPercent,
};

enum EEquipAbility
{
    EA_NON,                     // ��
    EA_FireRange,               // ���ӻ���ٰٷ�֮����
    EA_FarFireRange,            // ���ӻ���ٰٷ�֮����
    EA_Speed,                   // ���ӻ���ٰٷ�֮����
    EA_HP,                      // ���ӻ���ٶ���
    EA_HPPercent,               // ���ӻ���ٰٷ�֮����
    EA_PAttack,                 // ���ӻ���ٶ���
    EA_PAttackPercent,          // ���ӻ���ٰٷ�֮����
    EA_Mattack,                 // ���ӻ���ٶ���
    EA_MattackPercent,          // ���ӻ���ٰٷ�֮����
    EA_DefendPercent,           // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_PGuard,                  // ���ӻ���ٶ���
    EA_PGuardPercent,           // ���ӻ���ٰٷ�֮����
    EA_Resustance,              // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_MGuard,                  // ���ӻ���ٶ���
    EA_MGuardPercent,           // ���ӻ���ٰٷ�֮����
    EA_PPenetrate,              // ���ӻ���ٶ���
    EA_PPenetratePercent,       // ���ӻ���ٰٷ�֮����
    EA_Mpenetrate,              // ���ӻ���ٶ���
    EA_MpenetratePercent,       // ���ӻ���ٰٷ�֮����
    EA_AttackSpeed,             // ���ӻ���ٰٷ�֮����
    EA_Vampire,                 // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_Rebound,                 // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_Miss,                    // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_Crit,                    // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_CritDamage,              // ���ӻ���ٰٷ�֮���٣����ֵ������Ǹ��ٷֱ�ֵ�������2���ٷֱ�ֵ�ļӼ���
    EA_Rage,                    // ���ӻ���ٶ���
    EA_RagePercent,             // ���ӻ���ٰٷ�֮����
    EA_RageRecover,             // ���ӻ���ٰٷ�֮����
    EA_MP,                      // ���ӻ���ٶ���
    EA_MPPercent,               // ���ӻ���ٰٷ�֮����
    EA_MPRecover,               // ���ӻ���ٰٷ�֮����
    EA_Mass,                    // ���ӻ���ٶ���
    EA_Endure,                  // ���ӻ���ٶ���
    EA_EndurePercent,           // ���ӻ���ٰٷ�֮����
    EA_EndureRecover,           // ���ӻ���ٰٷ�֮����
    EA_Haterd,                  // ���ӻ���ٶ���
    EA_BuffID = 999,            // ��ʾΪ��ɫ���һ��ָ��ID��BUFF����Ӳ���Ϊ1��
    EA_SoldierID = 10000,       // 5λ�������������ʿ����ID��	�����ĸ�ʿ�����츳	��������Χ2-7	����ڼ����츳
};

#endif