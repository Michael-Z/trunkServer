#include "SessionHelper.h"

CSessionHelper::CSessionHelper()
{
}

CSessionHelper::~CSessionHelper()
{
}

void CSessionHelper::ServerSubSetPermission(char* buffer, unsigned int len, IKxComm *target, Head* head)
{
	unsigned int dataLen = len - sizeof(Head);
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getUser(head->id));
	if (dataLen == sizeof(SProSetPermission) && pSessionClient != NULL)
	{
		SProSetPermission *pPermissionSet = reinterpret_cast<SProSetPermission*>(buffer + sizeof(Head));
		pSessionClient->setPermission(pPermissionSet->Permission);
	}
}

void CSessionHelper::ServerSubDisconnect(char* buffer, unsigned int len, IKxComm *target, Head* head, CConnectModule* connectModule)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getUser(head->id));
	// ��ҵ���߼������Ƿ����˳ɹ����ɹ��Ž��û������ߣ�ҵ���߼�Ҳ����������֮ǰ
	if (pSessionClient != NULL && connectModule->processUserEvent(EventKick, pSessionClient))
	{
	// ����ͷ��
	head->MakeCommand(ServerProtocol::ServerMain, ServerProtocol::ServerSubPlayerOffLine);
	// ����������Ϣ����Щ���ݴ�����ķ�����
	pSessionClient->sendDataToAllServer(buffer, sizeof(Head));
	// ִ����ұ�T�����߼�
	pSessionClient->clean();
	}
}

void CSessionHelper::ServerSubSetRoute(char* buffer, unsigned int len, Head* head)
{
	unsigned int dataLen = len - sizeof(Head);
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getUser(head->id));
	if (dataLen == sizeof(SProSetRoute) && pSessionClient != NULL)
	{
	SProSetRoute *pRouteSet = reinterpret_cast<SProSetRoute*>(buffer + sizeof(Head));
	pSessionClient->setConServer(pRouteSet->nRouteKey, pRouteSet->nRoteValue);
	}
}

void CSessionHelper::ServerSubTransmit(char* buffer, unsigned int len, Head* head)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getUser(head->id));
	if (pSessionClient != NULL)
	{
	pSessionClient->sendData(buffer + sizeof(Head), len - sizeof(Head));
	}
}

void CSessionHelper::ServerSubBoardCast(char* buffer, unsigned int len)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	pNetWorkManager->broadCastData(buffer + sizeof(Head), len - sizeof(Head));
}

void CSessionHelper::ServerSubInit(char* buffer, unsigned int len, CConnectModule* connectModule)
{
	unsigned int dataLen = len - sizeof(Head);
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	if (dataLen < sizeof(SProInitSCC))
	{
		return;
	}

	unsigned int offset = sizeof(Head);
	SProInitSCC *pInitSS = reinterpret_cast<SProInitSCC*>(buffer + offset);
	CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getGuest(pInitSS->nGuestId));
	// ���ӶϿ�/�������Ӳ�����
	if (pSessionClient == NULL)
	{
		return;
	}

	offset += sizeof(SProInitSCC);
	if (pInitSS->nPermssion == 0)
	{
		// ��֤ʧ��
		pSessionClient->sendData(buffer + offset, len - offset);
		pSessionClient->clean();
	}
	else
	{
		pSessionClient->sendData(buffer + offset, len - offset);
		pSessionClient->setUserId(pInitSS->nRouteValue);
		pSessionClient->setPermission(pInitSS->nPermssion);
		// ������¼�ɹ��¼����ɾ���ӿڴ����ŵ�ʵ��
		if (connectModule->processUserEvent(EventLogin, pSessionClient))
		{
			// ��֤�ɹ���guestתuser
			pSessionClient->setConServer(pInitSS->nRouteKey, pInitSS->nRouteValue);
			pNetWorkManager->changeGuestToUser(pSessionClient, pInitSS->nRouteValue);
		}
		else
		{
			pSessionClient->clean();
		}
	}
}

void CSessionHelper::ServerSubQueryLoad(char* buffer, unsigned int len, Head* head)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getUser(head->id));
	if (pSessionClient != NULL)
	{
		unsigned int buffSize = sizeof(Head) + sizeof(SPServerLoad);
		char* buff = reinterpret_cast<char*>(kxMemMgrAlocate(buffSize));
		// ����ͷ��
		Head* head = reinterpret_cast<Head*>(buff);
		head->MakeCommand(ServerProtocol::ServerMain, ServerProtocol::ServerSubResponseLoad);
		head->length = buffSize;
		head->id = head->id;
		// ����������
		SPServerLoad *pServerLoad = reinterpret_cast<SPServerLoad *>(buff + sizeof(Head));
		pServerLoad->ClientNum = pNetWorkManager->getCurClientNum();
		pSessionClient->sendData(buff, buffSize);
		kxMemMgrRecycle(buff, buffSize);
	}
}

void CSessionHelper::ServerSubForward(char* buffer, unsigned int len, Head* head)
{
	if (len < sizeof(Head)) 
	{
		return;
	}

	int uid = head->id;
	Head* pHead = reinterpret_cast<Head*>(buffer + sizeof(Head));
	CServerConfig * pServerConfig = CSessionServer::getInstance()->getServerCoinfig();
	ServerRouteConfigData *pRouteConf = pServerConfig->getRouteConfigData(pHead->MainCommand(), pHead->SubCommand());
	if (NULL == pRouteConf)
	{
		return;
	}
	if (pRouteConf->RouteKey == emRouteLoginKey)
	{
		// ֱ��ת��, �����û�
		KxTCPConnector* pTcpConnector = NULL;
		pTcpConnector = dynamic_cast<KxTCPConnector*>(CNetWorkManager::getInstance()->getServer(pRouteConf->GroupID, uid));
		if (pTcpConnector != NULL)
		{
			pTcpConnector->sendData(buffer + sizeof(Head), len - sizeof(Head));
		}
	}
	else
	{
		CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
		CSessionClient *pSessionClient = dynamic_cast<CSessionClient *>(pNetWorkManager->getUser(uid));
		if (pSessionClient != NULL)
		{
			pSessionClient->sendDataToServer(pRouteConf->GroupID, pRouteConf->RouteKey,
				buffer + sizeof(Head), len - sizeof(Head));
		}
	}
}
