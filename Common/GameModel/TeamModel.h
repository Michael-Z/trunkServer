#ifndef __TEAM_MODEL_H__
#define __TEAM_MODEL_H__

#include <map>
#include <vector>
#include "Storage.h"
#include "IDBModel.h"

#define MAX_HERO_COUNT 7

struct TeamInfo
{
    int teamType;       // ��������
    int summonerId;     // �ٻ�ʦID
	int heroId[MAX_HERO_COUNT];		// Ӣ��id
};

class CTeamModel : public IDBModel
{
public:
    CTeamModel();
    ~CTeamModel();

    bool init(int uid);

    bool Refresh();
    // ���ö���
    bool SetTeamInfo(int teamType, TeamInfo &teamInfo);
    // ��ȡ����
    bool GetTeamInfo(int teamType, TeamInfo& teamInfo);
    // ĳ��Ӣ���Ƿ���ڶ�����,���ж���
    bool IsExistInTeams(int heroId);
    // ɾ�������е�ĳ��Ӣ��,���ж���
    void RemoveHeroFromTeams(int heroId);

    std::map<int, TeamInfo>& getAllTeamInfo(){ return m_mapTeamInfo; }

private:
    int                      m_nUID;             // �û�ID
    std::string              m_strTeamKey;       // ����KEY
    std::map<int, TeamInfo>  m_mapTeamInfo;      // ��������,������Ϣ
    Storage*                 m_pStorage;		 // ���ݿ�
};

#endif