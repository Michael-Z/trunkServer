#include "BattleHelper.h"
#include "Hero.h"
#include "Soldier.h"
#include "CallSoldier.h"
#include "Bullet.h"
#include "Monster.h"
#include "Boss.h"
using namespace std;

// �����л��������ӵ�
bool bulletListSort(CBullet* obj1, CBullet* obj2)
{
	return obj1->getObjectId() < obj2->getObjectId();
}

bool CBattleHelper::serialize(CBufferData& data)
{
    SERIALIZE_MATCH(data);

    // ���湫�����ݣ���Ϸ�л�ı�ı�����CurTick���⣩
    data.writeData(GameTick);
    data.writeData(IsBattleOver);
    data.writeData(m_nObjectId);
    data.writeData(m_nCurLine);
	data.writeData(m_nCurExecutCommandCount);
    data.writeData(m_nUserDeadSoldierCount);
    data.writeData(m_nEnemyDeadSoldierCount);
    data.writeData(m_nCurDispatchCount);

    // ����RnadNum
    data.writeData(RandNum.getGroup());
    data.writeData(RandNum.getIndex());
    data.writeData(RandNum.getMode());

    // �������������
    data.writeData(m_TreatCount[ECamp_Neutral]);
    data.writeData(m_TreatCount[ECamp_Blue]);
    data.writeData(m_TreatCount[ECamp_Red]);

#ifdef RunningInServer
    // �����������
    int length = m_CommandQueue.size();
    data.writeData(length);
    for (std::vector<BattleCommandInfo>::iterator iter = m_CommandQueue.begin();
        iter != m_CommandQueue.end(); ++iter)
    {
        data.writeData(&(*iter), sizeof(BattleCommandInfo));
    }
#else
    data.writeData(-1);
#endif // RunningInServer

    // ����ʿ��Ŀ¼
    serializeRoleTable(data, getRoleWithCamp(ECamp_Blue));
    serializeRoleTable(data, getRoleWithCamp(ECamp_Red));
    serializeRoleTable(data, getRoleWithCamp(ECamp_Neutral));
    // �����Ѿ��뿪���������������õĽ�ɫĿ¼
    serializeRoleTable(data, m_AutoReleaseRoles);

    // �����ӵ�Ŀ¼
    serializBulletTable(data, getBullets());

    // ����˫��Ӣ��
    getMainRole(ECamp_Blue)->serialize(data);
    getMainRole(ECamp_Red)->serialize(data);

    // ����˫��ʿ�����Լ��Ѿ��뿪���������������õĽ�ɫ
    serializeRoles(data, getRoleWithCamp(ECamp_Blue));
    serializeRoles(data, getRoleWithCamp(ECamp_Red));
    serializeRoles(data, getRoleWithCamp(ECamp_Neutral));
    serializeRoles(data, m_AutoReleaseRoles);

    // ����˫���ӵ�
    serializBullets(data, getBullets());

    SERIALIZE_MATCH(data);

    return true;
}

