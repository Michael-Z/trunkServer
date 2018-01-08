/*
* ��������(����&�Ŷ�)Э��
*/

#ifndef __UNION_TASK_PROTOCOL_H__
#define __UNION_TASK_PROTOCOL_H__

#include<string>

#pragma pack(1)

enum UNIONTASKPROTOCOL
{
	CMD_UNIONEXPIDITION_CSBEGIN,
	CMD_UNIONEXPIDITION_INFO_CS,			  //����Զ����Ϣ�·�
    CMD_UNIONEXPIDITION_MAPSET_CS,           //����Զ������
	CMD_UNIONEXPIDITION_STAGESTART_CS,       //����Զ���ؿ���ʼ
	CMD_UNIONEXPIDITION_STAGEFINISH_CS,      //����Զ���ؿ�����
	CMD_UNIONEXPIDITION_REWARDGET_CS,        //����Զ��������ȡ
	CMD_UNIONEXPIDITION_DAMAGERANK_CS,       //����Զ���˺�����
	CMD_UNIONEXPIDITION_STAGEINFO_CS,		 //����Զ���ؿ���Ϣ

	CMD_UNIONEXPIDITION_SCBEGIN = 100,
	CMD_UNIONEXPIDITION_INFO_SC,			  //����Զ����Ϣ�·�
	CMD_UNIONEXPIDITION_MAPSET_SC,           //����Զ������
	CMD_UNIONEXPIDITION_STAGESTART_SC,       //����Զ���ؿ���ʼ
	CMD_UNIONEXPIDITION_STAGEFINISH_SC,      //����Զ���ؿ�����
	CMD_UNIONEXPIDITION_INDEX_PASS_SC,		 //�ؿ�ͨ����Ϣ
	CMD_UNIONEXPIDITION_REWARDGET_SC,        //����Զ��������ȡ
	CMD_UNIONEXPIDITION_DAMAGERANK_SC,       //����Զ���˺�����
	CMD_UNIONEXPIDITION_STAGEINFO_SC,		 //����Զ���ؿ���Ϣ
	CMD_UNIONEXPIDITION_REWARD_FLAG_SC,		 //������ȡ���
    CMD_UNION_TASK_SCEND,
};

struct SUnionSendStageShowData
{
	int  nIndex;
	int  nHeadID;
	char szName[32];
	unsigned char cLevel;
	int nDamage;

	SUnionSendStageShowData()
	{
		memset(this, 0, sizeof(*this));
	}
};

// Զ���ؿ���Ϣ
struct SExpiditionIndexInfo
{
	int nIndex;					//�ؿ�����
	int nBossHp;				//Ѫ��
};

//CMD_UNIONEXPIDITION_INFO_SC
struct SUnionExpiditionInfo
{
	unsigned char cExpiditonTimes;			//Զ������
	int			  nFightFinshTime;			//Զ������ʱ��
	int			  nFightColdStartTime;		//Զ����Ϣ��ʼʱ��
	int			  nRewardSendTime;			//��������ʱ���
	int			  nWorldID;					//����ID
	int			  nMapID;					//��ͼID
	unsigned char cRewardFlag;				//������ʶ
	unsigned char cNum;						//BOSS����
	// ... SExpiditionIndexInfo
};

//CMD_UNIONEXPIDITION_MAPSET_CS
struct UnionExpiditionMapSetCS
{
    int nWorldID;            //Զ�������ͼ����
};

//CMD_UNIONEXPIDITION_MAPSET_SC
struct UnionExpiditionMapSetSC
{
	int nWorldID;
	int nMapID;
	int nExpiditionFinishTime;				//Զ������ʱ��
};

//CMD_UNIONEXPIDITION_STAGESTART_CS
struct UnionExpiditonStageStartCS
{
	int nIndex;					//��ս�Ĺؿ�����
	int summonerId;				//��ս�ٻ�ʦ
	int heroIds[7];				//Ӣ���б�
	int mercenaryId;			//Ӷ��
};

//CMD_UNIONEXPIDITION_STAGESTART_SC
struct UnionExpiditonStageStartSC
{
	// RoomData
};

//CMD_UNIONEXPIDITION_STAGEFINISH_CS
struct UnionExpiditionStageFinishCS
{
	int nDamage;
	int nSummerID;				//�ٻ�ʦID
	int nHeroID[7];				//����ID
	int nHeroStart[7];			//�����Ǽ�
	int mercenaryId;			//Ӷ��
};

//CMD_UNIONEXPIDITION_STAGEFINISH_SC
struct UnionExpiditionStageFinishSC
{
	int nIndex;
};

//CMD_UNIONEXPIDITION_REWARDGET_SC
struct UnionExpiditionRewardGetSC
{
	int nNum;
	//������ƷID��Ĭ�Ͼ�Ϊһ��
};

struct SDamageSendRankShowData
{
	int nIndex;							//����
	int nDamage;						//�˺�ֵ
	char szName[32];					//����
	int nSummerID;						//�ٻ�ʦID
	int nHeroID[7];						//����ID
	int nStartID[7];					//�����Ǽ�
	unsigned char cBDType;				//��������
	unsigned char cBDLev;				//����ȼ�

	SDamageSendRankShowData()
	{
		memset(this, 0, sizeof(*this));
	}
};

//CMD_UNIONEXPIDITION_DAMAGERANK_SC
struct UnionExpiditionDamageRankSC
{
	int nMapID;								//��ͼID
	int nMyIndex;
	int nNum;								//��������
	//SDamageSendRankShowData				//���а�����
};

//CMD_UNIONEXPIDITION_STAGEINFO_CS,		 //����Զ���ؿ���Ϣ
struct UnionExpiditionStageInfoCS
{
	int nIndex;
};

//CMD_UNIONEXPIDITION_STAGEINFO_SC,		 //����Զ���ؿ���Ϣ
struct UnionExpiditionStageInfoSC
{
	int nBossHp;
	int  nHeadID;
	char szName[32];
	unsigned char cLevel;
	int nDamage;
};

//CMD_UNIONEXPIDITION_REWARD_FLAG_SC
struct UnionExpditionRewardFlagSC
{
	unsigned char cFlag;
};

#pragma pack()

#endif //__UNION_TASK_PROTOCOL_H__
