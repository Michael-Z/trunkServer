#include "NetworkManager.h"
#include "ChatClient.h"
#include "ChatServer.h"
#include "ChatConnect.h"
#include "Protocol.h"

using namespace KxServer;
using namespace std;

CNetWorkManager::CNetWorkManager(void)
: m_GuestId(1)
{
}

CNetWorkManager::~CNetWorkManager(void)
{
    map<unsigned int, KxServer::IKxComm*>::iterator iterGuest = m_GuestMap.begin();
    for (; iterGuest != m_GuestMap.end(); ++iterGuest)
    {
        iterGuest->second->release();
    }
    map<unsigned int, KxServer::IKxComm*>::iterator iterUser = m_UserMap.begin();
    for (; iterUser != m_UserMap.end(); ++iterUser)
    {
        iterUser->second->release();
    }
    map<int, vector<KxServer::IKxComm*> >::iterator iterServer = m_ServerMapByGroup.begin();
    for (; iterServer != m_ServerMapByGroup.end(); ++iterServer)
    {
        vector<KxServer::IKxComm*>& vecServer = iterServer->second;
        for (vector<KxServer::IKxComm*>::iterator iterSock = vecServer.begin();
            iterSock != vecServer.end(); ++iterSock)
        {
            (*iterSock)->release();
        }
    }
}

CNetWorkManager* CNetWorkManager::m_Instance = NULL;
CNetWorkManager* CNetWorkManager::getInstance()
{
    if (NULL == m_Instance)
    {
        m_Instance = new CNetWorkManager();
    }

    return m_Instance;
}

