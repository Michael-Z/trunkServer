#ifndef _CREBOT_PLAYER_H__
#define _CREBOT_PLAYER_H__

#include "KxServer.h"
#include <vector>

struct SOpData;

class CRebotPlayer
{
public:
	CRebotPlayer();
	~CRebotPlayer();

	bool init(int nUid);							//��ʼ��

	bool SetKxComm(KxServer::IKxComm* pKxComm);		//����ͨѶ�˿�

	bool SetLoginState(bool bState);				//���õ�½״̬

	bool PlayerOperator();							//��Ҳ���

	void encryptBuffer(int mainCmd, int subCmd, char* buff, int len);
public:
	//�����������
	bool ProcessRecvData(int mainCmd, int nSubCmd, char *pszContext, int nLen);

	//�����½���ݽ���
	bool ProcessLoginData(int nSubCmd, char *pszContext, int nLen);

	//����PVPս��ƥ��ʱ��ս��ID
	bool ProcessPvpMatchData(int nSubCmd, char *pszContext, int nLen);

	//�������PVPս������
	bool processPvpResultData(int nSubCmd);


protected:
	void PlayerCheckLogin();							//�����֤

	void PlayerLogin();									//��ҵ�½

	void PlayerSendRequest(SOpData *pData);				//��ҷ�������

private:

	int						m_nUid;					//�û�UID
	KxServer::IKxComm*		m_pKxComm;				//����ͨѶ�˿�
	bool					m_bLoginOK;				//�Ƿ��¼���
	bool					m_bSendLogin;			//�Ƿ��͵�½��Ϣ
	bool					m_bFight;				//�Ƿ���ս��
	int						m_bBattleID;			//ս��ID
	bool					m_bSendMatching;		//�Ƿ���ƥ����
	std::vector<SOpData>	m_VectOpData;			//��ҵĲ����б�

};


#endif //_CREBOT_PLAYER_H__
