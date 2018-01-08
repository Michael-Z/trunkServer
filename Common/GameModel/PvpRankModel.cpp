#include "PvpRankModel.h"
#include "StorageManager.h"
#include "RedisStorer.h"
#include "ModelDef.h"
#include "CommonHelper.h"
#include "UserModel.h"
#include "UnionModel.h"
#include "PvpModel.h"
#include "UserUnionModel.h"
#include "CommonHelper.h"
#include "TimeCalcTool.h"
#include "ConfGameSetting.h"
#include "GameDef.h"
#include <algorithm>

using namespace KxServer;
using namespace std;

bool RankDataCompare(const RankData &Data1, const RankData &Data2)
{
	if (Data1.source > Data2.source)
	{
		return true;
	}
	else if (Data1.source == Data2.source)
	{
		if (Data1.id < Data2.id)
		{
			return true;
		}
	}

	return false;
}

CPvpRankModel *	CPvpRankModel::m_pInstance = NULL;

CPvpRankModel::CPvpRankModel()
{
	clearData();
}

CPvpRankModel::~CPvpRankModel()
{
}

CPvpRankModel* CPvpRankModel::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CPvpRankModel();
	}

	return m_pInstance;
}

void CPvpRankModel::destroy()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool CPvpRankModel::init(KxServer::KxBaseServer *pBaseServer)
{
	m_pStorage = StorageManager::getInstance()->GetStorage(STORAGEID_PVP);
	if (NULL == m_pStorage)
	{
		return false;
	}

	m_PvpRankKey = ModelKey::PvpRankKey();
	m_ChampionKey = ModelKey::ChampionRankKey();

	m_pServer = pBaseServer;

	return true;
}

bool CPvpRankModel::Refresh()
{
	clearData();
	BuildPvpRank();
	BuildChampionRank();
	
	return true;
}

//��ӽ�ɫ�����а���
bool CPvpRankModel::AddPvpRank(int uid, int nScore)
{
	if (nScore == 0)
	{
		int nRank = 0;
		if (!GetPvpRank(uid, nRank))
		{
			return false;
		}
	}

	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	if (SUCCESS != pRedisStorer->Zadd(m_PvpRankKey, uid, nScore))
	{
		return false;
	}

	m_CurGetIndex++;

	if (m_CurGetIndex >= 100)
	{
		m_CurGetIndex = 0;

		if (SUCCESS != pRedisStorer->ZCount(m_PvpRankKey, m_CurPvpRankCount))
		{
			return false;
		}
	}

	if (m_CurPvpRankCount > MAX_RANK_NUM)
	{
		//����1W����ȥ����Ĭ���ǰ���С��������
		if (SUCCESS != pRedisStorer->ZDel(m_PvpRankKey, MAX_RANK_NUM, m_CurPvpRankCount))
		{
			return false;
		}

		m_CurPvpRankCount = MAX_RANK_NUM;
	}

	return true;
}

//��Ӿ����������а���
bool CPvpRankModel::addChampionRank(int uid, int nScore)
{
	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	int nCurExpireTime = 0;
	if (SUCCESS != pRedisStorer->TTL(m_ChampionKey, nCurExpireTime))
	{
		return false;
	}

	if (nCurExpireTime < 0)
	{
		if (SUCCESS != pRedisStorer->DelKey(m_ChampionKey))
		{
			return false;
		}

		if (SUCCESS != pRedisStorer->Zadd(m_ChampionKey, uid,nScore))
		{
			return false;
		}

		//�������ܶ���ʱ��
		int CurTime = (int)time(NULL);
		int nNextTime = CTimeCalcTool::nextTimeStamp(CurTime, 0, 0, 2);
		int nIterval = nNextTime - CurTime;
		if (nIterval < 0)
		{
			nIterval = nIterval + 7 * 24 * 3600;
		}

		if (SUCCESS != pRedisStorer->Expire(m_ChampionKey,nIterval))
		{
			return false;
		}

	}
	else
	{
		if (SUCCESS != pRedisStorer->Zadd(m_ChampionKey, uid, nScore))
		{
			return false;
		}
	}

	m_CurGetIndex++;

	if (m_CurGetIndex >= 100)
	{
		m_CurGetIndex = 0;

		if (SUCCESS != pRedisStorer->ZCount(m_ChampionKey, m_CurChampRankCount))
		{
			return false;
		}
	}

	if (m_CurChampRankCount > MAX_RANK_NUM)
	{
		//����1W����ȥ����Ĭ���ǰ���С��������
		if (SUCCESS != pRedisStorer->ZDel(m_ChampionKey, MAX_RANK_NUM, m_CurChampRankCount))
		{
			return false;
		}

		m_CurChampRankCount = MAX_RANK_NUM;
	}

	return true;
}

