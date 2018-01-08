#ifndef __PVPTASK_MODEL_H__
#define __PVPTASK_MODEL_H__

#include <set>
#include <list>
#include <string>
#include "IDBModel.h"

class CRedisStorer;
class CPvpTaskModel : public IDBModel
{
public:
	CPvpTaskModel();
	~CPvpTaskModel();

public:
	// ��ʼ��
	bool init(int uid);
	// ˢ��
	bool Refresh();
	// �����Ƿ����
	bool isPvpTaskExsit(int taskId);
	// ����µ�pvp����
	bool addPvpTask(int taskId);
	// �Ƴ�pvp����
	bool removePvpTask(int taskId);
	// �Ƴ�����pvp����
	bool removeAllPvpTask();
	// �������pvp����
	const std::set<int>& getAllPvpTasks();

private:
	int							m_nUid;			    // uid
	CRedisStorer *				m_pRedisStorer;	    // ���ݿ����
	std::string					m_strPvpTaskKey;	// pvp�����
	std::set<int>				m_setPvpTasks;		// ��������
};

// ע:�洢�µı�����ͷ��,ɾ��β��
class CPvpChestModel : public IDBModel
{
public:
	CPvpChestModel();
	~CPvpChestModel();
	
public:
	//��ʼ��
	bool init(int uid);
	//ˢ��
	bool Refresh();
	//���ӱ���
	bool addChestId(int chestId);
	//ɾ����һ������, �����ص�һ������id, ����0���ޱ���
	int popChestId();
	//������б���
	const std::list<int>& getChestIds();

private:
	int				m_nUid;
	CRedisStorer*   m_pRedisStorer;		// ���ݿ����
	std::string		m_strPvpChestKey;	// pvp�����
	std::list<int>  m_listChestIds;		// ����id�б�
};

#endif //__PVPTASK_MODEL_H__
