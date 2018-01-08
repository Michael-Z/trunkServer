#ifndef __HERO_PROTOCOL_H__
#define __HERO_PROTOCOL_H__

#pragma pack(1)

enum HEROPROTOCOL
{
	CMD_HERO_CSBEGIN,
    CMD_HERO_BUYCARD_CS,			//�鿨
	CMD_HERO_GEN_CS,				//����Ӣ��
    CMD_HERO_UPGRADE_CS,			//Ӣ������
    CMD_HERO_UPSTAR_CS,				//Ӣ������
    CMD_HERO_EQUIP_CS,				//Ӣ��װ��
	CMD_HERO_ACTIVETALENT_CS,		//װ���츳
	CMD_HERO_CSEND,

    CMD_HERO_SCBEGIN = 100,
    CMD_HERO_BUYCARD_SC,			//�鿨����
	CMD_HERO_GEN_SC,				//����Ӣ��
    CMD_HERO_UPGRADE_SC,			//Ӣ����������
    CMD_HERO_UPSTAR_SC,				//Ӣ������
    CMD_HERO_EQUIP_SC,				//Ӣ��װ��
	CMD_HERO_ACTIVETALENT_SC,		//�����츳
	CMD_HERO_SCEND,
};

//HERO_BUYCARD_CS
struct HeroBuyCardCS
{
	int buyType;                // 0��һ��,1��ʮ��
};

struct HeroBuyCardInfo
{
	int heroId;					// Ӣ��id
	int heroStar;				// Ӣ���Ǽ�
};

//HERO_BUYCARD_SC
struct HeroBuyCardSC
{
	int cardCount;              // ���ؿ�Ƭ������
	// HeroBuyCardInfo
};

//CMD_HERO_BUYCARD_CS
struct HeroGenCS
{
	int heroId;					// ����Ӣ��
};

//CMD_HERO_BUYCARD_SC
struct HeroGenSC
{
	//HeroInfo					// Ӣ����Ϣ
};

//HERO_UPGRDE_CS
struct HeroUpgradeCS
{
    int heroId;                 // Ҫ������Ӣ��ID
	int itemCount;              // ���߲�������(���������顢������)
	//... array of itemID
};

//HERO_UPGRADE_SC
struct HeroUpgradeSC
{
    int ret;					// 0ʧ��,1�ɹ�
	int addExp;					// ��ӵ��ܾ���
	int multiple;				// ��������
	int moneyType;				// ���ĵĻ�������
	int money;					// ���ĵĻ�����ֵ
};

//HERO_UPSTAR_CS
struct HeroUpStarCS
{
    int heroId;					// Ҫ������Ӣ��ID
};

//HERO_UPSTAR_SC
struct HeroUpStarSC
{
    int heroId;                 // Ӣ��ΨһID
	int newStar;                // ���Ǽ�
};

struct HeroEquipInfo
{
    int equipId;                // װ��ΨһID
    int equipPos;               // װ��λ��
};

//HERO_EQUIP_CS
struct HeroEquipCS
{
	int heroId;                 // Ӣ��ΨһID
    int equipCnt;               // װ������
    // equipCnt EquipInfo
};

//HERO_EQUIP_SC
struct HeroEquipSC
{
	int heroId;                 // װ����Ӣ��
	int equipCnt;               // װ������
    // HeroEquipInfo
};

//CMD_HERO_TALENT_CS
//CMD_HERO_TALENT_SC
struct HeroTalentCSC
{
    int heroId;                 // Ӣ��ΨһID
    unsigned char talent[8];    // �츳
};

#pragma pack()

#endif //__HERO_PROTOCOL_H__

