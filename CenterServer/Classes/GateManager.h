/*
 * ��ڹ����������ڹ���Ψһ��Session���
 * 
 * 2013-11-1 By ��ү
 */

#ifndef __GATE_MANAGER_H__
#define __GATE_MANAGER_H__

#include "KXServer.h"
#include <vector>

class CGateManager
{
private:
    CGateManager(void);
    virtual ~CGateManager(void);

public:
    
    static CGateManager* getInstance();
    static void destroy();

    //���͸�Session, ���ݲ��޶�, �����Լ�����.
    int Send(char* buffer, unsigned int len);
    //������ָ���û���Ϣ
    int Transmit(int uid, int cmd, char* data, unsigned int len);
    //�㲥������û���Ϣ
    int Broadcast(std::vector<int>& uids, int cmd, char* data, unsigned int len);
    //����ָ��������
    void SetGate(KxServer::IKxComm* gate)
    {
        m_Gate = gate;
    }
    //���ָ������
    KxServer::IKxComm* GetGate()
    {
        return m_Gate;
    }

private:

    KxServer::IKxComm* m_Gate;
    static CGateManager* m_Instance;
};

#endif
