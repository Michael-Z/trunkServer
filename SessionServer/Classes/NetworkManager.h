/*
 *  
 *  �������������ṹ, ͨ��NetworkManager���Զ�ָ��������������Ϣ
 *  
 */
#ifndef __NETWORKMANAGER_H__
#define __NETWORKMANAGER_H__

#include <map>
#include<vector>

#include "KXServer.h"
#include "SessionClient.h"
#include "KXServer.h"

class CNetWorkManager : public KxServer::KxTimerObject
{
private:
    CNetWorkManager(void);
    virtual ~CNetWorkManager(void);

public:
    static CNetWorkManager* getInstance();
    static void destroy();

    //��������˷�������ID
    bool addServer(int nGroupID, KxServer::IKxComm* obj);
	//��ӱ��ݺ�˷�������ID
	bool addBakServer(int nGroupID, KxServer::IKxComm* obj);
	//���ĳ�����ݷ�������ID
	bool clearBakServer(int nGroupID);
	//ͨ�����ͻ�ȡ������,Key��Ӧ��Valueֵ��ȡģ
	KxServer::IKxComm* getServer(int nGroupID, int nValue = 0);
    //��ȡĳ����������
    std::vector<KxServer::IKxComm*>* getGroupServer(int nGroupID);
	//��ȡĳ���������鱸��
	std::vector<KxServer::IKxComm*>* getBakGroupServer(int nGroupID);
    //��ȡ���з�����
    std::map<int, std::vector<KxServer::IKxComm*> >& getAllServer() { return m_ServerMapByGroup; }

	//��������ӵ�����֤�����б���
	bool addGuest(unsigned int guestId, KxServer::IKxComm* obj);
	//��ȡ����֤���ӿͻ���
    KxServer::IKxComm* getGuest(unsigned int guestId);
    bool removeGuest(unsigned int guestId);

	//������תΪ����OK�Ŀͻ���
    bool changeGuestToUser(CSessionClient* guest, unsigned int userId);
	//��ȡ�Ѿ���֤���Ŀͻ���
    KxServer::IKxComm* getUser(unsigned int userId);
    bool removeUser(unsigned int userId);

    //���ݹ㲥���������
    bool broadCastData(char *pszContext, int nLen);

	bool sendDataToClient(CSessionClient* pClient, int nMainCmd, int nSubCmd, char *pszContext, int nLen);

    //��ȡ��ǰ���������
    unsigned int getCurClientNum();
	//���̬ΨһID
	unsigned int genGuestId();

	void closeAllServer();

	void onTimer(const KxServer::kxTimeVal& now);

	void setChangeFlag(bool bChange) { m_bChanging = bChange; }
	bool getChangeFlag() { return m_bChanging; }

private:
    static CNetWorkManager* m_Instance;

    unsigned int m_GuestId;													// δ��֤�ͻ���ΨһID
    std::map<int, std::vector<KxServer::IKxComm*> >	m_ServerMapByGroup;		// ��������ӹ���
	std::map<int, std::vector<KxServer::IKxComm*> >	m_BakServerMapByGroup;	// �������ӹ���
    std::map<unsigned int, KxServer::IKxComm*> m_GuestMap;		        // δ��֤�Ŀͻ������ӹ���
    std::map<unsigned int, KxServer::IKxComm*> m_UserMap;       		// �Ѿ���֤���Ŀͻ������ӹ���
	bool		m_bChanging;											//�Ƿ����л���
};

#endif
