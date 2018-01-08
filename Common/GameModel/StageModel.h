#ifndef __STAGE_MODEL_H__
#define __STAGE_MODEL_H__

#include <map>
#include "Storage.h"
#include "IDBModel.h"

enum EStageInfoField
{
	SIF_CANUSETIMES,
	SIF_USESTAMP,
	SIF_BUYTIMES,
	SIF_BUYSTAMP,
	SIF_NORMALCHAPTER_STATUS,
	SIF_ELITECHAPTER_STATUS,
	SIF_CURSTAGE,
	SIF_CURELITE,
};

struct DBEliteStageInfo
{
    int challengeTimes;		   //��Ӣ�ؿ���ս����
	int useRecoverStamp;	   //��սʱ��
	int buyTimes;			   //�������
	int buyRecoverStamp;	   //����ʱ��
};

enum EStageStatusType
{
	STAGESTATUS_HIDE = 0,		//����
	STAGESTATUS_LOCK,			//δ����
	STAGESTATUS_UNLOCK,			//�ѽ���
	STAGESTATUS_STAR1,			//1��״̬
	STAGESTATUS_STAR2,			//2��״̬
	STAGESTATUS_STAR3,			//3��״̬
};

enum EChapterStatusType
{
	CHAPTERSTATUS_LOCK = 0,		//δ����
	CHAPTERSTATUS_UNLOCK,		//�ѽ���
	CHAPTERSTATUS_FINISH,		//�����
	CHAPTERSTATUS_AWARD,		//����ȡ
};

enum EChapterBoxState
{
	CHAPTERBOX_UNGET,			//δ��ȡ
	CHAPTERBOX_GET,				//��ȡ
};

class CStageModel : public IDBModel
{
public:
    CStageModel();
    ~CStageModel();

public:
	// ��ʼ��
    bool init(int uid);
	// ˢ�¹ؿ�����
	bool Refresh();

	// ������ͨ�ؿ�״̬
	bool SetStageStatus(int stageId, int status, bool remove = true);
	// �������¹ؿ�
	bool SetCurStage(int stageId);
	// ��ùؿ�״̬, ���عؿ�״̬
	int GetStageStatus(int stageId);

	// �����½�״̬
	bool SetChapterStatus(int chapterId, int status);
	// ����½�״̬, �����½�״̬
	int GetChapterStatus(int chapterId);
	// ���þ�Ӣ�½�״̬
	bool SetEliteChapterStatus(int chapterId, int status);
	// ��þ�Ӣ�½�״̬, �����½�״̬
	int GetEliteChapterStatus(int chapterId);

	// ���þ�Ӣ�ؿ�״̬
	bool SetEliteStatus(int stageId, int status, int remove = true);
	// �����µľ�Ӣ�ؿ�
	bool SetCurElite(int stageId);
	// ��þ�Ӣ�ؿ�״̬
	int GetEliteStatus(int stageId);
	// ���þ�Ӣ�ؿ�״̬
	bool SetEliteInfo(int stageId, DBEliteStageInfo &info);
	// ���þ�Ӣ�ؿ���������
	bool SetEliteInfo(int stageId, int field, int value);
	// ���ָ����Ӣ�ؿ���Ϣ
	bool GetEliteInfo(int stageId, DBEliteStageInfo& info);
	// �Ƴ�ָ����Ӣ�ؿ��Ķ�����Ϣ
	bool RemoveEliteInfo(int stageId);
	// ɾ����ҹؿ�����
    bool DeleteStage();

	// ��ǰ�ؿ���Ϣ
	int GetCurStage() { return m_nCurStage; }
	int GetCurElite() { return m_nCurElite; }
	
	// ��������������
	std::map<int, int>& GetStageStatusMap() { return m_mapStageStatus; }
	std::map<int, int>& GetEliteStatusMap() { return m_mapEliteStatus; }
	std::map<int, int>& GetChapterStatusMap() { return m_mapChapterStatus; }
	std::map<int, int>& GetEliteChapterStatusMap() { return m_mapEliteChapterStatus; }
	std::map<int, DBEliteStageInfo>& GetEliteStageInfo() { return m_mapEliteStageInfo; }
	
	//��ȡ�½ڵ����б�����Ϣ
	std::map<int, int>* getChapterBoxStatusMap(int chapterId);
	bool getChapterBoxStatus(int chapterId, int nIndex, int &nStatus);
	//�����½ڱ�����Ϣ
	bool setChapterBoxStatus(int chapterId, int nIndex, int nStatus);
	std::map<int, std::map<int, int> >& getAllChapterBoxMap() { return m_mapChanpterBoxStatus; }
private:

	std::string GetStageInfoField(int id, int fieldType);

private:

	int								m_nUID;						// �û�ID
	int								m_nCurStage;				// ������ս����ͨ�ؿ�
	int								m_nCurElite;				// ������ս�ľ�Ӣ�ؿ�
	std::string						m_strStageStatusKey;		// �ؿ�Key    <stageId, status>
	std::string						m_strEliteStatusKey;		// ��Ӣ�ؿ�key <eliteId, status>
	std::string						m_strEliteInfoKey;			// �ؿ���Ϣkey <stageId/eliteId, usetimes/usestamp/buytimes/buystamp>
	std::string						m_strStageBoxStatusKey;		//�½ڱ���Key
	std::map<int, int>				m_mapStageStatus;			// ��ͨ�ؿ�״̬
	std::map<int, int>				m_mapEliteStatus;			// ��Ӣ�ؿ�״̬
	std::map<int, int>				m_mapChapterStatus;		    // �½�״̬
	std::map<int, std::map<int, int> > m_mapChanpterBoxStatus;	// �½ڱ���״̬
	std::map<int, int>				m_mapEliteChapterStatus;    // ��Ӣ�½�״̬
	std::map<int, DBEliteStageInfo>	m_mapEliteStageInfo;		// ��Ӣ�ؿ���Ϣ
	Storage *						m_pStorage;					// ���ݿ�
};

#endif