#ifndef _LOGIN_UNION_SERVICE_H__
#define _LOGIN_UNION_SERVICE_H__

#include "KXServer.h"

class CLoginUnionService
{
public:
    static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    
    // �����½�����
    static void processLoginUnionService(int uid, char *buffer, int len, KxServer::IKxComm *commun);

};

#endif //_LOGIN_UNION_SERVICE_H__
