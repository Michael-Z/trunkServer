#ifndef __GLOBAL_MAIL_MODEL_H__
#define __GLOBAL_MAIL_MODEL_H__

#include <map>
#include <vector>
#include <string>

#include "IDBModel.h"
#include "CommStructs.h"

struct DropItemInfo;
class CRedisStorer;
class CGlobalMailModel : public IDBModel
{
private:
	CGlobalMailModel();
	~CGlobalMailModel();

public:

	static CGlobalMailModel *getInstance();
	static void destroy();
	
public:

	bool init();

	bool Refresh();
	// �Ƴ�ȫ���ʼ�
	bool RemoveGlobalMail(int globalMailId);
	// ��ö�Ӧ��ָ��ȫ���ʼ�������Ϣ
	bool GetGlobalMailBase(int globalMailId, BaseMailInfo& mailBase);
	// ���ָ��ȫ���ʼ���������
	bool GetGlobalMailContext(int globalMailId, std::string& mailCtx);
	// ���ָ��ȫ���ʼ��ĵ�������
	bool GetGlobalMailItems(int globalMailId, std::vector<DropItemInfo> &items);

	// �������ȫ���ʼ�
	std::map<int, BaseMailInfo>& GetGlobalMailBase() { return m_GlobalMailBase; }
	// �������ȫ���ʼ��ļ�����
	std::map<int, std::string>& GetGlobalMailContexts() { return m_GlobalMailContexts; }
	// �������ȫ���ʼ���������
	std::map<int, std::vector<DropItemInfo> >& GetGlobalMailItems() { return m_GlobalMailItems; }
	bool getLoadingState() { return m_IsLoadIng; }
	void setLoadingState(bool bState) { m_IsLoadIng = bState;}

private:

	static CGlobalMailModel *					m_pInstance;			
	CRedisStorer *								m_pRedisStorer;			// ���ݿ�ڵ�
	std::string									m_szGlobalMailKey;		// ȫ���ʼ�key
	std::string									m_szGlobalMailCtxKey;	// ȫ���ʼ�����key
	std::map<int, BaseMailInfo>					m_GlobalMailBase;		// <ȫ���ʼ�id, �ʼ�������Ϣ>
	std::map<int, std::string>					m_GlobalMailContexts;	// <ȫ���ʼ�id, ����>
	std::map<int, std::vector<DropItemInfo> >	m_GlobalMailItems;		// <ȫ���ʼ�id, �ʼ�������Ϣ>
	bool										m_IsLoadIng;			// �������¼���
};

#endif //__GOLBAL_MAIL_MODEL_H__
