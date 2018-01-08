#ifndef _UNION_EXPIDITION_MODEL_H__
#define _UNION_EXPIDITION_MODEL_H__

#include "IDBModel.h"
#include "StorageManager.h"
#include <set>
#include <map>
#include <string.h>

enum 
{
	UNION_EXPIDITION_STAGELIST,			//�����ؿ��б�
	UNION_EXPIDITION_BOSS_HPLIST,		//BOSSѪ���б�
	UNION_EXPIDITION_STAGE_LIST,		//ͨ���˺������ʾ�б�
	UNION_EXPIDITION_WORLDID,			//ѡ��������ID
	UNION_EXPIDITION_MAPID,				//��ǰѡ����ͼID
	UNION_EXPIDITION_FINISHTIME,		//ս������ʱ���
	UNION_EXPIDITION_COLDFINISHTIME,	//��ȴ����ʱ���
	UNION_EXPIDITION_FINISHFLAG,		//�Ƿ�ͨ��
	UNION_EXPIDITION_REWARDSENDTIME,	//��������ʱ���
};

struct SUnionStageShowData
{
	int  nDamage;
	int  nHeadID;
	char szName[32];
	unsigned char cLevel;

	SUnionStageShowData()
	{
		memset(this, 0, sizeof(*this));
	}
};

class CUnionExpiditionModel : public IDBModel
{
public:
	CUnionExpiditionModel();
	~CUnionExpiditionModel();

	bool init(int nUnionID);
	bool Refresh();
	//��ȡ��ǰBOSSѪ��
	bool getCurExpiditionBossHp(int nIndex,int &BossHp);
	//����BOSSѪ��
	bool setCurExpiditionBossHp(int nIndex, int BossHp);
	//���BOSSѪ������
	bool clearCurExpiditionBossHp(int nIndex = 0);
	//�����ɹؿ�
	bool addExpiditionFinishIndex(int nIndex);
	//У��ؿ��Ƿ�Ϊ��ɹؿ�
	bool checkExpiditionFinishIndex(int nIndex);
	//�����ɹؿ�
	bool clearExpiditionFinishIndex();
	//��ȡ������ɹؿ�
	std::set<int>& getAllFinishIndex() { return m_SetFinishIndex; }
	//��ȡԶ������
	bool getExpiditionFieldValue(int nIndex, int &nValue);
	//����Զ������
	bool setExpiditinoFieldVale(int nIndex, int nValue);

	//���ùؿ�������ʾ����˺�
	bool setStageIndexShowData(int nIndex, SUnionStageShowData &Data);
	bool clearStageIndexShowData();
	bool getStageIndexShowData(int nIndex, SUnionStageShowData &Data);


private:
	//����BOSSѪ��
	bool saveBossHpToDB();
	//������ɹؿ�����
	bool saveExpiditionIndexToDB();
	//����ؿ�������ʾ����˺�����
	bool saveStageIndexShowData();
	//����ؿ�������ʾ����˺�����
	bool clearStageIndexDBShowData();
private:
	int					 m_nUnionID;
	Storage *			 m_pStorage;
	std::string			 m_strUnionExpiditionkey;

	std::map<int, int>	m_MapBossHp;
	std::set<int>		m_SetFinishIndex;
	std::map<int, int>	m_MapValue;
	std::map<int, SUnionStageShowData> m_MapDamageShowData;
};


#endif //_UNION_EXPIDITION_MODEL_H__
