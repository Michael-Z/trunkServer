/*
 * �ʼ�Э��
*/

#ifndef __MAIL_PROTOCOL_H__
#define __MAIL_PROTOCOL_H__

#pragma pack(1)

enum MAILPROTOCOL
{
    CMD_MAIL_CSBEGIN,
    CMD_MAIL_READMAIL_CS,              // ��ȡ�ʼ�
	CMD_MAIL_GETMAILGOODS_CS,		   // ��ȡ�ʼ�
    CMD_MAIL_WEBMAIL_CS,               // ���web�ʼ�
    CMD_MAIL_CSEND,

    CMD_MAIL_SCBEGIN = 100,
    CMD_MAIL_READMAIL_SC,              // �ط���ȡ�ʼ�
	CMD_MAIL_GETMAILGOODS_SC,		   // ��ȡ�ʼ�
    CMD_MAIL_WEBMAIL_SC,               // �ط����web�ʼ�
    CMD_MAIL_SENDMAIL_SC,              // �������������ʼ�

    CMD_MAIL_UPDATEMAIL_SS,            // �����ʼ�ģ��
    CMD_MAIL_SCEND
};

// CMD_MAIL_READMAIL_CS
struct ReadMailCS
{
	int mailType;						 // �ʼ����� EMailType
	int mailID;							 // �ʼ�id
};

// CMD_MAIL_READMAIL_SC
struct ReadMailSC
{
	int mailType;					     // �ʼ�����
	int mailID;							 // �ʼ�id
	int nCount;							 // �ʼ�������Ʒ
	int nContextLen;					 // �ʼ����ݳ���
	//DropItemInfo
	//�ʼ�����
};

struct GetMailGoodsCS
{
	char normalMailCount;                // ��ͨ�ʼ�����
	char webMailCount;                   // web�ʼ�����
	// normalMailCount ->int �ʼ�ID�б�
	// webMailCount ->int WEB�ʼ�ID�б�
};

struct GetMailGoodsSC
{
	char normalMailCount;                // ��ͨ�ʼ�����
	char webMailCount;                   // web�ʼ�����
	char itemCount;                      // �Ƿ��н���
	// normalMailCount ->int �ʼ�ID�б�
	// webMailCount ->int WEB�ʼ�ID�б�
	// itemCount ->DropItemInfo
};


// CMD_MAIL_WEBMAIL_CS
struct WebMailCS
{
    char title[16];                      // ����
    char sender[16];                     // ����
    char content[128];                   // ����
    int receiver;                        // ȫ�����ض����
    int liveTime;                        // ������(��)
    int itemCount;                       // ���߸���
    // itemCount ->DropItemInfo
};

// CMD_MAIL_WEBMAIL_SC
struct WebMailSC
{
    char result;
};

// CMD_MAIL_SENDMAIL_SC
struct SendMailSC
{
    char mailType;                      // �ʼ�����EMailType
	int  nMailID;						// �ʼ�ID
    // ��ͨ�ʼ� ->BaseMailInfo
    // web�ʼ� ->BaseMailInfo
};

#pragma pack()

#endif //__MAIL_PROTOCOL_H__