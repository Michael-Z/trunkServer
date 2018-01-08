#ifndef __SHOP_PROTOCOL_H__
#define __SHOP_PROTOCOL_H__

#pragma pack(1)

enum SHOPPROTOCOL
{
    CMD_SHOP_CSBEGIN,
    CMD_SHOP_BUY_CS,        // ������Ʒ
    CMD_SHOP_REFRESH_CS,    // ˢ����Ʒ
	CMD_UNIONSHOP_BUY_CS,	// ���򹫻���Ʒ

    CMD_SHOP_SCBEGIN = 100,
    CMD_SHOP_BUY_SC,        // ������Ʒ
    CMD_SHOP_REFRESH_SC,    // ˢ����Ʒ
	CMD_UNIONSHOP_BUY_SC,	// ���򹫻���Ʒ
	CMD_UNIONSHOP_FRESHSC,	// �����̵���Ʒʱˢ��
};

///////////////////////////����Э��
// CMD_SHOP_BUY_CS
struct ShopBuyCS
{
	int itemNum;				 // ��Ʒ����
	unsigned char cShopID;		  // �̵�ID
    unsigned char cIndex;         // ��Ʒ����
};

// CMD_SHOP_BUY_SC
struct ShopBuySC
{
	int nNum;						//�������
	unsigned char cIndex;			//��������
};

///////////////////////////ˢ��Э��
// CMD_SHOP_REFRESH_CS
struct ShopRefreshCS
{
    unsigned char cShopID;           // �̵�ID
	unsigned char cShopType;		//ˢ�����ͣ�0-�Զ���1-�ֶ�
};

// CMD_SHOP_REFRESH_SC
//�̵�ṹ
struct ShopRefreshSC
{
	int				nTimes;					//�Ѿ�ˢ�µĴ���
	int				nNextFreshTime;			//�´�ˢ��ʱ���
	unsigned char	cType;					//ˢ������
	unsigned char	cShopID;				//�̵�ID
	unsigned char   cCurCount;				//��ǰ��Ʒ��
	//��Ʒ�ṹ
};

//��Ʒ�ṹ
struct SSendShopGoodsData
{
	int nGoodsID;							//����ID
	int nGoodsNum;							//���߸���
	int nCoinNum;							//�۸�
	unsigned short  sGoodsShopID;			//��ƷID ������65535
	unsigned char	cSale;					//�ۿ�ֵ
	unsigned char	cCoinType;				//��������
	unsigned char	cIndex;					//��Ʒ����
};

///////////////////////////////////////////////////
//CMD_UNIONSHOP_BUY_CS
struct SUnionShopBuyCS
{
	unsigned short sShopGoodsID;				//�������ƷID
	unsigned char cGoodsNum;					//�������
};

struct SUnionShopBuySC
{
	int nBuyerId;								//������UID
	unsigned short sShopGoodsID;				//�������ƷID
	unsigned char cGoodsNum;					//�������
};

struct SUnionShopFreshSC
{
	short sNum;						//��Ʒ����
	unsigned char cFlag;			//0-��ͨ״̬��1-�����ϼ�״̬��2-�״�
	//... SUnionShopGoodsData
};

struct SUnionShopGoodsData
{
	int nGoodsID;							//����ID
	int nCoinNum;							//�۸�
	unsigned short sGoodsShopID;			//��ƷID
	unsigned char cGoodsNum;				//���߸���
	unsigned char cCoinType;				//��������
	unsigned char cSale;					//�ۿ�ֵ
};

#pragma pack()

#endif //__SHOP_PROTOCOL_H__
