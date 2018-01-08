#ifndef __GAMEDEF_H__
#define __GAMEDEF_H__

//#define GAME_QQ_BLUEDIAMOND			//����汾����

#define DEFAULT_EQUIPREF		1000000			//װ����СΨһid
#define ENERGY_RECOVER_TIME		300		        //�����ָ�ʱ�� 5����=300��
#define DAYSECOND				86400			//������
#define WEEKSECOND				604800			//������
#define ROOM_PACKAGE_LEN		4096			//������������
#define MAX_CHEST_NUM			5				//���pvp���Ӹ���

//�ӳٴ�������
#define CHECK_TIME_INTERVAL 300		//У��ʱ����
#define DELAY_TIME_NUM	3600
#define INFINITY_NUM   0x7FFFFFFF	//�������޴�����ʵ��Ϊ�з����������ֵ

#define DEFAULT_USER_ID 10000			//�û���СID

#define MONTH_CHECK_NUM 26				//��ǩ�������
#define MAX_MAIL_NUM 50					//����ʼ�����
#define FOND_PID 9						//��������
#define MONTH_PID 7						//�¿�PID

#define MAX_RANK_NUM	10000			//���а������
#define MAX_RANK_SHOW_NUM 50			//�·���ʾ���а�����
#define MAX_TOWERRANK_INDEX 9999999		//�������а����ʱ�������ݼ�

//����
enum EMoneyType
{
	MONEY_NON,                            // 
	MONEY_DIAMOND,                        // ��ʯ
	MONEY_GOLD,                           // ���
};

//�鿨��ʽ
enum ELotteryType
{
	LOTTERY_1,                            // ��һ��(���������)
	LOTTERY_10,                           // ��ʮ��
};

//��������, ������
enum PropType
{
	PROPTYPE_NONE = 0,			//��
	PROPTYPE_EQUIP,				//װ��
	PROPTYPE_EQUIPMATERIAL,		//װ������
	PROPTYPE_HEROCARD,			//Ӣ�ۿ�Ƭ
	PROPTYPE_SUMMONERCARD,		//�ٻ�ʦ��Ƭ
	PROPTYPE_EXPBOOK,			//������
	PROPTYPE_SKILLBOOK,			//������
	PROPTYPE_GOLDBAG,			//��Ҵ���
	PROPTYPE_ENERGYBAG,			//��������
	PROPTYPE_EXPBAG,			//��Ҵ���
	PROPTYPE_DIAMONDBAG,		//��ʯ����
	PROPTYPE_TREASURE,			//����
	PROPTYPE_MATERIAL,			//����
	PROPTYPE_EQUIP_CREATE,		//װ����������
	PROPTYPE_RESOURCE,			//��Դ����,���/��ʯ��
	PROPTYPE_HEROFRAGMENT,		//Ӣ����Ƭ
	PROPTYPE_HEAD,				//ͷ����Դ����
};

//��ԴID
enum ResourceID
{
    RESOURCEID_GOLD = 1,					//���
    RESOURCEID_DIAMOND = 2,					//��ʯ
	RESOURCEID_PVPCOIN = 3,					//PVP��
	RESOURCEID_TOWER = 4,					//������
	RESOURCEID_ENERGY = 5,					//����
	RESOURCEID_UNION = 6,					//���ṱ��
	RESOURCEID_EXP = 7,						//����
	RESOURCEID_xxxxx= 8,				    //(δʹ��)
	RESOURCEID_FLASHCARD10 = 9,				//10�鿨��
	RESOURCEID_FLASHCARD = 10,				//�鿨��
};

// ϡ�ж�
enum CARDRARITY
{
	RARITY_NONE = 0,		//ϡ�ж�
	RARITY_WHITE,			//��
	RARITY_GREEN,			//��
	RARITY_BLUE,			//��
	RARITY_PURPLE,			//��
	RARITY_GOLDEN,			//��
};

//����ʹ������
enum ItemUseType
{
	USETYPE_UNUSE = 0,		//����ʹ��
	USETYPE_BAGUSE,			//����ʹ��
	USETYPE_ATONCE,			//����ʹ��
	USETYPE_EXPTOCARD,		//��������ʹ��
	USETYPE_TOEQUIP,		//װ���ϳ�ʹ��
};

//����ģ��
enum ETeamType
{
	ETT_PASE,				// ͨ�ض���
	ETT_SPORTE				// ��������
};

//�ؿ���ս���
enum ChallengeResult
{
	CRESULT_CANCEL = -1,
	CRESULT_LOSE = 0,
	CRESULT_WIN = 1,
};

//�½�����
enum ChapterType
{
	CHAPTERTYPE_NONE,		//��
	CHAPTERTYPE_NORMAL,		//��ͨ�½�
	CHAPTERTYPE_ELITE,		//��Ӣ�½�
};

//�ؿ�����
enum StageType
{
	STAGETYPE_REWARDONE = 0,  //���潱��1��
	STAGETYPE_REWARDMULTI,    //���潱�������������ͨ����Ӣ�͸����ؿ���
	STAGETYPE_TOWER,		  //����������������ͨ����Ӣ�͸����ؿ���
	STAGETYPE_GOLD,			  //�������
	STAGETYPE_HERO,			  //Ӣ������
    STAGETYPE_UNION,          //����(����)��ս
};

//�ָ�����
enum TimesRecoverType
{
	RCVTYPE_PERSECOND = -2,	//ÿ��n��ָ�һ��
	RCVTYPE_OPEN,			//����ʱ�ָ�
	RCVTYPE_DONTRECOVER,	//���ָ�
	RCVTYPE_DAY,			//�ջָ�
	RCVTYPE_WEEK,			//�ָܻ�
};

//�Ѷ�����
enum DifficultyType
{
	DIFFICULTY_EASY = 0,	//���Ѷ�
	DIFFICULTY_NORMAL,		//��ͨ�Ѷ�
	DIFFICULTY_DIFFICULT,	//�����Ѷ�
	DIFFICULTY_HELL,		//�����Ѷ�
	DIFFICULTY_LEGEND,		//��˵�Ѷ�
};

//
enum TowerDiffculty
{
	TOWERDIFFCULTY_NONE = 0, //��
	TOWERDIFFCULTY_1,		 //�Ѷ�1
	TOWERDIFFCULTY_2,		 //�Ѷ�2
	TOWERDIFFCULTY_3,		 //�Ѷ�3
};

// ������������
enum EResetTaskType
{
	ERTT_NON,                   // ������
	ERTT_DAY,                   // ������
	ERTT_WEEK,                  // ������
};

// ����״̬(״̬������,��ǰ�˼���)
enum ETaskStatus
{
	ETASK_UNACTIVE = -1,       // δ����
	ETASK_ACTIVE,              // ����/����״̬
	ETASK_FINISH,              // ���(����ȡ)
	ETASK_GET,                 // �Ѿ���ȡ
};

enum EDayLiveness
{
    UNION_DAY_PVPLIVENESS,
    UNION_DAY_STAGELIVENESS,
};

// ����ת��������
enum EUnionForwardData
{
    UNION_FORWARD_PVPLIVENESS,         // ����pvp��Ծ��
    UNION_FORWARD_STAGELIVENESS,       // ���չؿ���Ծ��
    UNION_FORWARD_USERLV,              // ��ҵȼ�
    UNION_FORWARD_USERNAME,            // �������
    UNION_FORWARD_CONTRIBUTION,        // ��ҹ���
};

enum EUnionMessageType
{
    UNION_MESSAGE_LIVENESS,            // ��Ծ�� 
    UNION_MESSAGE_AUDIT,               // ���
	UNION_MESSAGE_MERCENARYDISPATCH,   // ����Ӷ����ǲ
	UNION_MESSAGE_MERCENARYRecall,	   // ����Ӷ���ٻ�
};

// �ؿ���������
enum EStageTaskType
{
    STAGE_TASK_TYPE_NON = -3,		// ��
    STAGE_TASK_TYPE_ELITE = -2,		// ��Ӣ�ؿ�
    STAGE_TASK_TYPE_NORMAL,			// ��ͨ�ؿ�
    STAGE_TASK_TYPE_ANY,			// ����ؿ�
};

enum EPvpDan
{
	DAN_NEWBIE = 1,					//���ֶ�λ
	DAN_BRONZE,						//��ͭ��λ
	DAN_SILVER,						//������λ
	DAN_GOLD,						//�ƽ��λ
	DAN_PLATINUM,					//�׽��λ
	DAN_DIAMOND,					//��ʯ��λ
	DAN_KING,						//���߶�λ
};

enum EPVPRewardType
{
	PRT_DAYWINTIMES = 0,			//���ۼ�ʤ������ 
	PRT_DAYCONTINUSWINTIMES,		//����ʤ������
	PRT_DAYRANK,					//����������
	PRT_NEWHIGHEST,					//��ʷ�¸߽���
	PRT_DAYBATTLETIMES,				//��ս�����ν���
};

//�̵깺���������
enum EBUYCOINTYPE
{
	BUY_GOLD_TYPE = 1,				//��ҹ�������
	BUY_PVP_TYPE,					//������
	BUY_TOWER_TYPE,					//����
	BUY_DIAMOND_TYPE,				//��ʯ��
	BUY_UNION_TYPE,					//�����
};

// �ʼ�״̬
enum EMailStatus
{
    MAIL_STATUS_UNREAD,				// δ��
    MAIL_STATUS_READ,				// �Ѷ�
};

//�̵�ˢ������
enum EFRESHTYPE
{
	EAUTOFRESH_TYPE,				//�Զ�ˢ��
	EMANNALFRESH_TYPE,				//�ֶ�ˢ��
};

