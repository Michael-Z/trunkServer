#ifndef __GM_PROTOCOL_H__
#define __GM_PROTOCOL_H__

#pragma pack(1)

enum PAYROTOCOL
{
	CMD_GM_CSBEGIN,
	CMD_GM_LOGIN_CS,					//GM�˺ŵ�½
	CMD_GM_CLOSE_CS,					//�������ر�����
	CMD_GM_SERVERNUM_CS,				//��ȡ��������������
	CMD_GM_DYNUPDATE_CS,				//�ȸ��·���������̬�л���������
	CMD_GM_ROUTE_CS,					//GM����ת�����������
	CMD_GM_CSEND,

	CMD_GM_SCBEGIN = 100,
	CMD_GM_LOGIN_SC,					//GM�˺ŵ�½
	CMD_GM_CLOSE_SC,					//�������ر�
	CMD_GM_SERVERNUM_SC,
	CMD_GM_DYNUPDATE_SC,				//�ȸ��·���������̬�л���������
	CMD_GM_SCEND,
};

//////////////////////////////////////////////////////////////////////////

//CMD_GM_LOGIN_CS
struct SGMLoginCS
{
	char szAdimin[32];			//�˺� 32���ַ�
	char szPassword[32];		//���� 32���ַ�
};

struct SGMLoginSC
{
	unsigned int nUid;
};


//CMD_GM_SERVERNUM_CS
struct SGMServerNumSC
{
	unsigned int nNum;			//��ȡ����������
};

struct SGMFlagSC
{
	unsigned int nFlag;			//�Ƿ�ɹ� 0-ʧ�ܣ�1-�ɹ�
};

//CMD_GM_CLOSE_CS
//SGMFlagSC

//CMD_GM_DYNUPDATE_CS
//���ݷ��������������������ͬһ̨������
struct SGMDynUpdateCS
{
	unsigned int nGroupID;		//����������ID
	unsigned int nNum;			//����������
	unsigned int nStartPort;	//��������ʼ�˿�
};

struct SGMRouteChatCS
{
	int nLen;
	//�ַ�������
};

#pragma pack()

#endif