//��ȡ��ɫ����
//Day ��ʶ��ȡ��������У����Ϊ0��������������
bool CPvpRankModel::GetPvpRank(int id, int &Rank, int Day)
{
	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	string Key;

	if (Day == 0)
	{
		Key = m_PvpRankKey;
	}
	else
	{
		Key = ModelKey::PvpBakRankKey(Day);
	}

	if (SUCCESS != pRedisStorer->ZRank(Key, id, Rank))
	{
		Rank = 0;
		return false;
	}

	return true;
}

//��ȡ����������
bool CPvpRankModel::getChampionRank(int id, int &Rank, int Day)
{
	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	string Key;
	if (Day == 0)
	{
		Key = m_ChampionKey;
	}
	else
	{
		Key = ModelKey::ChampionBakRankKey(Day);
	}

	if (SUCCESS != pRedisStorer->ZRank(m_ChampionKey, id, Rank))
	{
		Rank = 0;
		return false;
	}

	return true;
}

//��ȡ��ɫ����
bool CPvpRankModel::GetRankData(int id, int &Score, int Day)
{
	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	string Key;

	if (Day == 0)
	{
		Key = m_PvpRankKey;
	}
	else
	{
		Key = ModelKey::PvpBakRankKey(Day);
	}

	if (SUCCESS == pRedisStorer->ZScore(Key, id, Score))
	{
		return true;
	}

	Score = 0;
	return false;
}

//�������ݵ�����key��,ͬʱɾ����Ч����Key
bool CPvpRankModel::bakPvpData()
{
	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}
	//��ǰ����������
	//ʱ�������㣬�ǵڶ�����,�������60�뱣֤����
	int nCurTime = time(NULL) + 60;
	int nCurDay = CCommonHelper::getTotalDay(nCurTime) - 1;
	string BakKey = ModelKey::PvpBakRankKey(nCurDay);

	if (SUCCESS == pRedisStorer->ExistKey(BakKey))
	{
		return false;
	}

	vector<RankData> VectRankData;
	pRedisStorer->ZRange(m_PvpRankKey, VectRankData, 0, MAX_RANK_NUM);

	//�����������������÷�������֤������Ψһ��
	for (vector<RankData>::iterator riter = VectRankData.begin(); riter != VectRankData.end();++riter)
	{
		pRedisStorer->Zadd(BakKey, riter->id, riter->source);
	}

	int nExpireTime = 0;
	if (SUCCESS != pRedisStorer->TTL(BakKey, nExpireTime))
	{
		return false;
	}

	//-1ʱ����ʾkeyֵû�����ù���ʱ��
	//1��ʾkeyֵ�Ѿ�����
	if (nExpireTime < 0)
	{
		time_t nCurTime = time(NULL);
		nExpireTime = CTimeCalcTool::nextTimeStampToZero(nCurTime, 0) + 7 * 24 * 60 * 60 - (int)nCurTime;

		if (SUCCESS != pRedisStorer->Expire(BakKey, nExpireTime))
		{
			return false;
		}
	}

	return true;
}

