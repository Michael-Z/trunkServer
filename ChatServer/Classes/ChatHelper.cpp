#include "ChatHelper.h"
#include "ChatMessageManager.h"

using namespace KxServer;
using namespace ServerProtocol;

CChatHelper::CChatHelper()
{
}

CChatHelper::~CChatHelper()
{
}

void CChatHelper::ServerSubSetPermission(char* buffer, unsigned int len, IKxComm *target, Head* head)
{
	unsigned int dataLen = len - sizeof(Head);
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getUser(head->id));
	if (dataLen == sizeof(SProSetPermission) && pChatClient != NULL)
	{
		SProSetPermission *pPermissionSet = reinterpret_cast<SProSetPermission*>(buffer + sizeof(Head));
		pChatClient->setPermission(pPermissionSet->Permission);
	}
}

void CChatHelper::ServerSubDisconnect(char* buffer, unsigned int len, IKxComm *target, Head* head, CConnectModule* connectModule)
{
// 	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
// 	CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getUser(head->id));
// 	// ��ҵ���߼������Ƿ����˳ɹ����ɹ��Ž��û������ߣ�ҵ���߼�Ҳ����������֮ǰ
// 	if (pChatClient != NULL && connectModule->processUserEvent(EventKick, pChatClient))
// 	{
// 	// ����ͷ��
// 	head->MakeCommand(ServerProtocol::ServerMain, ServerProtocol::ServerSubPlayerOffLine);
// 	// ����������Ϣ����Щ���ݴ�����ķ�����
// 	pChatClient->sendDataToAllServer(buffer, sizeof(Head));
// 	// ִ����ұ�T�����߼�
// 	pChatClient->clean();
//	}
}

void CChatHelper::ServerSubSetRoute(char* buffer, unsigned int len, Head* head)
{
	unsigned int dataLen = len - sizeof(Head);
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getUser(head->id));
	if (dataLen == sizeof(SProSetRoute) && pChatClient != NULL)
	{
        SProSetRoute *pRouteSet = reinterpret_cast<SProSetRoute*>(buffer + sizeof(Head));
        pChatClient->setConServer(pRouteSet->nRouteKey, pRouteSet->nRoteValue);
	}
}

void CChatHelper::ServerSubTransmit(char* buffer, unsigned int len, Head* head)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getUser(head->id));
	if (pChatClient != NULL)
	{
	    pChatClient->sendData(buffer + sizeof(Head), len - sizeof(Head));
	}
}

void CChatHelper::ServerSubBoardCast(char* buffer, unsigned int len)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	pNetWorkManager->broadCastData(buffer + sizeof(Head), len - sizeof(Head));
}

void CChatHelper::ServerSubInit(char* buffer, unsigned int len, CConnectModule* connectModule)
{
	unsigned int dataLen = len - sizeof(Head);
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	if (dataLen < sizeof(SProInitSCC))
	{
		return;
	}

	unsigned int offset = sizeof(Head);
	SProInitSCC *pInitSS = reinterpret_cast<SProInitSCC*>(buffer + offset);
	CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getGuest(pInitSS->nGuestId));
	// ���ӶϿ�/�������Ӳ�����
	if (pChatClient == NULL)
	{
		return;
	}

	offset += sizeof(SProInitSCC);
	if (pInitSS->nPermssion == 0)
	{
		// ��֤ʧ��
		pChatClient->sendData(buffer + offset, len - offset);
		pChatClient->clean();
	}
	else
	{
		pChatClient->sendData(buffer + offset, len - offset);
		pChatClient->setUserId(pInitSS->nRouteValue);
		pChatClient->setPermission(pInitSS->nPermssion);

        //// ������¼�ɹ��¼����ɾ���ӿڴ����ŵ�ʵ��
        //if (connectModule->processUserEvent(EventLogin, pChatClient))
        //{
        //    // ��֤�ɹ���guestתuser
        //    pSessionClient->setConServer(pInitSS->nRouteKey, pInitSS->nRouteValue);
        //    pNetWorkManager->changeGuestToUser(pChatClient, pInitSS->nRouteValue);
        //}
        //else
        //{
        //    pSessionClient->clean();
        //}
        // ��֤�ɹ���guestתuser
        pChatClient->setConServer(pInitSS->nRouteKey, pInitSS->nRouteValue);
        pNetWorkManager->changeGuestToUser(pChatClient, pInitSS->nRouteValue);
	}
}

void CChatHelper::ServerSubQueryLoad(char* buffer, unsigned int len, Head* head)
{
	CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
	CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getUser(head->id));
	if (pChatClient != NULL)
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
		pChatClient->sendData(buff, buffSize);
		kxMemMgrRecycle(buff, buffSize);
	}
}

void CChatHelper::ServerSubForward(char* buffer, unsigned int len, Head* head)
{
    CNetWorkManager *pNetWorkManager = CNetWorkManager::getInstance();
    CChatClient *pChatClient = dynamic_cast<CChatClient *>(pNetWorkManager->getUser(head->id));
    if (pChatClient != NULL)
    {
        Head* head = reinterpret_cast<Head*>(buffer + sizeof(Head));
        CServerConfig * pServerConfig = CChatServer::getInstance()->getServerCoinfig();
        ServerRouteConfigData *pRouteConifgData = pServerConfig->getRouteConfigData(head->MainCommand(), head->SubCommand());
        if (NULL == pRouteConifgData)
        {
            return;
        }

        pChatClient->sendDataToServer(pRouteConifgData->GroupID, pRouteConifgData->RouteKey, 
            buffer + sizeof(Head), len - sizeof(Head));
    }
}

// �����Ƿ�Ϸ�
bool CChatHelper::isContentLegal(int chatType, const std::string& str)
{
    if (str.find("604662") != string::npos)
    {
        return false;
    }
    return true;
}

// ����Ƿ�Ϸ�
bool CChatHelper::isPlayerLegal(int uid)
{
    if (uid == 137780)
    {
        return false;
    }
    return !CChatMessageManager::getInstance()->isIllegal(uid);
}
