#ifndef __LOOK_PROTOCOL_H__
#define __LOOK_PROTOCOL_H__

#pragma pack(1)

enum LOOKPROTOCOL
{
	CMD_LOOK_CSBEGIN,
    CMD_LOOK_HERO_CS,             // ����鿴Ӣ����Ϣ
    CMD_LOOK_EQUIP_CS,            // ����鿴װ����Ϣ
	CMD_LOOK_CSEND,

    CMD_LOOK_SCBEGIN = 100,
    CMD_LOOK_HERO_SC,             // ���ز鿴Ӣ����Ϣ
    CMD_LOOK_EQUIP_SC,            // ���ز鿴װ����Ϣ
	CMD_LOOK_SCEND,
};

//CMD_LOOK_HERO_CS
struct LookHeroCS
{
    int uid;                     // ���id
    int heroId;                  // Ӣ��id
};

//CMD_LOOK_HERO_SC
struct LookHeroSC
{
    int heroId;                   // Ӣ��id
    int heroExp;                  // ����
    unsigned char heroLv;         // �ȼ�
    unsigned char heroStar;       // �Ǽ�
    unsigned char equipCount;     // װ������
    unsigned char heroTalent[8];  // �츳
    // equipCount EquipItemInfo
};

//CMD_LOOK_EQUIP_CS
struct LookEquipCS
{
    int uid;                      // ���id
    int equipDynId;               // װ����̬id
};

//CMD_LOOK_EQUIP_SC
struct LookEquipSC
{
    int uid;                      // ���id
    //EquipItemInfo
};

#pragma pack()

#endif //__LOOK_PROTOCOL_H__
