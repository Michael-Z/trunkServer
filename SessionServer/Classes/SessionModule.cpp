#include "SessionModule.h"
#include "KxCore.h"
#include "SessionClient.h"
#include "SessionConnect.h"
#include "NetworkManager.h"

#include "Protocol.h"
#include "LoginProtocol.h"
#include "ServerData.h"
#include "SessionServer.h"
#include "ServerProtocol.h"
#include <set>
#include "SessionGMService.h"

using namespace KxServer;
using namespace ServerProtocol;

CSessionModule::CSessionModule(void)
{
}

CSessionModule::~CSessionModule(void)
{
}

void CSessionModule::processLogic(char* buffer, unsigned int len, IKxComm *target)
{
    CSessionClient* pClient = dynamic_cast<CSessionClient*>(target);
	CHECK_RETURN_DEBUG_VOID(NULL != pClient && sizeof(ServerProtocol::Head) <= len);
	CServerConfig * pServerConfig = CSessionServer::getInstance()->getServerCoinfig();
	CHECK_RETURN_DEBUG_VOID(pServerConfig != NULL);
	// ֱ��ת������Ϸ������
	ServerProtocol::Head* head = reinterpret_cast<ServerProtocol::Head*>(buffer);
	CHECK_RETURN_DEBUG_VOID(head != NULL);
	int nMainCmd = head->MainCommand();
	int nSubCmd = head->SubCommand();

	if (CSessionServer::getInstance()->getClosingFlag())//�жϷ������Ƿ���ƽ���ر�״̬���Ծ��������Ƿ����ת��
	{
		std::set<int>* cmdSet = pServerConfig->getClosingWhiteCmd();
		std::set<int>::iterator it = cmdSet->find(head->MainCommand());
		if (it == cmdSet->end())
		{
			CSessionServer::getInstance()->getGameEvent()->onUserEvent(EventClosed, target);//������ƽ���ر�ʱ�Կͻ��˵Ĵ���
			return;
		}
	}
	
	ServerRouteConfigData *pRouteConifgData = pServerConfig->getRouteConfigData(nMainCmd,nSubCmd);
	CHECK_RETURN_DEBUG_VOID(pRouteConifgData != NULL);
    // ���Ȩ������
	int cmdID = head->id;//·�ɵ�Session����ʱ����ͷid��ӦΪ���ش���ķ���
    if (pRouteConifgData->Permission <= (unsigned int)pClient->getPermission())
    {
        if (pClient->getPermission() == 0)
        {
            head->id = pClient->getGuestId();
        }
        else
        {
            head->id = pClient->getUserId();
        }

        switch (pRouteConifgData->RouteType)
        {
		case RouteSingle:
            pClient->sendDataToServer(pRouteConifgData->GroupID, pRouteConifgData->RouteKey, buffer, len);
            break;
		case RouteBroadcast:
            pClient->sendDataToGroupServer(pRouteConifgData->GroupID, buffer, len);
            break;
		case RouteReturn:
            pClient->sendData(buffer, len);
            break;
		case SesssionProc:
			//CSessionServer::getInstance()->getGameEvent()->onUserProc(cmdID);
			CSessionGMService::processGMLogic(buffer, len, pRouteConifgData->GroupID,pRouteConifgData->RouteKey, target);
			break;
        default:
            break;
        }
    }
}

void CSessionModule::processError(IKxComm *target)
{
    // ��Ҷ���
    this->userDisconnect(target);
}

// 1. �������з����������XXX������
// 2. ��ClientManager���Ƴ������
void CSessionModule::userDisconnect(IKxComm *target)
{
    CSessionClient *pClient = dynamic_cast<CSessionClient*>(target);
    if (pClient == NULL)
    {
        return;
    }

    // ����������Ϣ
    ServerProtocol::Head head;
    head.MakeCommand(ServerProtocol::ServerMain, ServerProtocol::ServerSubPlayerOffLine);
    head.length = sizeof(head);
    head.id = pClient->getUserId();

    // �������ݵ���˸�֪��ɫ����
    // ����������Ϣ�����ָ���ķ�����
    pClient->sendDataToAllServer(reinterpret_cast<char*>(&head), sizeof(head));
    // �ر�socket����NetWorkManager�к�clean�Ƴ�
    pClient->clean();
}
