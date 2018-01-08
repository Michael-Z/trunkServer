#ifndef __CONF_ROLE_H__
#define __CONF_ROLE_H__

#include "ConfManager.h"
#include "CommTools.h"

// ��Ч���ŵ�
enum StatusEffPlayType
{
    StatusEff_None,                                     // ����Ч���ŵ�
    StatusEff_Head,                                     // ͷ����Ч��
    StatusEff_Body,                                     // ������Ч��
    StatusEff_Leg,                                      // �Ȳ���Ч�㣨�ƶ����ĵ㣩
};

enum StatusSpeedAffect
{
    StatusSpeed_None,                                   // ���ٶ�����
    StatusSpeed_Attack,                                 // ��������
    StatusSpeed_Move,                                   // ��������
    StatusSpeed_MPRecover,                              // MP�ָ�����
};

// ״̬����
class StatusConfItem
{
public:
    StatusConfItem() : Action(NULL)
    {
    }

    ~StatusConfItem()
    {
        SAFE_DELETE(Action);
    }

    bool				Lock;						    // ״̬�Ƿ����
    bool				AnimationAsh;				    // �һ�
    bool				IsFollow;					    // ����Ч���Ƿ�����ƶ�
    int 				AnimationLoop;				    // �Ƿ�ѭ��
    int     			StateId;					    // ״̬��ǩ
    int			        NextStateId;				    // ״̬�Զ���ת
    int					CanBreakParam;				    // Status_CanBreakParam
    int					UIEffectID;					    // ����UI��Ч��UI��Чֻ�ܶ�Ӧ1����
    int					LifeTimePrarm;					// ����Ӱ�����(0��,1��������,2�ƶ��ٶ�Ӱ��)
    float				LifeTime;					    // ״̬��������
    float				AnimationSpeed;				    // ���������ٶȵı���
    float				AnimationTransparency;		    // ͸����
    float				AnimationScale;				    // ����
    float				AnimationFadeOut;			    // ���1֡����
	float				hue;				            // ɫ��
    StatusSpeedAffect   SpeedAffect;				    // �����ٶ�Ӱ������
    StatusEffPlayType   EffectPlayType;				    // ����Ч�����ŵ㣺��0������1ͷ2��3�ţ�                                                   
    VecInt				EffectIds;					    // ���ӱ���ID
    VecMusicInfos       MusicInfos;                     // Ҫ���ŵ���Ч�б�
    std::string			AnimationTag;				    // ״̬�Զ���ת
    CDataFunction*		Action;						    // ��ɫ��Ϊ
#ifndef RunningInServer                                    
    Color3B				AnimationRGB;				    // ƫɫ[R+G+B]
#endif  
};

//AI����
class AIConfItem : public CConfBase
{
public:
    ~AIConfItem()
    {
        deleteAndClearVec<CDataFunction*>(Condition);
        deleteAndClearVec<CDataFunction*>(Action);
    }

    int					Order;							// AI˳��ֵ
    int					NeedRoleStatus;					// ����״̬
    std::vector<CDataFunction*>	Condition;				// AI������
    std::vector<CDataFunction*>	Action;					// AI������
};

//��ɫ��������
class Role
{
public:
    int					ClassID;					//ID
    int					AnimationID;				//����ID
    int					StatusID;					//״̬��ID
    int					AIID;						//AI��ID
    int 				Speed;						//�ƶ��ٶ�
    int 				FireRange;					//��ս������Χ
    int 				FarFireRange;				//Զ�̹�����Χ
    int					PAttack;					//��������
    int					PAttackGrowUp;				//���������ɳ�ϵ��
    int					MAttack;					//ħ��������
    int					MAttackGrowUp;				//ħ���������ɳ�ֵ
    int					HP;							//����ֵ
    int					HPGrowUp;					//Ѫ���ɳ�ϵ��
    int					PGuard;						//������
    int					PGuardGrowUp;				//�����׳ɳ�
    int					MGuard;						//ħ������
    int					MGuardGrowUp;				//ħ�����׳ɳ�
    int					PPenetrate;					//����͸
    int					MPenetrate;					//ħ����͸
    int					AttackSpeed;				//�����ٶ�
    int					Rage;						//ŭ������ֵ
    int					RageRecover;				//ŭ���ظ�ֵ
    int					MP;							//ħ������ֵ
    int					MPRecover;					//ħ���ظ�
    int					Strong;						//����ֵ
    int					StrongRecover;				//����ֵ�ָ��ٶ�
    int					Haterd;						//���ֵ
    float				Defend;						//�������
    float				Resustance;					//ħ������
    float				Mass;						//����
    float				Scale;						//��С����
    float				EffectScale;				//��Ч��С����ֵ
    float				Vampire;					//��Ѫֵ
    float				Rebound;					//��ɫ����ֵ
    float				Miss;						//����ֵ
    float				Crit;						//����ֵ
    float				CritGrowUp;					//�����ɳ�
    float				CritDamage;					//�����˺�
    Vec2				FireOffset;					//�ӵ������
    Vec2				HeadOffset;					//ͷ����Ч���ŵ�
    Vec2				HitOffset;					//������Ч���ŵ�
    VecInt				Skill;						//���� (ǰ���������ܿ���������, ���Լ�ʹ�ǿ�ֵҲҪ����0, ����ȷ�����������ļ������Ǽ���)
    VecInt				HPLine;						//Ѫ������, ������
	VecFloat			AnimationHSV;               //����HSV