bool CBattleHelper::unserialize(CBufferData& data)
{
    UN_SERIALIZE_MATCH(data);

    // ��ȡ�������ݣ���Ϸ�л�ı�ı�����CurTick���⣩
    data.readData(GameTick);
    data.readData(IsBattleOver);
    data.readData(m_nObjectId);
    data.readData(m_nCurLine);
	data.readData(m_nCurExecutCommandCount);
    data.readData(m_nUserDeadSoldierCount);
    data.readData(m_nEnemyDeadSoldierCount);
    data.readData(m_nCurDispatchCount);

    // ��ȡRnadNum
    int randGroup = 0;
    int randIndex = 0;
    int randModel = 0;
    data.readData(randGroup);
    RandNum.setGroup(randGroup);
    data.readData(randIndex);
    RandNum.setIndex(randIndex);
    data.readData(randModel);
    RandNum.setMode(randModel);

    // ��ȡ����������
    data.readData(m_TreatCount[ECamp_Neutral]);
    data.readData(m_TreatCount[ECamp_Blue]);
    data.readData(m_TreatCount[ECamp_Red]);

    // ��ȡ��������
    int cmdCounts = 0;
    data.readData(cmdCounts);
    if (cmdCounts != -1)
    {
        m_CommandQueue.clear();
        for (int i = 0; i < cmdCounts; ++i)
        {
            BattleCommandInfo info;
            data.readData(&info, sizeof(info));
            m_CommandQueue.push_back(info);
        }
    }

    // ֱ�����ObjectMap���ڷ����л��Ĺ����ж�̬����
    m_ObjectMap.clear();

    CRole* hero = getMainRole(ECamp_Blue);
    m_ObjectMap[hero->getObjectId()] = hero;
    hero = getMainRole(ECamp_Red);
    m_ObjectMap[hero->getObjectId()] = hero;

    // �����Ƚ����е�ʿ�����ӵ���Ϣ׼���ã�����ִ�з����л�
    // ��Ϊʿ�����ӵ���Ӣ��֮����ܻ��л������
    // ����ʿ��Ŀ¼
    vector<int> blueRoles;
    vector<int> redRoles;
    vector<int> neutralRoles;
    vector<int> cacheRoles;
    vector<CRole*> vecRemoveRoles;
    unserializeRoleTable(data, getRoleWithCamp(ECamp_Blue), vecRemoveRoles, blueRoles);
    unserializeRoleTable(data, getRoleWithCamp(ECamp_Red), vecRemoveRoles, redRoles);
    unserializeRoleTable(data, getRoleWithCamp(ECamp_Neutral), vecRemoveRoles, neutralRoles);
    unserializeRoleTable(data, m_AutoReleaseRoles, vecRemoveRoles, cacheRoles);

    // �����ӵ�Ŀ¼
    vector<int> masterBullets;
    vector<CBullet*> vecRemoveBullets;
    unserializeBulletTable(data, getBullets(), vecRemoveBullets, masterBullets);

    // ��ȡ˫��Ӣ��
    getMainRole(ECamp_Blue)->unserialize(data);
    getMainRole(ECamp_Red)->unserialize(data);

    // ��ȡ˫��ʿ��
    unserializeRoles(data, blueRoles);
    unserializeRoles(data, redRoles);
    unserializeRoles(data, neutralRoles);
    unserializeRoles(data, cacheRoles);

    // ��ȡ˫���ӵ�
    unserializeBullets(data, masterBullets);

    // ����Ѿ����ڳ����е�ʿ��
    removeRoles(vecRemoveRoles);
    // ����Ѿ����ڳ����е��ӵ�
    removeBullets(vecRemoveBullets);
    // �Ա��Ƴ��������Ķ�����ִ��һ��Remove
    for (vector<CRole*>::iterator iter = m_AutoReleaseRoles.begin();
        iter != m_AutoReleaseRoles.end(); ++iter)
    {
        (*iter)->remove();
    }

    UN_SERIALIZE_MATCH(data);

    RandNum.setGroup(randGroup);
    RandNum.setIndex(randIndex);
    RandNum.setMode(randModel);

    return true;
}

CRole* CBattleHelper::getRoleByObjectId(int objectId)
{
    map<int, CGameObject*>::iterator iter = m_ObjectMap.find(objectId);
    if (iter != m_ObjectMap.end())
    {
        return dynamic_cast<CRole*>(iter->second);
    }
    return NULL;
}

void CBattleHelper::serializeRoleTable(CBufferData& data, vector<CRole*>& vec)
{
    SERIALIZE_MATCH(data);

	int length = vec.size();
	data.writeData(length);
    // �����л�ID�����ڸ����ָ�
    for (vector<CRole*>::iterator iter = vec.begin();
        iter != vec.end(); ++iter)
    {
        CRole* role = *iter;
        data.writeData(role->getObjectId());
        data.writeData(role->getRoleType());
        data.writeData(role->getTypeId());
        data.writeData(role->getOwnerId());
        data.writeData(role->getIntAttribute(ECardLevel));
    }

    SERIALIZE_MATCH(data);
}

void CBattleHelper::serializeRoles(CBufferData& data, vector<CRole*>& vec)
{
    SERIALIZE_MATCH(data);

    for (vector<CRole*>::iterator iter = vec.begin();
        iter != vec.end(); ++iter)
    {
        (*iter)->serialize(data);
    }

    SERIALIZE_MATCH(data);
}

void CBattleHelper::serializBulletTable(CBufferData& data, list<CBullet*>& l)
{
    SERIALIZE_MATCH(data);

	int length = l.size();
	data.writeData(length);
    // �����л�ID�����ڸ����ָ�
    for (list<CBullet*>::iterator iter = l.begin();
        iter != l.end(); ++iter)
    {
        data.writeData((*iter)->getObjectId());
        data.writeData((*iter)->getTypeId());
        data.writeData((*iter)->getOwner()->getObjectId());
        data.writeData((*iter)->getOwnerId());
    }

    SERIALIZE_MATCH(data);
}

