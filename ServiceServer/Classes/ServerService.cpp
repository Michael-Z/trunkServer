#include "ServerService.h"
#include "ServerProtocol.h"

#include "ChallengeRoomManager.h"
#include "KxCommManager.h"
#include "GameUserManager.h"
#include "CommOssHelper.h"
#include "LoginProtocol.h"
#include "Protocol.h"
#include "ModelHelper.h"
#include "ServiceServer.h"

using namespace KxServer;

void CServerService::processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
	case ServerProtocol::ServerSubPlayerOffLine:
		processPlayerOffLine(uid, buffer, len, commun);
		break;
	case ServerProtocol::ServerSubDisconnect:
		processPlayerDisConnect(uid, buffer, len, commun);

	default:
		break;
	}
}

void CServerService::processPlayerOffLine(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CKxCommManager::getInstance()->delKxComm(uid);
    //����û��Ƿ����
    CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid, false);
    if (pGameUser == NULL)
	{
		return;
	}

	CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
	if (pUserModel == NULL)
	{
		return;
	}

    int nLoginTime = 0;
    int nCurTime = CServiceServer::getInstance()->getTimerManager()->getTimestamp();
	pUserModel->SetUserFieldVal(USR_FD_LOGINOUTTIME, nCurTime);
	pUserModel->GetUserFieldVal(USR_FD_LOGINTIME, nLoginTime);
	CModelHelper::DispatchActionEvent(uid, ELA_OLTIME, NULL, 0);
	CCommOssHelper::userLoginOutOss(uid,nLoginTime);
	CGameUserManager::getInstance()->removeGameUser(uid);
}

void CServerService::processPlayerDisConnect(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	if (len != sizeof(ServerProtocol::SProDisConect))
	{
		return;
	}

	ServerProtocol::SProDisConect *pMsg = (ServerProtocol::SProDisConect*)buffer;

	KxServer::IKxComm * pDisConectComm = CKxCommManager::getInstance()->getKxComm(pMsg->nDisConectuid);

	if (pDisConectComm == NULL)
	{
		//���Ͽ����û������Ѿ��Ͽ�
		return;
	}

	//���͸������Ϣ
	CKxCommManager::getInstance()->sendData(pMsg->nDisConectuid, CMD_LOGIN, CMD_LOGIN_TICK_SC, NULL, 0);

	//�ڲ���Ϣ��T�����
	unsigned int buffSize = sizeof(Head);
	char* buff = reinterpret_cast<char*>(KxServer::kxMemMgrAlocate(buffSize));

	//����ͷ��
	Head* head = reinterpret_cast<Head*>(buff);
	head->MakeCommand(ServerProtocol::ServerMain, ServerProtocol::ServerSubDisconnect);
	head->length = buffSize + len;
	head->id = pMsg->nDisConectuid;

	pDisConectComm->sendData(buff, buffSize);
}