    int					Name;						// ��Ƭ����ID
    int					Desc;						// ��Ƭ����ID
    int					Race;						// ����
    int					Sex;						// �Ա�
    int					AttackType;					// �������ͣ���ս Զ�̣�
    int					Vocation;					// ְҵ
    int					AttackDistance;				// ��ɫ��������(0=��,1=��,2=�У�3=Զ��4=��Զ)
    std::string			Picture;					// ��ӦͼƬ��Դ
    std::string         HeadIcon;					// ͷ��
};

// ��Ƭǿ��
struct CardEnhance
{
    int EnhanceType;                                //��ǿ����
    float CDParam;                                  //��ƬCD��ǿ
    float ConsumeParam;                             //��Ƭ���ļ�ǿ
};

//Ӣ������
class HeroConfItem
{
public:
    Role				Common;						//��ɫ��������
    float				CrystalSpeedPrarm;			//ˮ����������
    int					BasicExp;					//������߻�������ֵ
    int					ExpRatio;					//�����ṩ����ɳ�ϵ��
    int					RacialRatio;				//ͬ���徭��ӳ�ϵ��
    VecInt				PlayerSkill;				//��Ҽ���
    CardEnhance         RaceEnhance;                //��Ƭ�����ǿ
    CardEnhance         VocationEnhance;            //ְҵ��ǿ
    CardEnhance         SexEnhance;                 //�Ա��ǿ
    CardEnhance         AttackTypeEnhance;          //��������	
};

//BOSS����
class BossConfItem
{
public:
    Role				Common;						//��ɫ��������
};

//MONSTER����
class MonsterConfItem
{
public:
    Role				Common;						//��ɫ��������
};

//սʿ����
class SoldierConfItem
{
public:
    Role				            Common;		//��ɫ��������
    int					            Star;		//ʿ���Ǽ�
	int								Rare;		//ʿ��ϡ�ж�
    int					            Cost;		//��Ƭ�ɷ�����
    float				            CD;			//�ɷ�CD(��)
    int					            IsSingo;	//�Ƿ�Ψһ�ɷ�
};

// �ٻ�ʿ������
class CallConfItem
{
public:
    int                 CardCurrentLevel;           //���Ƶ�ǰ�ȼ�
    float				RoleLifeTime;				//��ɫ��������
    int					RoleType;					//��ɫ��Ӫ
    int                 RoleIdentity;               //��ɫ���
    int					RoleMoveType;				//��ɫ��·����
    int					RoleMoveDirection;			//�ƶ�����(0�Զ���ȡ,1->  2<-)

    Role				Common;						//��ɫ��������
};

// չʾ�����ɫ����
class ZoomItem
{
public:
    int                 RoleID;     // ��ɫID
    float               ZoomNumber; // չʾ�����С����ֵ
	float				HallZoom;	// ����չʾ�����С����ֵ
    int                 Priority;   // ���ȼ�(ԽСԽǰ)
    Vec2                StandOffSet;// վλƫ����
};

// һ����ɫ���е�״̬Map
class StatusConfMap : public CConfBase
{
public:
    bool LoadCSV(const std::string& str);
    const StatusConfItem* getStateItem(int stateId) const;
    //const std::map<int, StatusConfItem>& getStateMap() const;
};

// ���н�ɫ��״̬Map
class CConfStatus : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    bool LoadCSV(int roleId, const std::string& str);
    const StatusConfItem* getStateItem(int roleId, int stateId);
    const StatusConfMap* getStateMap(int roleId);
    std::map<int, std::string>& getStatusFileMap()
    {
        return m_StatusFileMap;
    }
private:
    std::map<int, std::string> m_StatusFileMap;
};

class AIConfMap : public CConfBase
{
public:
    virtual ~AIConfMap();
    bool LoadCSV(const std::string& str);
    const std::vector<AIConfItem*>* getAIItems(int stateid);
    const std::map<int, std::vector<AIConfItem*> >& getAIMap();

private:
    std::map<int, std::vector<AIConfItem*> >  m_AIDatas;
};

class CConfAI: public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    bool LoadCSV(int roleid, const std::string& str);
    const std::vector<AIConfItem*>* getAIItems(int roleId, int stateid);
    const AIConfMap* getAIMap(int roleId);
    std::map<int, std::string>& getAIMap()
    {
        return m_AIFileMap;
    }
