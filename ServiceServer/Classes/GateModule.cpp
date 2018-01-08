#include "GateModule.h"
#include "ServiceServer.h"
#include "KxCommManager.h"

using namespace KxServer;

CGateModule::CGateModule(void)
{
}

CGateModule::~CGateModule(void)
{
}

int CGateModule::processLength(char* buffer, unsigned int len)
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

void CGateModule::processLogic(char* buffer, unsigned int len, IKxComm *target)
{
	//IKxComm* gate = CGateManager::getInstance()->GetGate();
// 	if (NULL == gate)
// 	{
	//CGateManager::getInstance()->SetGate(target);
    CKxCommManager::getInstance()->setGate(target);
    CServiceServer::getInstance()->getMainPoller()->addCommObject(target, target->getPollType());
// 	}
// 	else
// 	{
// 		//�ر�֮ǰҲ����Է���һ����Ϣ��ȥ���������رյ�ԭ��
// 		target->close();
// 	}
}
