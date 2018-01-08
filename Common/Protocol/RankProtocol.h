#ifndef _RANK_PROTOCOL_H__
#define _RANK_PROTOCOL_H__

#pragma pack(1)

//���а�Э��
enum RankInfoProtocol
{
	CMD_RANKNFO_CSBEGIN,
	CMD_RANKNFO_UI_CS,	    //��ȡ���а���������
	CMD_RANKNFO_CSEND,

	CMD_RANKNFO_SCBEGIN = 100,
	CMD_RANKNFO_UI_SC,			//Ӧ�����а�������Ϣ
	CMD_RANKNFO_SCEND,
};

enum
{
	PVPRANK_TYPE,				//PVP
	LEVELRANK_TYPE,				//�ȼ����а�
	UNIONRANK_TYPE,				//�������а�
	TOWERRANK_TYPE,				//�������а�
	CHAMPIONRANK_TYPE,			//���������а�
};

//�����ȡ���а�����
//CMD_RANKNFO_UI_CS
struct SRankInfoUI_CS
{
	int nType;			//���а�����0-PVP ,1-�ȼ�,2-���ᣬ3-���� 4-������
};

//PVP���а�����/������
struct SRankShowPvpData
{
	int		nIndex;				//����
	int		nHeadID;			//ͷ��ID
	char	szName[32];			//����
	char	szUnionName[32];	//��������
	int		nScore;				//��������
	int		nLevel;				//��ҵȼ�
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�
};

//��ҵȼ���������
struct SRankShowLevelData
{
	int		nIndex;				//����
	int		nHeadID;			//ͷ��ID
	char	szName[32];			//����
	char	szUnionName[32];	//��������
	int		nLevel;				//��ҵȼ�
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�
};

//��������������
struct SRankShowUnionData
{
	int		nIndex;				//����
	char	szUnionName[32];	//��������
	int			nEmblemID;		//������
	int		nUnionNum;			//��������
	int		nUnionLevel;		//����ȼ�
};

//������������
struct SRankShowTowerData
{
	int		nIndex;				//����
	int		nHeadID;			//ͷ��ID
	char	szName[32];			//����
	int		nMaxTowerLevel;		//���¥��
	int		nScore;				//��¥����
	int		nLevel;				//��ҵȼ�
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�
	char    szUnionName[32];	//������

	SRankShowTowerData()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct SRankInfoUI_SC
{
	int nType;			//���а�����
	int nNum;			//���ݸ���
	int nIndex;			//���˶�Ӧ�����������Ϊ0�����ʾ�������а���
	//��Ӧ���͵����ݽṹ
};

#pragma pack()

#endif
