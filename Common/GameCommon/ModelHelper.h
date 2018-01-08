#ifndef __MODEL_HELPER_H__
#define __MODEL_HELPER_H__

#include <vector>

#include "IDBModel.h"
#include "EquipComm.h"
#include "ConfHall.h"

// ��һ�����Ϣ
struct UserBasicInfo
{
	int userLv;				           // ��ҵȼ�
    int lastLoginTime;                 // �������¼ʱ��
    int identity;                      // ���(������,����ȼ�*10+��������) 
    std::string name;                  // �������

    UserBasicInfo()
        : userLv(0)
        , lastLoginTime(0)
        , identity(0)
        , name("")
    {

    }
};  

struct UserUnionBasicInfo
{
    int todayPvpLiveness;              // ����pvp��Ծ��
    int todayStageLiveness;            // ���չؿ���Ծ��
    int totalContribution;             // �ۼƹ���

    UserUnionBasicInfo()
        : todayPvpLiveness(0)
        , todayStageLiveness(0)
        , totalContribution(0)
    {

    }
};

struct DropItemInfo;
struct SShopGoods;
struct DBEliteStageInfo;
class ShopGoodsConfigData;

class CModelHelper
{
public:
	// ���ģ��
	static IDBModel *getModel(int uid, int modelType);

	/////////////////////////////�û��������////////////////////////////////////
	//���ӽ��
	static bool addGold(int uid, int gold);
	//���Ӿ���
    static bool addExp(int uid, int exp);
	static bool addExp(int uid, int exp, int &newLv, int &newExp);
	//������ʯ
	static bool addDiamond(int uid, int diamond,int nParam1 = 0,int nParam2 = 0);
	//��������(���ӻ�����)����,����ʱ����Ļָ��洢����
	//static bool addEnergy(int uid, int count);
	//�������
	//static int getEnergy(int uid);
    //���Ӿ�����
    static bool addPvpCoin(int uid, int pvpCoin);
    //��������
    static bool addTowerCoin(int uid, int towerCoin);
    //���ӹ��ṱ��
    static bool addGuildContrib(int uid, int guildContrib);
	//���ӳ鿨ȯ
	static bool	addFlashCard(int uid, int flashcard);
	//����10�鿨��
	static bool	addFlashCard10(int uid, int flashcard10);

	//��ȡ�������(���ܲ�����)
	static std::string getUserName(int uid);
	//��ȡ�����Ϣ(���ܲ�����)
	static bool getUserInfo(int uid, UserBasicInfo &info);
    //��ȡ��ҹ�����Ϣ(���ܲ�����)
    static bool getUserUnionInfo(int uid, UserUnionBasicInfo &info);
	//�Ƿ���������
	//static bool isEnergyEnough(int uid, int energy, int times = 1);
	//�Ƿ�ȼ�����
	static bool isUserLevelEnough(int uid, int needLv);
	//���ͷ��
	static bool AddHeadID(int uid, int HeadID);
    //��ӹ�����ջ�Ծ��
    static bool addDayLiveness(int uid, int type, int count = 1);

	/////////////////////////////�û��������////////////////////////////////////

	//////////////////////////// ������� //////////////////////////////////////
	//���ӱ�������
	static bool addBagCapacity(int uid, int capacity);
	//����Ӣ������
	static bool addHeroCapacity(int uid, int capacity);
	//�����Ʒ, ���ʱ����������ж�
	static bool addEquip(int uid,int CreateId, SEquipInfo &equipData);
	//���������Ʒ
	static bool addEquips(int uid, std::vector<int>& equipConfIds, std::vector<int>& equipIds);
	//�����ͨ��Ʒ
	static bool addItem(int uid, int itemConfId, int count);
	//����Ƴ���Ʒ��, ����<=0, ���ӱ�����ɾ����Ʒ
	static bool removeItem(int uid, int itemConfId, int count);
	//��ñ���ʣ������
	static int getBagStoreCapacity(int uid);
    //��ⱳ���Ƿ��ܴ��µ���
	static bool checkBagCapacity(int uid, const std::vector<DropItemInfo> &awards);
	//��⿨���Ƿ��ܴ��µ���
	static bool checkHeroCapacity(int uid, const std::vector<DropItemInfo> &awards);
	//���������Ʒ(����, ����, ��Դ��Ʒ, �ٻ�ʦ)�Ƿ��ܹ�����
	static bool checkAllCapacity(int uid, const std::vector<DropItemInfo> &awards);

