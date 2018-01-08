/*
 * ս���������ģ��
 * ����ģ��
 * ���ģ�ͣ��û�ģ�;���棩
 * Ӣ��ģ�� 
 *
 * 2015-1-6 by ��ү
 */
#ifndef __BATTLE_MODELS_H__
#define __BATTLE_MODELS_H__

#include <vector>
#include <map>

#include "CommStructs.h"
#include "ConfManager.h"
#include "ConfHall.h"
#include "CallComponent.h"
#include "RoleComm.h"

class CRoleModel
{
public:
    CRoleModel();
    ~CRoleModel();

    //1. ǰ�󽻻� out = in << 16 + int >> 16
    //2. ����һλ��� out char[key%sizeof(in)] ^ key % 128
    template<typename T>
    T encryptNumber(const EAttributeTypes& key, T num)
    {
        if (m_bEncrypt)
        {
            char* p = reinterpret_cast<char*>(&num);

            for (int i = 0; i < sizeof(T)/2; ++i)
            {
                p[i] = p[i] ^ p[sizeof(T) - i - 1];
                p[sizeof(T) - i - 1] = p[i] ^ p[sizeof(T) - i - 1];
                p[i] = p[i] ^ p[sizeof(T) - i - 1];
            }
            p[key % sizeof(T)] ^= (key % 128);
        }
        return num;
    }

    template<typename T>
    T decodeNumber(const EAttributeTypes& key, T num)
    {
        if (m_bEncrypt)
        {
            char* p = reinterpret_cast<char*>(&num);

            p[key % sizeof(T)] ^= (key % 128);
            for (int i = 0; i < sizeof(T) / 2; ++i)
            {
                p[i] = p[i] ^ p[sizeof(T) - i - 1];
                p[sizeof(T) - i - 1] = p[i] ^ p[sizeof(T) - i - 1];
                p[i] = p[i] ^ p[sizeof(T) - i - 1];
            }
        }
        return num;
    }

    bool init(int level, int roleType, const Role* role, int battleType);
    void addEffect(int effId, int effValue);
    // Ӧ�ø���Ч�� - �ȼ���ٷֱȶԻ������Եļӳ���ֵ���ټ���̶��ӳɵ��ܺ�
    void applyEffect();

    bool classIntKeyExist(const EAttributeTypes& key) const;
    bool classFloatKeyExist(const EAttributeTypes& key) const;
    int getClassInt(EAttributeTypes type);
    float getClassFloat(EAttributeTypes type);
    void setClassInt(const EAttributeTypes& key, int value);
    void setClassFloat(const EAttributeTypes& key, float value);
    // ��ȡ���ܺ������
    std::map<EAttributeTypes, int> getClassIntMap();
    std::map<EAttributeTypes, float> getClassFloatMap();

    // ���������ͷ�Ϊ2�ദ�� 1: Hero, Soldier 2: Boss, Call, Monster
    void roleTypeLimit(int v, int min1, int max1, int min2, int max2);
    void roleTypeLimit(float v, float min1, float max1, float min2, float max2);
    
    inline int getLevel() { return m_nLevel; }
    inline int getRoleType() { return m_nRoleType; }
    inline const Role* getRoleComm() const { return m_pRole; }

protected:
    int m_nLevel;
    int m_nRoleType;
    const Role* m_pRole;
    std::map<EAttributeTypes, int> m_IntAddMap;
    std::map<EAttributeTypes, float> m_IntPercentMap;
    std::map<EAttributeTypes, float> m_FloatAddMap;
    std::map<EAttributeTypes, float> m_FloatPercentMap;

private:
    // �Ƿ������
    bool m_bEncrypt;
    // ������ܺ��ֵ
    std::map<EAttributeTypes, int> m_ClassIntMap;
    std::map<EAttributeTypes, float> m_ClassFloatMap;
};

// ʿ����Ƭģ��
class CSoldierModel : public CRoleModel, public ISerializable
{
public:
    // �ɹ�����offsetֵ��ʧ�ܷ���-1
    int init(SoldierCardData* soldierData, int battleType, bool bMercenary);

	//���л�,�����л� ʿ����Ƭģ��
	bool serialize(CBufferData& data);
	bool unserialize(CBufferData& data);

