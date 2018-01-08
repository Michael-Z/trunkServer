#include "SummonerEvent.h"
#include "NetworkManager.h"
#include "KXServer.h"
#include "SessionClient.h"
#include "SessionServer.h"
#include "Protocol.h"
#include "LoginProtocol.h"
#include "KXServer.h"

using namespace KxServer;

CSummonerEvent::CSummonerEvent()
{
}


CSummonerEvent::~CSummonerEvent()
{
}

bool CSummonerEvent::onUserProc(unsigned int id)
{
	
	return true;
}

bool CSummonerEvent::onUserEvent(int nType, KxServer::IKxComm* target)
{
    CSessionClient *pClient = static_cast<CSessionClient *>(target);
    if (pClient != NULL)
    {
        switch (nType)
        {
            // ��¼-����Ƿ��ظ���¼�����򶥺�
            case EventLogin:
            {
                CSessionClient* oldClient = dynamic_cast<CSessionClient*>(
                    CNetWorkManager::getInstance()->getUser(pClient->getUserId()));
                if (oldClient != NULL && oldClient != target)
                {
                    // ������Ϣ��ǰ�ˡ�����˺��Ѿ������豸��¼����Ȼ��ɵ�
                    oldClient->retain();
                    sendDataToActor(oldClient, CMD_LOGIN, CMD_LOGIN_RECHANGE_SC, NULL, 0);
                    oldClient->clean();
                    oldClient->release();
                }
            }
            break;
            // ��T��
            case EventKick:
            {
                sendDataToActor(pClient, CMD_LOGIN, CMD_LOGIN_TICK_SC, NULL, 0);
            }
            break;
        }
    }
    return true;
}

bool CSummonerEvent::sendDataToActor(CSessionClient* pClient, int nMainCmd, int nSubCmd, char *pszContext, int nLen)
{
    unsigned int buffSize = sizeof(Head);
    bool ret = false;
	char* buff = reinterpret_cast<char*>(kxMemMgrAlocate(buffSize));

	//����ͷ��
	Head* head = reinterpret_cast<Head*>(buff);
	head->MakeCommand(nMainCmd,nSubCmd);
	head->length = buffSize + nLen;
    head->id = pClient->getUserId();
    ret = pClient->sendData(buff, buffSize) >= 0;
    if (ret && NULL != pszContext && nLen > 0)
	{
        ret = pClient->sendData(pszContext, nLen) >= 0;
    }
	kxMemMgrRecycle(buff, buffSize);
    return ret;
}
