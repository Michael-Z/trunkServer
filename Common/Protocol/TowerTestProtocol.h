#ifndef __TOWERTESTPROTOCOL_H__
#define __TOWERTESTPROTOCOL_H__

#pragma pack(1)

enum TOWERTESTPROTOCOL
{
	CMD_TOWER_CSBEGIN,
	CMD_TOWER_FIGHTING_CS,		    //ӭս����
	CMD_TOWER_FINISH_CS,		    //ս������
	CMD_TOWER_CHOSEOUTERBONUS_CS,	//ѡ��buff
	CMD_TOWER_OPENTREASURE_CS,	    //�򿪱���
	CMD_TOWER_ONEKEYFIGHTING_CS,	//һ������
	CMD_TOWER_CSEND,

	CMD_TOWER_SCBEGIN = 100,
	CMD_TOWER_FIGHTING_SC,		    //ӭս���˽��
	CMD_TOWER_FINISH_SC,		    //ս��������õ���Ʒ
	CMD_TOWER_CHOSEOUTERBONUS_SC,   //ѡ��buff
	CMD_TOWER_OPENTREASURE_SC,	    //�򿪱���
	CMD_TOWER_ONEKEYFIGHTING_SC,	//һ������
	CMD_TOWER_SCEND,
};

//CMD_TOWER_FIGHTING_CS

//CMD_TOWER_FIGHTING_SC
struct TowerFightingSC
{
	int floor;						//¥����
	//+ RoomData..
};

//CMD_TOWER_FINISH_CS
struct TowerFinishCS
{
	int result;						//�ɹ�1/ʧ��0
	//struct ChallengeBattleInfo 
};

//CMD_TOWER_FINISH_SC
struct TowerFinishSC
{
	int floor;						//¥����
	int result;						//ʤ����ʧ��
};

// ����ṹ DropCurrency + TowerReward + count * DropItemInfo
struct TreasureReward
{
	int count;						//������Ʒ����
	// + count * DropItemInfo
};

//CMD_TOWER_CHOSEOUTERBONUS_CS
// �����Ϊ0,�������ѡ��
struct TowerChoseOuterBonusCS
{
	int outerBonus1;				//outerBonus1
	int outerBonus2;				//outerBonus2
	int outerBonus3;				//outerBonus3
};

//CMD_TOWER_CHOSEOUTERBONUS_CS
//����ѡ�����������¸�¥��
struct TowerChoseOuterBonusSC
{
	int costStars;					//���ĵ���������
	int outerBonus1;				//outerBonus1
	int outerBonus2;				//outerBonus2
	int outerBonus3;				//outerBonus3
};

//CMD_TOWER_OPENTREASURE_CS
//���ѡ�񲻴�������¸�¥��
struct TowerOpenTreasureCS
{
	int operate;					//���� 1��/0����
};

//CMD_TOWER_OPENTREASURE_SC
struct TowerOpenTreasureSC
{
	int diamond;					//���ĵ���ʯ����
	int count;						//������Ʒ����
	// ����ṹ DropCurrency + TowerReward + count * DropItemInfo
};

#pragma pack()

#endif //__TOWERTESTPROTOCOL_H__
