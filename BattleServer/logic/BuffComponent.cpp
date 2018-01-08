#include "BuffComponent.h"
#include "Buff.h"
#include "ConfFight.h"

#include "Role.h"

CBuffComponent::CBuffComponent()
: m_pOwner(NULL)
{
    m_eComponentType = EBuffComponent;
}

CBuffComponent::~CBuffComponent()
{
    // ��������buff
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
        iter->second->onExit(true);
        destoryBuff(iter->second);
    }

    for (std::vector<CBuff*>::iterator iter = m_ExitBuffList.begin();
        iter != m_ExitBuffList.end(); ++iter)
    {
        (*iter)->onExit(true);
        destoryBuff(*iter);
    }

	for (std::vector<NewBuffData>::iterator iter = m_AddBuffList.begin(); 
		iter != m_AddBuffList.end(); ++iter)
	{
		SAFE_RELEASE(iter->maker);
	}

    m_BuffList.clear();
    m_ExitBuffList.clear();
	m_AddBuffList.clear();
}

bool CBuffComponent::serialize(CBufferData& data)
{
	SERIALIZE_MATCH(data);

	CLogicComponent::serialize(data);

    // ���˳�buff����
    int buffCnt = m_ExitBuffList.size();
    data.writeData(buffCnt);
    for (std::vector<CBuff*>::iterator iter = m_ExitBuffList.begin();
        iter != m_ExitBuffList.end(); ++iter)
    {
        int makerObjId = (*iter)->getMaker()->getObjectId();
        int buffid = (*iter)->getBuffConf()->ID;
        int stack = (*iter)->getStack();
        
        data.writeData(makerObjId);
        data.writeData(buffid);
        data.writeData(stack);

        (*iter)->serialize(data);
    }

    // buff����
    buffCnt = m_BuffList.size();
    data.writeData(buffCnt);
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
        int makerObjId = iter->second->getMaker()->getObjectId();
        int buffid = iter->first;
        int stack = iter->second->getStack();

        data.writeData(makerObjId);
        data.writeData(buffid);
        data.writeData(stack);
        iter->second->serialize(data);
    }

    // addbuff����
    buffCnt = m_AddBuffList.size();
    data.writeData(buffCnt);
    for (std::vector<NewBuffData>::iterator iter = m_AddBuffList.begin();
        iter != m_AddBuffList.end(); ++iter)
    {
        int makerObjId = (*iter).maker->getObjectId();
        int buffid = (*iter).buffid;
        int stack = (*iter).addstack;

        data.writeData(makerObjId);
        data.writeData(buffid);
        data.writeData(stack);
    }

	SERIALIZE_MATCH(data);

    return true;
}

