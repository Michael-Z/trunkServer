#ifndef _SHOP_MODEL_H__
#define _SHOP_MODEL_H__

#include <map>
#include <list>

#include "IDBModel.h"
#include "Storage.h"
#include "ConfHall.h"

//����+�¼���,field ��Ӧ�̵�ID

struct SShopInfo
{
	int nTimes;				//ˢ�´���
	int nFreshEndTime;		//�´�ˢ�¿�ʼʱ���
};

struct SShopGoods
{
	int nShopGoodsID;				//��ƷID
	int nGoodsNum;					//��Ʒ��Ʒ����

	SShopGoods()
	{
		nShopGoodsID	= 0;
		nGoodsNum = 0;
	}
};

enum
{
	enFreshStartTime = 0,						//ˢ��ʱ���
};

typedef std::map<int, SShopGoods> ShopGoodsMap;

class CShopModel : public IDBModel
{
public:
	CShopModel();
	~CShopModel();

	bool init(int uid);

	bool Refresh();

	bool SetShopNum(int ShopID,int nIndex,int ShopNum);

	bool GetShopGoodsInfo(int ShopID, int nIndex, int &nShopGoodsID, int &ShopNum);

	//���
	bool AddShopData(int ShopID, ShopGoodsMap &GoodsMap);

	//����
	bool SetShopInfo(int nShopID,int nTimes,int nFreshTime);

	bool GetShopInfo(int nShopID, int &nTimes, int &nFreshTime);

	ShopGoodsMap* GetShopData(int nShopID);

	std::map<int, ShopGoodsMap>& GetAllShop() { return m_MapShopGoods; }

	std::map<int, int>& GetDiamondDouble() { return m_MapDiamond; }

	std::map<int, SShopInfo>& GetShopIDInfo() { return m_MapShopIDInfo; }

	bool SetFieldValue(int nField, int nValue);

	bool GetFieldValue(int nField, int &nValue);

protected:

	bool SaveShopToDB(int ShopID);

	bool SaveShopIDToDB(int ShopID);

	bool SaveShopInfoToDB(int nField, int nValue);

private:

	std::map<int, ShopGoodsMap>				m_MapShopGoods;				//��ɫ�����̵���Ϣ
	std::map<int,SShopInfo>					m_MapShopIDInfo;			//�����̵���Ϣ
	std::map<int, int>						m_MapShopInfo;				//�̵�������Ϣ
	std::map<int, int>						m_MapDiamond;				//��ʯ�̵��׳���Ϣ

	int										m_nUid;						//��ɫID
	std::string								m_szKey;					//�̵�Key
	std::string								m_szShopIDKey;				//�̵��Ӧ��ϢKey
	std::string								m_ShopInfoKey;				//�̵�������ϢKey
	std::string								m_ShopDiamondKey;			//��ʯ�̵�˫����ϢKey
	Storage*								m_pStorage;					//���ݿ�洢ָ��
	char									m_szBuffer[1024];			//�ַ���������
};


#endif //_SHOP_MODEL_H__
