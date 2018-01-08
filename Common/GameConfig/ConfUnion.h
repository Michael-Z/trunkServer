#ifndef _CONF_UNION_H_
#define _CONF_UNION_H_

#include "ConfManager.h"
#include "ConfGameSetting.h"

#define MAX_MAP_INDEX 15		//һ����ͼ���15�����е�

/////////////////////////////////�������/////////////////////////////////////////
class UnionLevelItem
{
public:
    int UnionLv;            // ����ȼ�
    int ViceChairmanNum;    // ���᳤����
    int MemberLimit;        // ��Ա����
    int ActiveMin;          // ����Ծ��׼
    int ActiveReward;       // �ﵽ�����Ļ�Ծ�ȱ�׼
    int RewardID;           // �����Ļ�Ծ�Ƚ���
    int ActiveSReward;      // �ﵽ������Ծ�Ƚ����ı�׼
    int SRewardID;          // ������Ծ�Ƚ���
    int UpLevelCost;        // ������������
    int DownLevelCost;      // ������������
    int UnActiveReduce;     // ������۳�����
};

struct UnionItem
{
    int UnLockLv;           // ��������ȼ�
    int CostCoin;           // ����������
    int AuditTime;          // ���ʧЧʱ��(��λ:��)
    int ApplyCD;            // �˻�������ʱ��(��λ:��)
    int ApplyCount;         // �������
    int ChangeNameCost;     // ��������(��ʯ)
	int nExpiditionTimes;	// Զ������
	int nRewardSendTime;	// ��������ʱ����
};

struct SMapWeight
{
    int mapId;
    int weight;

    SMapWeight()
    {
        mapId = 0;
        weight = 0;
    }
};

struct SExpeditonWorldItem
{
    int nWorldID;							  //�����ͼid
    int nUnLockLv;							  //�����ȼ�
    std::string strIcon;					  //������ǩ
	int nFightTime;							  //��սʱ��
	int nColdTime;							  //��ս��Ϣʱ��
    std::string strName;					  //��������
    std::string strDesc;					  //��������
	std::vector<SMapWeight> vecMap;			  //��ͼ

    SExpeditonWorldItem()
    {
        nWorldID = 0;
        nUnLockLv = 0;
        strIcon.clear();
        vecMap.clear();
        nColdTime = 0;
        nFightTime = 0;
        strName.clear();
        strDesc.clear();
    }
};

//�ؿ���������
struct SExpeditonIndexData
{
	int				nStageID;				  //�ؿ�ID
	int				nStageLv;				  //�ؿ��ȼ�
	int				nDescID;				  //�ؿ�����
	int				nTitleID;				  //�ؿ�����
	int		        nHeadDesc;				  //�ؿ���սͷ��tips
	int             nHeadName;				  //�ؿ���ս��
	int				nColor;					  //�ؿ���·��ɫ
	int				nBossHp;				  //�ؿ�bossѪ��
	std::string     strHeadRes;				  //�ؿ���ս������
	std::string     strHeadTag;				  //�ؿ��������ǩ
	std::string		strThumbnail;			  //�ؿ�����ͼͼ��
	std::string		strBackground;			  //�ؿ�����ͼͼƬ
	std::string		strHeadIcon;			  //�ؿ���սͷ��
	VecInt			unlockIndexs;			  //�ؿ������ؿ�
	VecInt			StageMapBuff;			  //�ؿ�Buff
	
    SExpeditonIndexData()
    {
        nStageID = 0;
        nStageLv = 0;
        nDescID = 0;
        nTitleID = 0;
        strThumbnail.clear();
        strBackground.clear();
        strHeadIcon.clear();
        nHeadDesc = 0;
        nHeadName = 0;
        strHeadRes.clear();
        strHeadTag.clear();
        unlockIndexs.clear();
        nColor = 0;
        StageMapBuff.clear();
        nBossHp = 0;
    }
};