    inline const SoldierConfItem* getConf() { return m_pConf; }
	inline int getSoldId() { return m_iId; }
	inline int getStar() { return m_iStar; }
	void setConf(SoldierConfItem* item) { m_pConf = item; }
    inline int getAI() { return m_nAI; }
    inline char* getTalent(){ return m_Talent; }
    inline const std::vector<int>& getSkillIds() { return m_pConf->Common.Skill; }
    inline std::map<int, int>& getAddBuff(){ return m_mapAddBuff; }
    inline const std::vector<SoldierEquip>& getEquips() { return m_vecEquips; }

protected:
    // ����װ�����Լӳ� ���� ��Ӧ��ִ��Apply��������Ϊ�ⲿ�������������Լӳ�
    // ���Լӳɵ�������Ч����ģ�ʹ�����ִ��
    bool calcEquipAddition(std::vector<SoldierEquip>& effects);
    // �츳���Լӳ�
    bool calcTalentAddition();

public:
    int CurCost;                    // ��ǰˮ������
    float CurCD;                    // CDʱ��
    float MaxCD;                    // ���CD
    bool IsLock;                    // �Ƿ���ס
    bool IsSingo;			        // �Ƿ�Ψһ�ɷ�
    bool IsMercenary;               // �Ƿ�ΪӶ��

private:
    int	m_iId;	                            // ����ID
    int m_iStar;                            // �Ǽ�
    int m_nAI;                              // AI
    char m_Talent[8];                       // �츳�б�
    const SoldierConfItem* m_pConf;         // ��Ӧ��Ƭ����
    std::map<int, int> m_mapAddBuff;        // ���buff
    std::vector<SoldierEquip> m_vecEquips;  // װ��
};

// ս���û�����
class CBattlePlayerModel
{
public:
    CBattlePlayerModel();
    virtual ~CBattlePlayerModel();

    inline int getUserId() { return m_nUserId; }
    inline int getUserLv() { return m_nUserLv; }
    inline int getCamp() { return m_Camp; }
    inline void setCamp(int camp) { m_Camp = camp; }
    inline int getMainRoleId() { return m_nMainRoleId; }
    inline int getMainRoleLv() { return m_nMainRoleLv; }
    inline int getIdentity() { return m_nIdentity; }
    inline const std::string& getUserName() { return m_UserName; }
    inline void setUserName(std::string name) { m_UserName = name; }
    inline CRoleModel* getRoleModel(){ return m_RoleModel; }

protected:
    int m_nUserId;
    int m_nUserLv;
    int m_Camp;
    int m_nMainRoleId;
    int m_nMainRoleLv;
    int m_nIdentity;
    std::string m_UserName;
    CRoleModel* m_RoleModel;
};

// ս���û�ģ�ͣ�ս���ڣ�������û�ģ�ͣ�
class CPlayerModel : public CBattlePlayerModel
{
public:
    CPlayerModel();
    virtual ~CPlayerModel();

    // �����û����ݽṹ������ʼ�����������ݰ���
    int initByUserData(PlayerData* data, int battleType);

    CSoldierModel* getSoldierCard(int index);
    inline std::vector<CSoldierModel*>& getSoldierCards() { return m_SoldierCards; }
    inline const std::vector<int>& getAdditions() { return m_Additions; }
    // ���buffdata
    bool fillBufferData(CBufferData& buff);

private:
    void calcAddition(CRoleModel* solider);

private:

    std::vector<int> m_Additions;
    std::vector<CSoldierModel*> m_SoldierCards;
};

// ս���ڵ����û�ģ��
class CComputerModel : public CBattlePlayerModel
{
public:
    CComputerModel();
    virtual ~CComputerModel();

    int init(int stageId, int level, int battleType);
    
    CRoleModel* getMonsterModel(int index);
    inline std::vector<CRoleModel*>& getMonsterModels() { return m_MonsterModels; }

private:
    std::vector<CRoleModel*> m_MonsterModels;
};

// ���л���
struct DropCurrency
{
	int exp;				//����
	int gold;				//���
	int diamond;			//��ʯ
	int pvpCoin;			//������
	int towerCoin;			//����
	int guildContrib;		//���ṱ��
};

// ����ģ��
class CSettleAccountModel
{
public:
	CSettleAccountModel();
	~CSettleAccountModel();

public:
	// ����ս�����
    void setChallengeResult(int type) { m_nChallengeResult = type; }
	// ����tick
	void setTick(int tick) { m_nTick = tick; }
	// 0-100
	void setHPPercent(int percent) { m_nHeroHpPercent = percent; }
	// ����boss�����ĵ�Ѫ��
	void addHitBossHP(int hp) { m_nHitBossHP += hp; }
	// �������ĵ�ˮ����
	void addCostCrystal(int crystal) { m_nCostCrystal += crystal; }
    // ʣ��ˮ����
    void setCrystal(int crystal) { m_nCrystal = crystal; }
	//����ˮ���ȼ�
	void setCrystalLv(int crystalLv) { m_nCrystalLv = crystalLv; }
	
