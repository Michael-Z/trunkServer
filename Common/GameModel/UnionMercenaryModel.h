#ifndef __UNIONMERCENARY__MODEL_H
#define __UNIONMERCENARY__MODEL_H

#define MAX_EQUIP_EFFECT_NUM 8

#include "KxLog.h"
#include "IDBModel.h"
#include "Storage.h"
#include "RedisStorer.h"
#include "StorageManager.h"
#include "ModelDef.h"
#include <string>
#include <map>
#include <vector>
#include "ConfAnalytic.h"
#include "GameUserManager.h"
#include <stdio.h>

#define EQUIPMENTSMIN 0
#define EQUIPMENTSMAX 6
#define EQUIPEFFETCMIN 0
#define EQUIPEFFECTMAX 8
#define TALENTMAX 8
#define MAX_MERCENARY_PROP_NUM 119		//�洢�����ݿⵥ��Ӷ����������Ŀ

using namespace std;

typedef struct
{
	int nUid;					
	int nDispatchTime;
	int nRecallTime;
	int nEmployedTimes;
	SoldierCardData stSoldierCardData;
	SoldierEquip stEquipments[EQUIPMENTSMAX];
						
}UnionMercenaryInfo;

class CUnionMercenaryModel : public IDBModel
{
public:
	CUnionMercenaryModel();
	~CUnionMercenaryModel();

	virtual bool init(int unionID);

	//�����ݿ��ж�ȡ����Ӷ����Ϣ��������ǲ�ĺ��Ѿ��ٻصķ��ڲ�ͬ��map��
	virtual bool Refresh();

	//��ǲӶ��
	int DispatchMercenary(UnionMercenaryInfo& info);

	//�ٻ�Ӷ��
	bool RecallMercenary(int nMercenaryID);

	//��ӶӶ��
	void EmployedMercenary(int nMercenaryID);

	//�ӱ��غ����ݿ���ɾ��Ӷ��
	void DelMercenary(int nMercenaryID);

	//�����ٻ�ʱ�䳬��24Сʱ��Ӷ����ÿ����㶨ʱ����
	void ClearRecalled24HoursMercenary();

	//���µ���Ӷ����Ϣ
	void UpdateSingleMercenary(int nMercenaryID);

	//��õ���Ӷ����ϸ��Ϣ
	UnionMercenaryInfo* GetSingleMercenaryInfo(int nMercenaryID);

    //ֱ�����ݿ��л�ȡӶ����Ϣ
    static bool GetSingleMercenaryInfo(int unionID, int nMercenaryID, UnionMercenaryInfo* info);

	//���������ǲ�е�Ӷ���б�keyΪӶ��ID
	map<int, UnionMercenaryInfo>& GeUniontDispatchMercenaryInfo(){ return m_mapUnionDispatchingMercenaryInfo; }
	
private:
	//��װ���ݿ�洢��ʽ�ͽṹ�໥ת���ĺ���
	static void DBtoSt(string& str, UnionMercenaryInfo& info);
	void StoDB(UnionMercenaryInfo& info, string& str);

	int m_nUnionId;
	string m_strUnionMercenaryKey;
	CRedisStorer* m_pRedisStorer;
	map<int, UnionMercenaryInfo> m_mapUnionDispatchingMercenaryInfo;		//������ǲ�е�Ӷ��
	map<int, UnionMercenaryInfo> m_mapUnionRecalledMercenaryInfo;			//�Ѿ��ٻأ������ٻ�ʱ��û����24Сʱ��Ӷ��
};

#endif