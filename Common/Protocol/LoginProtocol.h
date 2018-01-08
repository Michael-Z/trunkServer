#ifndef __LOGIN_PROTOCOL_H__
#define __LOGIN_PROTOCOL_H__

#pragma pack(1)

#include <map>
#include <vector>
/*
*   ��¼���̼��
*   Ŀǰ֧��3�ֵ�¼У�����̣�
*   1.���Ե�¼У�飬ֱ������ָ����UID��¼�����ַ�ʽ�����κ�У��
*       ǰ�˷���CMD_LOGIN_CHECK_TEST_CS���󣬰���Я��ָ����UID
*       ������ֱ�ӷ���CMD_LOGIN_CHECK_SC������Я��UID
*
*   2.�ο͵�¼У�飬�ο͵�¼У�������������״ε�¼�ͷ��״ε�¼
*
*   2.1 �״��ο͵�¼У�飬��������ҪΪ�ͻ��������ο͵�UID�Լ���Կ��ǰ��ֻ����һ�Ρ�
*       ǰ�˷���CMD_LOGIN_NEWGUEST_SC����
*       ��������ο͵�UID��ʹ���ض�ǰ׺sum + UID����MD5���ܺ���Ϊ����
*       ��guest-uid��Ϊkey��������Ϊvalue�洢��redis��
*       ����������CMD_LOGIN_NEWGUEST_SC��ǰ�ˣ�����Я��UID + ����
*
*   2.2 ���״��ο͵�¼У�飬�ύ�û�ID + ���뼴��
*       ǰ�˷���CMD_LOGIN_CHECK_GUEST_CS���󣬰���Я���û�ID + ����
*       ��������redis��ȡ��guest-uid��Ӧ�����룬������֤
*       ��֤�󷵻�CMD_LOGIN_CHECK_SC������Я��UID����֤ʧ��ʱΪ-1
*
*   2.3 �οͰ󶨵�ƽ̨
*       �ͻ����ȵ���ƽ̨SDK���е�¼���õ�token��openid
*       �ͻ�������CMD_LOGIN_CHECK_BINDGUEST_CS������Я��ƽ̨���͡�token��openid���ο�uid���ο�����
*       ��������֤�󷵻�CMD_LOGIN_CHECK_SC������Я��UID����֤ʧ��ʱΪ-1
*       ����������֤����Ҫ���ο͵��˺�����keyɾ����
*
*   3.ƽ̨��¼У��
*       �ͻ����ȵ���ƽ̨SDK���е�¼���õ�token��openid
*       �ͻ�������CMD_LOGIN_CHECK_PF_CS������Я��ƽ̨���͡�token��openid
*       ��������֤�󷵻�CMD_LOGIN_CHECK_SC������Я��UID����֤ʧ��ʱΪ-1
*
*   ��У��ͨ��֮�󣬿ͻ���ͳһ���������ĵ�¼����CMD_LOGIN_CS����ȡ�û�����
*/

enum LOGINPROTOCOL
{
    CMD_LOGIN_CSBEGIN,
    CMD_LOGIN_CHECK_TEST_CS,        //���Ե�½��֤��UID��¼��ֱ��ͨ����
    CMD_LOGIN_CHECK_PF_CS,          //ƽ̨��¼��֤
    CMD_LOGIN_CHECK_GUEST_CS,       //�ο͵�¼��֤
    CMD_LOGIN_CHECK_BINDGUEST_CS,   //��ƽ̨�˺Ű󶨵�ָ���ο�
	CMD_LOGIN_EXISTUSER_CS,			//ʹ�������˺������½
    CMD_LOGIN_CS = 6,				//��֤ͨ���������¼
    CMD_LOGIN_UNION_CS,             //��½����
    CMD_LOGIN_RECONECT_CS,		    //��½����
    CMD_LOGIN_USERINFO_CS,			//�ύ�û���Ϣ
    CMD_LOGIN_GUIDEINFO_CS,			//�ύ������Ϣ
    CMD_LOGIN_FRESH,				//��½���ݸ���
    CMD_LOGIN_CHECK_CHAT_CS,        //�����������֤
    CMD_LOGIN_CSEND,

