#ifndef __LOOK_SERVICE_H__
#define __LOOK_SERVICE_H__

#include "KXServer.h"

class CLookService
{
public:
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �鿴ĳ��Ӣ��
	static void ProcessLookHero(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �鿴ĳ��װ��
    static void ProcessLookEquip(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__LOOK_SERVICE_H__