	// ս�����
    int getChallengeResult() { return m_nChallengeResult; }
	// ���tick
	int getTick() { return m_nTick; }
	// ���Ѫ���ٷֱ�
	int getHPPercent() { return m_nHeroHpPercent; }
	// ���boss������Ѫ��
	int getHitBossHP() { return m_nHitBossHP; }
	// ������ĵ�ˮ������
    int getCostCrystal() { return m_nCostCrystal; }
    // ���ʣ��ˮ����
    int getCrystal() { return m_nCrystal; }
	// ���ˮ���ȼ�
	int getCrystalLv() { return m_nCrystalLv; }
	// ����
	void resetSettle();

private:

    int	    m_nChallengeResult;     // ��ս���
    int		m_nTick;				// ս�������ĵ�ʱ��tick(100ms)
    int		m_nHeroHpPercent;		// Ӣ��ʣ��Ѫ�������Ѫ����ֵ
    int		m_nHitBossHP;			// boss�����ĵ�Ѫ��
    int		m_nCostCrystal;			// ������ĵ�ˮ������
    int     m_nCrystal;             // ʣ��ˮ����
	int		m_nCrystalLv;			// ��ǰˮ���ȼ�
};

// ����ͨ�ýӿ� ���� ǰ���ͨ��
class CBattleRoom
{
public:
    CBattleRoom()
        : m_nMaster(0)
        , m_nOther(0)
        , m_nStageId(0)
        , m_nMonsterLevel(1)
        , m_nBattleType(0)
        , m_nExt1(0)
        , m_nExt2(1)
    {
    }
    inline void setMaster(int master) { m_nMaster = master; }
    inline int getMaster() { return m_nMaster; }
    inline int getStageId() { return m_nStageId; }
    inline void setStageId(int map) { m_nStageId = map; }
    inline int getStageLevel() { return m_nMonsterLevel; }
    inline int getBattleType() { return m_nBattleType; }
    inline void setBattleType(int battle) { m_nBattleType = battle; }
    inline int getExt1() { return m_nExt1; }
    inline int getExt2() { return m_nExt2; }

protected:
    int m_nMaster;
    int m_nOther;
    int m_nStageId;
    int m_nMonsterLevel;
    int m_nBattleType;
    int m_nExt1;
    int m_nExt2;
};

// ��������ģ��
class CRoomModel : public CBattleRoom
{
public:
    CRoomModel();
    virtual ~CRoomModel();

    // �����û����ݽṹ������ʼ�����������ݰ���
    bool initByRoomData(RoomData* data);
    // ����RoomData������Player����ʼ��
    bool initPVPRoomBylayer(int stageId, CBattlePlayerModel* master, CBattlePlayerModel* other);

    inline void addPlayerModel(CBattlePlayerModel* player)
    {
        if (NULL != player
            && m_Players.find(player->getUserId()) == m_Players.end())
        {
            m_Players[player->getUserId()] = player;
            if (m_nMaster != player->getUserId())
            {
                m_nOther = player->getUserId();
            }
        }
    }
    inline CBattlePlayerModel* getMasterModel()
    {
        std::map<int, CBattlePlayerModel*>::iterator iter = m_Players.find(m_nMaster);
        if (iter != m_Players.end())
        {
            return iter->second;
        }
        return NULL;
    }
    inline CBattlePlayerModel* getOtherModel()
    {
        std::map<int, CBattlePlayerModel*>::iterator iter = m_Players.find(m_nOther);
        if (iter != m_Players.end())
        {
            return iter->second;
        }
        return NULL;
    }
    inline CBattlePlayerModel* getPlayer(int i) 
    { 
        std::map<int, CBattlePlayerModel*>::iterator iter = m_Players.find(i);
        if (iter != m_Players.end())
        {
            return iter->second;
        }
        return NULL;
    }
    
	inline std::vector<BuffData>& getStageBuffs() { return m_StageBuffs; }
	inline std::map<int, CBattlePlayerModel*>& getPlayers(){ return m_Players; }
    inline CSettleAccountModel *getSettleAccountModel() { return m_pSettleAccount; }

private:
	CSettleAccountModel *m_pSettleAccount;
	std::vector<BuffData> m_StageBuffs;
    std::map<int, CBattlePlayerModel*> m_Players;
};

#endif
