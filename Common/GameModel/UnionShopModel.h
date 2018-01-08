#ifndef _UNION_SHOP_MODEL__
#define _UNION_SHOP_MODEL__

#include <map>
#include <string>
#include "IDBModel.h"
#include "Storage.h"
#include "RedisStorer.h"

class CUnionShopModel : public IDBModel
{
public:
	CUnionShopModel();
	~CUnionShopModel();

	bool init(int unionID);
	// ˢ�¹�������
	bool Refresh();
	//�����̵���Ʒ����
	bool setUnionShopGoodsData(int nShopGoodsID, int &nGoodsNum);
	//��ȡ�̵���Ʒ����
	bool getUnionShopGoodsData(int nShopGoodsID, int &nGoodsNum);
	//�����̵���Ʒ����
	bool incrUnionShopGoodsNum(int nShopGoodsID, int nAddNum);
	//�����Ʒ�б�
	bool clearUnionShop();
	// ��������
	bool setUnionShop(std::map<int, int> &kvs);
	//��ȡ�̵���Ʒ�б�
	std::map<int, int> &getUnionShopMap() { return m_UnionShopGoods; }

private:
	Storage*								m_pStorage;			// ���ݿ�
	std::string								m_UnionShopKey;		// �����̵�Key
	std::map<int, int>						m_UnionShopGoods;	// �̵���Ʒ�б�,������Ʒ��ƷΨһ

};


#endif //_UNION_SHOP_MODEL__
