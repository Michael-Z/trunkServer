#include "ChangeConnectModule.h"
#include "SessionConnect.h"
#include "NetworkManager.h"
#include "Protocol.h"
#include "GMProtocol.h"
#include "SessionServer.h"
#include "SummonerEvent.h"

using namespace KxServer;


CChangeConnectModule::CChangeConnectModule()
{
	m_nConnectNum = 0;
	m_GMClient = NULL;
	m_pConnectModule = NULL;
}


CChangeConnectModule::~CChangeConnectModule()
{
	m_nConnectNum = 0;
	m_GMClient = NULL;
	m_pConnectModule = NULL;
}

void CChangeConnectModule::init()
{
	if (m_pConnectModule == NULL)
	{
		CSummonerEvent *pGameEvent = new CSummonerEvent();
		m_pConnectModule = new CConnectModule;
		m_pConnectModule->init(pGameEvent);
	}
}

//��Ϸ���������ط�����Э�飬ת����ָ����ǰ��
void CChangeConnectModule::processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target)
{
	if (m_pConnectModule != NULL)
	{
		m_pConnectModule->processLogic(buffer, len, target);
	}
}

//����Ϸ�������Ͽ����ӣ���������
//����������newһ����ͨѶID�仯���ɿ�����������Դ��
void CChangeConnectModule::processError(KxServer::IKxComm *target)
{
	if (m_pConnectModule != NULL)
	{
		m_pConnectModule->processError(target);
	}
}

void CChangeConnectModule::processEvent(int eventId, IKxComm* target)
{
	CSessionConnector *pConnect = dynamic_cast<CSessionConnector*>(target);
	if (eventId == KXEVENT_CONNECT_FAILE)
	{
		KXLOGDEBUG("CChangeConnectModule Connect Server Failed");
		if (m_GMClient != NULL)
		{
			SGMFlagSC UpdateSC;
			UpdateSC.nFlag = 0;
			CNetWorkManager::getInstance()->sendDataToClient(dynamic_cast<CSessionClient*>(m_GMClient), CMD_GM, CMD_GM_DYNUPDATE_SC, (char*)&UpdateSC, sizeof(UpdateSC));
			m_GMClient = NULL;
		}

		CNetWorkManager::getInstance()->clearBakServer(pConnect->getGroupID());
	}
	else if (eventId == KXEVENT_CONNECT_SUCCESS)
	{
		m_nConnectNum++;
		target->setModule(m_pConnectModule);
		std::vector<KxServer::IKxComm*> *pvectBakComm = CNetWorkManager::getInstance()->getBakGroupServer(pConnect->getGroupID());
		if (m_nConnectNum == pvectBakComm->size())
		{
			if (m_GMClient != NULL)
			{
				SGMFlagSC UpdateSC;
				UpdateSC.nFlag = 1;
				CNetWorkManager::getInstance()->sendDataToClient(dynamic_cast<CSessionClient*>(m_GMClient), CMD_GM, CMD_GM_DYNUPDATE_SC, (char*)&UpdateSC, sizeof(UpdateSC));
				m_GMClient = NULL;
			}

			if (!CNetWorkManager::getInstance()->getChangeFlag())
			{
				CSessionServer::getInstance()->getTimerManager()->addTimer(CNetWorkManager::getInstance(), 10);
				CNetWorkManager::getInstance()->setChangeFlag(true);
			}
			
			//��Ϊtarget->setModule(m_pConnectModule),�����ڴ�����ɣ��ͷ�module
			release();
		}

		KXLOGDEBUG("CChangeConnectModule Connect Server Success");
	}
}


bool CChangeConnectModule::addCallBackClient(KxServer::IKxComm *target)
{
	m_GMClient = target;
	return true;
}