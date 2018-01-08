/*
��ս(�ؿ�����������)������
1���ж��Ƿ��ܹ���ս
*/

#ifndef __CHALLENGE_HELPER_H__
#define __CHALLENGE_HELPER_H__

#include <vector>
#include "CommStructs.h"
#include "UnionMercenaryModel.h"

// ��ս������Ϣ
struct BattleRoomInfo
{
	int battleType;
	int stageId;
	int stageLv;
	int ext1;
	int ext2;
	std::vector<int> outerBuffs;
	std::vector<BuffData> buffs;
};

class CChallengeHelper
{
public:
	// ����ս����Ϣ��ùؿ��Ǽ�
	static int getStageStar(int uid, int stageId, ChallengeBattleInfo &finishData, std::vector<int> &starReason);
	// ���Ӷ������, ������û�й�����߹���û��Ӷ��, ����false
	static bool getMercenaryInfo(int uid, int mercenary, UnionMercenaryInfo &merInfo);

	// �ؿ���ս
	// �Ƿ������ս�����ͼ, ɨ��times > 1
	static bool canChallengeStage(int uid, int chapterId, int stageId, int times = 1);
	// ����½ڹؿ��Ĺؿ��ȼ�
	static int getChapterStageLevel(int chapterId, int stageId);

	// �������
	// �Ƿ������ս
	static bool canChallengeGoldTest(int uid, int &stageId, int &stageLv);
	// �ж��ܷ���ȡ����, ����������ȡλ��
	static bool isEnoughGoldTestChest(int uid, int curChest);

	// Ӣ������
	// �Ƿ������սӢ������
	static bool canChallengeHeroTest(int uid, int instanceId, int diff);
	// ���Ӣ�������ؿ��ȼ�
	static int getHeroTestStageLevel(int uid, int instanceId, int diff);

	// ��������
	// �Ƿ������ս��������
	static bool canChallengeTowerTest(int uid, int floor);
	// ���������Ӧ¥��Ĺؿ��ȼ�
	static int getTowerTestStageLevel(int uid, int floor);

	// ������ս
	// �Ƿ������ս����
	static bool canChallengeSpecialTest(int uid);

};

#endif