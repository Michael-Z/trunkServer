#ifndef __NOTICE_PROTOCOL_H__
#define __NOTICE_PROTOCOL_H__

#pragma pack(1)

enum NOTICEPROTOCOL
{
	CMD_NOTICE_CSBEGIN,
	CMD_NOTICE_CSEND,
        
    CMD_NOTICE_SCBEGIN = 100,
    CMD_NOTICE_SC,                 // ����֪ͨ
    CMD_NOTICE_SS,                 // �������ڲ�ת��
	CMD_NOTICE_SCEND,
};

// ��������
enum EParamType
{
    PARAM_TYPE_INT,
    PARAM_TYPE_STRING,
};

// CMD_NOTICE_SC
struct NoticeSC
{
    char noticeCount;
};

// ֪ͨ��Ϣ
struct NoticeInfo
{
    int noticeId;
    char paramCount;
};

// ֪ͨ��������
struct NoticeParamContent
{
    char paramType;
    // int
    // short->len string
};

#pragma pack()

#endif //__NOTICE_PROTOCOL_H__
