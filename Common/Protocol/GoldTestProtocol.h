#ifndef __GOLDTRIALPROTOCOL_H__
#define __GOLDTRIALPROTOCOL_H__

#pragma pack(1)


enum GOLDTESTPROTOCOL
{
	CMD_GOLDTEST_CSBEGIN,
	CMD_GOLDTEST_CHALLENGE_CS,   // ���������ս
	CMD_GOLDTEST_FINISH_CS,      // �����ս
	CMD_GOLDTEST_REWARDCHEST_CS, // ��ȡ����

	CMD_GOLDTEST_CSEND,


	CMD_GOLDTEST_SCBEGIN = 100,
    CMD_GOLDTEST_CHALLENGE_SC,    // ������ս��Ϣ
    CMD_GOLDTEST_FINISH_SC,       // ���������ս
    CMD_GOLDTEST_REWARDCHEST_SC,  // ��ȡ���

	CMD_GOLDTEST_SCEND,

};

//CMD_GOLDTEST_CHALLENGE_CS
//CMD_GOLDTEST_CHALLENGE_SC

//CMD_GOLDTEST_FINISH_CS
struct GoldTestFinishCS
{
	int damage;                   // �����˺�ֵ
};

//CMD_GOLDTEST_FINISH_SC
struct GoldTestFinishSC
{
	int damage;                   // �����˺�ֵ
	int damageReward;             // �˺�����(���)
	int levelReward;              // �ȼ��ӳ�(���)
};

//CMD_GOLDTEST_REWARDCHEST_CS	  
struct GoldTestRewardChestCS
{
	int chestFlag;                // 0Ϊһ����ȡ,>0Ϊ���嵥��
};

//CMD_GOLDTEST_REWARDCHEST_SC
struct GoldTestReardChestSC
{
	int goldReward;              // �����������
	int chestFlag;               // ��ǰ���б����״̬
};

#pragma pack()


#endif