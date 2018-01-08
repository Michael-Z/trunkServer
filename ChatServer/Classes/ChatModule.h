#ifndef _CHAT_MODULE_H__
#define _CHAT_MODULE_H__

#include "BaseModule.h"

class CChatModule : public CBaseModule
{
public:
    CChatModule();
    ~CChatModule();

	// IKxComm������յ��������ݺ󣬵��õĻص�
	virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
	// IKxComm����������ʱ�����õĻص�
	virtual void processError(KxServer::IKxComm *target);
	// IKxComm�����ڲ����������¼�ʱ�����Ļص�������TCP���������ӳɹ�
	virtual void processEvent(int eventId, KxServer::IKxComm* target) {}

protected:
private:
    //��Ҷ���
    void userDisconnect(KxServer::IKxComm *target);
};

#endif //_CHAT_MODULE_H__
