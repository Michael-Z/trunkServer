#include "PersonMercenaryModel.h"

CPersonMercenaryModel::CPersonMercenaryModel() :m_pRedisStorer(NULL)
{
}

CPersonMercenaryModel::~CPersonMercenaryModel()
{
}

bool CPersonMercenaryModel::init(int uid)
{
	Storage* pStorage = StorageManager::getInstance()->GetStorage(STORAGEID_UNION);//�õ����ݿ����
	if (NULL == pStorage)
	{
		KXLOGDEBUG("PersonMercenaryModel GetStorage Error");
		return false;
	}

	m_pRedisStorer = dynamic_cast<CRedisStorer*>(pStorage->GetStorer(uid));
	if (NULL == m_pRedisStorer)
	{
		KXLOGDEBUG("PersonMercenaryModel GetCRedisStorer Error");
		return false;
	}

	m_nUid = uid;
	m_strDispatchingMercenaryKey = ModelKey::PersonDispatchMercenaryKey(uid);
	m_strEmployedMercenaryKey = ModelKey::PersonEmployedMercenaryKey(uid);
	Refresh();

	return true;
}

bool CPersonMercenaryModel::Refresh()
{
	m_pRedisStorer->GetHash(m_strDispatchingMercenaryKey, m_mapPersonDispatchMercenaryInfo);
	m_pRedisStorer->GetSetAllString(m_strEmployedMercenaryKey, m_setPersonEmployedMercenaryInfo);

	return true;
}

//��ǲӶ����ָ��field����¼��Ӷ��ID������Ӷ��ID����ȥ��������Ӷ����ϸ��Ϣ
bool CPersonMercenaryModel::DispatchMercenary(int nMercenaryID, int field)
{
	if (m_mapPersonDispatchMercenaryInfo.size() != PERSONMAXDISPATCH)
	{
		m_mapPersonDispatchMercenaryInfo[field] = nMercenaryID;
		m_pRedisStorer->SetHashByField(m_strDispatchingMercenaryKey, field, nMercenaryID);

		return true;
	}
	return false;
}

//�ٻ�Ӷ��
bool CPersonMercenaryModel::RecallMercenary(int nMercenaryID)
{
	for (map<int, int>::iterator mapit = m_mapPersonDispatchMercenaryInfo.begin(); mapit != m_mapPersonDispatchMercenaryInfo.end(); ++mapit)
	{
		if (mapit->second == nMercenaryID)
		{
			m_pRedisStorer->DelHashByField(m_strDispatchingMercenaryKey, mapit->first);
			m_mapPersonDispatchMercenaryInfo.erase(mapit->first);
			return true;
		}
	}

	return false;
}

//��¼��Ӷ��Ӷ��ID
void CPersonMercenaryModel::AddEmployedMercenaryInfo(int nMercenaryID)
{
	m_setPersonEmployedMercenaryInfo.insert(nMercenaryID);
	m_pRedisStorer->SetAdd(m_strEmployedMercenaryKey, nMercenaryID);

	//��½ʱ���ù�Ӷ����Ӷ���б����ʱ�䣬��ֹ�������رն�û����������Ӷ����Ӷ������
	int nNowTime = (int)time(NULL);
	m_pRedisStorer->Expire(m_strEmployedMercenaryKey, CTimeCalcTool::nextTimeStampToZero(nNowTime, 0) - nNowTime);
}

//ÿ����㶨ʱ�����Ӷ����Ӷ���б�
void CPersonMercenaryModel::ClearAllEmployedMercenaryInfo()
{
	m_setPersonEmployedMercenaryInfo.clear();
	m_pRedisStorer->DelKey(m_strEmployedMercenaryKey);
}

//���������ǲ��Ӷ��
void CPersonMercenaryModel::ClearAllDispatchedMercenary()
{
	m_mapPersonDispatchMercenaryInfo.clear();
	m_pRedisStorer->DelKey(m_strDispatchingMercenaryKey);
}

//����Ӷ��ID�����Ǽ���λ
int CPersonMercenaryModel::GetMercenaryField(int nMercenaryID)
{
	for (map<int, int>::iterator mapit = m_mapPersonDispatchMercenaryInfo.begin(); mapit != m_mapPersonDispatchMercenaryInfo.end(); ++mapit)
	{
		if (mapit->second == nMercenaryID)
		{
			return mapit->first;
		}
	}
	return 0;
}

void CPersonMercenaryModel::SetOffRemoveMercenaryInfo(int nUid, std::map<int, int>& MemrcenaryInfo)
{
	Storage* pStorage = StorageManager::getInstance()->GetStorage(STORAGEID_UNION);//�õ����ݿ����
	if (NULL == pStorage)
	{
		KXLOGDEBUG("PersonMercenaryModel GetStorage Error");
		return;
	}

	CRedisStorer* pRedisStorer = dynamic_cast<CRedisStorer*>(pStorage->GetStorer(nUid));
	if (NULL == pRedisStorer)
	{
		KXLOGDEBUG("PersonMercenaryModel GetCRedisStorer Error");
		return;
	}

	string strDispatchingMercenaryKey = ModelKey::PersonDispatchMercenaryKey(nUid);
	string strEmployedMercenaryKey = ModelKey::PersonEmployedMercenaryKey(nUid);

	pRedisStorer->GetHash(strDispatchingMercenaryKey,MemrcenaryInfo);
	pRedisStorer->DelKey(strDispatchingMercenaryKey);
	pRedisStorer->DelKey(strEmployedMercenaryKey);
}


