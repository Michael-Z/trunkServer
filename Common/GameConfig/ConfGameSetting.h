#ifndef __CONF_GAME_SETTING_H__
#define __CONF_GAME_SETTING_H__

#include "ConfManager.h"
#include <list>

#define NO_KEY_DATA(_DATA_TYPE_)	\
public:								\
	inline _DATA_TYPE_& getData()	\
	{								\
	return m_Data;				\
	}								\
protected:							\
	_DATA_TYPE_ m_Data;				\


/////////////////////////////////�������/////////////////////////////////////////

//����
struct Prob
{
	int		Probability;	//����
	float	Ratio;			//ϵ��
};

struct CardGambleSettingItem
{
	int					DiamondCardGamblePrice;		// ��һ�ε���
	int					DiamondCardGamble10Price;	// ʮ����۸�
	int                 exchangeRatio;              // Ӣ����Ƭ���Ҷһ���
	VecInt				SoldierLvUpScuccessRate;	// �����ɹ�����
	VecInt				FirstDrawCard;				// �׳�Ӣ��
	std::map<int, Prob> RareRatios;					// �Ǽ�����;  key: ����, value: ����
};

struct IconSettingItem
{
	std::vector<std::string> EqIcon;				// װ����λͼ�� ����, ͷ, �·�, Ь��, ��Ʒ, ����
	std::vector<std::string> RaceIcon;				// ����ͼ�� ��, ��, ����, ����, ����, ��
	std::vector<std::string> JobIcon;				// ְҵͼ�� ̹��, սʿ, ����, ��ʦ, �̿�, ����, ��ʿ
};

class SkillUpRateItem
{
public:
	int					SkillLv;// �ȼ�
	std::map<int, int>	Rate;	// �������� key ��������  value ����
};

//ʿ��������������
class SoldierLevelSettingItem
{
public:
	int					SoldierLv; // ʿ���ȼ�
	int					LvUpCost;	// �Ծ������Ľ��
	int					Exp;		// ���辭��
};

//ʿ���Ǽ��͵ȼ����� ����
class SoldierStarSettingItem
{
public:
	int				SoldierStar;	// ʿ���Ǽ�
	int				TopLevel;		// �ȼ�����
	int				UpStarLevel;	// ��������ĵȼ�
	int				UpStarCost;	    // �������Ľ��
	int             TurnCardCount;  // �״��ٻ�Ӣ��������Ƭ����
	int             TurnFragCount;  // �ٴλ��ת��Ϊ��Ƭ����
    int             UpStarCount;    // �������Ǽ�������Ƭ����
};

//ʿ��ϡ�жȱ߿���Ϣ
class SoldierRareSettingItem
{
public:
	int				Rare;			// ϡ�ж�
	std::string		UiRes;			// ��Ӧ����߿���Դ
	std::string		HeadboxRes;		// ͷ�����߿���Դ
	std::string     UnderboxRes;    // ˮ�����ֵ׿���Դ
	std::string     BorderboxRes;   // �±߿�U�ͻ�����Դ
	std::string     CircleboxRes;   // Ψһ��ʶ��Դ
	std::string		BigHeadboxRes;	// ��ͷ�����߿���Դ
	std::string		HeadboxBgRes;	// ͷ�񱳾���Դ
	std::vector<std::string> JobsIcon;  // ְҵͼ�� 1սʿ��2�̿͡�3���֡�4��ʦ��5������6��ʿ
	std::string		JobBg;
};

struct TaskActiveSettingItem
{
	int			TaskFinishSound;		// ���������Ч
	int			AchieveFinishSound;		// �ɾ������Ч
	std::string MainTaskIcon;			// ��������ͼ��
	std::string DailyTaskIcon;			// ÿ������ͼ��
};

//�����������͹�������������
class UserLevelSettingItem
{
public:
	int			Level;			// ��ҵȼ�
	int			Exp;			// �������辭��
	int         SummonerHP;     // �ٻ�ʦѪ��
	VecInt		BuyCoin;		// �����ҷ�Χ
};

// �����
class TowerTestSettingItem
{
public:
	int	FirstCrystal;           // ����������ʼˮ��
	int nStartWeekDay;			// �����ܿ�ʼ�ڼ���
	int nInterval;				// ���������೤ʱ��		//���ܳ���һ��ʱ��
};

// �½�����
struct ChapterSettingItem
{
	int NormalLastChapter;  // ��ͨ�½�,���һ��ID
	int EliteLastChapter;   // ��Ӣ�½�,���һ��ID
};

// ��������
struct ChatSettingItem
{
	int ChatUnlockLv;       // ��������ȼ�
	int RecoverTimes;       // �ָ�����
	int SpeedTimesLimit;    // ÿ�շ��Դ�������
	int WordNumLimit;       // ������������
	int IntervalTime;       // ����ʱ���� ��λ��s
	VecInt RecoverTime;     // ���Դ����ջָ�ʱ�� ��ʽ��ʱ-��
};

struct SMercenaryPrize
{
	int TimeProfit;//ÿ15��������
	int MaxTime;//���������פ�ص�ʱ�� �������
	int EmployedProfit;//ÿ�α���Ӷ�������
	int MaxEmployed;//��󱻹�Ӷ�������Ĵ���
};

#define MAX_EQUIP_ITEM_INDEX    5

typedef struct
{
	int nPart;
	int nLevel;
	int nVocation;
    int nSynthesisItemID[MAX_EQUIP_ITEM_INDEX];
    int nSynthesisItemCount[MAX_EQUIP_ITEM_INDEX];
	int nGoldSpend[2];
    int nEquipmentCreateID[MAX_EQUIP_ITEM_INDEX];
    int nNormalMakePercent[MAX_EQUIP_ITEM_INDEX];
    int nDelicateMakePercent[MAX_EQUIP_ITEM_INDEX];
}SEquipForCast;

//������������
class IncreasePayItem
{
public:
	int	BuyTimes;	        // �������
	int	GoldCost;	        // ������������ʯ��
	int ChallengeCost;      // ������ս����������ʯ��
	int TowerTreasureCost;	// ��������������ʯ
	int FreshShopCost;		// �����̵�ˢ��������ʯ
    VecInt EnergyCost;      // ��������������ʯ���Ͷ�Ӧ������
	VecInt GoldProb;        // ��ҹ������
	int BagCost;			//�������ӹ�������
};

class ItemCrit
{
public:
	int ItemType;           // ��������
	int	ItemQuality;        // ����Ʒ��
	int DoubleCrit;         // 2������(�����)
	int FourfoldCrit;       // 4������(�����)
};

struct StageSettingItem
{
	int WinMusic;           // ʤ������
	int LostMusic;          // ʧ������
};

class ItemLevelSettingItem
{
public:
	int ItemLevel;          // Ʒ��
	std::string ItemFrame;  // ͼ�����
	std::vector<int> Color;	//��ɫ
};

struct NewPlayerHeroInfo
{
	int heroId;
	int heroStarLv;
};

struct SoundEffectItem
{
    int MissChannel;         //����������ʧ����
    int VolumeDecayRate;     //����˥��ϵ����ÿ���پ�����������1���ٷֵ㣩
};

class NewPlayerItem
{
public:
	int						NewbieType;
	int						HeadId;
	int						UserLv;
	int						UserExp;
	int						Gold;
	int						Diamond;
	int						InitBagCapacity;
	int						MaxBagCapacity;
	int						MaxHeroCapacity;
	int						SummonerInGroup;
	std::vector<int>		SummonerIds;
	std::vector<NewPlayerHeroInfo> Heros;
	std::map<int, int>		Items;
	std::map<int, int>		Equips;
	std::vector<int>        Tasks;
	std::vector<int>        Achieves;
	std::vector<int>		Chapters;
	std::vector<int>		Guides;
	std::vector<int>		Heads;
    std::vector<int>		ChestIds;
    std::vector<int>		RobotIds;
	std::string				UserName;
};

class OutterBonusSetting
{
public:
	int ID;                     // Ч��ID
	int AttributeID;            // ���õ�����ID��RoleAttributeToID.csv��
	int Method;                 // ���õķ�����1�Ӽ�2�ٷֱȣ�
};

class PvpGradingChange
{
public:
	int ChangeMMRLower;			//MMR����
	int ChangeMMRUpper;			//MMR����
	int ChangePoint;			//�����仯����
};

class PvpGradingInitial
{
public:
	int PlayerLevelLower;		//��ҵȼ�����(���ڵ��ڸ�ֵ)
	int PlayerLevelUpper;		//��ҵȼ�����(С�ڸ�ֵ)
	int StartPoint;				//������ʼ������
	int StartMMR;				//������ʼ��MMR
};

class PvpMatchTime
{
public:
	int MatchOnlineLower;		//ƥ����������
	int MatchOnlineUpper;		//ƥ����������
	int MatchTime;				//ƥ��ʱ��
};

