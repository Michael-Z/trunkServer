#include "PlayerModule.h"
#include "RebotPlayer.h"
#include "Protocol.h"
#include "RebotManager.h"
#include "ServerProtocol.h"


CPlayerModule::CPlayerModule()
{
}


CPlayerModule::~CPlayerModule()
{
}

void CPlayerModule::SetRebotPlayer(CRebotPlayer* pPlayer)
{

}

int CPlayerModule::processLength(char* buffer, unsigned int len)
{
	if (len < sizeof(int))
	{
		//len����Ϊ4���ֽ�, �ϲ�����������4���ֽ���ȴ�������.
		return sizeof(int);
	}
	else
	{
		//KXLOGDEBUG("CBaseModule Request Len %d", *(int*)(buffer));
		return *(int*)(buffer);
	}
}

// IKxComm������յ��������ݺ󣬵��õĻص�
void CPlayerModule::processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target)
{
	Head *pHead = reinterpret_cast<Head*>(buffer);
	int cmd = pHead->MainCommand();
	int subCmd = pHead->SubCommand();

	char *pData = reinterpret_cast<char*>(pHead->data());
	int uid = pHead->id;
	int length = len - sizeof(Head);

	CRebotPlayer *pPlayer = CRebotManager::getInstance()->GetRebotPlayer(uid);

	if (pPlayer == NULL)
	{
		return;
	}

	pPlayer->ProcessRecvData(cmd, subCmd, pData, length);
}

// IKxComm����������ʱ�����õĻص�
void CPlayerModule::processError(KxServer::IKxComm *target)
{

}
