#ifndef __CONF_HALL_H__
#define __CONF_HALL_H__

#include "ConfManager.h"
#include "ConfGameSetting.h"

/////////////////////////////////�������/////////////////////////////////////////
class AchieveItem
{
public:
	int				ID;					//�ɾ�ID
	int				Title;				//�ɾͱ���
	int				Desc;				//�����������
	int				UnLockLv;			//�����ȼ�
	int				Show;				//�Ƿ���ʾ
	int				FinishCondition;	//�������
	int				CompleteTimes;		//��ɴ���
	int				Tips;				//δ�����ʾ
	int				AwardExp;			//��������
	int				AwardCoin;			//�������
	int				AwardDiamond;		//������ʯ
	int				AwardEnergy;		//��������
	int				PosType;			//λ������
	int				AchieveStar;		//��ɺ�����
	int				CloseDisplay;		//�رպ��Ƿ���ʾ
	std::string		Icon;				//�ɾ�ͼ��
	VecInt			FinishParameters;	//��������1,2,3
	VecInt			EndStartID;			//����������
	std::vector<ID_Num>	AwardItems;		//��������
};

struct DiffcultItem
{
	int DiffID;
	int MaxLevel;
	int BasicLevel;
	int ExLevel;
	int UnlockLevel;
	VecInt Pic;
};

class ActivityInstanceItem
{
public:
	int ID;					//����ID
	int Title;				//��������
	int Desc;				//��������
	int Place;				//�������ȼ�
	int Show;				//��ɺ����ͼ��
	int Type;				//��������
	int CompleteTimes;		//��ɴ�������
	int BuyTimes;			//�����������
	int RecoverType;		//�ָ�����
	int RecoverParam;		//�ָ�����
	VecInt PlaceTime;		//���ͼ����ʾʱ��
	VecInt StartTime;		//������ʼʱ��
	VecInt EndTime;			//��������ʱ��
	VecInt RecoverTime;		//���������ָ�ʱ��
	std::vector<DiffcultItem> Diffcult; //�Ѷ�
	std::string Pic;		//���ͼ��
};

class CardGambleItem
{
public:
	int			ID;			// ��ƬID
	int			Rare;		// ϡ�ж�1��2��3��4��5��
	int			Star;		// �Ǽ�
	int			Rate;		// ����
	int			Ratio;		// ϵ��
	//Prob		Probability;	//���ʺ�ϵ��
};

//Ч��
struct Ability
{
	int						AbilityID;		//��ʾЧ��ID
	int						AbilityParam;	//��ʾЧ������
	int						AbilityDesc;	//��ʾЧ������
};

//�ֽ����
struct DecompositMaterial
{
	int		Decomposit;				//װ���ֽ����,��Ӧ�ĵ���ID
	int		DecompositionParam;		//��������
};

//�ϳɲ���
struct SynthMaterial
{
	int		Synthesis;		//����ID
	int		SynthesisParam;	//��������
};

struct EquipmentEffect
{
	EquipmentEffect()
	: SoliderId(0)
	, SoliderStart(0)
	{
	}

	int SoliderId;              // �˲��������жϣ����ID��ͬ�������߲ſɼ����츳
	int SoliderStart;           // ���⼤����Ǽ��츳
	std::map<int, int> Buff;    // BuffId - Buff����
};

#define MAX_BASE_PROP	8			//��������������ѡ��
#define MAX_EXTRA_PROP	8			//��ѡ����������ѡ��

struct EffectData
{
	int nEffectID;				//Ч��ID
	int nMinValue;				//Ч����Сֵ
	int nMaxValue;				//Ч�����ֵ
	int nWeight;				//Ч��Ȩֵ

	EffectData()
	{
		//memset(this, 0, sizeof(*this));
	}

};

//װ������
class EquipmentItem
{
public:
	int								ID;				// װ��ID
	int								Suit;			// ��װID
	int								Level;			// װ��ȼ�
	int								Parts;			// װ�䲿λ
	int								Gold;			// װ���ֽ�������
    int                             Rank;           // װ������
	EquipmentEffect                 ExtEffect;      // ����Ч��
	std::vector<DecompositMaterial>	Decomposit;		// װ���ֽ����
	VecInt							Vocation;		// װ��ְҵ
};

//װ����������
class EquipPropCreate
{
public:
	int							nEquipID;			//װ��ID
	std::vector<EffectData>		VectBaseProp;		//װ����������
	std::vector<EffectData>		VectExtraProp;		//װ����ѡ����

	EquipPropCreate()
	{
		nEquipID = 0;
		VectBaseProp.clear();
		VectExtraProp.clear();
	}

};

