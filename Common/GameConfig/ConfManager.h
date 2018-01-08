/*
* �����ļ�������
* �ṩ���������ļ��ļ��غ͹���
* �ṩ���ö���Ļ���
*
* 2015-1-5 By ��ү
*/
#ifndef __CONF_MANAGER_H__
#define __CONF_MANAGER_H__

#include "KxCSComm.h"
#include "CommTools.h"
#include "CsvLoader.h"
#include "ConfAnalytic.h"

// ��������
inline void fillIntVec(int times, CCsvLoader& loader, VecInt& out)
{
	int value = 0;
	for (int i = 0; i < times; ++i)
	{
		value = loader.NextInt();
		if (0 != value)
		{
			out.push_back(value);
		}
	}
}

inline void fillFloatVec(int times, CCsvLoader& loader, VecFloat& out)
{
	float value = 0.0f;
	for (int i = 0; i < times; ++i)
	{
		value = loader.NextFloat();
		if (-0.00000001f > value || 0.00000001f < value)
		{
			out.push_back(value);
		}
	}
}

// �����ļ�������
enum ConfType
{
	CONF_BASE,                  // ���û���ƫ��ֵ����ʵ������
	CONF_AIDATA,                // AI
	CONF_BUFF,                  // Buff
	CONF_BULLET,                // �ӵ�
	CONF_CALL,                  // �ٻ�
	CONF_CAMERA,                // ���
	CONF_AUDIO,                 // ��Ч����
	CONF_CARD_COUNT,            // ��Ƭ����
	CONF_COUNT,                 // ����
	CONF_CRYSTAL,               // ˮ��
	CONF_HERO,                  // Ӣ��
	CONF_SKILL,                 // ����
	CONF_SOLDIER,               // սʿ
	CONF_BOSS,					// BOSS
	CONF_MONSTER,				// MONSTER
	CONF_STATUS,                // ״̬
	CONF_UI_EFFECT,             // UI
	CONF_EFFECT,                // ��Ч
	CONF_RESPATH,               // ��Դ·��
	CONF_ROLERES,				// Ӣ�۶�Ӧ��Դ
	CONF_ANIMATION_PLAY_ORDER,	// �����������˳��
	CONF_STAGE,					// �ؿ�����
	CONF_STAGE_SCENE,           // �ؿ���Դ����
	CONF_CHAPTER,				// �½�����
	CONF_MAP,					// ��ͼ����
	CONF_PVPSCENE,				// pvp�������
	CONF_SEARCH,				// ����
	CONF_EQUIPMENT,				// װ��
	CONF_SUIT,					// ��װ
	CONF_ITEM,					// ����
	//CONF_DROP,					// �ؿ�����
	CONF_SALESUMMONER,			// �ٻ�ʦ�����
	CONF_SOLDIER_UP_RATE,		// Ӣ������
	CONF_INCREASE_PAY,			// �������� (�����һ�����������ʯ��)
	CONF_TASK,					// ����
	CONF_ACHIEVE,				// �ɾ�
	CONF_STORYTRIGGER,			// ���鴥����
	CONF_STORY,					// ����
	CONF_MAIL,					// �ʼ�
	CONF_NEW_PLAYER,            // �����������Ϣ
	CONF_ACTIVITY_INSTANCE,		// �����
	CONF_BAG_SETTING,			// �������
	CONF_CARD_GAMBLE,	        // �鿨Ӣ�۱�
	CONF_CARD_GAMBLE_SETTING,	// ������Ƭ+�鿨����
	CONF_ICON_SETTING,			// ͷ�����
	CONF_SKILL_UP_RATE_SETTING,	// ������������
	CONF_SOLDIER_LEVEL_SETTING,	// ����������
	CONF_SOLDIER_STAR_SETTING,	// ����������
	CONF_SOLDIER_RARE_SETTING,	// ʿ��ϡ�ж����
	CONF_TASK_ACHIEVE_SETTING,	// ����,�ɾ����
	CONF_USER_LEVEL_SETTING,	// ��ҵȼ����
	CONF_TOWER_SETTING,			// ���������
	CONF_CHAPTER_SETTING,       // �ؿ����һ������
	CONF_ITEM_CRIT,             // ���߱���
	CONF_SYSTEM_HEAD_ICON,      // ϵͳͷ��
	CONF_STAGE_SETTING,         // �ؿ���������
	CONF_ITEM_LEVEL_SETTING,    // ����Ʒ�����
	CONF_CHAT_SETTING,          // ��������
	CONF_OUTTER_BONUS_SETTING,  // ս����������������
	CONF_OUTTER_BONUS,          // ս����ӳ�Ч��
	CONF_ITEMDROP,				// ��Ʒ����
    CONF_UNION_LEVEL,           // ����ȼ����
    CONF_UNION,                 // �����������
    CONF_UNIONEXPIDTION,		// ����Զ��
    CONF_UNIONEXPIDTION_MAP,    // ����Զ����ͼ
    CONF_UNIONBADGE,            // ������

