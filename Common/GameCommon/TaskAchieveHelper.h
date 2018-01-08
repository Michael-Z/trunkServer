/*
* ����㸨����
* 1���ṩ�������ݽӿ�
* 2���ṩ��������/�ɾ��¼��ӿ�
* 3���ṩ������Ӣ��ģ�ͽӿ�(����Ĭ�Ͻ�������/�츳)
* 4���ṩ����������ģ�ͽӿ�(������������ʱ�������)
* 5���ṩ��һ�ȡ�������ȼ��ӿ�(���������ж�)
*/

#ifndef __TASK_ACHIEVE_HELPER_H__
#define __TASK_ACHIEVE_HELPER_H__

#include "TaskModel.h"

class CTaskAchieveHelper
{
public:
    // ��ʼ������
    static bool initTask(int uid);
    // ��ʼ���ɾ�
    static bool initAchieve(int uid);

    // ���������
    static bool addTask(int uid, int taskID);
    // ��ȡ�������������ʱ���
    static int getTaskResetTimeStamp(int taskID);
    // ��������Ƿ���Ҫ����
    static void checkTask(int uid, int taskID, TaskDBInfo &info);
    // ʵ��������
    static bool instanceTask(int uid, int taskID);
    // �ж����������Ƿ����
    static bool isFinishSpecialTask(int uid, int taskID, const TaskDBInfo& taskInfo);
    // ���״̬�����Ƿ����
    static bool canFinishTask(int uid, int taskID);

    // ����³ɾ�
    static bool addAchieve(int uid, int achieveID, bool instance = false);
    // ʵ�����ɾ�
    static bool instanceAchieve(int uid, int achieveID);
    // ���ɾ��Ƿ����(���ص���ɳɾͻ��Զ������³ɾ�)
    static bool canFinishAchieve(int uid, int achieveID, int times = 0);
    // �ж�Ӣ����صĳɾ��Ƿ����
    static bool canFinishHeroAchieve(int uid, int actID, int compelteTimes);

private:
    static bool canFinishCommon(int uid, int type, int compelteTimes, int finishParam);
    static bool needListenTask(int actID);
    static bool needListenAchieve(int actID);
};

#endif 
