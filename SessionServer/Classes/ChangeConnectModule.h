#ifndef _CHANGE_CONNECT_MODULE_H__
#define _CHANGE_CONNECT_MODULE_H__

#include "ConnectModule.h"

class CChangeConnectModule : public CBaseModule
{
public:
	CChangeConnectModule();
	~CChangeConnectModule();

	void init();

	//��Ϸ���������ط�����Э�飬ת����ָ����ǰ��
	virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);

	//����Ϸ�������Ͽ����ӣ���������
	//����������newһ����ͨѶID�仯���ɿ�����������Դ��
	virtual void processError(KxServer::IKxComm *target);
	virtual void processEvent(int eventId, KxServer::IKxComm* target);

	bool addCallBackClient(KxServer::IKxComm *target);

protected:
	KxServer::IKxComm*								m_GMClient;					// �������ӻص�Ӧ������
	int												m_nConnectNum;				//�����л�ĳ�����������
	CConnectModule*									m_pConnectModule;			//��Ϸ���Ӵ������

};

#endif //_CHANGE_CONNECT_MODULE_H__