//��װ����
class SuitItem
{
public:
	int							    ID;			    // ��װID
	int							    Name;		    // ��װ����(��Ӧ���԰�ID)
	int							    Desc;		    // �ײ�����(��Ӧ���԰�ID)
	std::map<int, int>			    Eq;			    // ��װ�����װ��(1����,2�·�,3����,4Ь��,5��Ʒ,6��Ʒ)
	std::map<int, Ability>		    SuitAbility;    // ��װЧ��, key������, value����Ч������
	std::map<int, EquipmentEffect>  SuitExtEffect;  // ��װ����Ч��
};

//����
class PropItem
{
public:
	int						ID;						// ����ID
	int						Name;					// �������� ���԰�ID
	int						Desc;					// �������� ���԰�ID
	int						Quality;				// Ʒ��
	int						Type;					// ��������
	int						SellPrice;				// ���ۼ۸�
	int						UseLevel;				// ���ʹ�õȼ�
	int						BagLabel;				// ��������������ǩ
	float					Ratio;					// ����ϵ��
	std::string				Icon;					// ����ͼ��
	VecInt					TypeParam;				// �������Ͳ���
	VecVecInt				QuickToStage;			// ����ǰ���ؿ�
};

//���䵥��
struct DropIdData
{
	int DropID;
	int DropRate;
	VecInt DropNum;
};

//�������id
struct DropCurrencyData
{
	int CurrencyId;									//����id
	int UpperLimit;									//��������
	int LowerLimit;									//��������
};

// �������
class DropPropItem
{
public:
	int						DropRuleID;			   //�������ID
	int						IsCrit;				   //�ɷ񱩻�
	int						IsRepeat;			   //�ɷ��ظ�
	VecInt					MeanwhileDropNum;	   //���ε�����������
	VecInt					ExtraDropRuleID;	   //�������
	std::vector<DropCurrencyData> DropCurrencys;   //�������
	std::vector<DropIdData>	DropIDs;			   //����ID��

	DropPropItem& operator = (DropPropItem &Item)
	{
		DropRuleID = Item.DropRuleID;
		IsCrit = Item.IsCrit;
		IsRepeat = Item.IsRepeat;
		MeanwhileDropNum.assign(Item.MeanwhileDropNum.begin(), Item.MeanwhileDropNum.end());
		ExtraDropRuleID.assign(Item.ExtraDropRuleID.begin(), Item.ExtraDropRuleID.end());
		DropCurrencys.assign(Item.DropCurrencys.begin(), Item.DropCurrencys.end());
		DropIDs.assign(Item.DropIDs.begin(), Item.DropIDs.end());
		return (*this);
	}
};

//�ʼ�
class MailItem
{
public:
	int	ID;			// �ʼ�id
	int Topic;		// �ʼ�����
	int Sender;		// ������
	int Content;	// �ʼ�����
	int	LiveTime;	// ������(��)
};

//Ӣ����������
class SoldierUpRateItem
{
public:
	int				SoldierID;	           // Ӣ��ID
	int             DefaultStar;           // ��ʼ�Ǽ�
	int				TopStar;	           // ��������
    int             Source;                // ��ȡ;��
	VecVecInt       QuickToStage;          // ����ǰ���ؿ���ID
};

//�����ٻ�ʦ����
class SaleSummonerConfItem
{
public:
	int			ID;				// �ٻ�ʦID
	int			Type;			// ��������
	int			Num;			// ��������
	int			SummonerMusic;	// �ٻ�ʦ������Ч����ID
	int			NewLabel;		// �Ƿ���ʾnew��ǩ
	std::string	Head_Name;		// ��ɫͷ����Դ
	std::string	Bg_Name;		// ��ɫ������Դ
	std::string Bg_Texture;     // ��ɫ��������
	int			HeadID;			// ͷ��ID
};

class TaskItem
{
public:
	int				ID;						//����ID
	int				Title;					//�������
	int				Type;					//��������
	int				Desc;					//�����������
	int				UnlockLv;				//�����ȼ�
	int				Show;					//�Ƿ���ʾ
	int				FinishCondition;		//�������
	int				CompleteTimes;			//����ɴ���
	int				Tips;					//δ�����ʾ
	int				AwardExp;				//��������
	int				AwardCoin;				//�������
	int				AwardDiamond;			//������ʯ
	int				AwardEnergy;			//��������
	int				AwardFlashcard;			//�鿨ȯ
	int				TaskReset;				//��������
	std::string		Icon;					//����ͼ��
	VecInt			QuickTo;				//����ǰ��
	VecInt			FinishParameters;		//��������
	VecInt			EndStartID;				//����������
	VecInt			TaskResetParameters;    //�������ڲ���
	std::vector<ID_Num>	AwardItems;			//��������
};

