#ifndef __PVPRANKMODEL_H__
#define __PVPRANKMODEL_H__

#include "IDBModel.h"
#include "Storage.h"
#include "RedisStorer.h"
#include "KXServer.h"
#include <string>
#include <map>

#define MAX_FLAG_NUM 50

struct SPvpRankKey
{
	int nUid;
	int nScore;
	int nMMR;

	bool operator < (const SPvpRankKey &RankKey) const
	{
		if (nScore > RankKey.nScore)
		{
			return true;
		}
		else if (nScore == RankKey.nScore)
		{
			if (nMMR > RankKey.nMMR)
			{
				return true;
			}
			else if (nMMR == RankKey.nMMR)
			{
				if (nUid < RankKey.nUid)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		
		return false;
	}
};

struct SPvpRankData
{
	std::string UserName;
	int			nHeadID;
	std::string UnionName;
	int			nLevel;
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�	
};

class CPvpRankModel : public IDBModel
{
public:
	static CPvpRankModel* getInstance();

	static void destroy();

	bool init(KxServer::KxBaseServer *pBaseServer = NULL);

	bool Refresh();

	//��ӽ�ɫ�����а���
	bool AddPvpRank(int uid, int nScore);

	//��Ӿ����������а���
	bool addChampionRank(int uid, int nScore);

	//��ȡ��ɫ����
	//Day ��ʶ��ȡ��������У����Ϊ0��������������
	bool GetPvpRank(int id, int &Rank,int Day = 0);

	//��ȡ����������
	bool getChampionRank(int id, int &Rank, int Day = 0);

	//����PVPǰһ�����������
	bool bakPvpData();

	//�����������һʱ����������
	bool bakChampionRank();

	//��ս���������
	bool clearChampionRank();

	//��ȡ��ɫ����
	bool GetRankData(int id, int &Score,int Day = 0);

	//��ȡ����PVP��������
	std::map<SPvpRankKey, SPvpRankData>& GetShowPvpRankMap() { return m_ShowPvpRank; }

	//��ȡ���������а�����
	std::map<SPvpRankKey, SPvpRankData>& getShowChampionMap() { return m_ShowChampionRank; }

	bool setPvpRewardFlag(int uid, int nDay);

	bool getPvpRewardFlag(int uid, int nDay);

	bool cleanPvpRewardFlag(int uid, int nStartDay);	
	
	bool BuildPvpRank();

	bool BuildChampionRank();

private:

	CPvpRankModel();
	~CPvpRankModel();

	void clearData();

private:

	std::string										m_PvpRankKey;
	std::string										m_ChampionKey;			//������Key
	Storage*										m_pStorage;				//���ݿ�
	std::map<SPvpRankKey, SPvpRankData>				m_ShowPvpRank;			//�·�PVP���а�����
	std::map<SPvpRankKey, SPvpRankData>				m_ShowChampionRank;		//�·����������а�����
	static CPvpRankModel *							m_pInstance;
	KxServer::KxBaseServer *						m_pServer;				//����������ָ��
	int												m_CurPvpRankCount;		//��ǰ���а�����
	int												m_CurPvpMinScore;		//��ǰ���а���С����
	int												m_CurChampRankCount;	//��ǰ�������а�����
	int												m_CurChampMinScore;		//��ǰ�������а���С����
	int												m_CurGetIndex;			//���������ӳ�
	//�Ѿ�������
	std::set<int>									m_ChampionIDSet;		//�������б�
};

#endif //__PVPRANKMODEL_H__
