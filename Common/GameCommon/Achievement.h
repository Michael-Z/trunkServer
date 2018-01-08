#ifndef __ACHIEVEMENT_H__
#define __ACHIEVEMENT_H__

#include "UserActionListener.h"
#include "AchievementModel.h"
#include "ConfHall.h"

class CAchievement : public IUserActionListener
{
public:
    CAchievement();
    ~CAchievement();

    // ��ʼ���ɾ�
    bool init(int uid, int achieveID, AchieveDBInfo &info);
    // ��������
    virtual bool onAction(int actID, void *data, int len);

private:
    // ����ɾ͵ļ�������
    bool onUserLevelUpAction(int actID, void *data, int len);
    bool onAccumulateGoldAction(int actID, void *data, int len);
    bool onUpgradeHeroLvAction(int actID, void *data, int len);
    bool onHeroTestAction(int actID, void *data, int len);
    bool onGoldTestAction(int actID, void *data, int len);
    bool onShopBuyAction(int actID, void *data, int len);
    bool onUseSameRaceAction(int actID, void *data, int len);
    bool onDrawCardAction(int actID, void *data, int len);
    bool onBuyGoldAction(int actID, void *data, int len);

private:
    bool canFinishAchieve(int val = 1);
    bool finishHideAchieve();

private:
    int                 m_nUserID;          // �û�ID
    int                 m_nAchieveID;       // �ɾ�ID 
    AchieveDBInfo       m_AchieveInfo;      // ״̬/�ۼ�ֵ
    CAchievementModel*  m_pAchieveModel;    // �ɾ����ݿ�ģ��
    const AchieveItem*  m_pAchieveItemConf; // ���ñ�
};

#endif