enum EnhanceConditionTypes
{
	EnhanceConditionId = 1,                 // ����ID
	EnhanceConditionRoleType,               // ��ɫ����
	EnhanceConditionStar,                   // �Ǽ�
	EnhanceConditionCrystalCost,            // ˮ������
	EnhanceConditionRace,                   // ����
	EnhanceConditionSex,                    // �Ա�
	EnhanceConditionVocation,               // ְҵ
	EnhanceConditionAttackType,             // ������ʽ
};

enum EnhanceConditionJudgeTypes
{
	Greater = 1,            // ���� >
	Less,                   // С�� <
	Equal,                  // ���� ==
	GreaterEqual,           // ���ڵ��� >=
	LessEqual,              // С�ڵ��� <=
};

struct EnhanceCondition
{
	int Type;               //�ӳ����ö�������
	VecInt Param;           //��������
};

struct EnhanceValue
{
	int EffectId;           // �ӳ�����ID
	int Param;              // �ӳ�����ֵ
	int EffectLanID;        // ���԰�ID
};

class OutterBonusItem
{
public:
	int ID;                                             //ս�������Լӳ�ID
	int Name;											//����
	int Desc;											//����
	std::vector<EnhanceCondition> EnhanceConditions;    //�ӳ����ö��������б�
	std::vector<EnhanceValue> Enhances;                 //�ӳ�Ч��
	std::string Pic;
	std::string PicS;
};

class GoldTestConfItem
{
public:
	int						WeekNum;				//�ܼ�
	int						Stage;					//�ؿ�
	int						StageDesc;				//�ؿ�����
	int						Frequency;				//����
	int						StageLevel;				//�ȼ�
	VecFloat				Param;					//����
};

class GoldTestChestConfItem
{
public:
	int						Level;					//�ȼ�
	int						Gold;					//���
	int						Damage;					//�Ϻ�
};

class HeroTestItem
{
public:
	int							ID;					//ID
	VecInt						Time;				//ʱ��
	int							Occupation;			//ְҵ
	int							Times;				//����
	int							Desc;				//����
	int							UpDesc;				//��������
	int							Title;				//����
	std::vector<DiffcultItem>	Diff;				//�Ѷ�
	std::string					Pic;				//ͼƬ
};

//struct TowerDiffcultInfo
//{
//	int							MaxLevel;			//�ȼ�����
//	int							BasicLevel;			//�ȼ�����
//	int							EXLevel;			//�ȼ�ϵ��
//	int							Reward;				//��ʾ����
//	int                         ExtraStar;          //���⽱������
//	int							TCoin;				//����
//};

class TowerFloorItem
{
public:
	int								ID;					//¥����
	int								MaxLevel;			//�ȼ�����
	int								BasicLevel;			//�ȼ�����
	int								EXLevel;			//�ȼ�ϵ��
	VecInt							StageID;			//¥�㱸ѡ�ؿ�ID
	int								Place;				//ս���¼�������λ��
	int								Drop;				//����ID
};

struct TowerBuffInfo
{
	int							BuffID;				//ս�������Լӳ�BuffID
	int							Cost;				//buff��������
};

class TowerBuffItem
{
public:
	int							ID;					//ID
	std::vector<TowerBuffInfo>  Buff;				//Buff
	int							Max;				//¥������
	int							Min;				//¥������						
};

struct TowerRankInfo
{
	int							ID;
	int							Num;
};

class TowerRankItem
{
public:
	int							ID;					//ID
	VecInt						Rank;				//����
	std::vector<TowerRankInfo>	Item;				//��Ʒ
};

class ArenaRewardItem
{
public:
	int Reward_ID;
	int Reward_Type;
	int WinNum_Text;
	int Award_Coin;
	int Award_Diamond;
	int Award_PvpCoin;
	int Award_Flashcard;
	int Award_Items;
	std::string WinNum_Pic;
	VecInt Type_Parameter;
	VecInt AwardPic;
};


