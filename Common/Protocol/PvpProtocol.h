#ifndef __PVPPROTOCOL_H__
#define __PVPPROTOCOL_H__

#pragma pack(1)

//PVP����Э��, ����ServiceServer
enum PvpInfoProtocol
{
	CMD_PVPINFO_CSBEGIN,
	CMD_PVPINFO_REWARD_CS,			//������ȡ
	CMD_CHAMPIONSHIP_REWARD_CS,		//����������
	CMD_PVPINFO_CSEND,

	CMD_PVPINFO_SCBEGIN = 100,
	CMD_PVPINFO_REWARD_SC,			//������ȡ��Ϣ
	CMD_CHAMPIONSHIP_REWARD_SC,		//����������
	CMD_PVPINFO_SCEND,
};

//ƥ��Э��, ����MatchServer
enum MatchProtocol
{
	CMD_MATCH_CSBEGIN,
	CMD_MATCH_MATCHING_CS,			//����ƥ��
	CMD_MATCH_CANCEL_CS,			//ȡ��ƥ��
	CMD_MATCH_CSEND,

	CMD_MATCH_SCBEGIN = 100,
	CMD_MATCH_MATCHING_SC,			//����ƥ��
	CMD_MATCH_REMATCH_SC,			//����ƥ��
	CMD_MATCH_MATCHSUCCESS_SC,		//�ɹ�ƥ�䵽��Ϣ
	CMD_MATCH_CANCEL_SC,			//ȡ��ƥ��
	CMD_MATCH_SCEND,
};

//PVPս��Э��, ����BattleServer
enum PvpProtocol
{
	CMD_PVP_CSBEGIN,
	CMD_PVP_ENTERROOM_CS,			//������뷿��
	CMD_PVP_LOADING_CS,				//����ͻ��˼�����Ϣ
	CMD_PVP_READY_CS,				//����������
	CMD_PVP_RECONNECT_CS,			//��������
	CMD_PVP_RECONNECTROOMDATA_CS,	//���󷿼�����

	CMD_PVP_ENTERROBOTROOM_CS,		//��������˷���
	CMD_PVP_FINISHROBOTEROOM_CS,	//���������˷���
	CMD_PVP_CSEND,

	CMD_PVP_SCBEGIN = 100,
	CMD_PVP_ENTERROOM_SC,			//���ؽ��뷿�����Ϣ��״̬
	CMD_PVP_PREPARE_SC,				//2���ڷ���ʱ, ֪ͨ��ҿ��Կ�ʼloading��Դ, ��ʱ�з��������·�
	CMD_PVP_LOADING_SC,				//ͬ��������Ϣ
	CMD_PVP_READY_SC,				//֪ͨ����Ҽ������
	CMD_PVP_IDLETIME_SC,			//����Ԥ��ʱ��
	CMD_PVP_START_SC,				//֪ͨ��ҿ�ʼ��Ϸ
	CMD_PVP_END_SC,					//֪ͨ�����Ϸ����
	CMD_PVP_RESULT_SC,				//���͸�ʤ��/ʧ�����
	CMD_PVP_RECONNECT_SC,			//������Ϣ
	CMD_PVP_RECONNECTROOMDATA_SC,	//��������
	CMD_PVP_OPPRECONNECT_SC,		//��������
	CMD_PVP_OPPDISCONNECT_SC,		//���ֶ���֪ͨ
	CMD_PVP_ROBOTROOMDATA_SC,		//�����˷�����Ϣ
    CMD_PVP_SCEND,

    CMD_PVP_ENTERROOM_SS = 200,     //�����Ľ��뷿�䣬��ҽ��뷿��-Session-Service-Battle
    CMD_PVP_FINISH_SS,
};

//CMD_PVPINFO_REWARD_CS
struct PvpInfoRewardCS
{
	int taskId;					//����id
};

//CMD_PVPINFO_REWARD_SC
struct PvpInfoRewardSC
{
    int taskId;					//����id
	int count;					//��������
	//DropItemInfo
};

// CMD_CHAMPIONSHIP_REWARD_CS
// CMD_CHAMPIONSHIP_REWARD_SC
struct ChampionshipRewardSC
{
	int count;
};