    CMD_LOGIN_SCBEGIN = 100,
    CMD_LOGIN_CHECK_SC,			    //��֤Ӧ��
    CMD_LOGIN_NEWGUEST_SC,          //ע���οͳɹ�
    CMD_LOGIN_FINISH_SC,		    //��¼���
    CMD_LOGIN_SC,				    //��������·�
    CMD_LOGIN_USERMODEL_SC,		    //���ģ������
    CMD_LOGIN_BAGMODEL_SC,		    //����ģ������
    CMD_LOGIN_EQUIPMODEL_SC,	    //װ��ģ������
    CMD_LOGIN_SUMMONMODEL_SC,	    //�ٻ�ʦģ������
    CMD_LOGIN_HEROMODEL_SC,		    //Ӣ��ģ������
    CMD_LOGIN_STAGEMODEL_SC,	    //�ؿ�ģ������
    CMD_LOGIN_TEAMMODEL_SC,         //����ģ������
    CMD_LOGIN_TASKMODEL_SC,         //����ģ������
    CMD_LOGIN_ACHIEVEMODEL_SC,      //�ɾ�ģ������
    CMD_LOGIN_GUIDEMODEL_SC,	    //����ģ������
    CMD_LOGIN_UNIONMODEL_SC,	    //����ģ������
    CMD_LOGIN_MAILMODEL_SC, 	    //�ʼ�ģ������
    CMD_LOGIN_INSTANCEMODEL_SC,	    //�����ģ��
    CMD_LOGIN_GOLDTESTMODEL_SC,     //�����������ģ��
    CMD_LOGIN_HEROTESTMODEL_SC,	    //Ӣ����������ģ��
    CMD_LOGIN_TOWERTESTMODEL_SC,    //������������
    CMD_LOGIN_PVPMODEL_SC,		    //pvpģ������
    CMD_LOGIN_SHOPMODEL_SC,		    //�̵�ģ������
    CMD_LOGIN_ACTIVEMODEL_SC,	    //�ģ������
    CMD_LOGIN_BAN_SC,			    //��������ͣӦ��
    CMD_LOGIN_RECHANGE_SC,		    //��������ɫ������Ӧ��
    CMD_LOGIN_TICK_SC,			    //��������ɫ��TӦ��
	CMD_LOGIN_HEAD_SC,				//������ͷ������Ӧ��
	CMD_LOGIN_UNIONSHOP_SC,			//�����������̵�����Ӧ��
	CMD_LOGIN_7DAYACTIVE_SC,		//��������Ӧ��
	CMD_LOGIN_PVPCHESTMODEL_SC,		//pvp����
	CMD_LOGIN_CHECK_CHAT_SC,        //�����������֤Ӧ��
	CMD_LOGIN_QQACTIVE_SC,			//QQ���������·�
    CMD_LOGIN_SCEND
};


// ���������֤ CMD_LOGIN_CHECK_TEST_CS
struct LoginCheckTestCS
{
    int uid;
};

// �����ο���֤ CMD_LOGIN_CHECK_GUEST_CS
struct LoginCheckGuestCS
{
	char szMobile[40];				//�豸��
    char password[32];
};

// ����ƽ̨��֤ CMD_LOGIN_CHECK_PF_CS
// ʹ�������˺����� CMD_LOGIN_EXISTUSER_CS
struct LoginCheckPlatformCS
{
    int nChannelID;				//����ID
    char openid[128];
    char openkey[256];
};

// ������֤���󶨵��ο� CMD_LOGIN_CHECK_BINDGUEST_CS
struct LoginCheckBindGuestCS
{
    int nChannelID;
    int uid;
	char szMobile[40];
    char password[32];
    char openid[128];
    char openkey[128];
};

// ��֤�ɹ����� CMD_LOGIN_CHECK_SC
struct LoginCheckSC
{
    int uid;
	int nIsNew;				//Ĭ��0��ʶ�����û���1����ʶ���û�
	int nIsGuest;			//Ĭ��0�����οͣ�1-�ο�
};

//CMD_LOGIN_CS
struct LoginCS
{
    int userid;
};

//CMD_LOGIN_SC
struct LoginSC
{
    int serverStamp;
};

