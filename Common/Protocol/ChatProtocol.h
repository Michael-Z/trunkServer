#ifndef __CHAT_PROTOCOL_H__
#define __CHAT_PROTOCOL_H__

#pragma pack(1)

enum CHATPROTOCOL
{
	CMD_CHAT_CSBEGIN,
    CMD_CHAT_JOINROOM_CS,                   // ����������
    CMD_CHAT_QUITROOM_CS,                   // �˳�������(Ŀǰ����ʹ��),�޷�����Ϣ
    CMD_CHAT_SENDMESSAGE_CS,                // ������Ϣ,�޷�����Ϣ
	CMD_CHAT_CSEND,

    CMD_CHAT_SCBEGIN = 100,
    CMD_CHAT_RECEIVE_MOREMESSAGE_SC,        // ���ܶ�����Ϣ
    CMD_CHAT_RECEIVE_SINGLEMESSAGE_SC,      // ���ܵ�����Ϣ
	CMD_CHAT_SCEND,

    CMD_CHAT_SSBEGIN = 200,
    CMD_CHAT_FUNCTION_SS,                   // ��ع���,���Եȵ�
    CMD_CHAT_SSEND,
};

// CMD_CHAT_JOINROOM_CS
struct ChatJoinRoomCS
{
    char roomType;          // ��������
    int roomId;             // ����Id<����Id>
};

// CMD_CHAT_QUITROOM_CS
struct ChatQuitRoomCS
{
    int roomId;             // ����Id<����Id>
};

// CMD_CHAT_SENDMESSAGE_CS
//ChatMessageInfo

// CMD_CHAT_RECEIVE_MOREMESSAGE_SC
struct ChatReceiveMessageSC
{
    char chatCount;         // ��������
};

// CMD_CHAT_RECEIVE_SINGLEMESSAGE_SC
//ChatMessageInfo

enum EChatFunction
{
    ECF_SHUTUP,
    ECF_REMOVE_SHUTUP,
};

// CMD_CHAT_FUNCTION_SS
struct ChatFunctionSS
{
    char type;              // ���� 0����1�Ƴ�����
    int uid;                // ���id
};

#pragma pack()

#endif //__CHAT_PROTOCOL_H__
