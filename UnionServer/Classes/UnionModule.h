#ifndef _UNION_MODULE_H__
#define _UNION_MODULE_H__

#include "BaseModule.h"

class CUnionModule : public CBaseModule
{
public:
    CUnionModule();
    ~CUnionModule();

    // IKxComm������յ��������ݺ󣬵��õĻص�
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);

    // IKxComm����������ʱ�����õĻص�
    virtual void processError(KxServer::IKxComm *target);
};


#endif //_UNION_MODULE_H__