//CMD_LOGIN_USERINFO_CS,			//�ύ�û���Ϣ
struct LoginUserInfoCS
{
	int nIsGuest;					//�ǲ����οͣ�0���ǣ�1-��
	int nUid;
	int nIsNew;						//�ǲ������û���0-���ǣ�1-��
	int nChannelID;
	char szChannelName[128];
	char szMobile[40];
};

// CMD_LOGIN_GUIDEINFO_CS
struct LoginGuideInfoCS
{
    int nStepId;
};

//CMD_LOGIN_USERMODEL_SC
struct LoginUserModelInfo
{
    int userId;				      // ���id
    int headId;				      // ���ͷ��
    int	userExp;			      // ��Ҿ���
    int gold;				      // ��ҽ��
    int diamond;			      // �����ʯ
    int towerCoin;				  // ����
    int pvpCoin;				  // pvp��
    int unionContrib;			  // ���ṱ��
	int flashcard10;			  // 10���鿨ȯ
    int flashcard;				  // �鿨ȯ

	int resetTimeStamp;			  // �ָ�ʱ���(������ҹ������, �����������, �����ָ�)
    unsigned short sbagCapacity;  // �ֱ�������
	unsigned char  cuserLv;		  // ��ҵȼ�
    unsigned char  cfreeHeroTimes; //��ѳ鿨����
    unsigned char changeNameFree; // 0Ϊ���,1Ϊ��Ҫ����
	unsigned short sbuyGoldTimes; // �����Ҵ���

	int payment;				  // ��ֵ���
    int monthCardStamp;           // �¿�����ʱ���
	int nTotalSignDay;			  //�ۼ�ǩ������
	int nMonthSignDay;			  //�����ۼ�ǩ������
	int nTotalSignFlag;			  //�ۼ�ǩ���Ĵ���

	int nFondFlag;				  //������
	unsigned char cDaySignFlag;	  //�Ƿ��Ѿ�ǩ����0-δǩ����1-�Ѿ�ǩ��
	unsigned char cFirstPayFlag;  //�׳���
	unsigned char cBDType;		  //�������� //�������� 1,3,5,7
	unsigned char cBDLv;		  //����ȼ�
	int nBDTimeStamp;			  //�������ʱ���
    char name[32];
};

//CMD_LOGIN_BAGMODEL_SC
struct LoginBagModelInfo
{
    unsigned short scurCapacity;
    unsigned short count;
};

struct BagItemInfo
{
    int id;
    int val;
};

// CMD_LOGIN_EQUIPMODEL_SC
struct LoginEquipModelInfo
{
    int count;
    // ... EquipItemInfo
};

struct EquipItemInfo
{
    int equipId;
    int confId;
    unsigned char cMainPropNum;		//�����Ը���
    unsigned char cEffectID[8];
    unsigned short sEffectValue[8];
};

//CMD_LOGIN_SUMMONMODEL_SC
struct LoginSummonModelInfo
{
    unsigned char count;
    // summonerids;
};

//CMD_LOGIN_HEROMODEL_SC
struct LoginHeroModelInfo
{
    unsigned char count;
    //HeroInfo list
};

struct LoginHeroInfo
{
    int heroId;                   // Ӣ��id
	unsigned short heroFrag;	  // Ӣ����Ƭ
    unsigned char  heroLv;        // �ȼ�
    unsigned char  heroStar;      // �Ǽ�
    int heroExp;                  // ����
    unsigned char heroTalent[8];  // �츳
	int equip[6];				  // װ������, һ����6��, δװ��Ϊ0
};

//CMD_LOGIN_STAGEMODEL_SC
struct LoginStageModelInfo
{
    int curStage;							  // ��ǰ��ͨ�ؿ�id
    int curElite;							  // ��ǰ��Ӣ�ؿ�id
    unsigned short chapterCount;			  // ��ͨ�½ڸ���
	unsigned short eliteChapterCount;		  // ��Ӣ�½ڸ���
	unsigned short stageCount;				  // ��ͨ�ؿ�����
	unsigned short eliteCount;				  // ��Ӣ�ؿ�����
	unsigned short eliteRecordCount;		  // ��Ӣ�ؿ���¼����
	
    //ChapterInfo list
    //EliteChapterInfo list
    //StageInfo list
    //EliteInfo list
    //EliteRecordInfo List
};

