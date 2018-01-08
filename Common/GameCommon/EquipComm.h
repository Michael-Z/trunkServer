#ifndef _EQUIP_COMM_H__
#define _EQUIP_COMM_H__ 

#include <string.h>

#define MAX_EQUIP_EFFECT_NUM 8			//���Ч����Ŀ


#pragma pack(1)

struct SItemBase
{
	int nType;					//����
	int nItemID;				//��ƷID
};

struct SItemInfo :public SItemBase
{
	int nCount;					//��Ʒ����
};

struct SItemFrag : public SItemBase
{
	int nHeroId;
	int nCount;
};

struct SHeroInfo :public SItemBase
{
	int nStart;					//Ӣ���Ǽ�
	int nCardID;				//Ӣ������roleID
};

struct SEquipInfo :public SItemBase
{
	int nDnycEquipID;					           //��̬װ��ID
	unsigned char cMainPropNum;					   //�����Ը���
	unsigned char cEffectID[MAX_EQUIP_EFFECT_NUM];
	unsigned short sEffectValue[MAX_EQUIP_EFFECT_NUM];

	SEquipInfo()
	{
		memset(this, 0, sizeof(*this));
		nType = 1;
	}
};

#pragma  pack()

#endif //_EQUIP_COMM_H__



