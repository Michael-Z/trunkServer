#ifndef __PERSONMERCENARY__MODEL_H
#define __PERSONMERCENARY__MODEL_H

#include "IDBModel.h"
#include "Storage.h"
#include "RedisStorer.h"
#include "StorageManager.h"
#include <string>
#include "KxLog.h"
#include "ModelDef.h"
#include "TimeCalcTool.h"

#define PERSONMAXDISPATCH 100

using namespace std;

class CPersonMercenaryModel : public IDBModel
{
public:
	CPersonMercenaryModel();
	~CPersonMercenaryModel();

	virtual bool init(int uid);
	virtual bool Refresh();
	
	//��ǲӶ�����ֳɲ�ͬ��field��ǰ������λ��
	bool DispatchMercenary(int nMercenaryID, int field);

	//�ٻ�Ӷ��
	bool RecallMercenary(int nMercenaryID);

	//����һ����Ӷ����Ӷ��
	void AddEmployedMercenaryInfo(int nMercenaryID);

	//����Ѿ���Ӷ����Ӷ���б�
	void ClearAllEmployedMercenaryInfo();

	//���ȫ����ǲ��Ӷ����Ϣ
	void ClearAllDispatchedMercenary();

	//�ж�Ӷ���Ǵ����ĸ�field,1��λ��ȫ����Ҷ��У�2��λ��VIP�ȼ�4���ϲ�����ǲ
	int GetMercenaryField(int nMercenaryID);

	//�õ��û���ǲ��Ӷ���б�keyΪ�û���field��ֵΪӶ��ID
	map<int, int>& GetPersonDispatchMercenaryInfo(){ return m_mapPersonDispatchMercenaryInfo; }		

	//�õ��û���Ӷ����Ӷ���б�
	set<int>& GetPersonEmployedMercenaryInfo()
	{ 
		//�������´����ݿ�ȡ����ֹ���ݲ�һ��
		m_pRedisStorer->GetSetAll(m_strEmployedMercenaryKey, m_setPersonEmployedMercenaryInfo); 
		return m_setPersonEmployedMercenaryInfo; 
	}

	static void SetOffRemoveMercenaryInfo(int nUid, std::map<int, int>& MemrcenaryInfo);

private:
	CRedisStorer* m_pRedisStorer;
	int m_nUid;
	string m_strDispatchingMercenaryKey;
	string m_strEmployedMercenaryKey;
	//����ʾ����λ��ֵ��ʾӶ��ID
	map<int, int> m_mapPersonDispatchMercenaryInfo;
	set<int> m_setPersonEmployedMercenaryInfo;
};

#endif