struct ChapterStatusInfo
{
    int chapterID;				  // �½�id
    unsigned char chapterStatus;  // �½�״̬
	unsigned char chapterboxCount;// �½ڱ�����Ŀ
	//id unsigned char List;	  // ��������ֵ
};

struct EliteChapterStatusInfo
{
    int				chapterID;				  // �½�id
	unsigned char	chapterStatus;			  // ��Ӣ�½�״̬ 
};

struct StageStatusInfo
{
    int			  stageId;				  // �ؿ�id
	unsigned char stageStatus;			  // �ؿ�״̬
};

struct EliteStatusInfo
{
    int			  stageId;				  // �ؿ�id
	unsigned char stageStatus;			  // �ؿ�״̬
};

struct EliteRecordInfo
{
    int		stageId;				  // ��Ӣ�ؿ�id
	char	canUseTimes;			  // ��ʹ�ô���
    int		useStamp;				  // �ϴ�ʹ�õ�ʱ���
	char	buyTimes;				  // �������
    int		buyStamp;				  // ����ʱ���
};

//CMD_LOGIN_TEAMMODEL_SC
struct LoginTeamModelInfo
{
    int teamCount;                // ������(�������͵Ķ���)
    // TeamInfo list
};

struct TaskInfo
{
    int taskID;                 // �ɾ�ID
    int taskVal;                // ��ֵ
    int taskStatus;             // ״̬ETaskStatus
    int resetTime;              // ���õ�ʱ���
};

//CMD_LOGIN_TASKMODEL_SC
struct LoginTaskModelInfo
{
    int taskCount;               // �������
    // TaskInfo list
};

struct AchieveInfo
{
    int achieveID;               // �ɾ�ID
    int achieveVal;              // �ۼ���ֵ
    int achieveStatus;           // �ɾ�״̬EAchieveStatus
};

//CMD_LOGIN_ACHIEVEMODEL_SC
struct LoginAchieveModelInfo
{
    int achieveCount;            // �ɾ͸���
    // AchieveInfo list
};

struct LoginGuideInfo
{
    int nNum;				//��������
    //����ID
};

// CMD_LOGIN_UNIONMODEL_SC
struct LoginUnionModelInfo
{
    char hasUnion;                 // �Ƿ�ӵ�й���(0û��1��)
    // 0->NoUnionInfo 1->OwnUnionInfo
};

struct NoUnionInfo
{
    char applyCount;             // ����Ĺ����б�����
    int todayApplyCount;         // �����������
    int applyStamp;              // ����ʱ���
    // applyCount ->ApplyInfo
};

struct ApplyInfo
{
    int applyTime;                 // �������ʱ��
    int unionID;                   // ����ID
};

struct OwnUnionInfo
{
    int unionID;                   // ����Id
    int todayStageLiveness;        // ���չؿ���Ծ��
    int todayPvpLiveness;          // ����pvp��Ծ��
    int totalContribution;         // �ۼƹ���
    int unionLiveness;             // �����Ծ��
    int originUnionLv;             // �������ԭʼ�ȼ�
    int unionLv;                   // ����ȼ�
    int welfareTag;                // ������ȡ��ʶ
    int emblem;                    // ���
    char pos;                      // �����ߵĹ���ְλ
    char hasAudit;                 // �Ƿ��������Ϣ
	char hasExpiditionReward;	   // �Ƿ���Զ������
	unsigned int nExFinishTime;		//Զ������ʱ��
	unsigned int nExRestFinishTime;	//��Ϣ����ʱ��
    char unionName[20];            // ��������
    char notice[128];              // ����
};

// CMD_LOGIN_MAILMODEL_SC
struct LoginMailModelInfo
{
    char normalMailCount;        // ��ͨ�ʼ�
    char webMailCount;           // web�ʼ�
    // normalMailCount ->LoginMailInfo
    // webMailCount ->LoginMailInfo
};

struct LoginMailInfo
{
	LoginMailInfo()
	{
		memset(this, 0, sizeof(*this));
	}
	int nMailID;						//�ʼ�ID
	int mailConfID;                     // �ʼ�����ID
	int sendTimeStamp;                  // ����ʱ���
	char szTitle[32];					// �ʼ�����
};