struct ArenaTaskItem
{
	int Task_ID;				   //��������ID			
	int Task_Type;				   //��������  0=��ս������   1=���ۼ�ʤ����   2=����ʤ����
	int Task_Text;				   //����˵��
	int Complete_Times;			   //��ɴ���
	int Award_Exp;				   //��ʾ�������� ��0��ʾû�У�����ʾ
	int Award_Coin;				   //��ʾ������� ��0��ʾû�У�����ʾ
	int Award_Diamond;			   //��ʾ������ʯ ��0��ʾû�У�����ʾ
	int Award_Energy;			   //��ʾ�������� ��0��ʾû�У�����ʾ
	int Award_PvpCoin;			   //��ʾ������������0��ʾû�У�����ʾ
	int Award_Flashcard;		   //��ʾ�����鿨ȯ
	int DropID;					   //����ID
	int IsOpen;					   //�Ƿ�Ĭ�Ͽ���
	int IsReset;				   //�Ƿ�����
	VecInt Award_Items;			   //��ʾ�������� ��[]��ʾû�У�����ʾ ���漷�����������2������
	VecInt End_StartID;			   //��������������ID ��[]��ʾû��Ҫ����������
	std::string Task_Pic;		   //����ͼ��
};

struct AnimationPlayOrderItem
{
	int ResID;
	std::vector<std::vector<std::string> > VecAnimations;
};

struct HallStandingItem
{
	int SpotOrder;              // λ�����
	Vec2 Position;              // λ������
	int ZOrder;                 // �㼶
};

class ShopConfigData
{
public:
	ShopConfigData()
	{
		nShopName = 0;
		nLevLimit = 0;
		nTimeInterval = 0;
		VectType.clear();
		VectNum.clear();
	}

	int nShopName;				//�̵���
	std::string strShopIcon;    //�̵갴ťͼƬ
	int nLevLimit;				//�ȼ�����
	int nTimeInterval;			//ˢ��ʱ����������
	VecInt VectType;			//�̵�����
	std::vector<int> VectNum;		//�̵���Ʒ������PVP��λ��
};

class ShopGoodsConfigData
{
public:

	ShopGoodsConfigData()
	{
		nShopID = 0;
		nShopGoodsID = 0;
		nGoodsID = 0;
		nGoodsNum = 0;
		nCoinType = 0;
		nCoinNum = 0;
		nWeight = 0;
		nSale = 0;
		nKind = 0;
		nFreshMinLev = 0;
		nFreshMaxLev = 0;
	}

	int		nShopID;			//�����̵�ID
	int		nShopGoodsID;		//��ƷID
	int		nGoodsID;			//����ID
	int		nGoodsNum;			//���߸���
	int		nCoinType;			//��������
	int		nCoinNum;			//���������ֵ
	int		nWeight;			//��ƷȨֵ
	int		nSale;				//��Ʒ�ۿ�
	int		nKind;				//��Ʒϡ�ж�
	int		nFreshMinLev;		//ˢ����С�ȼ�
	int		nFreshMaxLev;		//ˢ�����ȼ�
};

class DiamondShopConfigData
{
public:
	int nGoodsID;               // ����ID
	std::string strPicName;     // ��ƷͼƬID
	int nNameLanID;             // ��Ʒ�������԰�ID
	int nDescLanID;             // ��Ʒ�������԰�ID
	int nPrice;                 // ����۸�
	int nDiamond;               // ��õ���ʯ
};

//PVPƥ������л���������
struct SPvpRobotItem
{
	int nRobotID;				//������id
	int nRobotMMR;				//������mmrֵ
	int nMinMMR;				//mmr����
	int nMaxMMR;				//mmr����
	int nMinRobotLevel;			//�����˵ȼ�����
	int nMaxRobotLevel;			//�����˵ȼ�����
	VecInt stageIdVec;			//�������б�
};


//ÿ��ǩ����ÿ������
struct SCheckInDayConfig
{
	int nGoodsID;
	int nGoodsNum;
	int nShowNum;						//��ʾ��Ŀ
};

//�ۼ�ǩ������
struct SConCheckInConfig
{
	int DayNeeds;						//����������������
	int nGoodsID[3];					//��������
	int nGoodsNum[3];					//����������Ŀ
	int nShowNum[3];					//����������ʾ��Ŀ
};

//�׳�����
struct SFirstPayData
{
	std::vector<int> vectGoodsID;
	std::vector<int> vectGoodsNum;
	int nGrowGiftPrice;
	int nGiftDiamonds;
	int nGetTimes;

	SFirstPayData()
	{
		vectGoodsID.clear();
		vectGoodsNum.clear();
		nGrowGiftPrice = 0;
		nGiftDiamonds = 0;
		nGetTimes = 0;
	}
};

//QQ��������
struct SBDActiveData
{
	int nActiveType;					//�����
	int nUserLimit;						//��ȡ�û�����
	int nConditionParam;				//��������
	std::vector<int> vectGoodsID;		//�����ID
	std::vector<int> vectGoodsNum;		//�������Ʒ����

	SBDActiveData()
	{
		nActiveType = 0;
		nUserLimit = 0;
		nConditionParam = 0;
		vectGoodsID.clear();
		vectGoodsNum.clear();
	}
};

//QQ����Key
struct SBDActiveKey
{
	int nActiveID;
	int nTaskID;

	bool operator < (const SBDActiveKey &Key) const
	{
		if (nActiveID < Key.nActiveID)
		{
			return true;
		}
		else if (nActiveID == Key.nActiveID)
		{
			if (nTaskID < Key.nTaskID)
			{
				return true;
			}
		}

		return false;
	}
};

struct STalentData
{
    int TalentID;                       // �츳id
    int TalentName;                     // �츳����
    int TalentDes;                      // �츳����
    std::string TalentPic;                      // �츳��Դ
    std::map<int, int> OutterBonus;     // ���Լӳ�
    std::vector<ID_Num> BuffId;         // buff�ӳ�
};

struct STalentArrangeData
{
    int ArrangeID;                      // �츳ҳid(ְҵ�츳�������츳�̶�)
    VecVecInt FloorTalent;              // �츳���ϵ��츳 
};

struct SPVPShareData
{
    int ReplayShowCount;                // �ط�Ƶ�������ʾ�ط���
    int ReplayShareCD;                  // �ط�Ƶ������CD���룩
    int ReplayShareCount;               // �ط�Ƶ��ÿ�շ����������
    int BattleShareCount;               // ÿ��ս�������������
    int RefreshCD;                      // �ۿ���ˢ�¼�����룩
    int ReplayShowRank;                 // �ط�Ƶ������X������ʾ����
    int ShareDescLength;                // ����������������
    int Desc;                           // Ĭ������
};

struct SPVPUploadData
{
    int AutoUploadHP;                   // �Զ��ϴ���ʣ��Ѫ����׼
    int ApplyRank;                      // �����ϰ��������
    int ApplyCount;                     // ���������������
    int AutoUploadRank;                 // �Զ��ϴ������η�Χ
};

////////////////////////////////�������//////////////////////////////////////////
class CConfGoldTest : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfGoldTestChest : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfHeroTest : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfTowerFloor : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	int getMaxFloor() { return m_nMaxFloor; }
private:
	int m_nMaxFloor;
};

class CConfTowerBuff : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
#ifdef RunningInServer
	std::vector<int> &getFloorsBuff(int id);
private:
	std::map<int, std::vector<int> > m_mapFloorsBuffId;
#endif 
};

class CConfTowerRank : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfAchieve : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	int getPreAchieveID(int achieveID);

private:
	std::map<int, int> m_mapPreAchieve;  // <��ǰ�ɾ�ID, ǰ�óɾ�ID>
};

class CConfActivityInstance : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

typedef std::map<int, std::vector<CardGambleItem*> > MAP_CARDGAMEBLEITEM;
class CConfCardGamble : public CConfBase
{
public:
    virtual ~CConfCardGamble();
	virtual bool LoadCSV(const std::string& str);

	bool getData(int star, std::vector<CardGambleItem*>& vec)
	{
		MAP_CARDGAMEBLEITEM::iterator iter = m_GardGambleData.find(star);
		if (iter != m_GardGambleData.end())
		{
			vec = iter->second;
			return true;
		}

		return false;
	}

private:
	MAP_CARDGAMEBLEITEM m_GardGambleData;
};

class CConfEquipment : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfEqupmentCreate :public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfProp : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfDropProp : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	virtual std::map<int, DropPropItem>& GetShowDropPropItem() { return m_ShowDropPropItem; }

	virtual DropPropItem* GetRealDropPropItem(int nDropID);

	virtual bool ReSetShowDropPropItem();

	virtual bool SetShowDropPropItem(int nDropID, DropPropItem &Item);

protected:
	std::map<int, DropPropItem>  m_ShowDropPropItem;				//�����޸ĵĵ�������
};

class CConfMail : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSoldierUpRate : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSuit : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfSaleSummoner : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfTask : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfOutterBonus : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfArenaReward : public CConfBase
{
public:
	CConfArenaReward();
	~CConfArenaReward();
	virtual bool LoadCSV(const std::string& str);

public:

	ArenaRewardItem *		m_pDayWinItem;				//���ۼ�ʤ������
	ArenaRewardItem *		m_pDayContinusWinItem;		//����ʤ������   
	ArenaRewardItem *		m_pDayBattleItem;			//��ս�����ν���
	std::vector<ArenaRewardItem *> m_RankRewards;		//��ƽ�������н���
	std::vector<ArenaRewardItem *> m_CampionRankRewards;		//���������н���
};

