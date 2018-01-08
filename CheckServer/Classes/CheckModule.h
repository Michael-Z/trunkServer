#ifndef _CCHECK_MODULE_H__
#define _CCHECK_MODULE_H__

#include "BaseModule.h"

class CCheckModule : public CBaseModule
{
public:
	CCheckModule();
	~CCheckModule();

	// IKxComm������յ��������ݺ󣬵��õĻص�
	virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
	// IKxComm����������ʱ�����õĻص�
	virtual void processError(KxServer::IKxComm *target);
	// IKxComm�����ڲ����������¼�ʱ�����Ļص�������TCP���������ӳɹ�
	virtual void processEvent(int eventId, KxServer::IKxComm* target) {}

protected:

	//������Ե�½
	void ProcessTestLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//����ƽ̨��¼
	void ProcessPFLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//�����ο͵�½
	void ProcessGuestLoign(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//�οͰ�ƽ̨��¼
	void ProcessBPFGuestLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//����ͨ��ƽ̨��¼����
	void processAnyPFLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//�����ο͵�½
	void ProcessGuestAnyPFLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//�οͰ�ͨ��ƽ̨��¼
	void processsBAnyPFGuestLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

    // ���������������¼
    void processsChatLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//����GM ��½
	void processGMLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//����ʹ�������˺������½
	void processExistUserLogin(char* buffer, unsigned int len, KxServer::IKxComm *target);
};

#endif //_CCHECK_MODULE_H__
