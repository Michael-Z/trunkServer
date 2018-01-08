#ifndef __TEAM_PROTOCOL_H__
#define __TEAM_PROTOCOL_H__

#pragma pack(1)

enum TEAMPROTOCOL
{
	CMD_TEAM_CSBEGIN,
	CMD_TEAM_SETTEAM_CS, //���ö���
	CMD_TEAM_CSEND,

    CMD_TEAM_SCBEGIN = 100,
	CMD_TEAM_SETTEAM_SC, //���ö���Ӧ��
	CMD_TEAM_SCEND,
};

// CMD_TEAM_SETTEAM_CS
struct TeamSetCS
{
	int teamType;       // ��������
	int summonerID;     // �ٻ�ʦID
	int heroID1;        // Ӣ��ID
	int heroID2;        // Ӣ��ID
	int heroID3;        // Ӣ��ID
	int heroID4;        // Ӣ��ID
	int heroID5;        // Ӣ��ID
	int heroID6;        // Ӣ��ID
	int heroID7;        // Ӣ��ID
};

#pragma pack()

#endif //__TEAM_PROTOCOL_H__
