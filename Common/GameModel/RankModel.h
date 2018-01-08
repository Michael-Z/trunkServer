#ifndef _RANK_MODEL_H__
#define _RANK_MODEL_H__

#include <string>
#include <list>
#include <map>

#include "IDBModel.h"
#include "Storage.h"
#include "RedisStorer.h"

#define TOWER_LEVEL_POSITIONNUM 100000000  //�����������������λ��

//����PVP���а������������а�
enum RankType
{
	NONE_RANK_TYPE,
	LEVEL_RANK_TYPE,				//�ȼ����а�
	UNION_RANK_TYPE,				//�������а�
	TOWER_RANK_TYPE,				//�������а�
	MAX_RANK_TYPE,
};

struct SLevelRankKey
{
	int nLevel;
	int nUid;

	bool operator < (const SLevelRankKey &RankKey) const
	{
		if (nLevel > RankKey.nLevel)
		{
			return true;
		}
		else if (nLevel == RankKey.nLevel)
		{
			if (nUid < RankKey.nUid)
			{
				return true;
			}
		}

		return false;
	}

};

struct SLevelRankData
{
	int nHeadID;
	std::string UserName;
	std::string UnionName;
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�
};

struct SUnionRankKey
{
	int nUnionID;
	int nUnionLevel;

	bool operator < (const SUnionRankKey &RankKey) const
	{
		if (nUnionLevel > RankKey.nUnionLevel)
		{
			return true;
		}
		else if (nUnionLevel == RankKey.nUnionLevel)
		{
			if (nUnionID < RankKey.nUnionID)
			{
				return true;
			}
		}

		return false;
	}
};

struct SUnionRankData
{
	std::string UnionName;
	int			nUnionNum;
	int			nEmblemID;		//������
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�
};

struct STowerRankKey
{
	int nScore;
	int nMaxTowerLevel;
	int nUid;

	bool operator < (const STowerRankKey &RankKey) const
	{
		if (nScore > RankKey.nScore)
		{
			return true;
		}
		else if (nScore == RankKey.nScore)
		{
			if (nMaxTowerLevel > RankKey.nMaxTowerLevel)
			{
				return true;
			}
			else if (nMaxTowerLevel == RankKey.nMaxTowerLevel)
			{
				if (nUid < RankKey.nUid)
				{
					return true;
				}
			}
		}

		return false;
	}
};

struct STowerRankData
{
	int nHeadID;
	std::string UserName;
	std::string UnionName;
	int nLevel;
	unsigned char cBDType;		//��������
	unsigned char cBDLev;		//����ȼ�
};

class CRankModel : public IDBModel
{
public:

	static CRankModel* getInstance();

	static void destroy();

	virtual bool init();				//���а�ģ�����ڷ���������ʱ���У������Ҫ����ģ�͵��л�
	virtual bool Refresh();

	//������������
	bool AddRankData(int RType, int id, int Score);

	//��ȡ��ɫ����
	bool GetRankData(int RType, int id, int &Score);

	//ɾ����������
	bool DelRankData(int RType);

	bool getUserRankID(int uid, int& value);

	//��ȡ�ȼ���������
	std::map<SLevelRankKey, SLevelRankData>& GetLevelRankData() { return m_LevelShowRankMap; }

	//��ȡ������������
	std::map<SUnionRankKey, SUnionRankData>& GetUnionRankData(){ return m_UnionShowRankMap; }

	//��ȡ������������
	std::map<STowerRankKey, STowerRankData>& GetTowerRankData() { return m_TowerShowRankMap; }
	
	bool BuildLevelRank();

	bool BuildTowerRank();

	bool BuildUnionRank();

	bool setRankRewardFlag(int uid, int RType, int nFlag);

	bool getRankRewardFlag(int uid, int RType, int nFlag);

protected:
	CRankModel();
	~CRankModel();

    void clearData();

private:
	static CRankModel *							m_pInstance;
	Storage*									m_pStorage;				//���ݿ�
	std::map<SLevelRankKey, SLevelRankData>		m_LevelShowRankMap;		//�·���ʾ�ȼ�����
	std::map<STowerRankKey, STowerRankData>		m_TowerShowRankMap;		//�·���ʾ��������
	std::map<SUnionRankKey, SUnionRankData>		m_UnionShowRankMap;		//�·���ʾ
	int											m_CurRankCount[MAX_RANK_TYPE-1];			//��ǰ���а�����
	int											m_CurMinScore[MAX_RANK_TYPE-1];				//��ǰ���а���С��ֵ
	int											m_CurGetIndex[MAX_RANK_TYPE-1];				//���������ӳ�
	int											m_CurTimeCount;							//�������ֵ
};


#endif //_RANK_MODEL_H__
