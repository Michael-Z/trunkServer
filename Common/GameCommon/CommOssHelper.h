#ifndef _COMMOSS_HELPER_H__
#define _COMMOSS_HELPER_H__

#include <string>
#include "TeamModel.h"

//��ʯ��������
enum DiamondPayType
{
	NONE_PAY_TYPE,						//0 ����
	SINGLE_FLOOP_CARD_TYPE,				//1.���γ鿨
	TEN_FLOOP_CARD_TYPE,				//2.ʮ����
	DIAMONDBUY_SUMMER_TYPE,				//3.�����ٻ�ʦ
	DIAMONDBUY_HERO_TYPE,				//4.����Ӣ��
	DIAMONDBUY_ACTIVE_TYPE,				//5.��������
	DIAMONDBUY_GOLD_TYPE,				//6.������
	DIAMONDBUY_GOODS_TYPE,				//7.�������
	DIAMONDBUY_EQUIP_TYPE,				//8.����װ��
	DIAMONDRESET_SHOP_TYPE,				//9.�����̵�
	DIAMONDRESET_ELITE_TYPE,			//10.��ֵ��Ӣ�ؿ�
	DIAMONDRRENAME_TYPE,				//11 ����������ʯ
	DIAMONDBUYPVPCHEST_TYPE,			//12 ����PVP����
};

//��ʯ���ԭ��
enum DiamnodGetReason
{
	MAIL_DIAMOND_GET_REASON,			//�ʼ����
	PAY_DIAMOND_GET_REASON,				//��ֵ�����ʯ
	MONTH_DIAMOND_GET_REASON,			//�¿�����
	TASK_DIAMOND_GET_REASON,			//��������
	ACTIVE_DIAMOND_GET_REASON,			//�����
	PVP_DIAMOND_GET_REASON,				//����������
	ITEMUSE_DIAMOND_GET_REASON,			//��Ʒʹ�û��
	SIGN_DIAMOND_GET_REASON,			//ǩ�������ʯ
	OTHER_DIAMOND_GET_REASON,			//������ʯ���
};

//��Դ����
enum ResOurceType
{
	NONE_RESOURCE_TYPE,
	GOLD_RESOURCE_TYPE,					//���
	TOWER_RESOURCE_TYPE,				//����
	PVP_RESOURCE_TYPE,					//������
	UNION_RESOURCE_TYPE,				//���ṱ��
};

//����ʯ֧������
enum ShopPayForType
{
	NONE_SHOP_PAYFOR_TYPE,				
	SUMMER_SHOP_PAYFOR_TYPE,
	HERO_SHOP_PAYFOR_TYPE,
	GOODS_SHOP_PAYFOR_TYPE,
	EQUIP_SHOP_PAYFOR_TYPE,
	TASK_PAYLER_TYPE,				//����/�ɾ�
};

//������ս����
enum ChallengeType
{
	NONE_CHALLENGE_TYPE,
	GOLD_CHALLENGLE_TYPE,
	HERO_CHALLENGLE_TYPE,
	TOWER_CHALLENGLE_TYPE,
	ACTIVE_CHALLENGLE_TYPE,
	UNIONEXPIDITION_TYPE,			//����Զ��
};

//���߻��ö��
enum GAINGOODSREASON
{
	NONE_GAIN_REASON,
	WORDSTAGE_GAIN_REASON,		//���縱�����ؿ���
	WORDSTAGEBOX_GAIN_REASON,	//���縱���½ڱ���
	SIGN_GAIN_REASON,			//ǩ��
	ACTIVE_GAIN_REASON,			//�
	FIRSTPAY_GAIN_REASON,		//�׳�
	SHOP_GAIN_REASON,			//�̵�
	MAIL_GAIN_REASON,			//�ʼ�
	MAKE_GAIN_REASON,			//����
	TASK_GAIN_REASON,			//����
	ACHIEVE_GAIN_REASON,		//�ɾ�
	FLOPCARD_GAIN_REASON,		//�鿨
	SUMMONER_GAION_REASON,		//�ٻ�ʦϵͳ
	GOODSUSE_GAION_REASON,		//��ʹ�õ���
	GOLDTEST_GAION_REASON,		//�������
	UNIONSTAGEBOX_GAION_REASON,	//����Զ������
	UNIONACTIVEBOX_GAION_REASON,//�����Ծ������
	PVPBOX_GAIN_REASON,			//����������
	CDKEY_GAIN_REASON,			//�һ���
	PVPDROP_GAIN_REASON,		//PVP����
};

