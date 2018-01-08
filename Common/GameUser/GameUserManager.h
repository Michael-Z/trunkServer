#ifndef __GAMEUSER_MANAGER_H__
#define __GAMEUSER_MANAGER_H__

#include <map>
#include "GameUser.h"
#include "KXServer.h"
#include "GameDef.h"
#include "UnionModel.h"
#include "MailModel.h"
#include "PassTeamModel.h"

//�ӳٴ���ṹ
struct SDelayDelData
{
	int nDelayTime;
	int nUid;
};

class CServerModel;
class CPvpRankModel;
class CPassTeamModel;
class CGameUserManager : public KxServer::KxTimerObject
{
private:
	CGameUserManager();
	~CGameUserManager();

public:

	static CGameUserManager *getInstance();
	static void destroy();

public:

	bool init(KxServer::KxTimerManager *pTimerManager);

	void addModelType(ModelType Type);

	void onTimer(const KxServer::kxTimeVal& now);
	// ��������û�����createWhenNullΪtrueʱ����ȡ�������Զ������ݿ��г�ʼ����
	CGameUser* getGameUser(int uid, bool createWhenNull = true);
	// ��ʼ���û�
	CGameUser* initGameUser(int uid);
	CGameUser *newGameUser(int uid);
	//�����û���Ҫ���õ�����
	void reSetGameUserData(int uid, bool bLogin = false);
    // ˢ���û�����
    void updateGameUserData(CGameUser* gameUsr, bool bLogin = false);
	// ����û�, ע���ʱ�����
	void addGameUser(int uid, CGameUser* gameUsr);
	//����û��ڸ÷������Ƿ����
	bool checkUserIsExist(int uid);
	// �Ƴ��û�
	void removeGameUser(int uid);
	// ɾ���Ƴ��û�����
	void donotDeleteUser(int uid);

	// ���ͨ�ض���ģ��
	CPassTeamModel *getPassTeamModel() { return m_pPassTeamModel; }
	// �������ģ������
	std::vector<ModelType>& getModelType() { return m_VectServerModel; }

	bool initUserData(CGameUser* gameUsr);

    const std::map<int, CGameUser *>& getGameUsers() { return m_GameUsers; }

private:

	//����ɾ���û�
	void RealremoveGameUser(int uid);

private:

	static CGameUserManager *							m_pInstance;
	CPassTeamModel *									m_pPassTeamModel;
	std::map<int, CGameUser *>							m_GameUsers;
	std::vector<ModelType>								m_VectServerModel;					//�����������������ģ��
	std::map<int, std::list<SDelayDelData>::iterator>	m_MapDelUserList;					//��Ҷ�Ӧ������������
	std::list<SDelayDelData>							m_DelUserList;						//�ӳ���������б�
	KxServer::KxTimerManager *							m_TimeManager;
};

#endif //__GAMEUSER_MANAGER_H__
