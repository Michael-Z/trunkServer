#ifndef _PLAYER_MODULE_H__
#define _PLAYER_MODULE_H__

//������ͨѶ���ݴ���ģ��

#include "KXServer.h"

class CRebotPlayer;

class CPlayerModule : public KxServer::IKxModule
{
public:
	CPlayerModule();
	~CPlayerModule();

	void SetRebotPlayer(CRebotPlayer* pPlayer);
	virtual int processLength(char* buffer, unsigned int len);
	// IKxComm������յ��������ݺ󣬵��õĻص�
	virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
	// IKxComm����������ʱ�����õĻص�
	virtual void processError(KxServer::IKxComm *target);
private:
	CRebotPlayer			*m_pRebot;
};


#endif //_PLAYER_MODULE_H__
