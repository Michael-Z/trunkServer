/*
* �����ṹͷ�ļ�
* ǰ��˹��õ����ݽṹ
* ��ֱ������Э��ṹ��
*
* 2015-2-5 By ��ү
*/
#ifndef __COMM_STRUCTS_H__
#define __COMM_STRUCTS_H__

#if KX_TARGET_PLATFORM == KX_PLATFORM_LINUX
#include<string.h>
#endif

enum ERoomState
{
	ROOMSTATE_NONE,					//��
	ROOMSTATE_NOROOM,				//���䲻����
	ROOMSTATE_PREPARELOAD,			//�ȴ���ҽ��뷿��
	ROOMSTATE_LOADING,				//������
	ROOMSTATE_IDLETIME,				//ս��Ԥ��(��������ʱ���)
	ROOMSTATE_FIGHTING,				//ս����
	ROOMSTATE_GAMEOVER,				//��Ϸ����
};
// ע��: �ͻ��˵�ͬƥ������,
// ���Դ�����Ҫ��ƥ�����Ͷ�Ӧ
enum EPvpRoomType
{
	PVPROOMTYPE_NONE,
	PVPROOMTYPE_PVP,				//pvp����		
	PVPROOMTYPE_CHAMPIONSHIP,		//����������
	PVPROOMTYPE_ROBOT,				//�����˷���
};

//PVP��ս���
enum ChallengeResultType
{
	CHALLENGE_CANCEL = -1,  //��սȡ��
	CHALLENGE_FAILED,		//��սʧ��
	CHALLENGE_WON,			//��ս�ɹ�
	CHALLENGE_DRAW,			//ƽ��
};

#pragma pack(1)

// ��ս����
enum EBattleType
{
	EBATTLE_NONE,			   // ��
	EBATTLE_CHAPTER,		   // �½ڹؿ�
	EBATTLE_PVP,			   // ƥ���ս
	EBATTLE_INSTANCE,		   // �����ؿ�
	EBATTLE_GOLDTEST,		   // �������
	EBATTLE_HEROTEST,		   // Ӣ������
	EBATTLE_TOWERTEST,		   // ��������
	EBATTLE_UNIONEXPEDITION,		   // ����Զ����ս
	EBATTLE_GUIDE,             // �����ؿ�
	EBATTLE_PVPROBOT,          // pvp������
};

// ս��ָ����Ϣ
// CommandId 1 �ٻ�ʿ�� Ext1 ʿ��ID Ext2 ʿ����·
// CommandId 2 �ͷż��� Ext1 ����ID Ext2 �ͷŵ�Xλ��
// CommandId 3 ����ˮ��
// CommandId 4 �ٻ��ٻ��� Ext1 �ٻ�ID Ext2 �ٻ��ߵȼ� Ext3 X���� Ext4 Y����
// CommandId 5 ˵�� Ext1 �ڼ��仰
enum BattleCommandType
{
	CommandSummoner = 1,
	CommandSkill,
	CommandCrystal,
	CommandCallSolider,
	CommandTalk,
};

struct BattleCommandInfo
{
	int Tick;
	int CommandId;
	int ExecuterId;
	int Ext1;
	int Ext2;
	int Ext3;
	int Ext4;
};

struct SoldierItem
{
	int SoldierID;             //ʿ��ID
};

struct SoldierEquip
{
	int confId;				   //����id
	unsigned char  cEffectID[8];
	unsigned short sEffectValue[8];

	SoldierEquip()
	{
		memset(this, 0, sizeof(SoldierEquip));
	}
};

// Э�顪��ʿ����Ƭ��Ϣ
struct SoldierCardData
{
	int SoldierId;			   // ʿ��id
	int SoldierLv;			   // ʿ���ȼ�
	int SoldierStar;		   // ʿ���Ǽ�
	int SoldierExp;			   // ʿ������
	unsigned char SoldierTalent[8];     // �츳
	int EquipCnt;			   // װ������
};

// Э�顪��Ӣ�ۿ�Ƭ��Ϣ
struct HeroCardData
{
	int HeroId;				   // Ӣ��id
};

// ս��buff
struct BuffData
{
	int Target;
	int BuffId;
	int BuffLv;
};

// Э�顪���û���Ϣ
struct PlayerData
{
	int UserId;				    // ���id
	int UserLv;				    // ��ҵȼ�
	int Camp;				    // �����Ӫ
	int OuterBonusCount;        // OuterBonus����
	int SoldierCount;		    // ���ʿ������
	int MecenaryCount;			// Ӷ��ʿ������
    int Identity;			    // ������(����֮��)
	char UserName[32];		    // �������
	// int*OuterBonusCount
	// HeroCardData*
	// SoldierCardData*
};

// Э�顪��������Ϣ
struct RoomData
{
	int StageId;			   // �ؿ�ID
	int StageLevel;			   // �ؿ��ȼ�
	int BattleType;			   // �����ս����
	int Ext1;				   // ��չ�ֶ�1
	int Ext2;				   // ��չ�ֶ�2
	int BuffCount;			   // Buff����
	int PlayerCount;		   // �����ڵ��������
	// BuffData *
	// PlayerData*
};

//�ʼ��������ݽṹ
struct BaseMailInfo
{
	int mailConfID;            // �ʼ�����ID
	int sendTimeStamp;         // ����ʱ���
	char szTitle[32];		   // �ʼ�����

	BaseMailInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ս����ʼ����
struct ChallengeTeamInfo
{
	int summonerId;				//��ս�ٻ�ʦ
	int heroIds[7];				//Ӣ���б�
	int mercenaryId;			//Ӷ��
};

// ս����������
struct ChallengeBattleInfo
{
	int summonerId;				//�ٻ�ʦ
	int heroIds[7];				//Ӣ���б�
	int mercenaryId;			//Ӷ��
	int timeCost;				//ʱ������
	int crystalCost;			//ˮ������
	int hpPercent;				//�ٻ�ʦѪ���ٷֱ�
	int nCrystalLv;				//ˮ���ȼ�
};

#pragma pack()

#endif