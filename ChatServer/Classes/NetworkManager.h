/*
 *  
 *  �������������ṹ, ͨ��NetworkManager���Զ�ָ��������������Ϣ
 *  
 */
#ifndef __CHAT_NETWORK_MANAGER_H__
#define __CHAT_NETWORK_MANAGER_H__

#include <map>
#include <vector>

#include "KXServer.h"
#include "ChatClient.h"

class CNetWorkManager
{
private:
    CNetWorkManager(void);
    virtual ~CNetWorkManager(void);

public:
    static CNetWorkManager* getInstance();
    static void destroy();

    //��������˷�������ID
    bool addServer(int nGroupID, KxServer::IKxComm* obj);
	//ͨ�����ͻ�ȡ������,Key��Ӧ��Valueֵ��ȡģ
	KxServer::IKxComm* getServer(int nGroupID, int nValue = 0);
    //��ȡĳ����������
    std::vector<KxServer::IKxComm*>* getGroupServer(int nGroupID);
    //��ȡ���з�����
    std::map<int, std::vector<KxServer::IKxComm*> >& getAllServer() { return m_ServerMapByGroup; }

	//��������ӵ�����֤�����б���
	bool addGuest(unsigned int guestId, KxServer::IKxComm* obj);
	//��ȡ����֤���ӿͻ���
    KxServer::IKxComm* getGuest(unsigned int guestId);
    bool removeGuest(unsigned int guestId);

	//������תΪ����OK�Ŀͻ���
    bool changeGuestToUser(CChatClient* guest, unsigned int userId);
	//��ȡ�Ѿ���֤���Ŀͻ���
    KxServer::IKxComm* getUser(unsigned int userId);
    bool removeUser(unsigned int userId);

    //���ݹ㲥���������
    bool broadCastData(char *pszContext, int nLen);
    bool broadCastData(int nMainCmd, int nSubCmd, char *pszContext, int nLen);
    bool broadCastUnionData(int unionId, int nMainCmd, int nSubCmd, char *pszContext, int nLen);

    bool sendDataToClient(CChatClient* pClient, int nMainCmd, int nSubCmd, char *pszContext, int nLen);
    bool sendDataToClient(int uid, int nMainCmd, int nSubCmd, char *pszContext, int nLen);
    //��ȡ��ǰ���������
    unsigned int getCurClientNum();
	//���̬ΨһID
	unsigned int getGuestId();

	void closeAllServer();

    void addUnionChatUser(int unionId, int uid);
    void removeUnionChatUser(int unionId, int uid);

private:
    static CNetWorkManager* m_Instance;

    unsigned int m_GuestId;			                                    // δ��֤�ͻ���ΨһID
    std::map<int, std::vector<KxServer::IKxComm*> >	m_ServerMapByGroup; // ��������ӹ���
    std::map<unsigned int, KxServer::IKxComm*> m_GuestMap;		        // δ��֤�Ŀͻ������ӹ���
    std::map<unsigned int, KxServer::IKxComm*> m_UserMap;       		// �Ѿ���֤���Ŀͻ������ӹ���
    std::map<unsigned int, std::set<unsigned int> > m_UnionUserMap;     // �������<unionId, set<uid> >
};

#endif