private:
    std::map<int, std::string> m_AIFileMap;
};

class CConfStrToID : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    int getIDByName(const std::string& str);

    std::map<std::string, int>& getConvertData()
    {
        return m_mapConvert;
    }
private:
    std::map<std::string, int> m_mapConvert;
};

class CConfRole : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str)
    {
        return true;
    }

    // ������ͬ���ֵ��߼���,���һ��������ʾ�Ƿ��ٻ���,Ĭ�ϲ���
    void LoadRoleLogic(CCsvLoader& pLoader, Role &common, bool noCall = true);

    // ������ͬ���ֵ���ֵ��,���һ��������ʾ�Ƿ�ʿ��,Ĭ�ϲ���
    void LoadRoleNumerial(CCsvLoader& pLoader, Role &common);
};

class CConfHero : public CConfRole
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfBoss : public CConfRole
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfMonster : public CConfRole
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfSoldier : public CConfRole
{
public:
    virtual bool LoadCSV(const std::string& str);

    // ��ȡʿ����Ϣ
    SoldierConfItem* getData(int id, int star)
    {
        std::map<int, std::map<int, SoldierConfItem*> >::iterator iter = m_mapSodierItem.find(id);
        if (iter != m_mapSodierItem.end())
        {
            std::map<int, SoldierConfItem*>::iterator iterItem = iter->second.find(star);
            if (iterItem != iter->second.end())
            {
                return iterItem->second;
            }
        }

        return NULL;
    }

    // ��ȡ����ʿ������Ϣ
    std::map<int, std::map<int, SoldierConfItem*> >& getSoldiersConfig()
    {
        return m_mapSodierItem;
    }

private:
    std::map<int, std::map<int, SoldierConfItem*> > m_mapSodierItem;
};

class CConfCall : public CConfRole
{
public:
    virtual bool LoadCSV(const std::string& str);
};

class CConfZoom : public CConfRole
{
public:
	virtual bool LoadCSV(const std::string& str);
};

/////////////////////////////// ��ѯ ////////////////////////////////////////

inline const StatusConfMap* queryConfStateMap(int roleId)
{
    CConfStatus* conf = dynamic_cast<CConfStatus*>(
        CConfManager::getInstance()->getConf(CONF_STATUS));
    return conf->getStateMap(roleId);
}

inline const StatusConfItem* queryConfStateItem(int roleId, int stateId)
{
    CConfStatus* conf = dynamic_cast<CConfStatus*>(
        CConfManager::getInstance()->getConf(CONF_STATUS));
    return conf->getStateItem(roleId, stateId);
}

// ��ѯAI���ñ�ĸ�������
inline const AIConfMap* queryConfAIMap(int roleId)
{
    CConfAI* conf = dynamic_cast<CConfAI*>(
        CConfManager::getInstance()->getConf(CONF_AIDATA));
    return conf->getAIMap(roleId);
}

// ��ѯӢ�����ñ�ĸ�������
inline const HeroConfItem* queryConfHero(int heroId)
{
    CConfHero* conf = dynamic_cast<CConfHero*>(
        CConfManager::getInstance()->getConf(CONF_HERO));
    return static_cast<HeroConfItem*>(conf->getData(heroId));
}

// ��ѯʿ�����ñ�ĸ�������
inline const SoldierConfItem* queryConfSoldier(int soldierId, int soldierStar)
{
    CConfSoldier* conf = dynamic_cast<CConfSoldier*>(
        CConfManager::getInstance()->getConf(CONF_SOLDIER));
    return static_cast<SoldierConfItem*>(conf->getData(soldierId, soldierStar));
}

// ��ѯBOSS���ñ�ĸ�������
inline const BossConfItem* queryConfBoss(int soldierId)
{
    CConfBoss* conf = dynamic_cast<CConfBoss*>(
        CConfManager::getInstance()->getConf(CONF_BOSS));
    return static_cast<BossConfItem*>(conf->getData(soldierId));
}

// ��ѯMonster���ñ�ĸ�������
inline const MonsterConfItem* queryConfMonster(int soldierId)
{
    CConfMonster* conf = dynamic_cast<CConfMonster*>(
        CConfManager::getInstance()->getConf(CONF_MONSTER));
    return static_cast<MonsterConfItem*>(conf->getData(soldierId));
}

//��ѯ�ٻ�������
inline const CallConfItem* queryConfCall(int callid)
{
    CConfCall *conf = dynamic_cast<CConfCall*>(
        CConfManager::getInstance()->getConf(CONF_CALL));
    return static_cast<CallConfItem*>(conf->getData(callid));
}

//��ѯ�����ɫ�������Ŵ�С����
inline const ZoomItem* queryConfZoom(int roleID)
{
    CConfZoom *conf = dynamic_cast<CConfZoom*>(
        CConfManager::getInstance()->getConf(CONF_ROLE_ZOOM));
    return static_cast<ZoomItem*>(conf->getData(roleID));
}

#endif