bool CBuffComponent::unserialize(CBufferData& data)
{
	UN_SERIALIZE_MATCH(data);

	CLogicComponent::unserialize(data);

    CBattleHelper *pHelper = m_pOwner->getBattleHelper();
    CHECK_RETURN(pHelper);

    // ���˳�buff����
    int buffCnt = 0;
    data.readData(buffCnt);
	// ���˳���Buffһ����Ҫ�����
    for (std::vector<CBuff*>::iterator iter = m_ExitBuffList.begin();
        iter != m_ExitBuffList.end(); ++iter)
    {
		// ǿ��������д��˳���Buff
		(*iter)->onExit(true);
        destoryBuff(*iter);
    }
    m_ExitBuffList.clear();
    for (int i = 0; i < buffCnt; ++i)
    {
        int makerObjId = 0, buffid = 0, stack = 0;
        data.readData(makerObjId);
        data.readData(buffid);
        data.readData(stack);
        // ͨ��helper���ʵ��
        CRole *pMaker = pHelper->getRoleByObjectId(makerObjId);
        CBuff* buff = createBuff(this, m_pOwner, pMaker, buffid, stack);
        if (NULL == buff)
        {
            // ǰ������һ��
            LOG("Error CBuffComponent::unserialize m_ExitBuffList init %d %d %d", buffid, makerObjId, stack);
            continue;
        }
        buff->unserialize(data);
        m_ExitBuffList.push_back(buff);
    }

    // buff����
    data.readData(buffCnt);
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
		iter->second->onExit(true);
        destoryBuff(iter->second);
    }
    m_BuffList.clear();
    for (int i = 0; i < buffCnt; ++i)
    {
        int makerObjId = 0, buffid = 0, stack = 0;
        data.readData(makerObjId);
        data.readData(buffid);
        data.readData(stack);
        CRole *pMaker = pHelper->getRoleByObjectId(makerObjId);
        CBuff* buff = createBuff(this, m_pOwner, pMaker, buffid, stack);
        if (NULL == buff)
        {
            // ǰ������һ��
            LOG("Error CBuffComponent::unserialize m_ExitBuffList init %d %d %d", buffid, makerObjId, stack);
            continue;
        }
        buff->onEnter();
        buff->unserialize(data);
        m_BuffList[buffid] = buff;
    }

    // addbuff����
    data.readData(buffCnt);
	for (std::vector<NewBuffData>::iterator iter = m_AddBuffList.begin();
		iter != m_AddBuffList.end(); ++iter)
	{
		SAFE_RELEASE(iter->maker);
	}
    m_AddBuffList.clear();
    for (int i = 0; i < buffCnt; ++i)
    {
        NewBuffData addBuffData;

        int temp = 0;
        data.readData(temp);
        addBuffData.maker = m_pOwner->getBattleHelper()->getRoleByObjectId(temp);
        data.readData(temp);
        addBuffData.buffid = temp;
        data.readData(temp);
        addBuffData.addstack = temp;

		SAFE_RETAIN(addBuffData.maker);
		m_AddBuffList.push_back(addBuffData);
    }
	UN_SERIALIZE_MATCH(data);

    return true;
}

bool CBuffComponent::init(CRole *owner)
{
    m_pOwner = owner;
    return m_pOwner != NULL;
}

void CBuffComponent::onExit()
{
    CLogicComponent::onExit();

    // ��������buff
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
        iter->second->onExit(true);
        destoryBuff(iter->second);
    }

    for (std::vector<CBuff*>::iterator iter = m_ExitBuffList.begin();
        iter != m_ExitBuffList.end(); ++iter)
    {
        (*iter)->onExit(true);
        destoryBuff(*iter);
    }

    for (std::vector<NewBuffData>::iterator iter = m_AddBuffList.begin();
        iter != m_AddBuffList.end(); ++iter)
    {
        SAFE_RELEASE(iter->maker);
    }

    m_BuffList.clear();
    m_ExitBuffList.clear();
    m_AddBuffList.clear();
}

void CBuffComponent::logicUpdate(float delta)
{
    if (!m_ExitBuffList.empty())
    {
        for (std::vector<CBuff*>::iterator iter = m_ExitBuffList.begin();
            iter != m_ExitBuffList.end(); ++iter)
        {
            (*iter)->onExit();
            destoryBuff(*iter);
        }

        m_ExitBuffList.clear();
    }

    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end();)
    {
        CBuff *buff = iter->second;
        if (buff->isFinish())
        {
            m_ExitBuffList.push_back(buff);
            m_BuffList.erase(iter++);
        }
        else
        {
            buff->update(delta);
            ++iter;
        }
    }

    if (!m_AddBuffList.empty())
    {
        for (std::vector<NewBuffData>::iterator iter = m_AddBuffList.begin();
            iter != m_AddBuffList.end(); ++iter)
        {
            NewBuffData &buff = (*iter);
            addBuffToRole(buff.maker, buff.buffid, buff.addstack);
			SAFE_RELEASE(buff.maker);
        }

        m_AddBuffList.clear();
    }
}

bool CBuffComponent::addBuff(CRole *maker, int buffid, int addstack)
{
    if (m_pOwner->currentState() == State_Death
        || m_pOwner->currentState() == State_None)
    {
        return false;
    }
    // ���Ҫ��ȥһ�������ڵ�BUFF����ֱ�ӷ���
    if (addstack < 0 && !hasBuff(buffid))
    {
        return false;
    }

	// �����ɫ��һ֡�ͱ�����, ������Ҫ��֤��ɫ�ݲ����ͷ�
	SAFE_RETAIN(maker);

    NewBuffData data;
    data.maker = maker;
    data.buffid = buffid;
    data.addstack = addstack;
    
    m_AddBuffList.push_back(data);
    return true;
}

