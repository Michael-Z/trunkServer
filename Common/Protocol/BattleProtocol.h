#ifndef __BATTLEPROTOCOL_H__
#define __BATTLEPROTOCOL_H__

#include "CommStructs.h"

#pragma pack(1)

enum BattleProtocol
{
    CMD_BAT_CSBEGIN,            // CS��Ϣ������ʼ
    CMD_BAT_PVPCOMMANDCS,       // ����ս��ָ������
    CMD_BAT_PVPUPDATECS,        // �����������
    CMD_BAT_CSEND,              // CS��Ϣ�������

    CMD_BAT_SCBEGIN = 100,      // SC��Ϣ������ʼ
    CMD_BAT_PVPCOMMANDSC,       // ��Ӧս��ָ����
    CMD_BAT_PVPENDSC,           // ֪ͨ�ͻ���PVP��Ϸ����
    CMD_BAT_PVPUPDATESC,        // ���ظ�������
    CMD_BAT_SCEND,              // SC��Ϣ�������
};

//CMD_BAT_PVPCOMMANDCS
//CMD_BAT_PVPCOMMANDSC
//����������ת�� BattleCommandInfo �ṹ

//CMD_BAT_PVPENDSC
struct BattlePvpEndSC
{
    int winner;                 //ʤ����
    int loser;                  //ʧ����
    int endType;                //���������� �Ƿ���Ҫ��������
};

#pragma pack()

#endif 
