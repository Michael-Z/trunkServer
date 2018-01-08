/*
*   
*
*
*/
#ifndef __SESSION_MODULE_H__
#define __SESSION_MODULE_H__

#include "BaseModule.h"

class CSessionServer;
class CServerModel;

class CSessionModule : public CBaseModule
{
public:
    CSessionModule(void);
    virtual ~CSessionModule(void);

    //����ͻ��˷�����Ϣ
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
    //�ͻ�����Session�������Ͽ���֪ͨ��Ϸ�������������������Դ
    virtual void processError(KxServer::IKxComm *target);

private:
    //��Ҷ���
    void userDisconnect(KxServer::IKxComm *target);
};

#endif