    // �ӱ�����������е�װ��
    static bool addExistEquipToBag(int uid, int equipId);
    // �ӱ������Ƴ����е�װ��
    static bool removeExistEquipFromBag(int uid, int equipId);

    // �����Ʒ����
    static int getItemType(int itemId);

	//////////////////////////// ������� //////////////////////////////////////

	////////////////////////// Ӣ����� ////////////////////////////////////////
	//Ӣ���Ƿ�Ϊ����
	static bool isHeroCompletion(int uid, int heroId);
	//���Ӣ����Ƭ
	static bool addHeroFragment(int uid, int heroId, int fragment);
	//���Ӣ��, ����Ӣ�۵�Ψһid
	static bool addHero(int uid, int heroId, int star, int lv);
	//����Ӣ��IDɾ��Ӣ��, ������
	static bool removeHero(int uid, int heroId);
	//����б��ڵ�Ӣ���Ƿ�ӵ��
	static bool checkHeros(int uid, std::vector<int> &heroIds);

	////////////////////////// Ӣ����� ////////////////////////////////////////

	////////////////////////// �ٻ�ʦ��� ////////////////////////////////////////
	//�Ƿ�ӵ���ٻ�ʦ
	static bool haveSummoner(int uid, int summonerId);
	//����ٻ�ʦ
	static bool addSummoner(int uid, int summonerId);

	////////////////////////// �ٻ�ʦ��� ////////////////////////////////////////

	////////////////////////// ������� ////////////////////////////////////////

	////////////////////////// ������� ////////////////////////////////////////

	////////////////////////// �ؿ�&������� ////////////////////////////////////
	//�Ƿ��״���ս
	static bool isFirstChllange(int uid, int chapterId, int stageId);
	//�ܷ���սָ���ؿ�
	static bool canChallengeStage(int uid, int chapterId, int stageId, int times = 1);
	//�ýӿ���finishChapter��ͬ����, ���ӿ�ֻ�ı��½�״̬, ���ı�ؽ�״̬
	static void unlockChapter(int uid, const std::vector<int> &chapters);
	//�½��Ƿ����
	static bool isChapterUnlock(int uid, int chapterId);
	//����Ƿ���Խ����ؿ�, �������ڵĻ���
	static bool checkChapterUnlock(int uid, int integral);

	//�����¸��ؿ�, stageIdΪ��ǰ�ؿ�
	static void finishChapterStage(int uid, int chapterId, int stageId, int status, std::vector<int> &unlockVec);
	//��ɸ�������
	static void finishInstance(int uid, int instance, int difficulty, int star);
	//��þ�Ӣ������Ϣ
	static bool getEliteInfo(int uid, int chapterId, int stageId, DBEliteStageInfo &eliteInfo);
	//��Ӣ�ؿ�ʹ�ô���
	static bool addEliteUseTimes(int uid, int chapterId, int stageId, int times);
    //��ȡ�½�����������
    static int getStageChapterStar(int uid, int chapterId);
	//��ȡΪ3�����Ĺؿ�������
	static int getChapterRewardStar(int uid, int chapterId);
	//��ùؿ�״̬
	static int getChapterStageState(int uid, int chapterId, int stageId);
    //���ý�������˺��ʹ���
    static void setGoldTestData(int uid, int damage, int count = 1);
	//��Ǳ���״̬
	static int markGoldTestChest(int uid, std::vector<int> &chests);

