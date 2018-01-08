#ifndef __TASK_PROTOCOL_H__
#define __TASK_PROTOCOL_H__

#pragma pack(1)

enum TASKPROTOCOL
{
    CMD_TASK_CSBEGIN,
    CMD_TASK_FINISH_CS,             // ǰ�˷����������
    CMD_TASK_AWARD_CS,              // ������ȡ������
    CMD_TASK_CSEND,

    CMD_TASK_SCBEGIN = 100,
    CMD_TASK_FINISH_SC,             // ǰ�˷����������ط�
    CMD_TASK_AWARD_SC,              // ������ȡ�����ط�
    CMD_TASK_SCEND,
};

// CMD_TASK_FINISH_CS
// CMD_TASK_FINISH_SC
struct TaskFinishCSC
{
    int taskID;             // ����ID
    int extend;             // ��չ
};

// CMD_TASK_AWARD_CS
struct TaskAwardCS
{
    int taskID;             // ����ID
};

// CMD_TASK_AWARD_SC
struct TaskAwardSC
{
    int taskID;             // ����ID
    int awardCount;         // ������Ʒ����
    // awardCount ->DropItemInfo
};

#pragma pack()

#endif