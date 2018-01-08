#ifndef __ITEM_DROP_H__
#define __ITEM_DROP_H__

#include <vector>
#include <map>

struct DropItemInfo
{
	int id;					//��Ʒid
	int num;				//��Ʒ����
};

class CardGambleItem;
class CItemDrop
{
public:
	//����
	static void Drop(int id, std::vector<DropItemInfo> &vecDropItem, bool isExtra = false);
	//�����̵����
	static void UnionShopDrop(int id, std::map<int,int> &MapDropShop);
	//�鿨 randType = 0Ϊ��ͨ�鿨, randType = 1Ϊ�߼��鿨
	static CardGambleItem * RandHeroCard(int randType);
};

#endif //__ITEM_DROP_H__
