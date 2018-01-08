#ifndef _ACTIVE_HELPER_H__
#define _ACTIVE_HELPER_H__

#include "KXServer.h"
#include "ConfHall.h"
#include "ActiveTimeObject.h"

struct SConfActiveExtraAdd;

class CActiveHelper
{
public:
	static CActiveHelper * getInstance();
	static void destroy();

	bool init(KxServer::KxBaseServer *pServer);

	//��ӳɼ���
	bool activeAddAccount();

	//��ӻ��ʱ������
	bool addActiveTimeObject(int nActiveID, int sec, bool bUseFull = false);

	//������û����
	bool addForeverActiveObject(int nActiveID);

	//ɾ�����ʱ������
	bool delActiveTimeObject(int nActiveID);

	//��ʼ������������¼�
	bool initActiveTask(int uid);

	//��ʼ������7�������¼�
	bool init7DayActiveTask(int uid);

	//������õĻ�Ƿ���Ч
	bool isUseFull(int uid, int nActiveID);

	//����7���Ƿ���Ч
	bool is7DayUseFull(int uid, int nActiveID);

	//��ȡ�ʱ��
	int getActiveTimeInterval(int uid,int nActiveID);

protected:
	//�����Ӧ��Ӧ����ID����ݵ�Ӱ��
	bool propDropData(SConfActiveExtraAdd *pDropData);

protected:
	CActiveHelper();
	~CActiveHelper();

private:
	std::map<int, CActiveTimeObject*>	m_MapActiveTimeObject;			//��ʱ�
	KxServer::KxBaseServer *			m_pServer;
	static CActiveHelper*				m_pInstance;
};


#endif //_ACTIVE_HELPER_H__