class CConfArenaTask : public CConfBase
{
public:
	bool LoadCSV(const std::string& str);
};

class CConfAnimationPlayOrder : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfHallStanding : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CShopGoodsData : public CConfBase
{
	virtual bool LoadCSV(const std::string& str);

public:
	std::list<ShopGoodsConfigData*>* GetShopList(int nShopID);

private:

	std::map<int, std::list<ShopGoodsConfigData*> > m_MapShopID;
};

class CShopData : public CConfBase
{
	virtual bool LoadCSV(const std::string& str);
};

class CConfDiamondShop : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

//ÿ��ǩ��
class CConfDaySign : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	SCheckInDayConfig * GetMonthSignDay(int nMonth, int nDay);

private:
	std::map<int, std::map<int, SCheckInDayConfig> > m_MapYearSign;			//һ��ǩ����������Ϣ
};

//�ۼ�ǩ��
class CConfConDaySign : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfFirstPay : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	SFirstPayData * GetFirstPayData() { return &m_FirstPayData; }
protected:
	SFirstPayData	m_FirstPayData;
};

class CConfBDActive : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	std::map<SBDActiveKey, SBDActiveData> & getBDAllData() { return m_MapBDActive; }
	SBDActiveData*		getDBActiveData(int nActiveID, int nTaskID);

protected:
	std::map<SBDActiveKey, SBDActiveData>	m_MapBDActive;
};

class CConfTalent : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfTalentArrange : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfPVPShare : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    NO_KEY_DATA(SPVPShareData);
};

class CConfPVPUpload : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    NO_KEY_DATA(SPVPUploadData);
};

////////////////////////////////��ѯ���//////////////////////////////////////////
// ��ѯ��������
inline const ActivityInstanceItem* queryConfActivityInstance(int instanceId)
{
	CConfActivityInstance *conf = dynamic_cast<CConfActivityInstance*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVITY_INSTANCE));
	return static_cast<ActivityInstanceItem*>(conf->getData(instanceId));
}

// ��ѯװ����
inline const EquipmentItem* queryConfEquipment(int equipID)
{
	CConfEquipment *conf = dynamic_cast<CConfEquipment*>(
		CConfManager::getInstance()->getConf(CONF_EQUIPMENT));
	return static_cast<EquipmentItem*>(conf->getData(equipID));
}

//��ѯװ�����ɱ�
inline const EquipPropCreate *queryConfEquipCreat(int equipCreatID)
{
	CConfEqupmentCreate *conf = dynamic_cast<CConfEqupmentCreate*>(
		CConfManager::getInstance()->getConf(CONF_EQUIP_CREATE));
	return static_cast<EquipPropCreate*>(conf->getData(equipCreatID));
}

// ��ѯ��װ��
inline const SuitItem* queryConfSuit(int suitId)
{
	CConfSuit* conf = dynamic_cast<CConfSuit*>(
		CConfManager::getInstance()->getConf(CONF_SUIT));
	return static_cast<SuitItem*>(conf->getData(suitId));
}

// ��ѯ���߱�
inline const PropItem* queryConfProp(int itemID)
{
	CConfProp *conf = dynamic_cast<CConfProp*>(
		CConfManager::getInstance()->getConf(CONF_ITEM));
	return static_cast<PropItem*>(conf->getData(itemID));
}

// ��ѯ�����
inline const TaskItem* queryConfTask(int taskID)
{
	CConfTask *conf = dynamic_cast<CConfTask*>(
		CConfManager::getInstance()->getConf(CONF_TASK));
	return static_cast<TaskItem*>(conf->getData(taskID));
}

// ��ѯ�ɾ�
inline const AchieveItem* queryConfAchieve(int achieveId)
{
	CConfAchieve* conf = dynamic_cast<CConfAchieve*>(
		CConfManager::getInstance()->getConf(CONF_ACHIEVE));
	return static_cast<AchieveItem*>(conf->getData(achieveId));
}

// ��ѯ�����ٻ�ʦ����
inline const SaleSummonerConfItem* queryConfSaleSummoner(int summonerId)
{
	CConfSaleSummoner* conf = dynamic_cast<CConfSaleSummoner*>(
		CConfManager::getInstance()->getConf(CONF_SALESUMMONER));
	return static_cast<SaleSummonerConfItem*>(conf->getData(summonerId));
}