//CMD_MATCH
//CMD_MATCH_MATCHING_CS
struct MatchMatchingCS
{
	int matchType;				//ƥ������, ����ƥ�������
};

//CMD_MATCH_MATCHING_SC

//CMD_MATCH_MATCHPLAYER_SC
struct MatchMatchSuccessSC
{
	int battleId;				//battleId
	int robotId;
};

//CMD_MATCH_CANCEL_CS

//CMD_MATCH_CANCEL_SC

//CMD_PVP
//CMD_PVP_ENTERROOM_CS
struct PvpEnterRoomCS
{
	int battleId;				//battleid
	int roomType;				//��������
	int summonerId;				//�ٻ�ʦid
	int heroCount;				//Ӣ�۸���
};

//CMD_PVP_ENTERROOM_SC
struct PvpEnterRoomSC
{
	int result;					//���뷿����, Ϊ�˸�����ҷ������ɹ���ӵ�����
	int camp;					//�Լ�����

	int uid;				    //�������id
	int usrLv;				    //������ҵȼ�
	int summonerId;			    //��������ٻ�ʦid
	int integral;				//������һ���
	int progress;				//���ؽ���, ����ʱ���ֵĿ��ܲ�Ϊ0
    int extend;				    //��չ�ֶ�(����ȼ�*10+����)
	char usrName[32];		    //�����������
};

//CMD_PVP_PREPARE_SC
//RoomData*
//֪ͨǰ�˿�ʼ����

//CMD_PVP_LOADING_CS
struct PvpLoadingCS
{
	int progress;               //���ؽ���
};

//CMD_PVP_LOADING_SC
struct PvpLoadingSC
{
    int uid;                    //˭�Ľ���
    int progress;               //��������
};

//CMD_PVP_READY_CS
//����Ѽ������

//CMD_PVP_READY_SC
struct PvpReadySC
{
	int uid;					//������֪ͨ˭׼�����
};

//CMD_PVP_RECONNECT_CS
struct PvpReconnectCS
{
	int battleId;				//����ս��id
};

//CMD_PVP_RECONNECT_SC
struct PvpReconnectSC
{
	int roomState;				//��ʱ����״̬
	int roomType;				//��������
};

//CMD_PVP_RECONNECTROOMDATA_CS
struct PvpReconnectRoomDataCS
{
	int battleId;				//ƥ��id
};

//CMD_PVP_ENTERROBOTROOM_CS
struct PvpEnterRobotRoomCS
{
	int summonerId;				//�ٻ�ʦid
	int heroCount;				//Ӣ�۸���
	// heroId ...
};

//CMD_PVP_FINISHROBOTEROOM_CS
struct PvpFinishRobotRoomCS
{
	int wonOrFailed;			//ʤ��ʧ��
};

//CMD_PVP_START_SC
struct PvpStartSC
{
	int startStamp;			    //��ʼʱ���
};

//CMD_PVP_END_SC

//CMD_PVP_RESULT_SC
struct PvpResultSC
{
	int roomType;				//��������
	int result;					//ս�����
	int integral;				//���л���
	int rankNow;				//������ (0Ϊ10000������)
	int itemCount;				//��������
};

//CMD_PVP_ENTERROOM_SS
struct PvpEnterRoomSS
{
    int battleId;               //ս��id
    int roomType;               //��������
    int mmr;                    //��ҵ�MMR
    int integral;               //��ҵľ�������
    int continueWin;            //��ҵ���ʤ����
    int dayWinTimes;            //��ҵ���ʤ����
    // PlayerData*
};

// CMD_PVP_FINISH_SS
struct PvpFinishSS
{
    int roomType;               // ��������
    int result;                 // ���
    int newintegral;            // �����Ļ���
    int integral;               // �ı�Ļ���
    int mmr;                    // �ı��MMR
    int crylv;                  // ˮ���ȼ�
    int dt;                     // ��ʱ���룩
	int summonerid;				// �ٻ�ʦID
	int heroid[7];				// Ӣ��ID
	int enemysummonerid;		// �з��ٻ�ʦ
    int itemCount;              // ��Ʒ����
    // DropItemInfo*
};

#pragma pack()

#endif 
