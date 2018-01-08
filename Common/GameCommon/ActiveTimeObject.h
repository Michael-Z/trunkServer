#ifndef _ACTIVE_TIME_OBJECT_H__
#define _ACTIVE_TIME_OBJECT_H__

#include "KXServer.h"

class CActiveTimeObject :public KxServer::KxTimerObject
{
public:
	CActiveTimeObject(int ActiveID);
	~CActiveTimeObject();

	// �ڼƻ���ʱ�䵽��ʱ�ᱻ����
	virtual void onTimer(const KxServer::kxTimeVal& now);

	//���ö�ʱ��
	virtual void setTimer(KxServer::KxTimerManager *pTimerManager, int sec, bool bUseFull);

	//���û�Ƿ���Ч
	void setUseFull(bool bUseFull) { m_bUseFull = bUseFull; }

	//�û�Ƿ���Ч
	bool isUseFull() { return m_bUseFull; }


protected:
	int							m_nActiveID;					//�ӳɻID
	bool						m_bUseFull;						//�Ƿ���Ч
	KxServer::KxTimerManager*	m_pTimeManager;					//ʱ��������
};


#endif //_ACTIVE_TIME_OBJECT_H__