struct SExpeditionMapData
{
	int nStageMapID;							//Զ���ؿ���ͼID
	int nUnlockLv;								//��ͼ�����ȼ�
	int nTotal;									//�����ٹ�
	int nMapName;								//�ؿ���ͼ����
	int nWinGoodsID;							//ͨ���������
	int nUnionPrestige;							//��������
	int nGoodsID[4];							//�˺���һ, �ڶ�, ����, 4-10���⽱��
	VecInt vecShopGoods;						//�����̵�������Ʒ
	VecInt vecStartStages;						//Ĭ�Ͽ����Ĺؿ����
    std::string strMapScence;					//�ؿ���ͼ����
    SExpeditonIndexData stageList[15];			//�ؿ��б���Ϣ
	
    SExpeditionMapData()
    {
        nStageMapID = 0;
        nUnlockLv = 0;
        nTotal = 0;
        nMapName = 0;
        nWinGoodsID = 0;
        memset(nGoodsID, 0, sizeof(nGoodsID));
		nUnionPrestige = 0;
    }
};

//���䵥��
struct UnionDropIdData
{
	int DropID;
	int DropRate;
	VecInt DropNum;
};

// �������
class UnionShopDropData
{
public:
	int						DropRuleID;			   //�������ID
	VecInt					MeanwhileDropNum;	   //���ε�����������
	std::vector<UnionDropIdData>	DropIDs;	   //����ID��

	UnionShopDropData& operator = (UnionShopDropData &Item)
	{
		DropRuleID = Item.DropRuleID;
		MeanwhileDropNum.assign(Item.MeanwhileDropNum.begin(), Item.MeanwhileDropNum.end());
		DropIDs.assign(Item.DropIDs.begin(), Item.DropIDs.end());
		return (*this);
	}
};


////////////////////////////////�������//////////////////////////////////////////
class CConfUnionLevel : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfUnion : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    NO_KEY_DATA(UnionItem);
};

class CConfExpeditionWorld : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfExpeditionMap : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfUnionBadge : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    std::map<int, std::string>& getBadges(){ return m_MapUnionBadge; }

private:
    std::map<int, std::string> m_MapUnionBadge;
};

class CConfUnionDrop : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

////////////////////////////////��ѯ���//////////////////////////////////////////
inline const UnionLevelItem* queryConfUnionLevel(const int& lv)
{
    CConfUnionLevel *confUnionLevel = dynamic_cast<CConfUnionLevel*>(
        CConfManager::getInstance()->getConf(CONF_UNION_LEVEL));
    return static_cast<UnionLevelItem*>(confUnionLevel->getData(lv));
}

inline const UnionItem& queryConfUnion()
{
    CConfUnion* conf = dynamic_cast<CConfUnion*>(
        CConfManager::getInstance()->getConf(CONF_UNION));
    return conf->getData();
}

inline const SExpeditonWorldItem* queryConfExpiditionWordData(int nWorldID)
{
    CConfExpeditionWorld* conf = dynamic_cast<CConfExpeditionWorld*>(
        CConfManager::getInstance()->getConf(CONF_UNIONEXPIDTION));
    return static_cast<SExpeditonWorldItem*>(conf->getData(nWorldID));
}

inline SExpeditionMapData* queryConfExpiditionMapData(int nMapID)
{
    CConfExpeditionMap* conf = dynamic_cast<CConfExpeditionMap*>(
        CConfManager::getInstance()->getConf(CONF_UNIONEXPIDTION_MAP));
    return static_cast<SExpeditionMapData*>(conf->getData(nMapID));
}

inline bool hasUnionBadge(int nBadgeID)
{
    CConfUnionBadge* conf = dynamic_cast<CConfUnionBadge*>(
        CConfManager::getInstance()->getConf(CONF_UNIONBADGE));

    std::map<int, std::string>& mapBadges = conf->getBadges();

    return mapBadges.find(nBadgeID) != mapBadges.end();
}

// ��ѯ����������
inline const UnionShopDropData* queryConfUnionDropProp(int UniondropId)
{
	CConfUnionDrop *conf = dynamic_cast<CConfUnionDrop*>(
		CConfManager::getInstance()->getConf(CONF_UNIONSHOPDROP));
	return static_cast<UnionShopDropData*>(conf->getData(UniondropId));
}

#endif //_CONF_UNION_H_
