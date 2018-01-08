#ifndef __ACTIVITY_PROTOCOL_H__
#define __ACTIVITY_PROTOCOL_H__

#pragma pack(1)

enum INSTANCEPROTOCOL
{
	CMD_INSTANCE_CSBEGIN,
	CMD_INSTANCE_CHALLENGE_CS,			//��ս
	CMD_INSTANCE_FINISH_CS,				//����
	CMD_INSTANCE_BUYTIMES_CS,			//�����������
	CMD_INSTANCE_CSEND,

	CMD_INSTANCE_SCBEGIN = 100,
	CMD_INSTANCE_CHALLENGE_SC,			//��ս
	CMD_INSTANCE_FINISH_SC,				//����
	CMD_INSTANCE_BUYTIMES_SC,			//�������
	CMD_INSTANCE_SCEND,
};

//CMD_INSTANCE_CHALLENGE_CS
struct InstanceChallengeCS
{
	int activityId;						//��ս����
	int difficulty;						//�Ѷ�
	// heroId
	// soldiers
};

//CMD_INSTANCE_CHALLENGE_SC
struct InstanceChallengeSC
{
	int activityId;					    //��ս����
	int difficulty;					    //�Ѷ�
};

//CMD_INSTANCE_FINISH_CS
//struct StageFinishData

//CMD_INSTANCE_FINISH_SC
struct InstanceFinishSC
{
	int activityId;					    //��ս����
	int difficulty;					    //�Ѷ�
	int	wonOrFailed;					//�ɹ�ʧ��
};

//CMD_INSTANCE_BUYTIMES_CS
struct InstanceBuyTimesCS
{
	int activityId;						//�����id
	int buyTimes;						//����
};

//CMD_INSTANCE_BUYTIMES_SC
struct InstanceBuyTimesSC
{
	int activityId;						//����Ļ����id
	int buyTimes;						//����Ĵ���
	int useTimes;						//����ʹ�ô���
	int costDiamond;					//���ĵ���ʯ
};

#pragma pack()

#endif //__ACTIVITY_PROTOCOL_H__
