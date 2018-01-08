#ifndef _SESSION_GM_SERVICE_H__
#define _SESSION_GM_SERVICE_H__

#include "KXServer.h"

class CSessionGMService
{
public:
	static void processGMLogic(char* buffer, unsigned int len,int nGroupID,int nKey,KxServer::IKxComm *target);

protected:
	//����GM ��ȡ�����������
	static void processGetServerNum(KxServer::IKxComm *target);
	//����GM ����ƽ���ط�����
	static void processCloseServer(KxServer::IKxComm *target);
	//����GM �����ȸ��·���������
	static void processDynUpdateServer(char *pData,int nLen,KxServer::IKxComm *target);
	//����GM ��������ת������� �����
	static void processGMRouteChatServer(char *pData, int nLen,int nGroupID,int nKey,KxServer::IKxComm *target);

};

#endif //_SESSION_GM_SERVICE_H__

