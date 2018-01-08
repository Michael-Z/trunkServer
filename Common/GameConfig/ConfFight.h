#ifndef __CONF_FIGHT_H__
#define __CONF_FIGHT_H__

#include "ConfManager.h"
#include "CommTools.h"

////////////////////////////////////�������//////////////////////////////////////
#ifndef RunningInServer
enum EffZOrderType
{
    EffZOrderLocal,
    EffZOrderGlobal,
    EffZOrderInherit,
};

// ��Ч���ñ���
struct EffectConfItem
{
	int EffectId;                                   // ��ЧID
    int Loop;                                       // ѭ��������0 ��ѭ�� ����Ϊѭ�� -1Ϊ�����趨
    int ZOrderType;                                 // 0���� 1ȫ�� 2�̳�
	int ZOrder;                                     // Z��㼶Ĭ��Ϊ0
	int AudioId;                                    // ��ЧID
	int ResID;										// �����ļ���
    int SpeedAffect;                                // �ٶ�����
    float LifeTime;                                 // �������ڣ�0Ϊ����ʱ�䣩
	float FadeInTime;                               // ����ʱ�䣨0��������
	float FadeOutTime;                              // ����ʱ�䣨0��������
	float AnimationSpeed;                           // ���������ٶ�
	Vec2 Offset;                                    // ����λ��ƫ��
	Vec2 Scale;                                     // ����ֵ
	VecFloat AddColor;                              // ����ɫ
	std::string AnimationName;                      // ����������
	VecMusicInfos MusicInfos;                       // ��Ч��Ӧ����Ч
};

class UIEffectConfItem
{
public:
    bool                DoodadLoop;                 // װ��������Ƿ�ѭ��
    bool                DoodadBackLoop;             // װ������ֻع��Ƿ�ѭ��
    bool                once;                       // �Ƿ���һ����
    int					ID;							// ID
    int					ShakeLevel;					// ����ǿ��
    int					DoodadDistance;				// װ������־���
    int                 DarkAlpha;                  // 0~100�������䰵��͸����
    int					BlinkingResID;				// ����������ԴID
    int					BackgroundResID;			// ����������ԴID
    float               DarkFadeInTime;             // ��������ʱ��
    float               DarkFadeOutTime;            // ��������ʱ��
    float               DarkTime;                   // �����䰵����λ�룬�����䰵��ʱ��
    float               DarkDelayTime;              // �����䰵����λ�룬��ʼ�䰵���ӳ�ʱ��
    float				ShakeTime;					// ��ʱ�䣬��λ��
    float				ShakeDelayTime;				// ��ʱ�䣬��λ�룬��ʼ�𶯵��ӳ�ʱ��
    float				DoodadStartTime;			// װ������ֿ���ʱ�䣬��λ��
    float				DoodadContinuedTime;		// װ�������ʱ�䣬��λ��
    std::string			BlinkingCsbAniName;			// ����������������
    std::string			BackgroundCsbAniName;		// ����������������
    std::string			DoodadName;					// װ������ֱ�ǩ
    std::string			DoodadBackName;				// װ������ֻع��ǩ
};
#endif // RunningInServer

//Buff����
class BuffConfItem
{
public:
	bool                IsEffectMove;               //��Ч�Ƿ�����ƶ� 0��1��
	int					ID;							//ID
	int                 Type;                       //1����2����3������ɢ��BUFF
	int                 Stack;                      //������1��ʼ
	float               lifeTime;                   //���������BUFF���� ����Ϊ����, 0Ϊ���̻���, �������������
	int                 NextBuffCondition;          //0Ϊ������,1Ϊ��������,2Ϊǿ�ƻ���
	int                 UIEffectID;                 //��ӦUI��Ч��ID
	int                 EffectWhere;                //buff��Ч���ŵ� 1ͷ2��3��
	int                 TargetType;                 //buffĿ�� 1buffЯ����,2buff������,3�Զ���ѡȡ
	float               FirstTime;                  //�״���Чʱ��
	int                 MaxCount;                   //��Ч�ܴ���
	int                 TriggerType;                //Buff��������
	int                 SkillID;                    //�Ƿ���ؼ���
	VecInt              nextBuffID;                 //����ʱ����ӵ�Buff����
	VecInt              AnimationID;                //��Ӧ������ID
    VecInt              CountID;                    //Buff��ЧʱĿ�����ID
    VecInt              SummonerCountID;            //Buff��Чʱ�ٻ�ʦ��boss����ID
    VecInt              LapseCountID;               //BuffʧЧʱĿ�����ID
    VecInt              SummonerLapseCountID;       //BuffʧЧʱ�ٻ�ʦ��bossĿ�����ID
	VecInt              TriggerTypeParam;           //Buff�������Ͳ���
	std::vector<CDataFunction *> Conditions;        //��Ч����, 4��
};

