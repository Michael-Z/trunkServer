#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include "CommStructs.h"
#include "BattleModels.h"

class CGameUser;
class CPvpRoom;
class CPlayer : public CPlayerModel
{
public:
    CPlayer(void);
    ~CPlayer(void);

public:
    //ͨ��uid�������ʼ����ɫ
    bool initWithData(char* data, int& len);
    //��������״̬
	void setIsConnecting(bool connecting);
	//�������״̬
	bool isConnecting();
    //��ȡ��������
    inline int getIntegral() { return m_Integral; }
    inline int getMMR() { return m_MMR; }
    inline int getCoutinueWin() { return m_ContinueWin; }
    inline int getDayWinTimes() { return m_DayWinTimes; }

private:
	bool						m_bIsConnecting;		//�Ƿ�������
    int                         m_Integral;             //��ҵľ�������
    int                         m_MMR;                  //��ҵ�MMRֵ
    int                         m_ContinueWin;          //��ʤ����
    int                         m_DayWinTimes;          //��ʤ����
};

#endif 
