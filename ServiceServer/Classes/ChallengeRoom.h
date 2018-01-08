#ifndef __CHALLENGE_ROOM_H__
#define __CHALLENGE_ROOM_H__

#include <vector>
#include <map>
/*
��ս����, ��¼һЩ��ս�Ļ�����Ϣ, ���ڸ�����֤�ύ��Ϣ�Ϸ���
CChallengeRoom: ��ս��ؿ�����
CStageRoom:		������ͨ�ؿ�, ��Ӣ�ؿ�, �Լ�������ؿ������ݼ�¼
3������room: 
*/

class CGameUser;
class StageConfItem;
class CChallengeRoom
{
public:
	CChallengeRoom();
	virtual ~CChallengeRoom();

public:
	//��ʼ��
	virtual bool init(int uid, int battleType, int stageId, int stageLevel);
	//��չ�ֶ�1
	virtual void setExt1(int ext) { m_nExt1 = ext; }
	//��չ�ֶ�2
	virtual void setExt2(int ext) { m_nExt2 = ext; }
	//�����ٻ�ʦ
	virtual bool setSummonerId(int summonerId);
	//���Ӣ��
	virtual bool addHeroId(int heroId);
	//���BUFF
	virtual void addBuff(int nID);
	//Ѫ���ٷֱ�
	virtual void setHPPercent(int percent) { m_nHPPercent = percent; }
	//����ˮ����
	virtual void setUseCrystal(int useCrystal) { m_nUseCrystal = useCrystal; }
	//��ʱ
	virtual void setTick(int tick) { m_nTick = tick; }
	//��÷�������
	virtual void getRoomData(char *data, int &len);
	//��÷������ݵĳ���
	virtual int getRoomDataLength();
	//���UID
	virtual int getUid() { return m_nUid; }
	//��÷�������
	virtual int getBattleType()	{ return m_nBattleType; }
	//��ʼ��ս
	virtual void challengeBegin();
	//��ս����
	virtual void challengeEnd();
	//��ս�ؿ�ID
	virtual int GetChallengeID() { return m_nStageId; }
    //ȡ��ս��
    virtual void cancelChallenge();

protected:

	virtual int getStarInfo();

protected:

	int					m_nUid;				//�û�id
	int					m_nStageId;			//�ؿ�id
	int					m_nStageLevel;		//�ؿ��ȼ�
	int					m_nBattleType;		//��������
	int					m_nExt1;			//��չ�ֶ�1
	int					m_nExt2;			//��չ�ֶ�2
	int					m_nBeginTime;		//��ʼʱ��
	int					m_nEndTime;			//����ʱ��
	int					m_nSummonerId;		//�ٻ�ʦid
	int					m_nHPPercent;		//Ѫ���ٷֱ�
	int					m_nUseCrystal;		//���ĵ�ˮ������
	int					m_nTick;			//��ʱ
	int					m_nStar2Reason;		//�������2ԭ��
	int					m_nStar3Reason;		//�������3ԭ��

	CGameUser *			m_pUserInfo;		//�����Ϣ
	const StageConfItem *m_pStageConf;		//�ؿ�����
	std::vector<int>	m_HeroList;			//Ӣ��id
	std::vector<int>	m_vectBuffID;		//�ⲿbuffId�б�
};


// ������ս����
class ActivityInstanceItem;
class CInstanceRoom : public CChallengeRoom
{
public:
	CInstanceRoom();
	~CInstanceRoom();

public:
	//��ʼ��
	bool init(int uid, int instanceId, int difficulty);
	//��ø���id
	int getInstanceId() { return m_nInstanceId; }
	//��ø����Ѷ�
	int getDifficulty() { return m_nDifficulty; }
	//��ս����
	void challengeEnd();

private:
	//��������
	void getRewardData();

private:

	int						m_nInstanceId;		//����id
	int						m_nDifficulty;		//�����Ѷ�
	int						m_nFinalStar;		//
	const ActivityInstanceItem *m_pInstanceConf;//��������
};

#endif //__CHALLENGE_ROOM_H__
