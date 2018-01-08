/*
* ����㸨����
* 1���ṩ�������ݽӿ�
* 2���ṩ��������/�ɾ��¼��ӿ�
* 3���ṩ������Ӣ��ģ�ͽӿ�(����Ĭ�Ͻ�������/�츳)
* 4���ṩ����������ģ�ͽӿ�(������������ʱ�������)
* 5���ṩ��һ�ȡ�������ȼ��ӿ�(���������ж�)
*/

#ifndef __SERVICEHELPER_H__
#define __SERVICEHELPER_H__

#include "KxCommInterfaces.h"
#include "Protocol.h"
#include "GateManager.h"
#include "ServiceDef.h"
#include "HeroModel.h"
#include "Task.h"
#include "GameUserManager.h"
#include "UnionProtocol.h"
#include "StageProtocol.h"
#include "ServerProtocol.h"

#define DAY_TO_SECOND(D) D * 24 * 3600

class CServiceHelper
{
public:
	// ������Ϣ
    static int SentMsg(int uid, int maincmd, int subcmd, char *buffer, int len, KxServer::IKxComm *commu)
    {
		//KXLOGDEBUG("sentMsg main = %d sub = %d len = %d\n", maincmd, subcmd, len);
        return CGateManager::getInstance()->Transmit(uid, MakeCommand(maincmd, subcmd), buffer, len);
    }

    // ���ͽ�����Ϣ
    static int SentAwardMsg(int uid, int maincmd, int subcmd, KxServer::IKxComm *commu)
    {
        char *buffer = NULL; 
        int len = 0;

        return SentMsg(uid, maincmd, subcmd, buffer, len, commu);
    }
};

#endif 