bool CBuffComponent::hasBuff(int buffid)
{
    return m_BuffList.find(buffid) != m_BuffList.end();
}

void CBuffComponent::removeBuff(int buffid)
{
    if (m_BuffList.find(buffid) != m_BuffList.end())
    {
        m_BuffList[buffid]->finish(true);
    }
}

void CBuffComponent::disperse()
{
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
        if (iter->second->getBuffType() == DEBUFF)
        {
            iter->second->finish(true);
        }
    }
}

void CBuffComponent::purification()
{
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
        if (iter->second->getBuffType() == BUFF)
        {
            iter->second->finish(true);
        }
    }
}

void CBuffComponent::clear()
{
    for (std::map<int, CBuff*>::iterator iter = m_BuffList.begin();
        iter != m_BuffList.end(); ++iter)
    {
        if (iter->second->getBuffType() != UNSTOPPABLE)
        {
            iter->second->finish(true);
        }
    }
}

CBuff* CBuffComponent::createBuff(CBuffComponent *cpnt, CRole *owner, CRole *maker, int buffid, int stack)
{
    CBattleHelper *pHelper = m_pOwner->getBattleHelper();
    if (NULL == pHelper)
    {
        KXLOGDEBUG("get Helper error, can't createBuff");
    }
    return pHelper->createBuff(cpnt, owner, maker, buffid, stack);
}

void CBuffComponent::destoryBuff(CBuff* pBuff)
{
    CBattleHelper *pHelper = m_pOwner->getBattleHelper();
    if (NULL == pHelper)
    {
        KXLOGDEBUG("get Helper error, can't destoryBuff");
    }
    pHelper->destoryBuff(pBuff);
}

void CBuffComponent::addBuffToRole(CRole *maker, int buffid, int addstack)
{
    int maxBuffCnt = queryMaxBuffStack(buffid);
    if (maxBuffCnt <= 0)
    {
        KXLOGDEBUG("error: buffid %d max buff <= 0", buffid);
        return;
    }

    int stack = 0, newStack = addstack;
    std::map<int, CBuff*>::iterator iter = m_BuffList.find(buffid);
    // �Ѿ��и�buff
    if (iter != m_BuffList.end())
    {
        CBuff* pBuff = iter->second;
        stack = pBuff->getStack();
        newStack += stack;
        // �����ǰ����������buff,������������
        if (stack == maxBuffCnt && addstack > 0)
        {
            pBuff->reset();
            return;
        }
        else
        {
            // ���/����buff����
            // 1����ǰbuff�������С�ڵ���0,buff����
            if (newStack <= 0)
            {
                pBuff->finish();
                m_ExitBuffList.push_back(pBuff);
                m_BuffList.erase(iter);
            }
            else
            {
                if (newStack > maxBuffCnt)
                {
                    // ���buff����
                    newStack = maxBuffCnt;
                }

                // 2��ֹͣ��ǰbuff�����ݺͱ��ֲ���ִ��ʧЧ����
                pBuff->onExit();
                if (!pBuff->init(this, m_pOwner, maker, buffid, newStack))
                {
                    destoryBuff(pBuff);
                    m_BuffList.erase(iter);
                    return;
                }
                // 3��������buff������ݺͱ���
                pBuff->onEnter();
            }
        }
    }
    else
    {
        if (newStack < 1)
        {
            // BUFF���Ƴ�����Ӧ��������µ�BUFF
            return;
        }
        else if (newStack > maxBuffCnt)
        {
            // ���buff����
            newStack = maxBuffCnt;
        }

        CBuff* pBuff = createBuff(this, m_pOwner, maker, buffid, newStack);
        CHECK_RETURN_VOID(pBuff);
        // �ټ����µ�buff
        pBuff->onEnter();
        m_BuffList[buffid] = pBuff;
    }
}