//�����������һʱ����������
bool CPvpRankModel::bakChampionRank()
{
	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}
	//��ǰ����������
	//ʱ�������㣬�ǵڶ�����
	//�����ܶ���ʱ���
	int nExpireTime = 0;
	time_t nCurTime = time(NULL);
	int nNextTime = CTimeCalcTool::nextTimeStamp(nCurTime, 0, 0, 2);
	int nKeyDay = CCommonHelper::getTotalDay(nNextTime);
	string Key = ModelKey::ChampionBakRankKey(nKeyDay);
	vector<RankData> VectRankData;
	if (SUCCESS != pRedisStorer->ZRange(m_ChampionKey, VectRankData, 0, MAX_RANK_NUM))
	{
		return false;
	}
	//�����������������÷�������֤������Ψһ��
	for (vector<RankData>::iterator riter = VectRankData.begin(); riter != VectRankData.end(); ++riter)
	{
		pRedisStorer->Zadd(Key, riter->id, riter->source);
	}

	if (SUCCESS != pRedisStorer->TTL(Key, nExpireTime))
	{
		return false;
	}

	//-1ʱ����ʾkeyֵû�����ù���ʱ��
	//1��ʾkeyֵ�Ѿ�����
	if (nExpireTime < 0)
	{
		nExpireTime = CTimeCalcTool::nextTimeStampToZero(nCurTime, 0) + 30 * 24 * 60 * 60 - (int)nCurTime;

		if (SUCCESS != pRedisStorer->Expire(Key, nExpireTime))
		{
			return false;
		}
	}

	return true;
}

bool CPvpRankModel::setPvpRewardFlag(int uid, int nDay)
{
	if (uid == 0)
	{
		return false;
	}

	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	std::string RewardKey = ModelKey::PvpRankReward(uid);
	std::set<int> RewardFlag;
	if (SUCCESS != pRedisStorer->GetSetAll(RewardKey, RewardFlag))
	{
		return false;
	}

	if (RewardFlag.find(nDay) != RewardFlag.end())
	{
		return false;
	}

	if (SUCCESS != pRedisStorer->SetAdd(RewardKey,nDay))
	{
		return false;
	}

	return true;
}

bool CPvpRankModel::getPvpRewardFlag(int uid, int nDay)
{
	if (uid == 0)
	{
		return false;
	}

	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	std::string RewardKey = ModelKey::PvpRankReward(uid);
	std::set<int> RewardFlag;
	if (SUCCESS != pRedisStorer->GetSetAll(RewardKey, RewardFlag))
	{
		return false;
	}

	if (RewardFlag.find(nDay) != RewardFlag.end())
	{
		return false;
	}

	return true;
}

bool CPvpRankModel::cleanPvpRewardFlag(int uid, int nStartDay)
{
	if (uid == 0)
	{
		return false;
	}

	CRedisStorer *pRedisStorer = static_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	std::string RewardKey = ModelKey::PvpRankReward(uid);
	std::set<int> RewardFlag;
	if (SUCCESS != pRedisStorer->GetSetAll(RewardKey, RewardFlag))
	{
		return false;
	}

	if (RewardFlag.size() < MAX_FLAG_NUM)
	{
		return false;
	}

	for (std::set<int>::iterator ator = RewardFlag.begin(); ator != RewardFlag.end();)
	{
		if ((*ator) < nStartDay)
		{
			RewardFlag.erase(ator++);
			if (SUCCESS != pRedisStorer->SetDel(RewardKey, (const char*)&(*ator), sizeof(int)))
			{
				continue;
			}
		}
		else
		{
			ator++;
		}
	}

	return true;
}

