#ifndef __SUMMONER_PROTOCOL_H__
#define __SUMMONER_PROTOCOL_H__

#pragma pack(1)

enum SUMMONERPROTOCOL
{
	CMD_SUM_CSBEGIN,
    CMD_SUM_BUY_CS,
	CMD_SUM_CSEND,

    CMD_SUM_SCBEGIN = 100,
    CMD_SUM_BUY_SC,
	CMD_SUM_SCEND,
};

//CMD_SUM_BUY_CS
struct SumBuyCS
{
	int summonerId;		//������ٻ�ʦid
};

//CMD_SUM_BUY_SC
struct SumBuySC
{
	int moneyType;		//��������
	int money;			//��������
};

#pragma pack()

#endif //__SUMMONER_PROTOCOL_H__
