/*
 * ս�������ࣨ�����Ϊ��������ÿ��ս���ᴴ��һ����
 * 
 * 1.������Ϸ����Ĵ����͹���
 * 2.������Ϸ�е��������
 * 3.��������Ϸ�����е�ȫ�ֱ���
 *
 * 2014-12-18 By ��ү
 *
 */
#ifndef __BATTLE_HELPER_H__
#define __BATTLE_HELPER_H__

#include "RoleComm.h"
#include "KxCSComm.h"
#include "CommStructs.h"
#include "CommTools.h"
#include "BattleScene.h"
#include "BattleModels.h"
#include "RandomNumber.h"
#include "BufferData.h"
#include "Dispatch.h"
#include "EventManager.h"

class CBattleLayer;
class CBullet;
class CRole;
class CHero;
class CSoldier;
class CMonster;
class CBoss;
class CCallSoldier;
class CGameObject;
class CBuff;
class CBuffComponent;

class CBattleHelper : public ISerializable
{
public:
    CBattleHelper();
    virtual ~CBattleHelper();

    // ����ս��ʱ�ص������������ID����ͼID��˫���û�ģ�ͣ�Director
    bool init(CRoomModel* room, CBattlePlayerModel* myModel
        , CBattlePlayerModel* enmeyModel,CEventManager<int>* eventMgr, Node* scene);
    // ս������ʱ�ص�
    void uninit();

    ///////////////////////////////ս���߼����/////////////////////////////////////////

    // �жϵ�ǰս���Ƿ�����������Զ�ִ��onBattleOver
    bool checkBattleOver();
    // ս������ʱ�ص�
    void onBattleOver();
    // ����ս���߼�
    void processBattle(float delta);

    ///////////////////////////////���л��ͷ����л�/////////////////////////////////////

    // ����Ϸ���������data��
    bool serialize(CBufferData& data);
    // ��data�н�ѹ����Ϸ����
    bool unserialize(CBufferData& data);

    // ����ObjectId��ȡRole
    CRole* getRoleByObjectId(int objectId);
    // ����ObjectId��ObjectMap���Ƴ�Role
    inline void removeObjectMapByObjectId(int objectId) { m_ObjectMap.erase(objectId); }

    /////////////////////////////////������Ϸ����/////////////////////////////////////////

    // ʹ���û�ģ�ͣ�ʿ��ID����ʿ��
    CSoldier* createSoldier(int uid, int soldierIndex, int line = -1);
    // ����MONSTER
    CMonster* createMonster(int monsterIndex, int line = -1);
    // �����ٻ���, ownerLvΪ�ٻ�ʿ�����ٻ��ߵ�ǰ�ȼ�
    CCallSoldier* createCallSoldier(int uid, int callId, int ownerLv, float x, float y);
    // ʹ���û�ģ�ͣ��ӵ�ID�������ߴ����ӵ�
    CBullet* createBullet(int uid, int bulletId, CRole* attacker, CRole* target = NULL, const Vec2& pos = Vec2::ZERO);
    // ʹ��buff���, Я����, ������, buffid, buff��������buff
    CBuff* createBuff(CBuffComponent *cpnt, CRole *owner, CRole *maker, int buffid, int stack);

    /////////////////////////////////��ȡ�������Ϸ����/////////////////////////////////////////
    
    // ���ָ����Ӫ��Ӣ��(�����ؿ���ս�з���boss)
    CRole* getMainRole(int camp);
    // ��ȡ������Ӫ��Ӣ��(�����ؿ���ս�з���boss)
    CRole* getEnmeyMainRole(int camp);
    // ���ָ����ҵ�����ʿ��(���߹ؿ���ս�з���monster)
    std::vector<CRole*>& getRoleWithCamp(int camp);
    // �����Լ���UID��ȡ�Է���ʿ��(�����ؿ���ս�з���monster)
    std::vector<CRole*>& getEnmeyRoleWithCamp(int camp);
    // �л���Ӫ
    void changeCampType(CRole* role, CampType camp);
    // ����ӵ��б�
    inline std::list<CBullet*>& getBullets() { return m_Bullets; }
    // ��ȡ��Ӫ�л��б�
    inline std::map<CRole*, CampType>& getChangeCamp() { return m_ChangeCampMap; }

    ////////////////////////////////�����ͷ���Ϸ����//////////////////////////////////////////

    // ����ʿ��
    void destorySoidler(CSoldier* role);
    // ����monster
    void destoryMonster(CMonster* monster);
    // �����ӵ�
    void destoryBullet(CBullet* bullet);
    // �Զ��ж�Ӣ�ۣ�ʿ�����ӵ�������
    void destroyObject(CGameObject* obj);
    // ����buff
    void destoryBuff(CBuff* buff);
    // ��ӽ�ɫ�����ͷ��б�
    void autoRleaseRole(CRole* role);
    // �Զ������Ѿ�û�������ط����õ��Ľ�ɫ
    void autoReleaseRoles();

    ///////////////////////////////�������Ŀ���б�ӿ�///////////////////////////////////////////

