#ifndef __USER_PROTOCOL_H__
#define __USER_PROTOCOL_H__

#pragma pack(1)

enum USERPROTOCOL
{
	CMD_USER_CSBEGIN,
	CMD_USER_BUY_CS,		   //������ݹ���
	CMD_USER_RENAME_CS,		   //���������
    CMD_USER_HEADICONCHANGE_CS,//����ͷ��
	CMD_USER_SIGN_CS,		   //ǩ��
	CMD_USER_GIFTKEY_CS,		//���
	CMD_USER_FIRSTPAY_CS,		//�׳���ȡ
	CMD_USR_FUND_CS,			//����
	CMD_USR_MODIFYPS_CS,		//��ɫ�˺������޸�
	CMD_USER_CSEND,

    CMD_USER_SCBEGIN = 100,
	CMD_USER_BUY_SC,		   //������ݹ����·�
	CMD_USER_RENAME_SC,		   //����������·�
    CMD_USER_HEADICONCHANGE_SC,//����ͷ���·�
	CMD_USER_SIGN_SC,		   //ǩ��
	CMD_USER_GIFTKEY_SC,		//���
	CMD_USER_FIRSTPAY_SC,		//�׳���ȡ
	CMD_USR_FUND_SC,			//����
	CMD_USR_MODIFYPS_SC,		//��ɫ�˺������޸�

    CMD_USER_FORWARD_SS = 200,  //�������ڲ�ת�����ݵ�ServiceServer
	CMD_USER_SCEND,
};

enum UserAttrType
{
	ATTR_ENERGY,			//����
	ATTR_GOLD,				//���
	ATTR_DIAMOND,			//��ʯ
	ATTR_VIP,				//vip
    ATTR_MONTH_CARD,        //�¿�
};

struct UserBuyCS
{
	int attrType;			//���������
	int extend;			    //������չ�ֶ�(vip��ֵ�����,�¿�������)
};

struct UserBuySC
{
	int attrType;			//���������
	int addCount;			//���ӵ�����
	int result;				//���
    int extend;				//��ҹ���Ϊ����,
};

struct UserRenameCS
{
	char userName[32];		//������
};

struct UserRenameSC
{
	int result;				//0ʧ��,1�ɹ�
};

struct UserHeadIconCSC
{
    int headIconID;			//ͷ��ID
};

struct SUserSignSC
{
	int nFlag;
};

struct SUserGiftKeyCS
{
	unsigned char cPatform;			//ƽ̨����
	unsigned int nChannelID;		//����
	char GiftKey[17];
};

//struct SGoodsData
//{
//	int nGoodsID;
//	int nGoodsNum;
//};

struct SUserGiftKeySC
{
	int nNum;			//��Ʒ����
};

//CMD_USER_FIRSTPAY_SC,		//�׳���ȡ
struct SUserFirstPaySC
{
	int nFlag;
};

//CMD_USR_FUND_SC,			//����
struct SUserFundCS
{
	int nBuyTime;				//����ʱ��
	int nGetTime;				//��ȡʱ��
};

//CMD_USR_MODIFYPS_CS
struct SUsrModifyPsCS
{
	int		 nChannelID;		//����ID
	char	 Ps[32];			//����
};

struct SUsrModifyPsSC
{
	int nFlag;				//�޸�����Ӧ����
};

#pragma pack()

#endif //__USER_PROTOCOL_H__
