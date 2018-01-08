#ifndef __COMMON_HELPER_H__
#define __COMMON_HELPER_H__

#define TIME_SQUARE_INTERVAL			28800	//ʱ����
#define DAYOFSECOND						86400	//һ������

#include <vector>

struct STalentArrangeData;
struct DropItemInfo;
struct ArenaChestItem;
class CCommonHelper
{
public:
	//����mmrֵ��ö�Ӧ��λ
	static int getUserMMRDan(int mmr);
	//���ݻ��ֻ�ö�Ӧ��λ
	static int getUserIntegralDan(int integral);
	//��ö�λ��DanMMR_K
	static float getDanMMR_K(int dan);
	//��ö�λ��MMR_Kx
	static float getDanMMR_Kx(int dan);
	//��ö�λ��Arena_K
	static float getDanArena_K(int dan);
	//��ö�λ������ƥ��ʱ��
	static int getDanRobotMatchTime(int dan);
	//����½����Ͷ�Ӧ����������
	static int getChapterTypeToTask(int chaperId);
    //��ȡ�ؿ�����
    static int getStageTypeToTask(int stageId);
	//��ùؿ���Ҫ������
	//static int getStageNeedEnergy(int chapterId, int stageId);
	//��ùؿ���Ҫ�ĵȼ�
	static int getStageNeedLevel(int chapterId, int stageId);
	//��ùؿ����¸�����Id
	static bool getNextChapterStage(int chapterId, int stageId, int &nextChapter, int &nextStage);
	//���Ӣ������Ǽ�
	static int getHeroTopStar(int heroId);

	//������Сֵ���ֵ���
	//static int RandomMinMax(int min, int max);
	//��dropItems�ֳ�4����Ʒ, ������Ʒ, Ӣ����Ʒ, �ٻ�ʦ��Ʒ, ��Դ��Ʒ(����ͷ��, Ӣ����Ƭ)
	static void classifyItems(const std::vector<DropItemInfo> &dropItems, std::vector<DropItemInfo> &bagItems,
        std::vector<DropItemInfo> &heroItems, std::vector<DropItemInfo> &summonerItems, std::vector<DropItemInfo> &resourceItems);
    //��dropItems�ֳ�2����Ʒ, ������Ʒ, �Ǳ�����Ʒ
    static void classifyItems(const std::vector<DropItemInfo> &dropItems, std::vector<DropItemInfo> &bagItems, std::vector<DropItemInfo> &otherItems);

	//��ȡ��ǰʱ�������
	static int getCurMinTime();
	//��ȡ��ǰʱ���ַ���
	static void getCurTimeStr(char *pBuf, int nLen);
	//��ȡ��ǰ������
	static int getTotalDay(int nTime = 0);

    //����/����Э��
    static void encryptProtocolBuff(int mainCmd, int subCmd, char *buff, int len);

    // ��ȡ�����Ӧ�������츳ҳ
    static const STalentArrangeData *getRaceTalentArrange(int race);
    // ��ȡְҵ��Ӧ��ְҵ�츳ҳ
    static const STalentArrangeData *getVocationTalentArrange(int vocation);

	// ���pvp����
	static const ArenaChestItem* getArenaChest(int pvpLv, int userLv, int quality);
	// ������ҵȼ�, ��ʤ����õ���id
	static const int getArenaDrop(int userLv, int dayWinTimes);

};

#endif //__COMMON_HELPER_H__

