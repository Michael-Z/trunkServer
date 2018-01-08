#ifndef __GAME_MODULE_H__
#define __GAME_MODULE_H__

#include "BaseModule.h"
#include "GameInterface.h"

class CConnectModule : public CBaseModule
{
public:
    CConnectModule(void);
    virtual ~CConnectModule(void);

	bool init(IGameEvent *pSink = NULL);

    //��Ϸ���������ط�����Э�飬ת����ָ����ǰ��
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);

    //����Ϸ�������Ͽ����ӣ���������
    //����������newһ����ͨѶID�仯���ɿ�����������Դ��
    virtual void processError(KxServer::IKxComm *target);
    virtual void processEvent(int eventId, KxServer::IKxComm* target);

    bool processUserEvent(int nType, KxServer::IKxComm* target);

private:
	IGameEvent* m_pSink;    // ������Ϸ�߼��ص�
};

#endif