//CMD_LOGIN_INSTANCEMODEL_SC
struct LoginInstanceModelInfo
{
    int instanceCount;		//���������
};

struct InstanceInfo
{
    int		activityId;		 //�����id
    int		useTimes;		 //��ս����
    int		useStamp;		 //��սʱ���
    int		buyTimes;		 //�������
    int		buyStamp;		 //����ʱ���
    int		easy;			 //�򵥻���Ǽ�
    int		normal;			 //��ͨ����Ǽ�
    int		difficult;		 //���ѻ���Ǽ�
    int		hell;			 //��������Ǽ�
    int		legend;			 //��˵����Ǽ�
};

//CMD_LOGIN_GOLDTESTMODEL_SC
struct LoginGoldTestModelInfo
{
    int nUseTimes;		//��ս����
    int nUseStamp;		//ʱ���
    int nDamage;		//���˺�
    int nChestData;		//����״̬
};

//CMD_LOGIN_HEROMODEL_SC
struct HeroTestTimesInfo
{
    int nInstanceId;
    int nTimes;
};
struct LoginHeroTestModelInfo
{
    int nUseStamp;
    int nTimesNum;
    /*nTimesNum * HeroTestTimesInfo*/
};

//CMD_LOGIN_TOWERTESTMODEL_SC
struct LoginTowerTestModelInfo
{
    int floor;			   //��ǰ¥����
};

struct TowerOuterBonusInfo
{
    int id;
};

//CMD_LOGIN_PVPMODEL_SC
struct LoginPvpModelInfo
{
    int battleId;					//ս��id, ���ս��id��Ϊ0, �����������
    int rank;						//��ǰ����
    int integral;					//��������
    int continusWinTimes;			//����ʤ��
	int historyHighestRank;			//��ʷ�������
	int historyHighestIntegral;		//��ʷ��߻���
    int historyContinusWinTimes;    //��ʷ�����ʤ

	//int cpnRank;					//����������
	//int cpnWeekResetStamp;			//������������ʱ��
	//int cpnGradingNum;				//����������������
	//int cpnGradingDval;				//�������������ֲ�
	//int cpnIntegral;				//��������������
	//int cpnContinusWinTimes;		//��������λ����ʤ��
	//int cpnHistoryHigestRank;		//��������ʷ�������
	//int cpnHistoryHigestIntegral;	//��������ʷ��߻���
	//int cpnHistoryContinusWinTimes;	//��������ʷ�����ʤ����

	int dayResetStamp;				//������ʱ��
	int dayWinTimes;				//��ʤ��
	int dayContinusWinTimes;		//������ʤ����
	int dayMaxContinusWinTimes;		//�������ʤ����
	int dayBattleTimes;				//��ս������
	int dayBuyChestTimes;			//�ձ��乺�����
	int lastChestGenStamp;			//�����ʱ��
	int chestStatus;				//����״̬
	int chestInsurance;				//���䱣�մ���
	int rewardFlag;					//�ս�����ȡ��ʾ��
};

// CMD_LOGIN_PVPTASKMODEL_SC
struct LoginPvpTaskModelInfo
{
	int taskCount;
};

// CMD_LOGIN_PVPCHESTMODEL_SC
struct LoginPvpChestModelInfo
{
    char chestCount;
};

//CMD_LOGIN_SHOPMODEL_SC
struct LoginShopModelInfo
{
    unsigned char cCount;					//�̵����
    //�̵�����
};

//�̵�ṹ
struct SLoginShopData
{
	unsigned int    nTimes;					//�Ѿ�ˢ�µĴ���
	int				nNextFreshTime;			//�´�ˢ��ʱ���
    unsigned char	cShopID;				//�̵�ID
    unsigned char   cCurCount;				//��ǰ��Ʒ��
    //��Ʒ�ṹ�б�
};

//��Ʒ�ṹ
struct SLoginShopGoodsData
{
    int				nGoodsID;				//����ID
    int				nGoodsNum;				//���߸���
	int				nCoinNum;				//�۸�
	unsigned short	sGoodsShopID;			//��ƷID
    unsigned char	cCoinType;				//��������
	unsigned char	cIndex;					//��Ʒ����
    unsigned char	cSale;					//�ۿ�ֵ
};