    // ͨ���������id
    bool getTargetsWithId(int searchid, CGameObject* searcher, std::vector<CRole *> *targets);
    // Ӣ���Ƿ��ڷ�Χ��
    bool isHeroInRange(int uid, int camp, Vec2 &curPos, int range);
    // ����з����ѷ��б�
    void sortSoldiers();

    ////////////////////////////////������Ϸ����//////////////////////////////////////////

    // ����һ��������������ʱ����в�������
    // �����ѹ��������ʧ��
    bool insertBattleCommand(BattleCommandInfo cmd);
    // ��ѯ����ͷ��������
    BattleCommandInfo& topBattleCommand();
    void popBattleCommand();
    const std::vector<BattleCommandInfo>& getBattleCommandQueue(){ return m_CommandQueue; }
	void resetBattleCommandQueue(std::vector<BattleCommandInfo>& newCmdQueue);
    void clearBattleCommandQueue();
    void processCommand();
    inline int getCurExecutCommandCount() { return m_nCurExecutCommandCount; }
    inline int commandCount() { return m_CommandQueue.size() - m_nCurExecutCommandCount; }

    ////////////////////////////////getter and setter////////////////////////////////////

    // ��ȡ������ڵ���Ӫ�����ID�����ڣ��򷵻�����
    int getCampWithUid(int uid);
    // ��ȡ������ڵĵж���Ӫ�����ID�����ڣ��򷵻�����
    int getEnmeyCampWithUid(int uid);
    // ��ȡ��Ӫ�ĵж���Ӫ��������Ӫû�еж�
    int getEnmeyCamp(int camp);

    inline int getBattleType() { return m_nBattleType; }
    inline CRoomModel* getRoom() { return m_pRoom; }
    inline int getMasterId() { return m_nMasterId; }
    inline int getStageId() { return m_nStageId; }
    inline CBattleLayer* getBattleScene() { return m_pBattleScene; }
    inline int getUserId() { return m_nUserId; }
    inline int getEnmeyUserId() { return m_nEnmeyUserId; }
    inline CComputerModel* getComputerModel() { return dynamic_cast<CComputerModel*>(pEnemyUserModel); }
    inline CSettleAccountModel* getSettleAccountModel()
    {
        return NULL != m_pRoom ? m_pRoom->getSettleAccountModel() : NULL;
    }
    inline int getEnmeyUserId(int uid)
    {
        return uid == m_nUserId ? m_nEnmeyUserId : m_nUserId; 
    }
    inline CBattlePlayerModel* getUserModel(int uid)
    {
        return uid == m_nUserId ? pMyUserModel : pEnemyUserModel;
    }
    // ���Ӣ��վ����
    const Vec2& getOrigin(int camp);
    // ������Ӫ��ȡվ���㣬lineĬ��Ϊ-1��ʾѭ���л�λ��
    Vec2 getSoliderOrigin(int camp, int line = -1);
	// ���0, 1, 2��·��yֵ
	float getSoldierLineY(int line);

    // ��ȡ�Ѿ�������ʿ������
    bool isEnoughDeadSoldierCount(int uid, int compareNum);
    // ��ȡ�ɷ�������
    bool isEnoughDispatchCount(int compareNum);
    // ��ȡָ����Ӫ
    int getTreatCount(int camp);

private:
    /////////////////////////////////// ��ʼ������ ///////////////////////////////////////
    bool initStage();
    bool initModel();

    //////////////////////////////// ���л��뷴���л� ////////////////////////////////////
    // ���л���ɫĿ¼
    void serializeRoleTable(CBufferData& data, std::vector<CRole*>& vec);
    // ���л���ɫ�б�
    void serializeRoles(CBufferData& data, std::vector<CRole*>& vec);
    // ���л��ӵ�Ŀ¼
    void serializBulletTable(CBufferData& data, std::list<CBullet*>& l);
    // ���л��ӵ��б�
    void serializBullets(CBufferData& data, std::list<CBullet*>& l);
    // �����л�ʱ������ɫ
    bool createRole(int ownerId, int objId, int objType, int typeId, int lv,
		CBufferData& data, std::vector<CRole*>& vec);
    // �����л���ɫĿ¼
    void unserializeRoleTable(CBufferData& data, std::vector<CRole*>& vec, std::vector<CRole*>& vecRemove, std::vector<int>& idVec);
    // �����л���ɫ�б�
    void unserializeRoles(CBufferData& data, std::vector<int>& vec);
    // �����л�ʱ�����ɫ
    void removeRoles(std::vector<CRole*>& roles);
    // �����л��ӵ�Ŀ¼
    void unserializeBulletTable(CBufferData& data, std::list<CBullet*>& l, std::vector<CBullet*>& vecRemove, std::vector<int>& idVec);
    // �����л��ӵ��б�
    void unserializeBullets(CBufferData& data, std::vector<int>& vec);
    // �����л�ʱ����ӵ�
    void removeBullets(std::vector<CBullet*>& bullets);