class PvpSetting
{
public:
	int GradingNum;				//����������
	int CriticalPoint;			//��ͻ���
	VecInt ArenaDay;			//�����������շ�Χ(�ܼ����ܼ�)
	VecInt ArenaTime;			//����������ʱ��㣨ʱ+��+ʱ+�֣�
};

class TimeRecoverItem
{
public:
	int AllTimeReset;
};

struct SSystemHeadIconItem
{
    std::string     IconName;
    int             IconTips;
};

/////////////////////////////////�������/////////////////////////////////////////

class CConfCardGambleSetting : public CConfBase
{
public:
	
	virtual bool LoadCSV(const std::string& str);

	NO_KEY_DATA(CardGambleSettingItem);
};
class CConfTowerTestSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfChapterSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	NO_KEY_DATA(ChapterSettingItem);
};

class CConfIconSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	NO_KEY_DATA(IconSettingItem);
};

class CConfSkillUpRateSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSoldierLevelSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSoldierStarSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSoldierRareSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfTaskAchieveSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	NO_KEY_DATA(TaskActiveSettingItem);
};

class CConfUserLevelSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	int GetUserMaxLv(){ return m_nUserMaxLv; }

private:
	int m_nUserMaxLv;
};

class CConfNewPlayerSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfChatSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	NO_KEY_DATA(ChatSettingItem);
};

class CConfIncreasePaymentPrice : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

// ��Ʒ����
class CConfItemCrit : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSystemHeadIcon : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	bool hasHeadIcon(int headID);
    const std::map<int, SSystemHeadIconItem>& getAllHeadIcon() { return m_mapHeadIcom; }

private:
    std::map<int, SSystemHeadIconItem> m_mapHeadIcom;   // <headID, {png, tip}>
};

class CConfStageSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	NO_KEY_DATA(StageSettingItem);
};

class CConfItemLevelSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfOutterBonusSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CEquipSet : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	int GetPropNum(int nQuality);		//����Ʒ�ʻ�ȡװ��������Ŀ

private:
	std::map<int, int>		m_MapQuality;
};

class CConfPvpGradingChange : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	std::vector<PvpGradingChange> &GetGradingChange();

private:
	std::vector<PvpGradingChange> m_vecGradingChange;
};

class CConfPvpGradingInitial : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	std::vector<PvpGradingInitial> &GetGradingInitial();

private:
	std::vector<PvpGradingInitial> m_vecGradingInitial;
};

class CConfPvpMatchTime : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	std::vector<PvpMatchTime> &GetMatchTime();

private:
	std::vector<PvpMatchTime> m_vecMatchTime;
};

class CConfPvpSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
public:
	PvpSetting m_PvpSetting;
};

class CConfTimeRecover : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSoundEffectSetting : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    NO_KEY_DATA(SoundEffectItem);
};

class CConfSysAutoName : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);

    std::map<int, std::vector<std::string> > getAutoNames(){ return m_mapAutoName; }
private:
    std::map<int, std::vector<std::string> > m_mapAutoName;
};

class CConfUnionMercenaryPrize : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
private:
};

class CConfEquipmentForCast : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
private:
};

/////////////////////////////////��ѯ���/////////////////////////////////////////

// ��ѯ��Ʒ������
inline const ItemCrit* queryConfItemCrit(int type, int quality)
{
	int key = type * 100 + quality;
	CConfItemCrit *conf = dynamic_cast<CConfItemCrit*>(
		CConfManager::getInstance()->getConf(CONF_ITEM_CRIT));
	return static_cast<ItemCrit*>(conf->getData(key));
}

// ��ѯ�������ı�
inline const IncreasePayItem* queryConfIncreasePay(int buyTimes)
{
	CConfIncreasePaymentPrice *conf = dynamic_cast<CConfIncreasePaymentPrice*>(
		CConfManager::getInstance()->getConf(CONF_INCREASE_PAY));
	return static_cast<IncreasePayItem*>(conf->getData(buyTimes));
}

// ��ѯ�����������ҹ����
inline const UserLevelSettingItem* queryConfUserLevel(int userLv)
{
	CConfUserLevelSetting *conf = dynamic_cast<CConfUserLevelSetting*>(
		CConfManager::getInstance()->getConf(CONF_USER_LEVEL_SETTING));
	return static_cast<UserLevelSettingItem*>(conf->getData(userLv));
}

inline const CardGambleSettingItem& queryConfCardGambleSetting()
{
	CConfCardGambleSetting* conf = dynamic_cast<CConfCardGambleSetting*>(
		CConfManager::getInstance()->getConf(CONF_CARD_GAMBLE_SETTING));
	return conf->getData();
}

