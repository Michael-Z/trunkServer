#ifndef __ACHIEVEMENT_PROTOCOL_H__
#define __ACHIEVEMENT_PROTOCOL_H__

#pragma pack(1)

enum ACHIEVEMETNPROTOCOL
{
    CMD_ACHIEVEMENT_CSBEGIN,
    CMD_ACHIEVEMENT_FINISH_CS,             // ǰ�˷�����ɳɾ�
    CMD_ACHIEVEMENT_GAIN_CS,               // ������ȡ�ɾͽ���
    CMD_ACHIEVEMENT_CSEND,
    
    CMD_ACHIEVEMENT_SCBEGIN = 100,
    CMD_ACHIEVEMENT_FINISH_SC,             // ǰ�˷�����ɳɾͻط�
    CMD_ACHIEVEMENT_GAIN_SC,               // ������ȡ�ɾͽ����ط�
    CMD_ACHIEVEMENT_SCEND,
};

// CMD_ACHIEVEMENT_FINISH_CS
// CMD_ACHIEVEMENT_FINISH_SC
struct AchieveFinishCSC
{
    unsigned short sachieveID;          // �ɾ�ID
    int				extend;				// ��չ
};

// CMD_ACHIEVEMENT_GAIN_CS
struct AchieveGainCS
{
	unsigned short sachieveID;          // �ɾ�ID
};

// CMD_ACHIEVEMENT_GAIN_SC
struct AchieveGainSC
{
	unsigned short			achieveID;          // �ɾ�ID
    unsigned char			cCount;				// ������Ʒ����
    // awardCount ->DropItemInfo
};

#pragma pack()

#endif