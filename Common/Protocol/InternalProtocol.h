#ifndef __INTERNAL_PROTOCOL_H__
#define __INTERNAL_PROTOCOL_H__

#pragma pack(1)

enum InternalProtocol
{
	INTER_CMD_DISCONNECT = 0,		//��Ҷ���
};

struct InternalDisconnectSS
{
	int uid;						//���id
	int tag;						//��չ�ֶ�
};

#pragma pack()

#endif //__INTERNAL_PROTOCOL_H__
