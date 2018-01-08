#ifndef __PVP_SETTLE_RESULT_H__
#define __PVP_SETTLE_RESULT_H__

#define PVP_INTEGRAL_BOUNDARY   750

class CPvpSettleResult
{
public:
	CPvpSettleResult();
	~CPvpSettleResult();

public:
	// ��ʼ��
	bool init(int roomType);
	// ����
	void pvpSettleAccount(int winner, int loser, int result,int winerCryLv,int loserCryLv,int dt);
	
	// ����ʤ���ߵ���Ϣ
	static void setWinnerPvpModel(int roomType, int uid, int changeMMR, int changeInte, int newRank);
	// ����ʧ���ߵ���Ϣ
	static void setLoserPvpModel(int roomType, int uid, int changeMMR, int changeInte, int newRank);
	// ����ƽ����Ϣ, ƽ��ʱ, �ı��mmr�ͻ��ֶ�Ϊ0
	static void setDrawPvpModel(int roomType, int uid, int changeMMR, int changeInte, int newRank);
	// ���뱾�˵�MMR, ���ֵ�MMR, ��ʤ/����ϵ��, ʤ�����, ���ؼ���֮��ĸı��mmrֵ, ����Ϊ��
	static int calcMMR(int myMMR, int otherMMR, int k, int result);
	// ���뱾�˵Ļ���, ���ֵĻ���, ʤ�����, ���ؼ���֮��ĸı�Ļ���ֵ, ����Ϊ��
	static int calcIntegral(int myIntegral, int otherIntegral, int result);

    // ��λ��������
    static void integralDanUp(int uid, int oldDan, int newDan, bool isFirst);

private:
	// ���㹫ƽ����
	void overFairPvp(int winner, int loser, int result, int winerCryLv, int loserCryLv, int dt);
	// ���������
	void overChampionPvp(int winner, int loser, int result, int winerCryLv, int loserCryLv, int dt);

private:

	int m_nRoomType;
};

#endif //__PVP_SETTLE_RESULT_H__