	CONF_PROP_LAN,				// ������������
	CONF_STAGE_LAN,				// �ؿ���������
	CONF_UI_LAN,				// ui��������
	CONF_BMC_LAN,				// BossMonsterCall ��������
	CONF_BMC_SKILL_LAN,		    // BossMonsterCall_Skill ��������
	CONF_HS_LAN,				// HeroSoldier ��������
	CONF_HS_SKILL_LAN,			// HeroSoldier_Skill ��������
	CONF_STORY_LAN,				// ��������
	CONF_TASK_LAN,				// ��������
	CONF_ACHIEVE_LAN,			// �ɾ�����
	CONF_ROLE_ATTRIBUT_LAN,     // ��ɫ��������
	CONF_ERROR_CODE_LAN,        // errorCode ����
	CONF_LOADING_TIPS_LAN,       // loading��ʾ����

	CONF_RICH_TEXT,				// ���ı�
	CONF_GOLD_TEST,				// �������
	CONF_GOLD_TEST_CHEST,		// ��ұ���
	CONF_HERO_TEST,				// Ӣ������
	CONF_TOWER_FLOOR,
	CONF_TOWER_BUFF,
	CONF_TOWER_RANK,
	CONF_ROLE_ZOOM,             // ��ɫ��������
	CONF_HALL_STANDING,         // ��������վλ
	CONF_EQUIP_QUALITY,			// װ��Ʒ��
	CONF_EQUIP_CREATE,			// װ������
	CONF_SHOP_GOODS,			// �̵���Ʒ��
	CONF_SHOP,					// �̵��
	CONF_FEATURE,				// Ӣ������

	CONF_GUIDE,					// ����
	CONF_GUIDE_STEP,			// ��������
	CONF_UI_NODE,				// UI�ڵ�
	CONF_UI_STATUS,				// UI״̬
	CONF_GUIDE_BATTLE,          // ս������

	CONF_SHOP_DIAMOND,          // ��ʯ�̵�
	CONF_TIMERECOVER,			// �����ָ����
	CONF_BG_MUSIC,              // ������������
	CONF_BG_MUSIC_SETTING,      // ����������������
	CONF_ACTIVE_TIME,			// �����
	CONF_ACTIVE_SHOP,			// ��̵�
	CONF_ACTIVE_DROP,			// �����
	CONF_ACTIVE_TASK,			// �����
	CONF_7DAY_ACTIVE,			// 7����
	CONF_BLUEDIAMOND_ACTIVE,	// ��������
	CONF_MONTH_ACTIVE,			// �¿����ͻ
	CONF_EXCHANGE_ACTIVE,		// �һ������
	CONF_BUTTON_EFFECT,			// ��ť��Ч
	CONF_SOUND_EFFECT,			// UI��Ч·��
	CONF_SOUND_EFFECT_SETTING,  // ս����Ч����

	CONF_DAYSIGN,				// ÿ��ǩ����
	CONF_CONDAYSIGN,			// �ۼ�ǩ������
    CONF_SYSAUTONAME,           // ������ֿ�

	CONF_FIRSTPAY_SETING,		// �׳�����
	CONF_UNION_MERCENARY,		// ����Ӷ����������
	CONF_EQUIP_CAST,			// װ������
	CONF_UNIONSHOPDROP,			// �����̵����

	CONF_TALENT,			    // �����츳����
	CONF_TALENT_ARRANGE,        // �츳ҳ

	CONF_PVP_GRADING_CHANGE,	// ���������ֱ仯
	CONF_PVP_GRADING_INITIAL,	// ��������ʼ����
	CONF_PVP_MATCH_TIME,		// ƥ��ʱ��
	CONF_PVP_SETTING,			// pvp��������
	CONF_ARENA_TASK,			// ����������
	CONF_ARENA_LEVEL,			// ����������
	CONF_ARENA_TRAINING,		// ������ѵ����
	CONF_ARENA_CHEST,           // pvp����
	CONF_ARENA_SETTING,			// pvp������� ����ƥ����Ϣ
	CONF_ARENA_ROBOTNAME,		// ������ͷ���
	CONF_ARENA_ROBOT,			// ����������
	CONF_ARENA_RANK,            // ������λ
	CONF_ARENA_SCORLLBAR,		// ������ƥ��������ͼƬ
	CONF_ARENA_REWARD,          // ��������

    CONF_PVP_SHARE,             // pvp��������
    CONF_PVP_UPLOAD,            // pvp�ϴ�����
};

// �����ļ��Ļ���
class CConfBase
{
public:
	virtual ~CConfBase();

	virtual bool LoadCSV(const std::string& str)
	{
		return false;
	}

	inline void* getData(int dataId)
	{
		std::map<int, void*>::iterator iter = m_Datas.find(dataId);
		if (iter != m_Datas.end())
		{
			return iter->second;
		}

		return NULL;
	}

	void addData(int dataId, void* pData)
	{
		void* data = getData(dataId);
		if (!data)
		{
			m_Datas[dataId] = pData;
		}
	}

	inline std::map<int, void*>& getDatas()
	{
		return m_Datas;
	}

	// ����csv�ļ�,��������(-1)
	bool loadCsv(CCsvLoader& csvLoader, const std::string& csvFile, int skipRow)
	{
		if (skipRow <= 0 || "" == csvFile)
		{
			return false;
		}

		if (!csvLoader.LoadCSV(csvFile.c_str()))
		{
			return false;
		}

		// ע�ͱ�ע,����
		for (int i = 0; i < skipRow; i++)
		{
			if (!csvLoader.NextLine())
			{
				return false;
			}
		}

		return true;
	}

protected:
	std::map<int, void*> m_Datas;
};

// �����ļ�������
class CConfStrToID;
class CConfManager
{
private:
	CConfManager();
	virtual ~CConfManager();

public:
	static CConfManager* getInstance();

	static void destroy();

	// �����������е������ļ�
	// ���سɹ�����true������ʧ�ܷ���false����ӡ��־
	bool init();

	inline CConfBase* getConf(int confId)
	{
		std::map<int, CConfBase*>::iterator iter = m_Confs.find(confId);
		if (iter != m_Confs.end())
		{
			return iter->second;
		}

		return NULL;
	}

	bool addConf(int confId, const std::string& csvFile, CConfBase* conf);

	int getFunID(const std::string& str);
	int getStateID(const std::string& str);
	int getRoleAttributeID(const std::string& str);

    CConfStrToID* getStateNameToIDConf()
    {
        return m_pStateNameToID;
    };

private:
	static CConfManager* m_Instance;
	std::string m_szConfRoot;
	CConfStrToID* m_pFunNameToID;
	CConfStrToID* m_pStateNameToID;
	CConfStrToID* m_pRoleAttributeToID;
	std::map<int, CConfBase*> m_Confs;
};

#endif