    //////////////////////////////// ��Ϸ�������˽�з��� ////////////////////////////////////
    // ʹ���û�ģ�ʹ���Ӣ��
    CHero* createHero(CPlayerModel* player);
    // ����BOSS
    CBoss* createBoss(int bossID);
    // �ӳ��л�ȡһ��ʿ��
    CSoldier* newSoldier();
    // ����һ��ʿ��������
    void freeSoldier(CSoldier* soldier);
	//�ӳ��л�ȡһ������
	CMonster* newMonster();
	//����һ�����ﵽ����
	void freeMonster(CMonster* monster);
    // �ӳ��л�ȡһ���ӵ�
    CBullet* newBullet();
    // ����һ���ӵ�������
    void freeBullet(CBullet* bullet);
    // �ӳ����л�ȡһ��Buff
    CBuff* newBuff();
    // ����һ��buff������
    void freeBuff(CBuff* buff);

private:
    // ���ʤ��
    void setUserWinRolesState();
    // ���ʧ��
    void setUserLoseRolesState();

public:
    int CurTick;                                    // ��ǰʱ����е��ڼ�֡�����ٲ���
    int GameTick;                                   // ��ǰ��Ϸ�߼����е��ڼ�֡������ʱ�䲹�����ٵȱ仯
    int MaxTick;                                    // ��һ����Ϸ���ж����߼�֡��������ƣ����������������㣩
    int TickPerSecond;                              // ��Ϸÿһ���ж����߼�֡
    float MinX;                                     // �������������
    float MaxX;                                     // �������ұ�����
    CDispatch* Dispatch;                            // �������󣨴���Monster��
    CEventManager<int>* pEventManager;              // �¼�������
    CBattlePlayerModel* pMyUserModel;               // �ҵ��û�ģ�ͣ������Ϊ������
    CBattlePlayerModel* pEnemyUserModel;            // ���˵��û�ģ��
    bool IsBattleOver;                              // ս���Ƿ��ѽ���
    CRandomNumber RandNum;							// �������

private:
    int m_nUserId;                                  // �ҵ��û�ID�������Ϊ����ID��
    int m_nMasterId;                                // ��ID������û���ID
    int m_nEnmeyUserId;                             // ����ID
    int m_nStageId;                                 // ��ͼID
    int m_nStageLevel;                              // �ؿ��ȼ�
    int m_nBattleType;                              // ս������
    int m_nObjectId;                                // �ۼӵĶ���ΨһID
    int m_nCurLine;                                 // ��ǰ�ǵڼ�����

	int m_nCurExecutCommandCount;					// ��ǰִ����ɵ�������

    int m_nUserDeadSoldierCount;					// �����Ѿ�������ʿ������(����ʹ��,������)
    int m_nEnemyDeadSoldierCount;					// �з��Ѿ�������ʿ������(����ʹ��,������)
    int m_nCurDispatchCount;						// ������ǰ�ɷ���ʿ������(����ʹ��,������)

    Vec2 m_BlueHeroOrigin;                          // ����Ӣ�۵�
    Vec2 m_RedHeroOrigin;                           // �췽Ӣ�۵�
    CBattleLayer* m_pBattleScene;                   // ս������
    CRoomModel* m_pRoom;                            // ����ģ��
    CRole* m_BlueRole;                              // ��ߵ�Ӣ��
    CRole* m_RedRole;                               // �ұߵ�Ӣ��(������BOSS)
    CRole* m_MyRole;                                // �ҵ�Ӣ�ۣ�������Ӣ�ۣ�
    CRole* m_EnmeyRole;                             // �з���Ӣ��(������Ӣ�ۣ�������BOSS)
    const StageConfItem* m_pStageConf;              // ս������

    std::map<CRole*, CampType> m_ChangeCampMap;     // �л���Ӫ�Ķ����б��������л��ͷ����л�
    std::map<int, CGameObject*> m_ObjectMap;        // ��ǰ��Ϸ�������õ������ж��󣨰����ӵ��ͽ�ɫ�����Ѿ��ӳ������Ƴ��Ķ���
    std::vector<CRole*> m_BlueRoles;                // ��ߵ�ʿ���б�
    std::vector<CRole*> m_RedRoles;                 // �ұߵ�ʿ���б�(�ؿ�����monster)
    std::vector<CRole*> m_NeutralRoles;             // ������ʿ���б�
    std::vector<CRole*> m_AutoReleaseRoles;         // �Ѿ��ӳ������Ƴ�
    std::list<CRole*> m_RoleCache;                  // ʿ�������
	std::list<CRole*> m_MonsterCache;				// ���ﻺ���
    std::list<CBullet*> m_Bullets;                  // �ӵ��б�
    std::list<CBullet*> m_BulletCache;              // �ӵ������
    std::list<CBuff*> m_BuffCache;                  // buff�����
    std::vector<BattleCommandInfo> m_CommandQueue;  // �������
    std::vector<float> m_BlueLineX;
    std::vector<float> m_RedLineX;
    std::vector<float> m_LineY;
    std::map<int, int> m_TreatCount;                // ÿ����Ӫ��Ӧ��Ҫ���Ƶļ����������ڿ����ж��Ƿ���Ҫ������������
};

#endif
