/*
* �ɾͷ����
* 1���жϼ����ǰ�˼����ĳɾ��Ƿ����
* 2����ɳɾ��·�����
*/

#ifndef __STORY_SERVICE_H__
#define __STORY_SERVICE_H__

#include "KxCommInterfaces.h"

class CGuideService
{
public:

	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);

	static void processRecord(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif