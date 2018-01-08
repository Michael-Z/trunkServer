#ifndef __CONF_ARENA_H__
#define __CONF_ARENA_H__

#include "ConfManager.h"

// pvp��λ��Ϣ
struct ArenaRankItem
{
	int ArenaLevel;				//�׶εȼ�
	float MMR_K;				//MMR_Kֵ
	float MMR_kx;				//MMR_kxֵ
	float Arena_K;				//Arena_Kֵ
	std::string GNPic;			//��λͼƬ�ز�
	VecInt GNRank;				//����������
	VecInt MMR_Range;			//MMR������
};

class CConfArenaRank : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

// pvpѵ��ʦ����
class CConfArenaTraining : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	std::map<int, int>& getArenaTrainings()
	{
		return m_mapArenaTraining;
	}

private:
	std::map<int, int> m_mapArenaTraining;
};

// ƥ���������Ϣ����
struct SArenaRobotItem
{
	int index;						  // id
	int robotType;					  // ����������, 1��, 2��ͨ, 3�߼�
	VecInt robotAP;					  // �����˻��ֲ�ֵ
	VecInt robotLevel;				  // �����˵ȼ���ֵ
	VecInt stageID;					  // ʵ�ʹؿ�
	VecInt robotTime;				  // ����ƥ������
};

class CConfArenaRobot : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	std::vector<SArenaRobotItem*> m_RobotEasy;
	std::vector<SArenaRobotItem*> m_RobotNormal;
	std::vector<SArenaRobotItem*> m_RobotAdvance;
};

// ������������ͷ������
struct ArenaRobotNameItem
{
	int IndexId;
	std::string robotName;
};

class CConfArenaRobotName : public CConfBase
{
public:
	virtual ~CConfArenaRobotName();
	virtual bool LoadCSV(const std::string& str);

	std::vector<ArenaRobotNameItem *>& getRobotItems()
	{
		return m_vecPvpRobotName;
	}

private:
	std::vector<ArenaRobotNameItem *> m_vecPvpRobotName;
};

// �����˹�������
class ArenaScollNameItem
{
public:
	int ComputerID;                // ����ID
	int ComputerName;              // ������(���԰�)
	std::string ComputerPic;       // �����ٻ�ʦͷ��
};

class CConfArenaScrollBar : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

// ʤ������
struct ArenaWinDrop
{
	VecInt WinLevelLimit;
	VecInt DropId;
};

// pvp�������
struct ArenaSettingItem
{
	int ChestPrice; 
	int ChestHalfPriceNum;
	int ActivityChestRate;
	int ChestRefreshTime;
	int LimitWaitingTime;
	int CriticalPoint;
	VecInt Low;
	VecInt Mid;
	VecInt High;
	VecInt LowTime;
	VecInt MidTime;
	VecInt HighTime;
	VecInt LowWaitingTime;
	VecInt MidWaitingTime;
	VecInt HighWaitinghTime;
	VecInt WinTimesLimit;
	VecInt ChestOrders;
	std::vector<ArenaWinDrop> WinDrops;
};

class CConfArenaSetting : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

// ��������
struct ArenaChestItem
{
	int ChestId;				//����id
	int ChestType;				//�������� 1��ͨ 2�
	int ChestLevel;				//����T�� 7���ȼ�
	int ChestQuality;			//Ʒ��
	int ChestWeight;			//Ȩ��
	int ChestName;				//��������
	int ChestDesc;				//��������
	int ChestDropId;			//�������
	std::string ChestIcon1;		//����ر�ͼ��
	std::string ChestIcon2;		//���俪��ͼ��
	VecInt ChestAppearLevel;	//���ֵȼ�������
};

class CConfArenaChest: public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);

	const std::vector<ArenaChestItem*>& getAreanChest(int type, int dan);

private:

	std::map<int, std::map<int, std::vector<ArenaChestItem*> > > m_Chests;
};

// ��ȡ������ģʽ�����ȼ�
class CConfArenaLevel : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	
	int getUnlockLevel(int mode);

	std::map<int, int>& getArenaLevels() 
	{ 
		return m_mapArenaLevel; 
	}

private:
	std::map<int, int> m_mapArenaLevel;
};

inline const ArenaRobotNameItem *randConfRobotName()
{
	CConfArenaRobotName *pConf = dynamic_cast<CConfArenaRobotName*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_ROBOTNAME));
	std::vector<ArenaRobotNameItem *> &datas = pConf->getRobotItems();
	if (datas.empty())
	{
		return NULL;
	}
	int index = rand() % datas.size();
	return reinterpret_cast<ArenaRobotNameItem *>(datas[index]);
}

inline const SArenaRobotItem *queryConfArenaRobotItem(int robotId)
{
	CConfArenaRobot *pConf = dynamic_cast<CConfArenaRobot*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_ROBOT));

	return reinterpret_cast<SArenaRobotItem*>(pConf->getData(robotId));
}

inline const SArenaRobotItem *randRobotWithLevel(int level)
{
	CConfArenaRobot *pConf = dynamic_cast<CConfArenaRobot*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_ROBOT));
	int index = 0;
	if (level == 0)
	{
		if (pConf->m_RobotEasy.empty())
		{
			return NULL;
		}
		index = rand() % pConf->m_RobotEasy.size();
		return pConf->m_RobotEasy[index];
	}
	else if (level == 1)
	{
		if (pConf->m_RobotNormal.empty())
		{
			return NULL;
		}
		index = rand() % pConf->m_RobotNormal.size();
		return pConf->m_RobotNormal[index];
	}
	else
	{
		if (pConf->m_RobotAdvance.empty())
		{
			return NULL;
		}
		index = rand() % pConf->m_RobotAdvance.size();
		return pConf->m_RobotAdvance[index];
	}
}

inline const ArenaSettingItem *queryArenaSetting()
{
	CConfArenaSetting *pSettingConf = dynamic_cast<CConfArenaSetting*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_SETTING));
	return reinterpret_cast<ArenaSettingItem *>(pSettingConf->getData(0));
}

inline const ArenaChestItem* queryConfArenaChestItem(int cheseId)
{
	CConfArenaChest *pConf = dynamic_cast<CConfArenaChest*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_CHEST));
	return static_cast<ArenaChestItem*>(pConf->getData(cheseId));
}

inline int queryPvpChestPrice(int count)
{
	CConfArenaSetting *pConf = dynamic_cast<CConfArenaSetting*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_SETTING));
	ArenaSettingItem *pSettingItem = reinterpret_cast<ArenaSettingItem *>(pConf->getData(0));
	if (count < pSettingItem->ChestHalfPriceNum) 
	{
		return pSettingItem->ChestPrice / 2;
	}
	return pSettingItem->ChestPrice;
}

inline const ArenaRankItem* queryConfArenaRankItem(int level)
{
	CConfArenaRank *conf = dynamic_cast<CConfArenaRank*>(
		CConfManager::getInstance()->getConf(CONF_ARENA_RANK));
	return static_cast<ArenaRankItem*>(conf->getData(level));
}


#endif //__CONF_ARENA_H__
