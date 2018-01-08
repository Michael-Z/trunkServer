/*
*   �ٻ�ʦ��Ŀ�����¼�����
*
*   1. ���Ŵ���
*
*/
#ifndef _GAME_CHANGE_CLIENT_H__
#define _GAME_CHANGE_CLIENT_H__

#include "GameInterface.h"
#include "KXServer.h"
#include "SessionClient.h"

class CSummonerEvent :public IGameEvent
{
public:
	CSummonerEvent();
	virtual ~CSummonerEvent();

    virtual bool onUserEvent(int eventId, KxServer::IKxComm* target);
	virtual bool onUserProc(unsigned int id);

protected:
    bool sendDataToActor(CSessionClient* pClient, int nMainCmd, int nSubCmd, char *pszContext, int nLen);
};

#endif //_GAME_CHANGE_CLIENT_H__
