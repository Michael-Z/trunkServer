#ifndef __PVPROBOTROOM_H__
#define __PVPROBOTROOM_H__

#include <vector>
#include "GameUserManager.h"

class CRobotHelper
{
public:
	// �Ƿ���ƥ����Ϣ
	static bool canChallengeRobot(int uid, int &robotId);
	// �ܷ���ս������
	static bool canChallengeRobot(int uid, int summonerId, std::vector<int>& heroList);
	// ��÷��䳤��, ���������ڴ�
	static int getRoomDataLength(int uid, int summonerId, std::vector<int>& heroList);
	// ���ս������
	static bool getRobotBattleData(int uid, int robotId, int summonerId, std::vector<int>& heroList, char *data, int &length);
	// սʤ���������û�����ģ��
	static void setModelWinRobot(int uid, int robotId);
	// ս�ܻ��������û�����ģ��
	static void setModelLoseRobot(int uid, int robotId);

};

#endif //__PVPROBOTROOM_H__
