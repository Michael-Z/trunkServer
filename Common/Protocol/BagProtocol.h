#ifndef __BAG_PROTOCOL_H__
#define __BAG_PROTOCOL_H__

#pragma pack(1)

enum BAGPROTOCOL
{
	CMD_BAG_CSBEGIN,
	CMD_BAG_SALE_CS,					//����������Ʒ
	CMD_BAG_UNLOCK_CS,					//��������
	CMD_BAG_USE_CS,						//����ʹ��
	CMD_BAG_MAKEEQUIPMENT_CS,			//װ������
	CMD_BAG_DECOMPOSEEQUIPMENT_CS,		//װ���ֽ�
	CMD_BAG_CSEND,

	CMD_BAG_SCBEGIN = 100,
	CMD_BAG_SALE_SC,					//����������Ʒ
	CMD_BAG_UNLOCK_SC,					//��������
	CMD_BAG_USE_SC,						//����ʹ��
	CMD_BAD_ITEM_ADD_SC,				//������Ʒ���
	CMD_BAG_EQUIP_MAKE_SC,				//װ������
	CMD_BAG_EQUIP_DECOMPOSE_SC,			//װ���ֽ�
	CMD_BAG_SCEND,
};

//CMD_BAG_SALE_CS
struct BagSaleCS
{
	int saleId;			  //����id
	int saleCount;		  //���۸���, װ����Զ��1��
};

//CMD_BAG_UNLOCK_CS
// ʧ�ܳ��۶������������Ϣ
struct BagSaleSC
{
	int saleId;			  //���۵�id
	int saleCount;		  //���۸���
	int gold;			  //��ý��
};

//CMD_BAG_UNLOCK_CS
struct BagUnlockCS
{
	int bagLine;		  //��������
};

//CMD_BAG_UNLOCK_SC
struct BagUnlockSC
{
	int diamond;		  //������ʯ
	int bagLine;		  //��������
};

//CMD_BAG_USE_CS
struct BagUseCS
{
	int id;				  //��Ʒid
	int val;			  //��Ʒ������װ������id
};

//CMD_BAG_USE_SC
//���, ��ʯ, ����, ����, �ٻ�ʦ 
struct BagUseSC
{
	int count;			  //�򿪸���
	//count * DropItemInfo
};

// CMD_BAG_MAKEEQUIPMENT_CS
struct MakeEquipmentCS
{
    int row;
    int eqLevel;
    int eqPart;
    int eqVocation;
    bool isDelicate;
};

struct DecomposeEquimentCS
{
    int equipCount;
    // EquipInfo*
};

struct ItemDataInfo
{
	int nCount;							//װ������
    //װ���� SEquipInfo/��װ���� SHeroInfo/SItemInfo/SItemBase
};

#pragma pack()

#endif //__BAG_PROTOCOL_H__