inline const SoldierLevelSettingItem* queryConfSoldierLevelSetting(int lv)
{
	CConfSoldierLevelSetting *conf = dynamic_cast<CConfSoldierLevelSetting*>(
		CConfManager::getInstance()->getConf(CONF_SOLDIER_LEVEL_SETTING));
	return static_cast<SoldierLevelSettingItem*>(conf->getData(lv));
}

inline const NewPlayerItem* queryConfNewPlayerItem(int type)
{
	CConfNewPlayerSetting *conf = dynamic_cast<CConfNewPlayerSetting*>(
		CConfManager::getInstance()->getConf(CONF_NEW_PLAYER));
	return static_cast<NewPlayerItem*>(conf->getData(type));
}

inline const SoldierStarSettingItem* queryConfSoldierStarSetting(int star)
{
	CConfSoldierStarSetting *conf = dynamic_cast<CConfSoldierStarSetting*>(
		CConfManager::getInstance()->getConf(CONF_SOLDIER_STAR_SETTING));
	return static_cast<SoldierStarSettingItem*>(conf->getData(star));
}

inline const SoldierRareSettingItem* queryConfSoldierRareSetting(int rare)
{
	CConfSoldierRareSetting *conf = dynamic_cast<CConfSoldierRareSetting*>(
		CConfManager::getInstance()->getConf(CONF_SOLDIER_RARE_SETTING));
	return static_cast<SoldierRareSettingItem*>(conf->getData(rare));
}

// ��ѯ�����������ʱ�
inline const SkillUpRateItem* queryConfSkillUpRate(int skillLv)
{
	CConfSkillUpRateSetting *conf = dynamic_cast<CConfSkillUpRateSetting*>(
		CConfManager::getInstance()->getConf(CONF_SKILL_UP_RATE_SETTING));
	return static_cast<SkillUpRateItem*>(conf->getData(skillLv));
}

// ��ѯս�����������Ա�
inline const OutterBonusSetting* queryConfOutterBonusSetting(int ID)
{
	CConfOutterBonusSetting *conf = dynamic_cast<CConfOutterBonusSetting*>(
		CConfManager::getInstance()->getConf(CONF_OUTTER_BONUS_SETTING));
	return static_cast<OutterBonusSetting*>(conf->getData(ID));
}

// ��ѯս�����������Ա�
inline const TowerTestSettingItem* queryConfTowerSetting()
{
	CConfTowerTestSetting *conf = dynamic_cast<CConfTowerTestSetting*>(
		CConfManager::getInstance()->getConf(CONF_TOWER_SETTING));
	return static_cast<TowerTestSettingItem*>(conf->getData(1));
}

// ��ѯ����ϵͳͷ��
inline const std::map<int, SSystemHeadIconItem>& queryConfHeadIcon()
{
	CConfSystemHeadIcon *conf = dynamic_cast<CConfSystemHeadIcon*>(
		CConfManager::getInstance()->getConf(CONF_SYSTEM_HEAD_ICON));
	return conf->getAllHeadIcon();
}

// ��ѯ�Ƿ�������ĳ��ͷ��
inline bool queryConfHasHeadIcon(int headIconID)
{
	CConfSystemHeadIcon *conf = dynamic_cast<CConfSystemHeadIcon*>(
		CConfManager::getInstance()->getConf(CONF_SYSTEM_HEAD_ICON));
	return conf->hasHeadIcon(headIconID);
}

//��ѯĳ��Ʒ��װ����Ӧ��������
inline const int queryConfQualityProp(int nQuality)
{
	CEquipSet *conf = dynamic_cast<CEquipSet*>(
		CConfManager::getInstance()->getConf(CONF_EQUIP_QUALITY));
	return conf->GetPropNum(nQuality);
}

inline const TimeRecoverItem* queryConfTimeRecoverSetting()
{
	CConfTimeRecover *conf = dynamic_cast<CConfTimeRecover*>(
		CConfManager::getInstance()->getConf(CONF_TIMERECOVER));
	return static_cast<TimeRecoverItem*>(conf->getData(1));
}

inline const SoundEffectItem& queryConfEffectSetting()
{
    CConfSoundEffectSetting *conf = dynamic_cast<CConfSoundEffectSetting*>(
        CConfManager::getInstance()->getConf(CONF_SOUND_EFFECT_SETTING));
    return conf->getData();
}

inline const IconSettingItem& queryConfIconSetting()
{
    CConfIconSetting *pConf = dynamic_cast<CConfIconSetting*>(
        CConfManager::getInstance()->getConf(CONF_ICON_SETTING));
    return pConf->getData();
}

#endif
