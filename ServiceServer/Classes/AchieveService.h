/*
* �ɾͷ����
* 1���жϼ����ǰ�˼����ĳɾ��Ƿ����
* 2����ɳɾ��·�����
*/

#ifndef __ACHIEVE_SERVICE_H__
#define __ACHIEVE_SERVICE_H__

#include "KxCommInterfaces.h"

class CAchieveService
{
public:
    static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��ɳɾ�
    static void processFinishService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��ȡ�ɾͽ���
    static void processAwardService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif