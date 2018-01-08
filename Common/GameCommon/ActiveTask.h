#ifndef _ACTIVE_TASK_H__
#define _ACTIVE_TASK_H__

#include "UserActionListener.h"

enum PayType
{
	emCum_PayType,			//���ۼƳ�ֵ
	emActive_PayType,		//�ʱ�����ۼƳ�ֵ
	emPoint_PayType,		//ָ����ȳ�ֵ
};

class CActiveTask : public IUserActionListener
{
public:
	CActiveTask();
	~CActiveTask();

	bool init(int uid, int nActiveID,int nTaskID);

	virtual bool onAction(int actID, void *data, int len);

	//����Ƿ�ǰ��������������
	bool checkAllTaskIsFinish();

protected:
	bool processActiveTaskData(int nActionID,void *data, int len);

	//��������¼�����
	bool processTimesEventFinish(int actID,int Param1, int Param2);

	//������ֵ�����¼�Param1 ������Param2 ���ӵ���ֵ
	bool processNumEventFinish(int actID,int Param1, int Param2);

	//�������ǵȼ�����
	bool processLvUpEventFinish(int actID,int Param1);

	//������������¼�
	bool processParamEvent(int actID);

	//������ҳ�ֵ
	bool processUserPay(int actID,int Param1);

	//�������Ӣ���Ǽ�����
	bool processHeroStarEvent(int actID);

	//�������Ӣ����ɫ����
	bool processHeroColorEvent(int actID);

	//�������Ӣ�۵ȼ�����
	bool processHeroLevEvent(int actID);

	//�����������ʱ���ۼ�
	bool processOLTimeEvent(int actID);

	//������һ�������
	bool processFund(int actID);

private:
	int						m_uid;
	int						m_ActiveID;
	int						m_TaskID;				//����ID
};

#endif //_ACTIVE_TASK_H__