void CBattleHelper::serializBullets(CBufferData& data, list<CBullet*>& l)
{
    SERIALIZE_MATCH(data);

    for (list<CBullet*>::iterator iter = l.begin();
        iter != l.end(); ++iter)
    {
        (*iter)->serialize(data);
    }

    SERIALIZE_MATCH(data);
}

bool CBattleHelper::createRole(int ownerId, int objId, int objType, int typeId, int lv, CBufferData& data, vector<CRole*>& vec)
{
    CRole* role = NULL;
    switch (objType)
    {
        // ʿ��
    case RT_SOLDIER:
        {
            CSoldier* soldier = newSoldier();
            if (NULL == soldier || !soldier->init(typeId, ownerId, objId, this))
			{
				LOG("createRole RT_SOLDIER faile");
                // ���յ������
                freeSoldier(soldier);
                return false;
            }
            role = soldier;
        }
        break;

        // �ٻ���
    case RT_SUMMON:
        {
            CCallSoldier* call = new CCallSoldier();
            if (NULL == call || !call->init(typeId, lv, ownerId, objId, this))
			{
				LOG("createRole RT_SUMMON faile");
                delete call;
                return false;
            }
            role = call;
        }
        break;

        //����
    case RT_MONSTER:
        {
            CMonster* monster = newMonster();
            if (NULL == monster || !monster->init(typeId, ownerId, objId, this))
			{
				LOG("createRole RT_MONSTER faile");
                // ���յ������
                freeMonster(monster);
                return false;
            }
            role = monster;
        }
        break;
    }

    m_ObjectMap[objId] = role;
    m_pBattleScene->addChild(role);
    vec.push_back(role);
    return true;
}

void CBattleHelper::unserializeRoleTable(CBufferData& data, std::vector<CRole*>& vec, std::vector<CRole*>& vecRemove, std::vector<int>& idVec)
{
    UN_SERIALIZE_MATCH(data);

    // 1.data��û�У������е�ɾ��
    // 2.data���У������еĸ���
    // 3.data���У�����û�еĴ���
    // 4.�����Ƚ����ж��󴴽��ã�����˳����з����л����м����֮��������໥���ã�
    // ȡ����ɫ����
    int roleCounts = 0;
    data.readData(roleCounts);

    // �Ƚ������б��Ŀ¼����
    set<int> roleIds;           // ��ɫ��Id���ϣ����ڿ��ٲ���
    vector<int> roleTypeVec;    // ��ɫ���������飬��ʿ�����������ٻ���
    vector<int> roleTypeIdVec;  // ��ɫ������ID���飬��ĳ�����͵ľ���ID
    vector<int> roleOwnerIdVec; // ��ɫ��OwnerId����
    vector<int> roleExtVec;     // ��ɫ����չ����(�ȼ�)
    for (int i = 0; i < roleCounts; ++i)
    {
        int var = 0;
        data.readData(var);
        roleIds.insert(var);
        idVec.push_back(var);
        data.readData(var);
        roleTypeVec.push_back(var);
        data.readData(var);
        roleTypeIdVec.push_back(var);
        data.readData(var);
        roleOwnerIdVec.push_back(var);
        data.readData(var);
        roleExtVec.push_back(var);
    }

    // ����һ��Map���棬��ߴ����͸��µ�Ч�ʣ����޳������ڵĶ���
    map<int, CRole*> roles;
    for (vector<CRole*>::iterator iter = vec.begin();
        iter != vec.end();)
    {
        CRole* role = *iter;
        if (roleIds.find(role->getObjectId()) == roleIds.end())
        {
            // ֱ��ɾ���Ѿ������ڵĶ���
            // ���ﲻ����remove������Ϊ�п��ܸö����ƶ�������һ���б���
            // role->remove();
            iter = vec.erase(iter);
            vecRemove.push_back(role);
        }
        else
        {
            // ���½���ObjectMap
            roles[role->getObjectId()] = role;
            m_ObjectMap[role->getObjectId()] = role;
            ++iter;
        }
    }

    // ����Ҫ�´����Ľ�ɫ��������
    for (int i = 0; i < roleCounts; ++i)
    {
        int id = idVec[i];
        map<int, CRole*>::iterator roleIter = roles.find(id);
        // �������û�У��򴴽�
        if (roleIter == roles.end())
        {
            //����: monster�ȼ���ʱĬ��Ϊ1,��Ҫ���
            // ����ʿ��/����ʿ��/�ٻ���
            createRole(roleOwnerIdVec[i], id, roleTypeVec[i], roleTypeIdVec[i], roleExtVec[i], data, vec /*,�ȼ�*/);
        }
    }

    UN_SERIALIZE_MATCH(data);
}

