#ifndef __PROP_USE_HELPER_H__
#define __PROP_USE_HELPER_H__

#include <vector>
#include <map>
#include "ItemDrop.h"
#include "EquipComm.h"
#include "BufferData.h"

struct AwardInfo;
class IDBModel;
class CPropUseHelper
{
private:
	CPropUseHelper();
	~CPropUseHelper();

public:

	static CPropUseHelper *getInstance();
	static void destroy();

public:
	// ��Ӷ��������Ʒ���������ͻ��Ƿ��ʼ�
	bool AddItems(int uid, std::vector<DropItemInfo> &dropItem, bool bSendEmail = true,int nOssActionID = 0);
	// ��ӵ�������
	bool AddItems(int uid, DropItemInfo Info, bool bSendEmail = true, int nOssActionID = 0);
	// ����ʹ�õ���
	bool bagUseProp(int uid, int propId, int count, std::vector<DropItemInfo> &newItems);
    // �����Դ(��ҡ�����ҡ�����...)�����ӿ�
    void AddResource(int resourceID, int val, std::vector<DropItemInfo> &dropItem);

    void AddItems(const std::vector<AwardInfo> &items, std::vector<DropItemInfo> &dropItem);

private:

	//ʵ���Ͻ���Ʒ��ӵ�����
	bool RealAddItem(int uid,std::vector<DropItemInfo> &dropItem);
	//���������Ʒ����ƴװ
    bool ProcessMapItemAdd(int uid, std::vector<DropItemInfo> &dropItem, CBufferData* bufferData);
	//������Ʒ����
	bool ProcessMapItemSend(int uid, std::vector<SItemBase*> &sendItems);
	
private:

	static CPropUseHelper *m_pInstance;
};

#endif //__PROP_USE_HELPER_H__
