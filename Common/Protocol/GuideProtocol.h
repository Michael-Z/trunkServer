#ifndef __STORY_PROTOCOL_H__
#define __STORY_PROTOCOL_H__

#pragma pack(1)

enum GuideProtocol
{
	CMD_GUIDE_CSBEGIN,
	CMD_GUIDE_RECORD_CS,			//��¼�����
	CMD_GUIDE_CSEND,

	CMD_GUIDE_SCBEGIN = 100,
	CMD_GUIDE_RECORD_SC,			//��¼����㷵��״��
	CMD_GUIDE_SCEND,
};

//CMD_GUIDE_RECORD_CS
struct GuideRecordCS
{
	int nEndNum;			//������������
	//����ID ����
};

//CMD_GUIDE_RECORD_SC
struct StoryRecordSC
{
	int nFlag;				//����ɹ���ʶ
};

#pragma pack()

#endif //__STORY_PROTOCOL_H__