bool CPvpRankModel::BuildPvpRank()
{
	m_ShowPvpRank.clear();
	string strUserKey;
	string strPvpKey;
	string UnionKey;
    string strUserUnionKey;
	string strDBKey;

	SPvpRankData PvpRankData;
	SPvpRankKey  PvpRankKey;
	int nUnionID = 0;

	CRedisStorer *pRedisStorer = dynamic_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	if (SUCCESS != pRedisStorer->ZCount(m_PvpRankKey, m_CurPvpRankCount))
	{
		return false;
	}

	vector<RankData> pvpRankData;
	if (SUCCESS != pRedisStorer->ZRange(m_PvpRankKey, pvpRankData, 0, MAX_RANK_SHOW_NUM - 1))
	{
		return false;
	}

	for (vector<RankData>::iterator ator = pvpRankData.begin(); ator != pvpRankData.end(); ++ator)
	{
		CRedisStorer *pStorer = dynamic_cast<CRedisStorer*>(m_pStorage->GetStorer(ator->id));
		if (NULL == pStorer)
		{
			continue;
		}

		PvpRankKey.nUid = ator->id;
		PvpRankKey.nScore = ator->source;
		PvpRankKey.nMMR = 0;
		PvpRankData.nHeadID = 0;
		PvpRankData.nLevel = 0;
		PvpRankData.UnionName = "";
		PvpRankData.UserName = "";
		PvpRankData.cBDType = 0;
		PvpRankData.cBDLev = 0;
		strUserKey	= ModelKey::UsrKey(ator->id);
		strPvpKey = ModelKey::PvpKey(ator->id);
        strUserUnionKey = ModelKey::UserUnionKey(ator->id);
		strDBKey = ModelKey::ckExtraDataKey(ator->id);
		nUnionID = 0;

		//�û���
		pStorer->GetHashByField(strUserKey, USR_FD_USERNAME, PvpRankData.UserName);
		//ͷ��
		pStorer->GetHashByField(strUserKey, USR_FD_HEADICON, PvpRankData.nHeadID);
		//�ȼ�
		pStorer->GetHashByField(strUserKey, USR_FD_USERLV, PvpRankData.nLevel);

		std::string strQQExtraData;
		//û������
		if (SUCCESS == pStorer->GetString(strDBKey, strQQExtraData))
		{
			strQQExtraData = "[" + strQQExtraData + "]";
			//�����꣬���ݱ���Ϸ�
			VecInt VectValue;
			CConfAnalytic::ToJsonInt(strQQExtraData, VectValue);
			if (VectValue.size() != 3)
			{
				return false;
			}

			PvpRankData.cBDType = VectValue[0];
			PvpRankData.cBDLev = VectValue[1];
		}

		//����ID
		pStorer->GetHashByField(strUserUnionKey, USER_UNION_ID, nUnionID);
		if (nUnionID != 0)
		{
			UnionKey = ModelKey::UnionKey(nUnionID);
			pStorer->GetHashByField(UnionKey, UNION_FIELD_UNIONNAME, PvpRankData.UnionName);
		}

		//PVP MMR
		pStorer->GetHashByField(strPvpKey, PVP_FD_MMR, PvpRankKey.nMMR);
		m_ShowPvpRank[PvpRankKey] = PvpRankData;
	}

	if (m_CurPvpRankCount >= MAX_RANK_NUM)
	{
		vector<RankData> lastRankData;
		if (SUCCESS != pRedisStorer->ZRange(m_PvpRankKey, lastRankData, 10000, 10000)) //��ȡ���һ�����а�����
		{
			return false;
		}

		if (lastRankData.size() != 0)
		{
			m_CurPvpMinScore = lastRankData[0].source;
		}
	}
	else
	{
		m_CurPvpMinScore = 0;
	}

	return true;
}

