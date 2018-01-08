#ifndef __ROOM_H__
#define __ROOM_H__

#include <map>
#include <vector>
#include "KxTimerManager.h"
#include "KxCommInterfaces.h"
#include "CommStructs.h"

#define PVPROOM_PREPARE_EXPIRE	 10  //PVP���뷿�䳬ʱʱ��
#define PVPROOM_LOADING_EXPIRE	 60	 //PVP�����ʱ��

class CBufferData;
class CPlayer;
class CBattleLayer;
class Director;
template <typename T>
class CEventManager;
class CPvpModel;
class CPvpSettleResult;

class IRoom
{
public:
	IRoom();
	virtual ~IRoom();

public:
	// ��÷�������
	virtual int getRoomType() { return m_nRoomType; }

protected:

	int		m_nRoomType;
};

class CPvpRoom : public IRoom, public KxServer::KxTimerObject
{
public:
	CPvpRoom(void);
	~CPvpRoom(void);

public:
	// ��Ϸ��ʼ, ��ʼ����Ϸ֡��, ��ͼ
	bool initRoom(int battleId, int stageId, int pvpType = PVPROOMTYPE_PVP);
	// ��ʼ����
	void loadingRoom();
	// ��Ϸ��ʼ
	bool startRoom();
	// ս����
	void fightingRoom();
	// �˳�����
	void exitRoom();
	// timer�ص�
	void onTimer(const KxServer::kxTimeVal& now);

	// ִ����Ϸ �������ִ��֡��
	void executeGame(float dt);
	// ����ǰ����Ϸ����
	void onProcessGame(int cmd, int subcmd, void *data, int len);
	// �������, ������һ��
	void overGame();

    // ����ʤ����
    void processPlayer(int result, CPlayer* player,int nEnemySummerID,int mmr,
        int integral, int newintegral, int crylv, int dt);

	// ��Ҽ��뷿��
	bool playerJoin(int uid, int summonerId, std::vector<int> &heroIds);
    bool playerJoin(int uid, char* playerData, int len);

	// ����˳�����
	bool playerQuit(int uid);

	// �����Ƿ�����
	bool isFull();
	// �Ƿ�׼�����
	bool isAllReady();
	// �Ƿ��Ѽ��뷿��
	bool isRoomPlayer(int uid);
	// �Ƿ�ȫ������
	bool isAllDisconnect();
	// ����Ƿ����
	bool isPlayerDisconnect(int uid);

	// ��¼��Ҽ��ؽ���
	void playerLoading(int uid, int progress);
	// ���׼��
	void playerReady(int uid);
	// �������
	void playerReconnect(int uid);
	// ����ҵ���
	void playerDisconnect(int uid);

	// ����״̬
	int getRoomState();
	// ��ö���uid
	int getOpponentUid(int uid);
	// �����ҽ���
	int getPlayerProgress(int uid);

	// ��ͼid
	int getStageId();
	// ���ս��id
	int getBattleId();
	// �þ���Ϸ�ڵ��øú���ʱ��ִ�еڼ�֡, �����һ����ʱ֡��    
	int getCurrentTick();
	// ��÷����ʱ
	int getRoomTick();
	// ���ս��Ԥ��ʣ��ʱ��
	float getPrepareTick();
	// ��÷������
	CPlayer *getRoomPlayer(int uid);
	// ��÷���
	CPlayer *getMasterPlayer();
    // ��÷Ƿ������
    CPlayer *getOtherPlayer();
	// ��÷����������
	std::map<int, CPlayer*> &getRoomPlayers();
	// ��÷�����Ϣ, �������������
	void getRoomData(CBufferData& bufferData);
	// ���ս����Ϣ
	void getBattleData(CBufferData &bufferData);

	// ����
	void setRoomTick(int tick);
	// ��ͼid
	int setStageId();
	// ����ʤ����
	void setWinnerId(int uid);
	// ����ʧ����
	void setLoserId(int uid);
	// ������ս���, ��ս���������Ϊ�ο�, ��m_nChallengeResultΪӮʱ�췽��
	void setChallengeResult(int result) { m_nChallengeResult = result; }
	// �����������
	void setPlayerCommunication(int uid, KxServer::IKxComm *target);
	// �����ڹ㲥��Ϣ
	int sendRoom(int maincmd, int subcmd, char *data, unsigned int len);
	// ָ����ҷ���
	int sendPlayer(int uid, int maincmd, int subcmd, char *data, unsigned int len);

private:
	// room info
	bool                     m_bClose;                      //�����Ƿ�׼���ر�
	int                      m_nStartStamp;                 //��ʼʱ��  (����)
	int                      m_nCurStamp;                   //��ǰʱ��� (����)
	int                      m_nPrevStamp;                  //�ϴ�ִ�е�ʱ��� (����)
	int                      m_nFPS;                        //��Ϸ֡��
	int                      m_nStageId;                    //��ͼId
	int						 m_nBattleId;					//ս��id
	int                      m_nRoomState;                  //������е�״̬
	int                      m_nRoomCapacity;               //���������������
	int						 m_nRoomTick;					//�����ʱ
	float					 m_fPrepareFightTick;			//׼��ս��ʱ���ʱ

	//������Ϣ
	int						 m_nWinnerId;					//ʤ����
	int						 m_nLoserId;					//ʧ����
	int						 m_nChallengeResult;			//��ս���(�������)
	Director*                m_pDirector;                   //������Ϸ�߼��ĵ�����
	CBattleLayer*            m_pBattleScene;                //ս������
	CPlayer*                 m_pMaster;                     //����
    CPlayer*                 m_pOther;                      //�Է�
    KxServer::KxTimerManager *m_pTimerManager;				//��ʱ��
	CEventManager<int>*      m_pEventManager;               //�����¼�������
	KxServer::kxTimeVal      m_LastTime;
	std::map<int, CPlayer*>  m_Players;						//����Ľ�ɫ
	std::map<int, int>       m_PlayerProgress;              //��ҽ���
	std::map<int, KxServer::IKxComm *> m_PlayerCommu;		//���session����
};

#endif //__ROOM_H__
