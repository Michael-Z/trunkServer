#ifndef __PVPCHEST_PROTOCOL_H__
#define __PVPCHEST_PROTOCOL_H__

#pragma pack(1)

enum PvpChestProtocol
{
	CMD_PVPCHEST_CSBEGIN,
	CMD_PVPCHEST_REFRESH_CS,		//ˢ�±���
	CMD_PVPCHEST_BUY_CS,            //������
	CMD_PVPCHEST_OPEN_CS,           //�򿪱���
	CMD_PVPCHEST_OPENATONCE_CS,     //�����򿪱���
	CMD_PVPCHEST_CSEND,
        
    CMD_PVPCHEST_SCBEGIN = 100,
    CMD_PVPCHEST_REFRESH_SC,		//�·�ˢ�±�����
    CMD_PVPCHEST_BUY_SC,            //�·���������
    CMD_PVPCHEST_OPEN_SC,           //�·��򿪱�����
    CMD_PVPCHEST_OPENATONCE_SC,     //�·������򿪱�����
    CMD_PVPCHEST_SCEND,
};

//CMD_PVPCHEST_REFRESH_CS
//CMD_PVPCHEST_REFRESH_SC
struct PvpChestRefreshSC
{
    char chestCount;            //�������
    // int[] chestId            //�����б�
};

//CMD_PVPCHEST_BUY_CS
//CMD_PVPCHEST_BUY_SC
struct PvpChestBuySC
{
    int chestId;                //����id >0
    int costDiamond;            //�۳���ʯ
};

//CMD_PVPCHEST_OPEN_CS
struct PvpChestOpenCS
{
    int chestId;                //����id
};

//CMD_PVPCHEST_OPEN_SC
struct PvpChestOpenSC
{
    int chestId;                //����id
    int rewardCount;		    //��������
    //...DropItemInfo
};

//CMD_PVPCHEST_OPENATONCE_CS
//CMD_PVPCHEST_OPENATONCE_SC

#pragma pack()

#endif 
