/*
* ������
*   ��������������񲢴洢����
*/

#ifndef __TASK_H__
#define __TASK_H__

#include "UserActionListener.h"
#include "ConfHall.h"
#include "TaskModel.h"

class CTask : public IUserActionListener
{
public:
    CTask();
    ~CTask();

    // ��ʼ������
    bool init(int userID, int taskID, const TaskDBInfo& info);
    
    // ��������
    virtual bool onAction(int actID, void *data, int len);

private:
    //////////////////////������������////////////////////////
    bool onUserLevelUpAction(int actID, void *data, int len);
    bool onPassStageAction(int actID, void *data, int len);
    bool onHeroEquipAction(int actID, void *data, int len);
    bool onHeroLevelUpAction(int actID, void *data, int len);
    bool onHeroStarAction(int actID, void *data, int len);
    bool onHeroSkillAction(int actID, void *data, int len);
    bool onDrawCardAction(int actID, void *data, int len);
    bool onUseExpBookAction(int actID, void *data, int len);
    bool onBuyGoldAction(int actID, void *data, int len);

    bool onHeroTestAction(int actID, void *data, int len);
    bool onGoldTestAction(int actID, void *data, int len);
    bool onTowerFloorAction(int actID, void *data, int len);
    bool onPvpAction(int actID, void *data, int len);
    bool onEquipAction(int actID, void *data, int len);

private:
    bool canFinish(int val = 1);

private:
    int             m_nUserID;        // �û�ID
    int             m_nTaskID;        // ����ID 
    TaskDBInfo      m_TaskInfo;       // ������Ϣ
    CTaskModel     *m_pTaskModel;     // ��������ģ��
    const TaskItem* m_pTaskItemConf;  // ���ñ�
};

#endif