//�ӵ�����
class BulletConfItem
{
public:
	bool				    LockDirect;					//���������Ƿ�����
	int					    ID;							//�ӵ�ID
	int					    Locus;						//�����켣
	int					    StartType;					//�ӵ���ʼλ��
	int					    EndType;					//�ӵ�����λ��
	int					    HitType;					//�ӵ���������
	int					    HitJudgeType;				//�����ж�����
	int				        HitJudgeParam;				//�����ж����Ͳ���
	int                     UiEffectTime;               //UI��Ч����ʱ��
	int					    UiEffectId;					//ui��Ч���ID
	int				        HitJudgeMaxTimes;			//�����ж�������
	int                     ZOrderType;                 //0Ϊ�����Զ���������-2�����ñ���ZOrder
	int				        UnitMaxCountTimes;			//��λ���������
	float				    FlyTime;					//�ӵ�����֡��
	float				    LifeTime;					//�ӵ���������
	float				    HitJudgeFirstTime;			//�״��ж�ʱ��
	float				    HitJudgePerTime;			//�����ж����ʱ��
	VecFloat			    Locus_Param;				//��������
    VecInt				    CountId;					//����ID���� 
    VecInt				    SummonerCountId;			//���ٻ�ʦ�Ľ���ID���� 
	VecInt				    LinkId;						//�ӵ�����ID����
	VecInt				    AnimationId;				//�ӵ����ֶ���ID
	VecInt                  EndAnimationId;             //�ӵ���������ID
	VecInt				    HitAnimationId;				//�ӵ����ж�������ID
	VecInt                  HitAllAnimationId;          //�ӵ�Ⱥ�����б��ֶ���
	VecInt				    StartParam;					//��ʼλ�ò���
	VecInt				    EndParam;					//����λ�ò���
	std::vector<ID_Num>		BuffId;                     //��Ӧbuff��ID
};

// ��Ƭ��������
class CardCountConfItem
{
public:
	int				    ChangeSingo;					// Ψһ������
	bool				CardLockChange;					// ���������
	int					CardCountID;					// ��Ƭ����ID
	int					StarCondition;					// �Ǽ�����
	int					StarConditionPrarm;				// �Ǽ���������
	int					CrystalCondition;				// ˮ������
	int					CrystalConditionPrarm;			// ˮ����������
	int					CrystalChangeType;              // ���ˮ������
	int					CrystalChangePrarm;				// ���ˮ�����Ĳ���
	int					CDChangeType;                   // ���CD
	int					CDChangePrarm;					// ���CD����
	VecInt				RaceCondition;					// ��������
	VecInt				SexCondition;					// ��Ӧ�Ա�����
	VecInt				VocationCondition;				// ְҵ����
	VecInt				AttackTypeCondition;			// ������ʽ����
};

enum CountExpressionOperatorType
{
    OperatorUnvalid,                                    // ��Ч
    OperatorMulti,                                      // *
    OperatorDiv,                                        // /
    OperatorAdd,                                        // +
    OperatorMinus,                                      // -
    OperatorEqual,		                                // =
};

enum CountExpressionTargetType
{                                                       
    TargetTypeSource,                                   // Դ����
    TargetTypeTarget,                                   // Ŀ�����
    TargetTypeVar,                                      // Var����
    TargetTypeValue,                                    // �̶���ֵ
};

// ���㹫ʽ����
class CountExpressionConfItem
{
public:
    CountExpressionConfItem();
    virtual ~CountExpressionConfItem();

    bool init(const std::string& str);

    float Value;                                        // �̶���ֵ
    int AttributeId;                                    // ����ID
    std::string VarName;                                // Var����
    CountExpressionTargetType Target;                   // ����Ŀ��
    CountExpressionOperatorType Operator;               // ���������
    CountExpressionConfItem* LeftExpression;            // �����ʽ
    CountExpressionConfItem* RightExpression;           // �Ҳ���ʽ

private:
    // û�в�����ʱ���������ʽ�����ֵ����
    bool initValue(const std::string& str);
    // �����ַ������������λ�ã���ʼ������������ʽ
    bool initExpression(const std::string& str, size_t operatorPos);
    // �����ַ�����ͬ���������ķ��ţ��ҳ����ȼ���͵Ĳ�������������λ�������posOut������
    bool checkExpression(const std::string& str, char op1, char op2,
        CountExpressionOperatorType opType1, CountExpressionOperatorType opType2, size_t& posOut);
};

// ��ͨ��������
class CountConfItem
{
public:
	~CountConfItem()
	{
        deleteAndClearVec<CountExpressionConfItem*>(TargetProperty);
	}

	bool						Miss;					// ���ܿ���
	bool						Crit;					// ��������
	bool						Damage;					// �˺����̿���
	bool						DamageReturn;			// ��������
	int							ID;						// ����ID
	int							Strong;					// ����ǿ��
	int							Status;					// ����״̬id
	int							DamageType;				// �˺�����
	int							BuffListDel;			// �Ƿ������ɢ
	int							DeBuffListDel;			// �Ƿ���о���
	int							BuffDel;				// ָ��Buffɾ��
	int							AIIDChange;				// �Ƿ���и���AI
	int							StatusIDChange;			// �Ƿ���и���״̬
	int                         CampChange;             // �Ƿ������Ӫ�л�
	float						StatusTime;				// ����״̬����ʱ��
	float                       PowerPercent;           // �������̳аٷֱ�
	float                       PowerExt;               // ����Ĺ�����
	VecInt						CardCountID;			// ��Ӧ�Ŀ�Ƭ����ID
    std::vector<CountExpressionConfItem*> TargetProperty;   // �仯����
};

//ˮ������
class CrystalConfItem
{
public:
	int					Level;						//ˮ���ȼ�
	float				Speed;						//ˮ�������ٶ�
	int					Max;						//ˮ�����ֵ
	int                 Price;                      //������������
};

enum SearchCondition
{
    ConditionIdentity,
    ConditionHP,
    ConditionCareer,
    ConditionSex,
    ConditionAttackType,
    ConditionRace,
    ConditionStar,
    ConditionBuff,
    ConditionType,
    ConditionState,
    ConditionRoleId,
};

// ����
class SearchConfItem
{
public:
	int					ID;							 //����ID
	int					ListType;					 //�б�ѡȡ(0ȫ��1����2����)
	int					RangeType;					 //��������(1����ѡȡ2˫��ѡȡ)
	int					RangeParam;					 //�������(0ȫ��-1����-2Զ��,����������̶���ֵ)
	int					Type;						 //��������(1 A��,2 B��,3 C��)
	int					Reorder;					 //ָ����������(1Ѫ��, 2 X������)
	int					Num;						 //�����Ľ�ɫ����
	int				    AttackType;					 //��ɫ�Ƿ��ǽ�ս(0����1��ս2Զ��)
	bool				Self;						 //�Ƿ�ȥ���Լ�
	bool                Death;                       //�Ƿ���������
    VecInt              RoleID;                      //��ɫId�Ƿ�ƥ��
    VecInt              Conditions;                  //��Ҫ�жϵ�����
	VecInt				Identity;					 //�������(�Ƿ�ȡ��+��ݲ���)	
	VecInt				Career;						 //��ɫְҵ(�Ƿ�ȡ��+ְҵ)
	VecInt				Sex;						 //��ɫ�Ա�(�Ƿ�ȡ��+�Ա�)
	VecInt				Race;						 //��ɫ����(�Ƿ�ȡ��+����)
	VecInt              Buff;                        //��ɫBuff(�Ƿ�ȡ��+Buff����)
	VecInt				Star;						 //��ɫ�Ա�(�Ƿ�ȡ��+�Ա�)
	VecInt              State;                       //��ĳ��״̬���Ƿ�ȡ��+״̬ID����
	VecFloat			HP;							 //��ɫѪ��(�Ƿ�ȡ��+Ѫ��������)
};

 enum SkillCastType
 {
     SkillCastNone = 0,                              //��Ч
     SkillCastAtOnce,                                //ֱ���ͷ�
     SkillCastAtPoint,                               //���ͷ�
 };