// ��ѯ�齱���ʱ�
inline const bool queryConfCardGamble(int rare, std::vector<CardGambleItem*> &vec)
{
	CConfCardGamble *conf = dynamic_cast<CConfCardGamble*>(
		CConfManager::getInstance()->getConf(CONF_CARD_GAMBLE));
	return conf->getData(rare, vec);
}

// ��ѯʿ�����Ǳ�
inline const SoldierUpRateItem* queryConfSoldierUpRateItem(int soldierID)
{
	CConfSoldierUpRate *conf = dynamic_cast<CConfSoldierUpRate*>(
		CConfManager::getInstance()->getConf(CONF_SOLDIER_UP_RATE));
	return static_cast<SoldierUpRateItem*>(conf->getData(soldierID));
}

// ��ѯ�ʼ���
inline const MailItem* queryConfMailItem(int mailID)
{
	CConfMail *conf = dynamic_cast<CConfMail*>(
		CConfManager::getInstance()->getConf(CONF_MAIL));
	return static_cast<MailItem*>(conf->getData(mailID));
}

// ��ѯս��������Ч����
inline const OutterBonusItem* queryConfOutterBonusItem(int ID)
{
	CConfOutterBonus *conf = dynamic_cast<CConfOutterBonus*>(
		CConfManager::getInstance()->getConf(CONF_OUTTER_BONUS));
	return static_cast<OutterBonusItem*>(conf->getData(ID));
}

// ��ѯ����������
inline const DropPropItem* queryConfDropProp(int dropId)
{
	CConfDropProp *conf = dynamic_cast<CConfDropProp*>(
		CConfManager::getInstance()->getConf(CONF_ITEMDROP));
	return static_cast<DropPropItem*>(conf->GetRealDropPropItem(dropId));
}

//���������Ϣ
inline const GoldTestConfItem * queryConfGoldTest(int nWeekDay)
{
	CConfGoldTest * conf = dynamic_cast<CConfGoldTest *>(
		CConfManager::getInstance()->getConf(CONF_GOLD_TEST));
	return static_cast<GoldTestConfItem *>(conf->getData(nWeekDay));
}

// ��ѯ��������
inline const HeroTestItem* queryConfHeroTest(int instanceId)
{
	CConfHeroTest *conf = dynamic_cast<CConfHeroTest*>(
		CConfManager::getInstance()->getConf(CONF_HERO_TEST));
	return static_cast<HeroTestItem*>(conf->getData(instanceId));
}

//�����������
inline const TowerFloorItem *queryConfTowerFloor(int floor)
{
	CConfTowerFloor *conf = dynamic_cast<CConfTowerFloor*>(
		CConfManager::getInstance()->getConf(CONF_TOWER_FLOOR));
	return static_cast<TowerFloorItem*>(conf->getData(floor));
}

// ������¥����
inline int queryMaxTowerFloor()
{
	CConfTowerFloor *conf = dynamic_cast<CConfTowerFloor*>(
		CConfManager::getInstance()->getConf(CONF_TOWER_FLOOR));
	return conf->getMaxFloor();
}

//�������buff
inline const TowerBuffItem *queryConfTowerBuff(int id)
{
	CConfTowerBuff *conf = dynamic_cast<CConfTowerBuff*>(
		CConfManager::getInstance()->getConf(CONF_TOWER_BUFF));
	return static_cast<TowerBuffItem*>(conf->getData(id));
}

//��ȡ����һ�ܽ���
inline const TowerRankItem* queryConfTowerRankReward(int nIndex)
{
	CConfTowerRank *conf = dynamic_cast<CConfTowerRank*>(
		CConfManager::getInstance()->getConf(CONF_TOWER_RANK));
	std::map<int, void*> MapData = conf->getDatas();

	for (std::map<int,void*>::reverse_iterator rator = MapData.rbegin(); rator != MapData.rend(); ++rator)
	{
		TowerRankItem * pRewardItem = static_cast<TowerRankItem*>(rator->second);

		if (pRewardItem == NULL || pRewardItem->Rank.size() != 2)
		{
			continue;
		}

		if (nIndex <= pRewardItem->Rank[1] && nIndex >= pRewardItem->Rank[0])
		{
			return pRewardItem;
		}
	}

	return NULL;
}

// ��ȡ���������������
inline const GoldTestChestConfItem *queryConfGoldTestChest(int lv)
{
	CConfGoldTestChest *conf = dynamic_cast<CConfGoldTestChest*>(
		CConfManager::getInstance()->getConf(CONF_GOLD_TEST_CHEST));
	return static_cast<GoldTestChestConfItem*>(conf->getData(lv));
}

