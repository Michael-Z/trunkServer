#ifndef __HEROTESTPROTOCOL_H__
#define __HEROTESTPROTOCOL_H__
#pragma pack(1)

enum HEROTESTPROTOCOL
{
	CMD_HEROTEST_CSBEGIN,
	CMD_HEROTEST_CHALLENGE_CS,			//��սӢ������
	CMD_HEROTEST_FINISH_CS,				//��ս����
	CMD_HEROTEST_CSEND,

	CMD_HEROTEST_SCBEGIN = 100,
	CMD_HEROTEST_CHALLENGE_SC,			//��սӢ���������
	CMD_HEROTEST_FINISH_SC,				//��ս�������
	CMD_HEROTEST_SCEND,
};

//CMD_HEROTEST_CHALLENGE_CS
struct HeroTestChallengeCS
{
    int instanceId;						//Ӣ����������
	int diff;							//��ս�Ѷ�
};

//CMD_HEROTEST_FINISH_CS
struct HeroTestFinishCS
{
	int instanceId;					    //��ս����
	int diff;							//�Ѷ�
	int result;							//���						
	// ����ɹ������ChallengeBattleInfo
};

//CMD_HEROTEST_FINISH_SC
struct HeroTestFinishSC
{
	int instanceId;					    //��ս����
	int diff;							//�Ѷ�
	int	result;						    //�ɹ�ʧ��
};

#pragma pack()

#endif