bool CPvpRankModel::BuildChampionRank()
{
	m_ShowChampionRank.clear();
	string strUserKey;
	string strPvpKey;
	string UnionKey;
    string strUserUnionKey;
	string strDBKey;

	SPvpRankData PvpRankData;
	SPvpRankKey  PvpRankKey;
	int nUnionID = 0;

	CRedisStorer *pRedisStorer = dynamic_cast<CRedisStorer*>(m_pStorage->GetStorer(1));
	if (pRedisStorer == NULL)
	{
		return false;
	}

	if (SUCCESS != pRedisStorer->ZCount(m_ChampionKey, m_CurChampRankCount))
	{
		return false;
	}

	vector<RankData> championRankData;
	if (SUCCESS != pRedisStorer->ZRange(m_ChampionKey, championRankData, 0, MAX_RANK_SHOW_NUM - 1))
	{
		return false;
	}


	for (vector<RankData>::iterator ator = championRankData.begin(); ator != championRankData.end(); ++ator)
	{
		CRedisStorer *pStorer = dynamic_cast<CRedisStorer*>(m_pStorage->GetStorer(ator->id));
		if (NULL == pStorer)
		{
			continue;
		}

		PvpRankKey.nUid = ator->id;
		PvpRankKey.nScore = ator->source;
		PvpRankKey.nMMR = 0;
		PvpRankData.nHeadID = 0;
		PvpRankData.nLevel = 0;
		PvpRankData.UnionName = "";
		PvpRankData.UserName = "";
		PvpRankData.cBDType = 0;
		PvpRankData.cBDLev = 0;
		nUnionID = 0;
		strUserKey = ModelKey::UsrKey(ator->id);
		strPvpKey = ModelKey::PvpKey(ator->id);
        strUserUnionKey = ModelKey::UserUnionKey(ator->id);
		strDBKey = ModelKey::ckExtraDataKey(ator->id);

		//�û���
		pStorer->GetHashByField(strUserKey, USR_FD_USERNAME, PvpRankData.UserName);
		//ͷ��
		pStorer->GetHashByField(strUserKey, USR_FD_HEADICON, PvpRankData.nHeadID);
		//�ȼ�
		pStorer->GetHashByField(strUserKey, USR_FD_USERLV, PvpRankData.nLevel);
		
		std::string strQQExtraData;
		//û������
		if (SUCCESS == pStorer->GetString(strDBKey, strQQExtraData))
		{
			strQQExtraData = "[" + strQQExtraData + "]";
			//�����꣬���ݱ���Ϸ�
			VecInt VectValue;
			CConfAnalytic::ToJsonInt(strQQExtraData, VectValue);
			if (VectValue.size() != 3)
			{
				return false;
			}

			PvpRankData.cBDType = VectValue[0];
			PvpRankData.cBDLev = VectValue[1];
		}

		//����ID
		pStorer->GetHashByField(strUserUnionKey, USER_UNION_ID, nUnionID);
		if (nUnionID != 0)
		{
			UnionKey = ModelKey::UnionKey(nUnionID);

			if (SUCCESS != pStorer->GetHashByField(UnionKey, UNION_FIELD_UNIONNAME, PvpRankData.UnionName))
			{
				continue;
			}
		}
		//PVP MMR
		pStorer->GetHashByField(strPvpKey, CPN_FD_MMR, PvpRankKey.nMMR);

		m_ShowChampionRank[PvpRankKey] = PvpRankData;
	}

	if (m_CurChampRankCount >= MAX_RANK_NUM)
	{
		vector<RankData> lastRankData;
		if (SUCCESS != pRedisStorer->ZRange(m_ChampionKey, lastRankData, 10000, 10000)) //��ȡ���һ�����а�����
		{
			return false;
		}

		if (lastRankData.size() != 0)
		{
			m_CurChampMinScore = lastRankData[0].source;
		}
	}
	else
	{
		m_CurChampMinScore = 0;
	}

	return true;
}

void CPvpRankModel::clearData()
{
	m_CurPvpRankCount = 0;			//��ǰ���а�����
	m_CurPvpMinScore = 0;		//��ǰ���а���С����
	m_CurGetIndex = 0;			//���������ӳ�
	m_ShowPvpRank.clear();
	m_ShowChampionRank.clear();
	m_CurChampRankCount = 0;	//��ǰ�������а�����
	m_CurChampMinScore = 0;
	m_CurGetIndex = 0;
	m_ChampionIDSet.clear();
}