void CNetWorkManager::destroy()
{
    if (NULL != m_Instance)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

// ͨ�����ͻ�ȡ������,Key��Ӧ��Valueֵ��ȡģ
KxServer::IKxComm* CNetWorkManager::getServer(int nGroupID, int nValue)
{
	map<int, std::vector<KxServer::IKxComm*> >::iterator ator = m_ServerMapByGroup.find(nGroupID);
	if (ator == m_ServerMapByGroup.end())
	{
		return NULL;
	}

	vector<KxServer::IKxComm*> &vectServer = ator->second;
    if (vectServer.size() == 0)
    {
        return NULL;
    }

	// ��Valueֵ����Valueȡģ
	if (nValue != 0)
	{
        return vectServer[nValue % vectServer.size()];
	}
	else
	{
        return vectServer[rand() % vectServer.size()];
	}
}

// ��ȡĳ����������
std::vector<KxServer::IKxComm*>* CNetWorkManager::getGroupServer(int nGroupID)
{
    map<int, std::vector<KxServer::IKxComm*> >::iterator ator = m_ServerMapByGroup.find(nGroupID);
    if (ator == m_ServerMapByGroup.end())
    {
        return NULL;
    }
    vector<KxServer::IKxComm*> *pVectServer = &(ator->second);
    if (pVectServer->size() == 0)
    {
        return NULL;
    }
    return pVectServer;
}

bool CNetWorkManager::addServer(int nGroupID, KxServer::IKxComm* obj)
{
    if (obj == NULL)
    {
        return false;
    }

	std::map<int, vector<KxServer::IKxComm*> >::iterator ator = m_ServerMapByGroup.find(nGroupID);
	if (NULL != obj && ator == m_ServerMapByGroup.end())
    {
		vector<KxServer::IKxComm*> ServerList;
		ServerList.push_back(obj);
		m_ServerMapByGroup[nGroupID] = ServerList;
    }
	else
	{
		vector<KxServer::IKxComm*> &serverList = ator->second;
		serverList.push_back(obj);
	}
    obj->retain();
    return true;
}

bool CNetWorkManager::addGuest(unsigned int guestId, KxServer::IKxComm* obj)
{
	map<unsigned int, KxServer::IKxComm*>::iterator iter = m_GuestMap.find(guestId);
	if (iter != m_GuestMap.end())
	{
		//�ͻ���ID�ظ�
		return false;
	}

	m_GuestMap[guestId] = obj;
	KXSAFE_RETAIN(obj);
	return true;
}

// ��ȡ����֤���ӿͻ���
KxServer::IKxComm* CNetWorkManager::getGuest(unsigned int guestId)
{
	map<unsigned int, KxServer::IKxComm*>::iterator iter = m_GuestMap.find(guestId);
	if (iter == m_GuestMap.end())
	{
		//�ͻ���ID�Ҳ���
		return NULL;
	}
	return iter->second;
}

bool CNetWorkManager::removeGuest(unsigned int guestId)
{
	map<unsigned int, KxServer::IKxComm*>::iterator iter = m_GuestMap.find(guestId);
    if (iter == m_GuestMap.end())
    {
        return false;
    }

    KXSAFE_RELEASE(iter->second);
    m_GuestMap.erase(iter);
    return true;
}

// ������תΪ����OK�Ŀͻ���
bool CNetWorkManager::changeGuestToUser(CChatClient* guest, unsigned int userId)
{
    map<unsigned int, KxServer::IKxComm*>::iterator iter = m_GuestMap.find(guest->getGuestId());
	if (iter == m_GuestMap.end())
	{
		// �ͻ���ID�Ҳ���
		return false;
	}
    // �˴�������һ��release��retain���໥����
    m_UserMap[userId] = guest;
    m_GuestMap.erase(iter);
	return true;
}

// ��ȡ�Ѿ���֤���Ŀͻ���
KxServer::IKxComm* CNetWorkManager::getUser(unsigned int userId)
{
	map<unsigned int, KxServer::IKxComm*>::iterator iter = m_UserMap.find(userId);
	if (iter == m_UserMap.end())
	{
		//�ͻ���ID�Ҳ���
		return NULL;
	}
	return iter->second;
}

bool CNetWorkManager::removeUser(unsigned int userId)
{
    map<unsigned int, KxServer::IKxComm*>::iterator iter = m_UserMap.find(userId);
    if (iter == m_UserMap.end())
    {
        return false;
    }

    KXSAFE_RELEASE(iter->second);
    m_UserMap.erase(iter);
    return true;
}

// ���ݹ㲥���������
bool CNetWorkManager::broadCastData(char *pszContext, int nLen)
{
    for (map<unsigned int, KxServer::IKxComm*>::iterator ator = m_UserMap.begin();
        ator != m_UserMap.end(); ++ator)
    {
        ator->second->sendData(pszContext, nLen);
    }
    return true;
}

bool CNetWorkManager::broadCastData(int nMainCmd, int nSubCmd, char *pszContext, int nLen)
{
    for (map<unsigned int, KxServer::IKxComm*>::iterator ator = m_UserMap.begin();
        ator != m_UserMap.end(); ++ator)
    {
        CChatClient* pClient = dynamic_cast<CChatClient*>(ator->second);
        if (pClient)
        {
            sendDataToClient(pClient, nMainCmd, nSubCmd, pszContext, nLen);
        }
    }
    return true;
}

bool CNetWorkManager::broadCastUnionData(int unionId, int nMainCmd, int nSubCmd, char *pszContext, int nLen)
{
    map<unsigned int, std::set<unsigned int> >::iterator ator = m_UnionUserMap.find(unionId);
    if (ator == m_UnionUserMap.end())
    {
        return false;
    }

    set<unsigned int>::iterator iterMember = ator->second.begin();
    for (; iterMember != ator->second.end(); ++iterMember)
    {
        CChatClient* pClient = dynamic_cast<CChatClient*>(getUser(*iterMember));
        if (pClient)
        {
            sendDataToClient(pClient, nMainCmd, nSubCmd, pszContext, nLen);
        }
    }

    return true;
}

bool CNetWorkManager::sendDataToClient(CChatClient* pClient, int nMainCmd, int nSubCmd, char *pszContext, int nLen)
{
	unsigned int buffSize = sizeof(Head);
	bool ret = false;
	char* buff = reinterpret_cast<char*>(kxMemMgrAlocate(buffSize));

	//����ͷ��
	Head* head = reinterpret_cast<Head*>(buff);
	head->MakeCommand(nMainCmd, nSubCmd);
	head->length = buffSize + nLen;
	head->id = pClient->getUserId();
	ret = pClient->sendData(buff, buffSize) >= 0;
	if (ret && NULL != pszContext && nLen > 0)
	{
		ret = pClient->sendData(pszContext, nLen) >= 0;
	}

	kxMemMgrRecycle(buff, buffSize);
	return ret;
}

bool CNetWorkManager::sendDataToClient(int uid, int nMainCmd, int nSubCmd, char *pszContext, int nLen)
{
    CChatClient* pClient = dynamic_cast<CChatClient*>(getUser(uid));
    if (NULL == pClient)
    {
        return false;
    }

    return sendDataToClient(pClient, nMainCmd, nSubCmd, pszContext, nLen);
}

//��ȡ��ǰ���������
unsigned int CNetWorkManager::getCurClientNum()
{
    return m_GuestMap.size() + m_UserMap.size();
}

unsigned int CNetWorkManager::getGuestId()
{
	if (m_GuestId >= 0xFFFFFFFF)
	{
		m_GuestId = 1;
	}
	return m_GuestId++;
}

void CNetWorkManager::closeAllServer()
{
	/*std::map<int, std::vector<KxServer::IKxComm*> >::iterator mapit = m_ServerMapByGroup.begin();
	std::vector<KxServer::IKxComm*>::iterator vecit;
	for (; mapit!=m_ServerMapByGroup.end(); ++mapit)
	{
		vecit = mapit->second.begin();
		for (; vecit!=mapit->second.end(); ++vecit)
		{
			CSessionServer::getInstance()->getMainPoller()->removeCommObject((*vecit));
			(*vecit)->close();
			(*vecit)->release();
		}
	}*/
	m_ServerMapByGroup.clear();
}

void CNetWorkManager::addUnionChatUser(int unionId, int uid)
{
    m_UnionUserMap[unionId].insert(uid);
}

void CNetWorkManager::removeUnionChatUser(int unionId, int uid)
{
    map<unsigned int, set<unsigned int> >::iterator iter = m_UnionUserMap.find(unionId);
    if (iter == m_UnionUserMap.end())
    {
        return;
    }

    set<unsigned int>::iterator iterUser = iter->second.find(uid);
    if (iterUser != iter->second.end())
    {
        iter->second.erase(iterUser);
    }
}
