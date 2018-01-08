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
	// ��ս�������͹ؿ���������
	// ����ٻ�ʦ��Ӣ�ۿ�Ƭ
	static bool checkChallengeTeam(int uid, int summonerId, const std::vector<int> &heroList);
	// ��÷�������
	static bool getBattleData(int uid, int summonerId, std::vector<int>& heroList, UnionMercenaryInfo *pMerInfo, BattleRoomInfo &roomInfo, char *data, int &length);
	// ��÷��䳤��, ͨ���ٻ�ʦ, Ӣ���б�, Ӷ����Ϣ, ս����buff, ս����buff.
	static int getBattleDataLength(int uid, int summonerId, std::vector<int> &heroList, UnionMercenaryInfo *pMerInfo, int buffSize, int outerBonusSize);
	// ����ս����Ϣ��ùؿ��Ǽ�
	static int getStageStar(int uid, int stageId, ChallengeBattleInfo &finishData, std::vector<int> &starReason);
	// ���Ӷ������, ������û�й�����߹���û��Ӷ��, ����false
	static bool getMercenaryInfo(int uid, int mercenary, UnionMercenaryInfo &merInfo);

};

#endif