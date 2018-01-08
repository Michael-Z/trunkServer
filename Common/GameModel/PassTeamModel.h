#ifndef __PASSTEAMMODEL_H__
#define __PASSTEAMMODEL_H__

#include <map>
#include <string>
#include "IDBModel.h"

enum EPassTeamType
{
	PASSTEAM_FIRST = 0,
	PASSTEAM_TEAM1,
	PASSTEAM_TEAM2,
	PASSTEAM_TEAM3,
};

struct PassTeam
{
	int			stamp;
	int			summonerId;
	int			userLv;
	int			heroCount;
	std::string usrname;
};

struct PassTeamHero
{
	int			confId;
	int			star;
	int			level;
	int			talent;
	int			equipCount;
};

struct passTeamEquip
{
	int			confId;
};

struct PassTeamData
{
	char *data;
	int len;
};

class Storage;
class CPassTeamModel : public IDBModel
{
public:
	CPassTeamModel();
	~CPassTeamModel();

public:
	//��ʼ��
	bool init();
	//ˢ��������Ϣ
	bool Refresh();
	// ����ͨ�ض���, �����ڵ���ʱ���, д��ʱ��������ͷ��ѹ���������teamtype
	bool SetPassTeam(int stageId, int teamType, void *data, int len);
	// ���ͨ�ض���, passTeamDataͷ4���ֽ�Ϊ��������teamType
	void GetPassTeam(int stageId, int teamType, PassTeamData &teamData);
	// ���ָ���ؿ����������͵�ͨ�ض���
	void GetPassTeam(int stageId, std::map<int, PassTeamData>& passTeamMap);

private:
	Storage *									  m_pStorage;
	std::string									  m_strPassTeamKey;
	std::map<int, std::map<int, PassTeamData> >   m_mapPassTeam;
};

#endif 
