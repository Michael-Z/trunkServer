/*
* ��������߼�
* 1���������,�ж�ǰ�˴����״̬�����Ƿ���ɲ��洢
* 2����ȡ����,�ж������Ƿ����,��ɿ�����������
*   �����������ʱ���,������������ɾ��
*/

#ifndef __TASK_SERVICE_H__
#define __TASK_SERVICE_H__

#include "KxCommInterfaces.h"

class CTaskService
{
public:
    static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �������
    static void processFinishService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��ȡ����
    static void processAwardService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif