#ifndef _ACTIVE_PROTOCOL_H__
#define _ACTIVE_PROTOCOL_H__

#pragma pack(1)

enum ACTIVEPROTOCOL
{
	CMD_ACTIVE_CSBEGIN,
	CMD_ACTIVE_GET_CS,				//���ȡ
	CMD_ACTIVE_SEVENDAY_CS,			//������ȡ
	CMD_ACTIVE_BLUEDIMAND_CS,		//������ȡ
	CMD_EXCHANGE_GET_CS,			//�һ����ȡ
	CMD_ACTIVE_CSEND,

	CMD_ACTIVE_SCBEGIN = 100,
	CMD_ACTIVE_GET_SC,				//���ȡ
	CMD_ACTIVE_TASKUPDATE_SC,		//����ݸ���,�����������ͻ�������������ݸ���
	CMD_ACTIVE_SEVENDAY_SC,			//������ȡ
	CMD_ACTIVE_BLUEDIMAND_SC,		//������ȡ
	CMD_EXCHANGE_GET_SC,			//�һ����ȡ
	CMD_ACTIVE_SCEND,
};

////CMD_ACTIVE_FRESH_CS
//struct SActiveFreshCS
//{
//	int nActiveID;
//};
//
////CMD_ACTIVE_FRESH_SC,	  //�����ˢ��
//struct SActiveFreshSC
//{
//	int nActiveID;
//	int nActiveType;
//	int nStartTime;		//��ʼʱ��
//	int nEndTime;		//����ʱ��
//	//�������� SActiveFreshActiveShop/SLoginActiveTaskData
//};
//
//struct SActiveFreshActiveShop
//{
//	int nShopNum;				//�������
//	//����������� SLoginActiveShopData
//};
//
//struct SActiveFreshShopData
//{
//	int nGoodsID[4];
//	int nGoodsNum[4];
//	int nGoldType;					//��������
//	int nPrice;
//	int nSaleRate;
//	int nMaxBuyTimes;
//	int nBuyTimes;					//�Ѿ��������
//};
//
//struct SActiveFreshTask
//{
//	int nActiveTaskNum;				//�������
//};
//
//struct SActiveFreshTaskData
//{
//	int			nFinishCondition;
//	int			nConditionParam[2];
//	int			nRewardDimand;
//	int			nRewardGold;
//	int			nRewardEnergy;
//	int			nRewardGoodsID;
//	int			nRewardGoodsNum;
//	int			nValue;					//������ɽ���
//	int			nFinishFlag;			//�Ƿ���ȡ��0-δ��ȡ��1-��ȡ
//};

//CMD_ACTIVE_SEVENDAY_CS
//CMD_ACTIVE_GET_CS
struct SActiveGet_CS
{
	unsigned short sActiveID;
	unsigned char  cParamID;			//���ID/����ID
};

//CMD_ACTIVE_GET_SC
//CMD_ACTIVE_SEVENDAY_CS
struct SActiveGet_SC
{
	unsigned short sActiveID;
	unsigned char  cParamID;			//���ID/����ID
	unsigned char  cFlag;				
};

//CMD_ACTIVE_TASKUPDATE_SC
struct SActiveTaskUpdateSC
{
	unsigned short sActiveID;
	unsigned short sCount;				//����
	//SActiveTaskUpdateData
};

struct SActiveTaskUpdateData
{
	unsigned int  nParam;			//��������
	unsigned char  cID;				//�����ID
};


#pragma pack()

#endif //_ACTIVE_PROTOCOL_H__
