/*
 * ������
 * 1.�������������(��������)����ģ��
 * 2.��ȡ�������ģ��
 */

#ifndef __UNION_H__
#define __UNION_H__

#include "UnionModel.h"
#include "UnionExpiditionModel.h"
#include "ExpiditonRankModel.h"
#include "KXServer.h"
#include "UnionMercenaryModel.h"
#include "UnionShopModel.h"

class CUnion : public KxServer::KxTimerObject
{
public:
    CUnion();
    ~CUnion();

	void onTimer(const KxServer::kxTimeVal& now);

    // �¹����ʼ��(��û��ģ��)
    bool init(int unionID, int chairmanID, std::string unionName);
    // �Ѵ��ڹ������ģ�ͳ�ʼ��
    bool init(int unionID);

    CUnionModel* getUnionModel() { return m_pUnionModel; }
	CExpiditonRankModel* getExpiditionRankModel() { return m_pExpiditionRankModel; }
	CUnionExpiditionModel* getExpiditionModel() { return m_pUnionExpiditionModel; }
	CUnionMercenaryModel* getMercenaryModel(){ return m_pMercenaryModel; }
	CUnionShopModel*	  getUnionShopModel() { return m_pUnionShopModel; }
    // ��ȡ��������
    std::string getUnionName();
    // �Ƴ�����
    bool removeUnion();
    // ɾ������
    bool deleteUnion();
	//���ý�����ͼID
	void setRewardMapID(int nMapID) { m_RewardMapID = nMapID; }
	//��ȡ������ͼID
	int getRewardMapID() { return m_RewardMapID; }
	//�·�ˢ�¹����̵�UI
	bool sendUnionShopData(int nUid);

private:
    CUnionModel*			m_pUnionModel;
	CExpiditonRankModel*	m_pExpiditionRankModel;
	CUnionExpiditionModel*	m_pUnionExpiditionModel;
	CUnionShopModel*		m_pUnionShopModel;
	int						m_RewardMapID;
	CUnionMercenaryModel*	m_pMercenaryModel;
};

#endif