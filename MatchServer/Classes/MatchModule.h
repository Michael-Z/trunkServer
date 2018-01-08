#ifndef _MATCH_MODULE_H__
#define _MATCH_MODULE_H__

#include "KXServer.h"

class CMatchModule : public KxServer::IKxModule
{
public:
	CMatchModule();
	virtual ~CMatchModule();

	virtual int processLength(char* buffer, unsigned int len);

	// IKxComm������յ��������ݺ󣬵��õĻص�
	virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
	// IKxComm����������ʱ�����õĻص�
	virtual void processError(KxServer::IKxComm *target);

};

#endif //_MATCH_MODULE_H__
