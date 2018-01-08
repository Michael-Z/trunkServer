#ifndef __ACHIEVEMENT_MODEL_H__
#define __ACHIEVEMENT_MODEL_H__

#include <map>
#include "Storage.h"
#include "IDBModel.h"

struct AchieveDBInfo
{
    int achieveVal;              // �ɾ�ֵ
    int achieveStatus;           // �ɾ�״̬
};

enum EAchieveStatus
{
    EACHIEVE_STATUS_UNACTIVE = -1,// δ����
    EACHIEVE_STATUS_ACTIVE,       // ����
    EACHIEVE_STATUS_FINISH,       // ���
    EACHIEVE_STATUS_GET,          // ��ȡ
};

class CAchievementModel : public IDBModel
{
public:
    CAchievementModel();
    ~CAchievementModel();

    bool init(int uid);

    bool Refresh();
    // ��ӳɾ�
    bool AddAchievement(int achieveID, AchieveDBInfo &info);
    // �洢�ɾ�
    bool SetAchievement(int achieveID, AchieveDBInfo &info);
    // ��ȡĳ���ɾ͵�ֵ
	bool GetAchievementByID(int achieveID, AchieveDBInfo &info, bool bNew = false);
    // ��ȡ���гɾ�
    std::map<int, AchieveDBInfo>& GetAllAchievement();
    // ɾ��ĳ���ɾ�
    bool RemoveAchievementByID(int achieveID);
    // ɾ���ɾ�
    bool RemoveAchievement();

protected:
	bool GetAchievementFromDB(int achieveID, AchieveDBInfo &info);

private:
    int					 m_nUid;
    Storage *			 m_pStorage;
    std::map<int, AchieveDBInfo>   m_mapAllAchievement;   // <�ɾ�ID, �ɾ���Ϣ>
};

#endif