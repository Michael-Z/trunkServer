#include "ConnectModule.h"
#include "ServerProtocol.h"
#include "NetworkManager.h"
#include "ChatClient.h"
#include "ChatServer.h"
#include "ChatHelper.h"

using namespace KxServer;
using namespace ServerProtocol;

CConnectModule::CConnectModule(void)
{
}

CConnectModule::~CConnectModule(void)
{
}

void CConnectModule::processLogic(char* buffer, unsigned int len, IKxComm *target)
{
    // ����ָ����ǰ��
    Head* head = reinterpret_cast<Head*>(buffer);
    if (head->MainCommand() != ServerMain || len < sizeof(Head))
    {
        return;
    }
    switch (head->SubCommand())
    {
        // Ϊ�û�����Ȩ��
    case ServerSubSetPermission:
		CChatHelper::ServerSubSetPermission(buffer, len, target, head);
        break;
        // �Ͽ��û�,����̨������Ϣ
    case ServerSubDisconnect:
		CChatHelper::ServerSubDisconnect(buffer, len, target, head, this);
        break;
        // ����ĳ����ҵ�·��ֵ
    case ServerSubSetRoute:
		CChatHelper::ServerSubSetRoute(buffer, len, head);
        break;
        // ��������
    case ServerSubTransmit:
		CChatHelper::ServerSubTransmit(buffer, len, head);
        break;
        // �㲥���������
    case ServerSubBoardCast:
		CChatHelper::ServerSubBoardCast(buffer, len);
        break;
		// ��֤����
    case ServerSubInit:
		CChatHelper::ServerSubInit(buffer, len, this);
		break;
        // ��ѯ��ǰ���������
    case ServerSubQueryLoad:
		CChatHelper::ServerSubQueryLoad(buffer, len, head);
        break;
    case ServerSubForward:
        CChatHelper::ServerSubForward(buffer, len, head);
        break;
    default:
        break;
    }
}

void CConnectModule::processError(IKxComm *target)
{
	// ������ӶϿ�
    KXLOGDEBUG("Socket Connect To Server Failed");
}

void CConnectModule::processEvent(int eventId, IKxComm* target)
{
    if (eventId == KXEVENT_CONNECT_FAILE)
    {
        KXLOGDEBUG("Connect Server Failed");
    }
    else if (eventId == KXEVENT_CONNECT_SUCCESS)
    {
        KXLOGDEBUG("Connect Server Success");
    }
}
