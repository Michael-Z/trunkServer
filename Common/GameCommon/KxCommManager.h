#ifndef _KXCOMM_MANAGER_H__
#define _KXCOMM_MANAGER_H__

#include "KXServer.h"
#include <map>
#include <vector>

class CKxCommManager
{
public:

    static CKxCommManager * getInstance();

    static void destroy();

    KxServer::IKxComm * getKxComm(unsigned int nId);

    bool addKxComm(unsigned int nId, KxServer::IKxComm* pClient);

	//�㲥������û���Ϣ
	int broadcast(std::vector<int>& uids, int cmd, char* data, unsigned int len);

    bool delKxComm(unsigned int nId);

    bool sendData(int uid, int main, int sub, char *buffer, int len, KxServer::IKxComm *pKxComm = NULL);

    bool forwardData(int uid, int main, int sub, char *buffer, int len, KxServer::IKxComm *pKxComm = NULL);

	int setSessionRoute(int uid, int key, int value);

	int getOnLineNum() { return m_MapClient.size(); }

    void setGate(KxServer::IKxComm* gate) 
    {
        if (gate != m_Gate)
        {
            KXSAFE_RELEASE(m_Gate);
            m_Gate = gate;
            KXSAFE_RETAIN(m_Gate);
        }
    }

private:
    
	CKxCommManager();
    ~CKxCommManager();

private:
    KxServer::IKxComm*                          m_Gate;         // ���أ����������Gate���������ݶ��ᷢ��Gate
    std::map<unsigned int, KxServer::IKxComm*>	m_MapClient;    // �ͻ������ӹ���
    static CKxCommManager *						m_pInstance;    //
};


#endif //_KXCOMM_MANAGER_H__