enum PvpMatchType
{
	enNormalMathType = 1,
	enChamopinMatchType,
};

//��ɫPVE��־����
struct SOssPveLogData
{
	int  nSummonerID;
	int  nHeroID[MAX_HERO_COUNT];
	int  nStageID;
	bool bWin;
	int  UnLockStageID;
	int  nCryStalLevel;
	int  nStartNum;
	int  nTime;
};

//��ɫPVP��־����
struct SOssPvpLogData
{
	bool bWin;
	int  nPvpNum;
	int  nSummonerID;
	int  nOpponentSummer;
	int  nPvpType;
	int  nHeroID[MAX_HERO_COUNT];
	int  nCryStalLevel;
	int  nTime;
};

class CCommOssHelper
{
public:
	CCommOssHelper();
	~CCommOssHelper();

	//ע����־
	static void registerStatOss(int uid, std::string Mobile, int nChannelType, std::string ChannelName);

	//�û���������Ϣ
	static void userChannelOss(int uid, int nChannelType, std::string ChannelName);

	//������ɫ��Ϣ��־
	static void crateActorStatOss(int uid);

	//��ɫ��½��Ϣ��־
	static void userLoginOss(int uid, int LoginOutTime);

	//��ɫ�ǳ���־
	static void userLoginOutOss(int uid, int LoginTime);

	//�û�����������־
	static void newGuidOss(int uid, int nStep);

	//�û�������Ϣ
	static void userLevelUpOss(int uid);

	//�û���ֵ��Ϣ
	static void userCashOss(int uid, int nPayValue);

	//�û���ʯ������Ϣ
	static void userDiamondPayOss(int uid, int Diamond, int nPayType, int nNum, int Info = 0);

	//�û���ʯ������Ϣ
	static void userDiamondGetOss(int uid, int Diamond,int nReason);

	//�û�����ʯ������Ϣ
	static void userTokenShopOss(int uid, int TokenType, int Cost, int PayforType, int Num, int Info);

	//�û�VIP������Ϣ
	static void UserVipUpOss(int uid);

	//PVE��־ͳ��
	static void StageLogicOss(int uid, SOssPveLogData &LogData);

	//PVP��־ͳ��
	static void PvpLogicOss(int uid, SOssPvpLogData &LogData);

	//��������ս����Ϣ
	static void UserSpStageOss(int uid, int StageID, int StageType, bool bWin);

	//�ؿ�ɨ��ͳ��
	static void StageSweepOss(int uid, int nStageID, int nTimes);

	//����������Ϣ
	static void UserCardUpStartOss(int uid, int heroId);

	//����������Ϣ
	static void UserCardLvUpOss(int uid, int ndynCardID);

	//��ǰ�����û���Ϣ
	static void ServerStatusOss();

	//�û������ȡ��Ϣ
	static void UserGiftGetOss(int uid, int nChannelID, int nGiftID, std::string CDKey);

	//�û�ƥ����Ϣ
	static void UserMatchOss(int uid, int PvpType, int nMathTime, int nMatchFlag);

	//�������Ϣ
	static void UserActiveOss(int uid, int nActiveID, int nParamID);

	//�û�������Ϣ
	static void userGoodsGainOss(int uid, int nActionID, int nGoodsID, int nGoodsNum);
};


#endif