//��������
class SkillConfItem
{
public:
	bool                    CanBreak;                   // �ܷ���
	int					    ID;							// ����ID
	int					    CastType;					// �����ͷ�����
    int                     CastRange;                  // �����ͷŷ�Χ
	int					    LockType;					// ���ܽ�������
	int					    LockTypePrarm;				// ���ܽ�������
	float                   CD;                         // ����cd
	int                     CostType;                   // ������������
	int                     CostTypeParam;              // �������Ĳ���
	int                     MaxCast;                    // ����ѭ������
	float                   TargetBulletDelay;          // Ŀ���ӵ�����ʱ��
	float                   TargetBulletInterval;       // Ŀ���ӵ����м��ʱ��
	float                   PointBulletDelay;           // Ŀ����ӵ�����ʱ��
	float                   PointPointBulletInterval;   // Ŀ����ӵ����м��ʱ��
	int                     BulletParam;                // �ӵ��������ʣ�1Ϊ����������
	int                     Name;                       // �����������԰�
	int						CostDesc1;					// ������������1
	int						CostDesc2;					// ������������2
	int                     Desc;                       // �����������԰�
	int                     StateID;                    // ������ת״̬
	float                   CDParam;                    // Ĭ��cd�ٷֱ�
	float                   CastTime;                   // ����ѭ��ʱ��
	VecInt                  TargetBullet;               // �Լ���Ŀ�귢���ӵ�����
	VecInt                  PointBullet;                // �Լ���Ŀ��㷢���ӵ�����
	VecInt                  Call;                       // �ڼ���Ŀ����ٻ�����
	std::vector<ID_Num>     Buff;                       // �Լ���Ŀ�����Buff����
	std::string			    IconName;					// ����ͼ��
    std::string             BattleSkillIcon;             // ս������ͼ��
};

//////////////////////////////////�������////////////////////////////////////////
#ifndef RunningInServer
class CConfEffect : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfUIEffect : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

// ��ѯ��Ч���ñ�ĸ�������
inline const EffectConfItem* queryConfEffect(int effId)
{
	return reinterpret_cast<EffectConfItem*>(
		CConfManager::getInstance()->getConf(CONF_EFFECT)->getData(effId));
}
#endif // RunningInServer

class CConfBuff : public CConfBase
{
public:
    virtual ~CConfBuff();
	virtual bool LoadCSV(const std::string& str);
	BuffConfItem * getData(int buffid, int stack);
	int getMaxStack(int buffid);

	std::map<int, std::map<int, BuffConfItem*> >& getBuffData()
	{
		return m_BuffData;
	}

private:
	std::map<int, std::map<int, BuffConfItem*> > m_BuffData;
};

class CConfBullet : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfCardCount : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfCount : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfCrystal : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSearch : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSkill : public CConfBase
{
public:
    virtual ~CConfSkill();
	virtual bool LoadCSV(const std::string& str);
};

/////////////////////////////////��ѯ���/////////////////////////////////////////
// ��ѯbuff���
inline const BuffConfItem* queryConfBuff(int buffId, int stack)
{
	CConfBuff *conf = dynamic_cast<CConfBuff*>(
		CConfManager::getInstance()->getConf(CONF_BUFF));
	return static_cast<BuffConfItem*>(conf->getData(buffId, stack));
}

// ��ѯ���buff����
inline const int queryMaxBuffStack(int buffid)
{
	CConfBuff *conf = dynamic_cast<CConfBuff*>(
		CConfManager::getInstance()->getConf(CONF_BUFF));
	return conf->getMaxStack(buffid);
}

// ��ѯ�ӵ����
inline const BulletConfItem* queryConfBullet(int bulletId)
{
	CConfBullet *conf = dynamic_cast<CConfBullet*>(
		CConfManager::getInstance()->getConf(CONF_BULLET));
	return static_cast<BulletConfItem*>(conf->getData(bulletId));
}

inline const CardCountConfItem* queryConfCardCount(int countId)
{
	return  reinterpret_cast<CardCountConfItem*>(
		CConfManager::getInstance()->getConf(CONF_CARD_COUNT)->getData(countId));
}

// ��ѯˮ�����ñ�ĸ�������
inline const CrystalConfItem* queryConfCrystal(int crystalLevel)
{
	CConfCrystal* conf = dynamic_cast<CConfCrystal*>(
		CConfManager::getInstance()->getConf(CONF_CRYSTAL));
	return static_cast<CrystalConfItem*>(conf->getData(crystalLevel));
}

inline const CountConfItem* queryConfCount(int countId)
{
	return  reinterpret_cast<CountConfItem*>(
		CConfManager::getInstance()->getConf(CONF_COUNT)->getData(countId));
}

// ��ѯSearch���
inline const SearchConfItem* queryConfSearch(int searchId)
{
	CConfSearch *conf = dynamic_cast<CConfSearch*>(
		CConfManager::getInstance()->getConf(CONF_SEARCH));
	return static_cast<SearchConfItem*>(conf->getData(searchId));
}

// ��ѯskill���
inline const SkillConfItem* queryConfSkill(int skillId)
{
	CConfSkill *conf = dynamic_cast<CConfSkill*>(
		CConfManager::getInstance()->getConf(CONF_SKILL));
	return static_cast<SkillConfItem*>(conf->getData(skillId));
}


#endif