	///////////////////////// �ؿ�&������� //////////////////////////////////////

	////////////////////////// Ӷ�� /////////////////////////////////////////
	// Ӷ���Ƿ����ʹ��
	static bool canMercenaryUse(int uid, int mercenary);
	// ���Ӷ����ʹ���б�
	static void addMercenaryUseList(int uid, int mercenary);

	////////////////////////// Ӷ�� /////////////////////////////////////////


	//////////////////////////PVP���//////////////////////////////////////////
	//����pvpʱ��
	static void updatePvpInfo(int uid);
	//�־û�ս��idkey
	static bool persistPvpBattleKey(int uid);
	//�Ƴ����pvpս��idkey
	static bool deletePvpBattleKey(int uid);
	//����ս��id
	static bool setPvpBattleId(int uid, int battleId, int robotId);
	//��ȡ��ɫ��һ�����������
	static bool getUserPreChampionRank(int uid, int &Rank);
	//����pvpÿ������
	static bool resetPvpDayInfo(int uid);
	//���ý�����
	static bool resetPvpCPN(int uid);

    // ���pvp����(����),�ɹ����ر���id>0
    static int addPvpChest(int uid);
	// ���ָ����pvp����
	static bool addPvpChest(int uid, int chestId);
	// ������pvpʤ�������id
	static int getWinDropId(int uid);
	// ɾ����һ������, �����ر���id, �������id����0, ��û�б���
	static int popPvpChest(int uid);
	// pvp�����Ƿ�ﵽ����, �������Ϊ5��, �̶��Ҳ����by�߻�
	static bool isPvpChestFull(int uid);
	// pvp�����б��Ƿ�Ϊ��
	static bool isPvpChestEmpty(int uid);
    // ˢ��pvp���䵽ǰ��
    static void refreshPvpChest(int uid);

	//////////////////////////PVP���//////////////////////////////////////////

	///////////////////////////�̵����/////////////////////////////////////////

    static bool FreshShop(int uid, int nShopID, int usrLv, std::map<int, SShopGoods> &ShopMap);
    // ���ͼ��������¼�
    static void DispatchActionEvent(int uid, int actID, void *data, int len);

	///////////////////////////�̵����/////////////////////////////////////////

	/////////////////////////// �ʼ���� ////////////////////////////////////

	static bool removeExpireMail(int uid);

	static bool removeExpireGlobalMail();

	/////////////////////////// �ʼ���� ////////////////////////////////////

	//////////////////////////ʱ���������//////////////////////////////////////////

	//������õ������Ϣ
	static void updateUserModelReset(int uid, bool bLogin = false);
	//���»������Ϣ
	static void updateInstanceInfo(int uid, int activityId);
	//��������������Ϣ
	static void updateTowerTestInfo(int uid);
	//���½��������Ϣ
	static void updateGoldTestInfo(int uid);
	//����Ӣ��������Ϣ
	static void updateHeroTestInfo(int uid);
	//�������о�Ӣ�ؿ���Ϣ
	static void updateAllEliteInfo(int uid);
	//����ָ����Ӣ�ؿ���Ϣ
	static void updateEliteInfo(int uid, int chapterId, int stageId);
    //����������Ϣ
    static void updateTaskInfo(int uid);
    //���¸��˹�����Ϣ
    static void updateUserUnionInfo(int uid, bool bRefresh = true);
	//�����̵�����
	static bool updateShopData(int uid);
	//����̵�ˢ�´���
	static bool checkShopFreshTimes(int uid);

	//////////////////////////ʱ���������//////////////////////////////////////////
	static bool PvpMailSend(int uid);

	static bool TowerMailSend(int uid);

	//////////////////////////��ֵ���//////////////////////////////////////////
	static void checkPayData(int uid);
};

#endif //__MODEL_HELPER_H__
