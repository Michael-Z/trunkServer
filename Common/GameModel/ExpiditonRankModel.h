#ifndef _EXPIDITION_RANK_MODEL_H__
#define _EXPIDITION_RANK_MODEL_H__

#include "IDBModel.h"
#include "StorageManager.h"
#include <set>
#include <map>
#include <string.h>

struct SExpiditionRankDBData
{
	char szName[32];
	int nSummerID;						//�ٻ�ʦID
	int nHeroID[7];						//����ID
	int nStartID[7];					//�����Ǽ�
	unsigned char cBDType;				//��������
	unsigned char cBDLev;				//����ȼ�

	SExpiditionRankDBData()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct SDamageRankShowData
{
	int nIndex;							//����
	int nDamage;						//�˺�ֵ
	char szName[32];					//����
	int nSummerID;						//�ٻ�ʦID
	int nHeroID[7];						//����ID
	int nStartID[7];					//�����Ǽ�
	unsigned char cBDType;				//��������
	unsigned char cBDLev;				//����ȼ�

	SDamageRankShowData()
	{
		memset(this, 0, sizeof(*this));
	}

};

class CExpiditonRankModel : public IDBModel
{
public:
	CExpiditonRankModel();
	~CExpiditonRankModel();

	bool init(int nUnionID);
	bool Refresh();

	//������ݵ�Զ���˺����а���
	bool addDamageData(int nUid,int &nDamage,SExpiditionRankDBData &DBData);

	//��ȡĳ��Զ�����а�����
	bool getDamageData(int nUid, SDamageRankShowData &DBData);

	//��ȡ����
	bool getDamageRank(int nUid, int &nRank);

	//���õ�ͼID
	bool setMapID(int nMapID);

	int getMapID() { return m_nMapID; }

	//���Զ���˺����а�����
	bool clearDamageRankData();

	//��ȡ������ʾ��������
	std::map<int, SDamageRankShowData>& getShowDamageRankData() { return m_MapShowRankData; }

private:
	bool buildDamageRank();


private:
	int										 m_nUnionID;
	int										 m_nMapID;
	Storage *								 m_pStorage;
	std::string								 m_strExpiditionRankkey;
	std::string								 m_strExpiditionRankDataKey;
	std::map<int, SDamageRankShowData>		 m_MapShowRankData;
};


#endif //_EXPIDITION_RANK_MODEL_H__