struct SLoginDiamondData
{
	int				nTimeStamp;
	unsigned char	cPid;
};

//��ʯ�̵�����
struct SLoginDiamondShopData
{
	unsigned char cNum;					//�Ѿ���ֵ�ĸ���
	//PID+ʱ���
};

//CMD_LOGIN_ACTIVEMODEL_SC
struct SLoginActiveSC
{
    unsigned short sCount;				//�����
};

struct SLoginActiveData
{
	int			   nStartTime;		//��ʼʱ��
	int			   nEndTime;		//����ʱ��
	unsigned short sActiveID;
    unsigned char  cActiveType;
	unsigned char  cTimeType;		//�ʱ������
    unsigned char  cLevLimit;		//��ȼ�����
    //�������� SLoginActiveShopData/SLoginActiveTaskData
};

struct SLoginActiveShop
{
    unsigned short sShopNum;				//�������
    //����������� SLoginActiveShopData
};

struct SLoginActiveShopData
{
    int nGoodsID[4];
    int nGoodsNum[4];
    int nPrice;
	unsigned char cGiftID;
	unsigned char cGoldType;					//��������
    unsigned char cSaleRate;
    unsigned char cMaxBuyTimes;
    unsigned char cBuyTimes;					//�Ѿ��������
};

struct SLoginActiveTask
{
    unsigned short sActiveTaskNum;				//�������
};

struct SLoginActiveTaskData
{
    int						nConditionParam[2];
    int						nRewardGold;
    int						nRewardGoodsID[4];
    int						nRewardGoodsNum[4];
    int						nValue;					//������ɽ���
	unsigned short			sFinishCondition;
	unsigned short			sRewardDimand;
	unsigned char			cTaskID;
    unsigned char			cFinishFlag;			//�Ƿ���ȡ��0-δ��ȡ��1-��ȡ
};

struct SLoginActiveMonthCard
{
	unsigned char cNum;					//�¿������
};

struct SLoginActiveMonthCardData
{
	int nMonthCardID;				//�¿�ID
	int nRewardTime;				//�¿���ȡʱ��
	int nFinishTimestamp;			//�¿���ֵʱ��
	unsigned char cType;			//�¿�����(0-���ã�1-ʱ��)
};

struct SLoginExChangeActive
{
	unsigned char cNum;
};

struct SLoginAcitveExChangeData
{
	unsigned short sActiveID;
	unsigned char  cTaskID;
	unsigned char cCurTimes;
};

//CMD_LOGIN_BAN_SC
//���ֽ�����

struct LoginHeadInfo
{
	int nNum;				//ͷ�����
	//ͷ��ID
};

//CMD_LOGIN_UNIONSHOP_SC
///�����̵�ṹ
struct SLoginUnionShopGoodsData
{
	int				nGoodsID;			    //����ID
	int			   nCoinNum;				//�۸�
	unsigned short sGoodsShopID;			//��ƷID
	unsigned char cGoodsNum;				//���߸���
	unsigned char cCoinType;				//��������
	unsigned char cSale;					//�ۿ�ֵ
};

//�����̵�����
struct SLoginUnionShop
{
	unsigned short sNum;
	unsigned char cFlag;		//0-��ͨ״̬��1-�����ϼ�״̬��2-�״�
	//SLoginUnionShopGoodsData
};

//CMD_LOGIN_7DAYACTIVE_SC
struct SLogin7DayActiveSC
{
	unsigned int Day;			//�����
	unsigned short sNum;
	//�����
};

struct SLogin7DayActiveData
{
	unsigned short sActiveID;
	unsigned char  cActiveType;
	//�������� SLoginActiveShopData/SLoginActiveTaskData
};

//CMD_LOGIN_QQACTIVE_SC
struct SLoginBDActiveSC
{
	int nNum;
	//��������
};

struct SLoginBDActiveData
{
	int nActiveID;				//�ID
	int nTaskID;				//������
	int nFlag;					//����
};

// ���������֤ CMD_LOGIN_CHECK_CHAT_CS
struct LoginCheckChatCS
{
    int uid;
};

#pragma pack()

#endif //__LOGIN_PROTOCOL_H__
