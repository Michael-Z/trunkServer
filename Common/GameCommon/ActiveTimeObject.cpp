#include "ActiveTimeObject.h"
#include "ActiveHelper.h"
#include "ConfActive.h"


CActiveTimeObject::CActiveTimeObject(int ActiveID)
{
	m_nActiveID = ActiveID;
	m_bUseFull = false;
	m_pTimeManager = NULL;
}


CActiveTimeObject::~CActiveTimeObject()
{
}

// �ڼƻ���ʱ�䵽��ʱ�ᱻ����
void CActiveTimeObject::onTimer(const KxServer::kxTimeVal& now)
{
	int CurTime;
	CurTime = (int)time(NULL) + 60; //��һ��ʱ�����

	//��ܱ�
	CConfActiveTime *conf = dynamic_cast<CConfActiveTime*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVE_TIME));

	SConfActiveTime *pActiveTime = static_cast<SConfActiveTime *>(conf->getData(m_nActiveID));

	if (CurTime >= pActiveTime->nEndTime)
	{
		CActiveHelper::getInstance()->delActiveTimeObject(m_nActiveID);
		KXLOGDEBUG("CActiveTimeObject::onTimer Stop CurTimes Is %d", CurTime);

	}
	else if (CurTime >= pActiveTime->nStartTime && CurTime < pActiveTime->nEndTime)
	{
		int nInterval = pActiveTime->nEndTime - time(NULL);
		stop();
		setTimer(m_pTimeManager,nInterval, true);
		KXLOGDEBUG("CActiveTimeObject::onTimer CurTimes Is %d", CurTime);
	}

	//����ǵ����仯������Ҫ�Ե���������¼���
	if (pActiveTime->nActiveType == DROP_ACTIVE_TYPE)
	{
		CActiveHelper::getInstance()->activeAddAccount();
	}
}

//���ö�ʱ��
void CActiveTimeObject::setTimer(KxServer::KxTimerManager *pTimerManager, int sec, bool bUseFull)
{
	if (pTimerManager == NULL)
	{
		return;
	}

	m_pTimeManager = pTimerManager;

	m_pTimeManager->addTimer(this, sec, KXREPEAT_FOREVER);

	m_bUseFull = bUseFull;
}
