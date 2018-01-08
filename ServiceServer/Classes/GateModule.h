/*
* Gate���ģ��
* ����ǰ�˻Ự������������
* ��GameServer��Listener����
* ���ڲ�����Ҫ������ӻᱻǿ�ƹر�
*
* 2013-11-1 By ��ү
*
*/
#ifndef __GATE_MODULE_H__
#define __GATE_MODULE_H__

#include "KXServer.h"

class CGateModule : public KxServer::IKxModule
{
public:
	CGateModule(void);
	virtual ~CGateModule(void);

	//return the length your package need
	virtual int processLength(char* buffer, unsigned int len);

	//when you recv a msg, Process will be call, the target is the msg sender
	virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);

	virtual void processError(KxServer::IKxComm *target) { }
};

#endif
