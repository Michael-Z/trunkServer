/*
* ��������ģ��(��ͨ������)
* 1���洢������������
* 2�����������,���������豣���´θ��µ�ʱ���
* 3����ȡ(����/����)������Ϣ
* 4��ɾ�������,ɾ����ɵ�����(��������ִ�и���ʱ���)
*/

#ifndef __TASK_MODEL_H__
#define __TASK_MODEL_H__

#include <map>
#include "Storage.h"
#include "IDBModel.h"

struct TaskDBInfo
{
    int taskVal;               // ��ֵ
    int taskStatus;            // ״̬
    int resetTime;             // ���õ�ʱ���
};

class CRedisStorer;
class CTaskModel : public IDBModel
{
public:
    CTaskModel();
    ~CTaskModel();

    bool init(int uid);

    bool Refresh();
    // �������
    bool AddTask(int taskID, TaskDBInfo& info);
    // �ۼƻ��л�ĳ������״̬
    bool SetTask(int taskID, TaskDBInfo& info);
    // ��ȡĳ������
    bool GetTask(int taskID, TaskDBInfo& info);
    // ��ȡ��������
    std::map<int, TaskDBInfo>& GetTasks();
    // ɾ��ĳ������
    bool RemoveTask(int taskID);
    // ɾ������
    bool DeleteTasks();

private:

	bool GetTaskDataFromDB(int nTaskID);

private:

    int					                m_nUid;				// ���id
	CRedisStorer *						m_pStorer;			// ���ݿ��������
	std::string							m_strTaskKey;		// ��ͨ����key
    std::map<int, TaskDBInfo>           m_mapTasks;         // ��ͨ����
};

#endif