void CBattleHelper::unserializeRoles(CBufferData& data, vector<int>& vec)
{
    UN_SERIALIZE_MATCH(data);

    // ˳�����л����ж���
    for (int i = 0; i < static_cast<int>(vec.size()); ++i)
    {
        int id = vec[i];
        m_ObjectMap[id]->unserialize(data);
    }

    UN_SERIALIZE_MATCH(data);
}

void CBattleHelper::removeRoles(vector<CRole*>& roles)
{
    for (vector<CRole*>::iterator iter = roles.begin();
        iter != roles.end(); ++iter)
    {
        CRole* role = *iter;
        if (m_ObjectMap.find(role->getObjectId()) == m_ObjectMap.end())
        {
            role->remove();
            destroyObject(role);
        }
        else
        {
            // ��ʾ��Ϣ���н�ɫ�ƶ�������������
            LOG("Some Role %d Change Queue", role->getObjectId());
        }
    }
}

void CBattleHelper::unserializeBulletTable(CBufferData& data, list<CBullet*>& l, vector<CBullet*>& vecRemove, vector<int>& idVec)
{
    UN_SERIALIZE_MATCH(data);

    // 1.data��û�У������е�ɾ��
    // 2.data���У������еĸ���
    // 3.data���У�����û�еĴ���
    // 4.�����Ƚ����ж��󴴽��ã�����˳����з����л����м����֮��������໥���ã�

    // ȡ���ӵ�����
    int bulletCount = 0;
    data.readData(bulletCount);

    // �Ƚ������б��Ŀ¼����
    set<int> bulletIds;
    vector<int> bulletTypeIdVec;
    vector<int> attackerIdVec;
    vector<int> ownerIdVec;
    for (int i = 0; i < bulletCount; ++i)
    {
        int var = 0;
        data.readData(var);
        bulletIds.insert(var);
        idVec.push_back(var);
        data.readData(var);
        bulletTypeIdVec.push_back(var);
        data.readData(var);
        attackerIdVec.push_back(var);
        data.readData(var);
        ownerIdVec.push_back(var);
    }

    map<int, CBullet*> bullets;
    // ���޳������ڵ��ӵ�
    for (list<CBullet*>::iterator iter = l.begin(); iter != l.end();)
    {
        CBullet* bullet = *iter;
        if (bulletIds.find(bullet->getObjectId()) == bulletIds.end())
        {
            bullet->remove();
            iter = l.erase(iter);
        }
        else
        {
            bullets[bullet->getObjectId()] = bullet;
            m_ObjectMap[bullet->getObjectId()] = bullet;
            ++iter;
        }
    }

    // ����µ��ӵ�
    for (int i = 0; i < bulletCount; ++i)
    {
        int id = idVec[i];
        map<int, CBullet*>::iterator bulletIter = bullets.find(id);
        // �������û�У��򴴽�
        if (bulletIter == bullets.end())
        {
            CBullet* bullet = newBullet();
            CRole* attacker = getRoleByObjectId(attackerIdVec[i]);
            if (bullet->init(bulletTypeIdVec[i], ownerIdVec[i], id, attacker, this))
            {
                m_pBattleScene->addChild(bullet);
                l.push_back(bullet);
                m_ObjectMap[id] = bullet;
            }
            else
			{
				LOG("init Bullet Error! objectId %d bulletTypeId %d ownerId %d attacker %d",
                    id, bulletTypeIdVec[i], ownerIdVec[i], attacker);
                // ����
                freeBullet(bullet);
            }
        }
    }

	// �����л����ӵ���˳���뷴���л�ǰ��˳����ܻ᲻ͬ
	l.sort(bulletListSort);

    UN_SERIALIZE_MATCH(data);
}

void CBattleHelper::unserializeBullets(CBufferData& data, vector<int>& vec)
{
    UN_SERIALIZE_MATCH(data);

    // ˳�����л����ж���
    for (int i = 0; i < static_cast<int>(vec.size()); ++i)
    {
        int id = vec[i];
        m_ObjectMap[id]->unserialize(data);
    }

    UN_SERIALIZE_MATCH(data);
}

void CBattleHelper::removeBullets(std::vector<CBullet*>& bullets)
{
    for (vector<CBullet*>::iterator iter = bullets.begin();
        iter != bullets.end(); ++iter)
    {
        SAFE_RELEASE((*iter));
    }
}