//��ȡPVPÿ�ս���(type 0Ϊ��ƽ����,1Ϊ������)
inline ArenaRewardItem* queryConfPvpRankReward(int nIndex, int type = 0)
{
	CConfArenaReward *conf = dynamic_cast<CConfArenaReward*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_REWARD));

	std::vector<ArenaRewardItem *> VectPvpRankReward;
	if (0 == type)
	{
		VectPvpRankReward = conf->m_RankRewards;
	}
	else
	{
		VectPvpRankReward = conf->m_CampionRankRewards;
	}

	for (std::vector<ArenaRewardItem *>::reverse_iterator rator = VectPvpRankReward.rbegin(); rator != VectPvpRankReward.rend(); ++rator)
	{
		ArenaRewardItem * pRewardItem = *rator;

		if (pRewardItem == NULL || pRewardItem->Type_Parameter.size() != 2)
		{
			continue;
		}

		if (nIndex <= pRewardItem->Type_Parameter[1] && nIndex >= pRewardItem->Type_Parameter[0])
		{
			return pRewardItem;
		}
	}

	return NULL;
}

inline const ArenaTaskItem * queryArenaTaskItem(int taskId)
{
	CConfArenaTask *pConf = dynamic_cast<CConfArenaTask*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_TASK));
	return static_cast<ArenaTaskItem*>(pConf->getData(taskId));
}

inline const AnimationPlayOrderItem* queryConfAnimationPlayOrder(const int& resID)
{
	CConfAnimationPlayOrder *confPlayOrder = dynamic_cast<CConfAnimationPlayOrder*>(
		CConfManager::getInstance()->getConf(CONF_ANIMATION_PLAY_ORDER));
	return static_cast<AnimationPlayOrderItem*>(confPlayOrder->getData(resID));
}

inline const HallStandingItem* queryConfHallStanding(const int& order)
{
	CConfHallStanding *conf = dynamic_cast<CConfHallStanding*>(
		CConfManager::getInstance()->getConf(CONF_HALL_STANDING));
	return static_cast<HallStandingItem*>(conf->getData(order));
}

//��ȡĳ���̵��Ӧ�������б�
inline const std::list<ShopGoodsConfigData*>* queryConfShopList(int nShopID)
{
	CShopGoodsData *pConf = dynamic_cast<CShopGoodsData*>(
		CConfManager::getInstance()->getConf(CONF_SHOP_GOODS));

	return pConf->GetShopList(nShopID);
}

inline const ShopGoodsConfigData * queryConfShopData(int nShopGoodsID)
{
	CShopGoodsData *pConf = dynamic_cast<CShopGoodsData*>(
		CConfManager::getInstance()->getConf(CONF_SHOP_GOODS));

	return static_cast<ShopGoodsConfigData *>(pConf->getData(nShopGoodsID));
}

//ǩ��
inline const SCheckInDayConfig *queryCheckInDaySign(int nMonth, int Days)
{
	CConfDaySign *pConf = dynamic_cast<CConfDaySign*>(
		CConfManager::getInstance()->getConf(CONF_DAYSIGN));
	return reinterpret_cast<SCheckInDayConfig *>(pConf->GetMonthSignDay(nMonth, Days));
}

//�ۼ�ǩ������
inline const SConCheckInConfig * queryConCheckInSign(int nTimes)
{
	CConfConDaySign *pConf = dynamic_cast<CConfConDaySign*>(
		CConfManager::getInstance()->getConf(CONF_CONDAYSIGN));
	return reinterpret_cast<SConCheckInConfig *>(pConf->getData(nTimes));
}

//�׳�
inline const SFirstPayData *queryFirstPayData()
{
	CConfFirstPay * pConf = dynamic_cast<CConfFirstPay*>(
		CConfManager::getInstance()->getConf(CONF_FIRSTPAY_SETING));
	return pConf->GetFirstPayData();
}

//��ȡ�츳��Ϣ
inline const STalentData *queryTalentData(int talentId)
{
    CConfTalent * pConf = dynamic_cast<CConfTalent*>(
        CConfManager::getInstance()->getConf(CONF_TALENT));
    return static_cast<STalentData *>(pConf->getData(talentId));
}

//��ȡ�츳ҳ��Ϣ
inline const STalentArrangeData *queryTalentArrangeData(int arrangeId)
{
    CConfTalentArrange * pConf = dynamic_cast<CConfTalentArrange*>(
        CConfManager::getInstance()->getConf(CONF_TALENT_ARRANGE));
    return static_cast<STalentArrangeData *>(pConf->getData(arrangeId));
}

#endif
