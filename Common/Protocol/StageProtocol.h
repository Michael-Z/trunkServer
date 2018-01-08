#ifndef __STAGE_PROTOCOL_H__
#define __STAGE_PROTOCOL_H__

#pragma pack(1)

enum STAGEPROTOCOL
{
	CMD_STAGE_CSBEGIN,
	CMD_STAGE_CHALLENGE_CS,			//�ؿ���ս
	CMD_STAGE_FINISH_CS,			//�ؿ�����
	CMD_STAGE_SWEEP_CS,				//ɨ��
	CMD_STAGE_STRATEGY_CS,			//����
	CMD_STAGE_CHAPTERAWARD_CS,		//�½ڽ���
	CMD_STAGE_BUYTIMES_CS,			//������ս����
	CMD_STAGE_BUYCHPATER_CS,		//�����½�
	CMD_STAGE_CSEND,

	CMD_STAGE_SCBEGIN = 100,
	CMD_STAGE_CHALLENGE_SC,			//�ؿ���ս�ʸ���֤
	CMD_STAGE_FINISH_SC,			//�ؿ�����ս��Ʒ�·�
	CMD_STAGE_SWEEP_SC,				//ɨ�����
	CMD_STAGE_STRATEGY_SC,			//����
	CMD_STAGE_CHAPTERAWARD_SC,		//�½ڽ���
	CMD_STAGE_BUYTIMES_SC,			//������ս����
	CMD_STAGE_BUYCHPATER_SC,		//�����½�
	CMD_STAGE_SCEND,
};

// CMD_STAGE_CHALLENGE_CS
struct StageChallengeCS
{
	int chapterId;			//�½�id
	int stageId;			//��ս�ؿ�id
};

//CMD_STAGE_CHALLENGE_SC
struct StageChallengeSC
{
	int result;				//�ܷ���ս
	// ������������roomData
};

//CMD_STAGE_FINISH_CS
 struct StageFinishCS
 {
 	int chapterId;			//�½�id
 	int stageId;			//�ؿ�id
	int result;				//���
 	//...StageFinishData
 };

struct StageFinishData
{
	int result;				//ʤ����ʧ��
	int time;				//ͨ��ʱ��
	int hpStatus;			//Ѫ��״��
	int costCrystal;		//���ѵ�ˮ������
};

//CMD_STAGE_FINISH_SC
struct StageFinishSC
{
	int chapterId;			//�½�id
	int stageId;			//�ؿ�id
	int result;				//ʤ��1��ʧ��0
	// result���Ϊ1, ������stageReward������
};

struct StageReward
{
	int star;				//�Ǽ�
	int star2Reason;		//����1�������, �����Ϊ0
	int star3Reason;		//����2�������, �����Ϊ0
	int rewardCount;		//��������
	//...DropItemInfo
};

//CMD_STAGE_SWEEP_CS
struct StageSweepCS
{
	int chapterId;			//�½�id
	int stageId;			//�ؿ�id
	int times;				//ɨ������
};

//CMD_STAGE_SWEEP_SC
struct StageSweepSC
{
	int chapterId;
	int stageId;
	int times;				//ɨ���������
	//...StageReward * count
};

//CMD_STAGE_STRATEGY_CS
struct StageStrategyCS
{
	int stageId;			//ָ���ؿ�id
};

//CMD_STAGE_STRATEGY_SC
struct StageStrategySC
{
	int stageId;
	int teamCount;			//������Ϣ����
	//... teamType(int) + PassTeam + PassTeamHero + passTeamEquip
};

//CMD_STAGE_CHAPTERAWARD_CS
struct StageChapterAwardCS
{
	int chapterId;			//��ȡ�������½�
	unsigned char cIndex;	//��ȡ�ı����ʶ
};

//CMD_STAGE_CHAPTERAWARD_SC
struct StageChapterAwardSC
{
	int chapterId;			//��ȡ�������½�
	int itemCount;			//��Ʒ����
	//... DropItemInfo
};

//CMD_STAGE_BUYTIMES_CS
struct StageBuyTimesCS
{
	int chapterId;			//�½�id
	int stageId;			//����ָ����Ӣ�ؿ�����ս����
};

//CMD_STAGE_BUYTIMES_SC
struct StageBuyTimesSC
{
	int chapterId;			//�½�id
	int stageId;			//���ؾ�Ӣ�ؿ�id
	int addTimes;			//�������ӵĴ���
};

//CMD_STAGE_BUYCHPATER_CS
struct StageBuyChapterCS
{
	int chapterId;			//������½�id
};

//CMD_STAGE_BUYCHPATER_SC
struct StageBuyChapterSC
{
	int chapterId;			//������½�id
	int diamond;			//������ʯ
};

#pragma pack()

#endif //__STAGE_PROTOCOL_H__
