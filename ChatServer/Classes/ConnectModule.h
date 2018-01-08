#ifndef __CHAT_CONNECT_MODULE_H__
#define __CHAT_CONNECT_MODULE_H__

#include "BaseModule.h"

class CConnectModule : public CBaseModule
{
public:
    CConnectModule(void);
    virtual ~CConnectModule(void);

    //��Ϸ���������ط�����Э�飬ת����ָ����ǰ��
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);

    //����Ϸ�������Ͽ����ӣ���������
    //����������newһ����ͨѶID�仯���ɿ�����������Դ��
    virtual void processError(KxServer::IKxComm *target);
    virtual void processEvent(int eventId, KxServer::IKxComm* target);
};

#endif