// �ʼ�����
enum EMailType
{
    MAIL_TYPE_NORMAL,				// ��ͨ�ʼ�(�����������)
    MAIL_TYPE_WEB,					// web�ʼ�(�ֶ���д���⡢���ݵ�)
};

// �ʼ�����ID
enum EMailConfID
{
    MAIL_CONFID_NON,                // 
    MAIL_CONFID_FULLBAG,            // ��������
    MAIL_CONFID_RESETTASK,          // ��������
	MAIL_CONFID_PVPRANK,			// ����������
    MAIL_CONFID_UNIONKICK,          // ��������
    MAIL_CONFID_UNIONDISMISS,       // �����ɢ
	MAIL_CONFID_TOWERRANK,			// ��������
};

//PVPʤ��ʧ�ܱ�ʶ
enum EPvpBattleFlag
{
	EPVP_NONE_BATTLE_FLAG,
	EPVP_WIN_BATTLE_FLAG,			//ʤ����ʶ
	EPVP_FAILED_BATTLE_FLAG,		//ʧ�ܱ�ʶ
	EPVP_DRAW_BATTLE_FLAG,			//ƽ�ֱ�ʶ
};

enum EClientLoginType
{
	NORMAL_LOGIN_TYPE,				//Ĭ���ֻ���½
	WEIXIN_LOGIN_TYPE,				//΢�ŵ�½
};

enum MailFiledType
{
	MAIL_NONE_TYPE,		//��
	MAIL_BASE_TYPE,		//��������
	MAIL_ADDGOODS_TYPE,	//��������
	MAIL_CONTEXT_TYPE,  //�ʼ�����
	MAIL_GMID_TYPE,		//GM�ʼ�״̬
};

enum EActiveTaskType
{
	ACTIVE_TASK_NORMAL_TYPE,			//��ͨ��������
	ACTIVE_TASK_GROWUP_TYPE,			//�ɳ���������
};

// ����Ʒ��      
enum EItemQuality
{
    EITEM_QUALITY_WHITE = 1,         // ��
    EITEM_QUALITY_GREEN,             // ��
    EITEM_QUALITY_BLUE,              // ��
    EITEM_QUALITY_PURPLE,            // ��
    EITEM_QUALITY_GOLD,              // ��
    EITEM_QUALITY_ORANGE,            // ��
    EITEM_QUALITY_PLATINA,           // �׽�
};

enum ENoticeID
{
    // ϵͳ��
    NOTICE_SYSTEM,
    NOTICE_SYSTEM_CHAMPIONSHIP_START,         // ��������ʼ
    NOTICE_SYSTEM_END,
    // ������
    NOTICE_UNION,
    NOTICE_UNION_NEWMEMBER = NOTICE_SYSTEM_END, // �³�Ա����
    NOTICE_UNION_QUIT,                        // ��Ա�˳�
    NOTICE_UNION_APPOINT,                     // ����
    NOTICE_UNION_TRASFER,                     // תְ
    NOTICE_UNION_NOTICEUPDATE,                // �������
    NOTICE_UNION_EXPIDITION_SET,              // Զ������
    NOTICE_UNION_EXPIDITION_START,            // Զ����ʼ
    NOTICE_UNION_EXPIDITION_SUCCESS,          // Զ��ʤ��
    NOTICE_UNION_EXPIDITION_FAIL,             // Զ��ʧ��
    NOTICE_UNION_EXPIDITION_AWARD,            // Զ������
    NOTICE_UNION_END,
    // �����
    NOTICE_USER,
    NOTICE_USER_DRAWCARD_PURPLE = NOTICE_UNION_END, // �鿨����Ͽ�
    NOTICE_USER_DRAWCARD_GOLD,                // �鿨��ý�
    NOTICE_USER_FORGE_PURPLE,                 // ��������װ
    NOTICE_USER_FORGE_GOLD,                   // �����ý�װ
};

enum ETalentID
{
    // �����츳
    TALENT_HUMAN = 1,                      // ����
    TALENT_NATURE,                         // ��Ȼ
    TALENT_GHOST,                          // ����
    
    // ְҵ�츳
    TALENT_SOLIDER = 101,                  // սʿ
    TALENT_ASSASSIN,                       // �̿�
    TALENT_SHOOT,                          // ����
    TALENT_MAGIC,                          // ��ʦ
    TALENT_ASSIST,                         // ����
    TALENT_BODYGUARD,                      // ��ʿ
};

//��ֵ
enum PAYBACK_TYPE
{
	PAYBACK_INVALID_TYPE = -1,	//��ֵ�쳣
	PAYBACK_FAILE_TYPE,		//��ֵʧ��
	PAYBACK_SUCESS_TYPE,	//��ֵ�ɹ�
};

#endif  //__GAMEDEF_H__
