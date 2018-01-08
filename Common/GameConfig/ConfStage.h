#ifndef __CONF_STAGE_H__
#define __CONF_STAGE_H__

#include "ConfManager.h"

#define MAX_CHAPTER_BOX_INDEX 3
  
//��ɫ��Ϣ
struct SRoleInfo     
{                    
	int						RoldID;					//ID
	int						RoldLv;					//�ȼ�
};

//NPC��Ϣ
struct SNPCInfo
{
	int						NpcId;					//npcId
	int						NpcLv;					//npc�ȼ�
	int						NpcPosX;				//npcλ��x
	int						NpcPosY;				//npcλ��y
};     

//�½ڶ�����Ϣ
struct ChapterExInfo
{
	int						PrevID;					//ǰһ�½�
	int						NextID;					//��һ�½�
	int						FirstStageID;			//��һ�ؿ�
};

//�ؿ�������Ϣ
struct StageExInfo
{
	int						NextID;					//��һ���ؿ�
};
                    
//�ؿ���Ϣ
struct StageInfo
{
    VecInt					ID;						//ID+�ȼ�								
    int						Name;					//����
    int						Desc;					//����
	VecInt					Drop;					//����
	StageExInfo				ExInfo;					//������Ϣ
    std::string				Thumbnail;				//����ͼ
	std::string				BG;						//����ͼ
};

//������Ϣ
struct SDispatchInfo
{
	float					Delay;					 //�ӳ�ʱ��
	int						DispatchLine;			 //������·
	int						DispatchSoldierID;		 //����ID���
	VecInt					ConditionOpenID;		 //��������ID
	VecInt					ConditionCloseID;		 //�ر�����ID
};

// �����ٻ���
struct SSceneCall
{
    int                     callID;     // �ٻ���ID
    float                   callPosX;   // �ٻ�������x
    float                   callPosY;   // �ٻ�������y
};

//��ͼ������
class MapConfItem
{
public:
	int						ID;						//��ͼID
	std::string				Sky;					//��ղ���Դ����
	std::string				Map;					//��ͼ����Դ����
    std::string				Fog;					//�������Դ����
    std::vector<std::string> MoodEffect;             //��ͼ��Ч
};

struct ChapterBoxData
{
	int Star;									//����������������
	std::vector<AwardInfo>	StarAward;			//����
};

//�½�������
class ChapterConfItem
{
public:
	int						ID;						//�½�ID
	int						Name;					//�½�����						
	int						Type;					//�½�����
	int						MapID;					//��ͼID
	int						UnlockLevel;			//�����ȼ�
	int						UnlockDiamond;			//������Ҫ��ʯ����
	int						TotalStageNum;			//�����ؿ�����
	std::vector<AwardInfo>  ChapterRewardPreview;	//Ԥ���½ڽ���
	VecInt					UnlockChapters;			//�����½�
	ChapterBoxData			Reward[3];				//���齱��
	std::map<int, StageInfo> Stages;				//�����ؿ�
	ChapterExInfo			ExInfo;					//������Ϣ
};

//��ͼ����������
class StageSceneConfItem
{
public:
	int                     Id;                     //��������id
	int                     BlueHeroPos;            //��ɫ��Ӣ��λ��
	int                     RedHeroPos;             //��ɫ��Ӣ��λ��
    int                     BgMusicId;              //��������
	float                   FrontScene_Size;        //ǰ���㳤��
	float                   FightScene_Size;        //ս���㳤��
	float                   BgScene_Size;           //�����㳤��
	float                   FarScene_Size;          //Զ���㳤��
	std::string				FrontScene_ccs;			//ǰ����ccs
	std::string				FightScene_ccs;			//ս����ccs
	std::string				BgScene_ccs;			//������ccs
	std::string				FarScene_ccs;			//Զ����ccs
	std::vector<float>      BlueSoldierPos;         //��ɫ��ʿ��λ��
	std::vector<float>      RedSoldierPos;          //��ɫ��ʿ��λ��
	std::vector<float>      Walkline;               //ʿ���ƶ�·��
    std::vector<std::vector<int> >   IMSControl1;    //���������л�����1
    std::vector<std::vector<int> >   IMSControl2;    //���������л�����2
};

//����������
class CDispatchItem
{
public:
    int						ID;						 //���
    int						Group;					 //����0�����κη��鶼Ҫ����
    int						ConditionType;			 //��������
    int						ConditionParam;			 //�������Ͳ���	
    bool					IsOpen;					 //0����1�ر�
    SDispatchInfo			DispatchInfo;			 //������Ϣ
};

//�ؿ�������
class StageConfItem
{
public:
	int						ID;						//�ؿ�ID		
	int                     StageSenceID;           //����id
	int						Boss;					//Boss
	int						Type;					//�ؿ�����
	int						TimeLimit;				//�ؿ���ʱ
	int						WinStar1;				//������������
	int						WinStar1Param;			//������������
	int						WinStar2;				//������������
	int						WinStar2Param;			//������������
	int						FirstItemDrop;			//��һ��ͨ�ص���id
	VecInt					Monsters;				//ʿ��
	VecInt					Win;					//ʤ������
	VecInt					Fail;					//ʧ������
	VecFloat				TypeParam;				//���Ͳ���
	VecInt					ItemDrop;				//����
    std::vector<SSceneCall> SceneCall;              //�����ٻ���
};

//��ͼ����
class CConfMap : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class PvpSceneConfItem
{
public:
	int						Dan;					//��λ
	VecInt					Stage;					//��λ��Ӧ����id�б�
};

//�½ڽ���
class CConfChapter : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
	void toExInfo();
};

//�ؿ���������
class CConfStageScene : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
};

//�ؿ��������ý���
class CConfDispatch : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    std::vector<int>& getGroup(){ return m_vecGroup; }
private:
	std::vector<int> m_vecGroup;
};

//�ؿ�����
class CConfStage : public CConfBase
{
public:
	~CConfStage();
	virtual bool LoadCSV(const std::string& str);
	void ToRoleInfo(std::string str, SRoleInfo &info);
	void ToNpc(const std::string &str, std::vector<SNPCInfo>& vec);
	void ToTime(const std::string& str, int type, TimeInfo& info);
	CConfDispatch *getDispatchData(const int &stageID);
private:
	std::map<int, CConfDispatch*> m_StageDispatchList;
};

class CConfPvpScene : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

// ��ѯ�½���Ϣ
inline const ChapterConfItem * queryConfChapter(int chapterId)
{
	CConfChapter *conf = dynamic_cast<CConfChapter *>(
		CConfManager::getInstance()->getConf(CONF_CHAPTER));
	return static_cast<ChapterConfItem*>(conf->getData(chapterId));
}

// ��ѯ�ؿ����
inline const StageConfItem* queryConfStage(int stageId)
{
    CConfStage *conf = dynamic_cast<CConfStage*>(
        CConfManager::getInstance()->getConf(CONF_STAGE));
    return static_cast<StageConfItem*>(conf->getData(stageId));
}

// ��ѯ�������
inline CConfDispatch* queryConfDispatch(int stageId)
{
    CConfStage *conf = dynamic_cast<CConfStage*>(
        CConfManager::getInstance()->getConf(CONF_STAGE));
    return static_cast<CConfDispatch*>(conf->getDispatchData(stageId));
}

// ��ѯ�ؿ���Դ����
inline const StageSceneConfItem* queryConfStageScene(int stageSceneId)
{
    CConfStageScene *conf = dynamic_cast<CConfStageScene*>(
        CConfManager::getInstance()->getConf(CONF_STAGE_SCENE));
    return static_cast<StageSceneConfItem*>(conf->getData(stageSceneId));
}

// ��ѯ��������
inline const StageInfo *queryStageInfoInChapter(int chapterId, int stageId)
{
    CConfChapter *conf = dynamic_cast<CConfChapter *>(
        CConfManager::getInstance()->getConf(CONF_CHAPTER));
    const ChapterConfItem * pChapterConf = static_cast<ChapterConfItem*>(conf->getData(chapterId));
    for (std::map<int, StageInfo>::const_iterator iter = pChapterConf->Stages.begin();
        iter != pChapterConf->Stages.end(); ++iter)
    {
        if ((*iter).second.ID[0] == stageId)
        {
            return &(iter->second);
        }
    }
    return NULL;
}

inline const int qureyRandomPvpStage(int dan)
{
	CConfPvpScene *conf = dynamic_cast<CConfPvpScene *>(
		CConfManager::getInstance()->getConf(CONF_PVPSCENE));
	PvpSceneConfItem *item = reinterpret_cast<PvpSceneConfItem*>(conf->getData(dan));
	if (NULL != item)
	{
		if (item->Stage.size() > 0)
		{
			int index = rand() % item->Stage.size();
			return item->Stage[index];
		}
	}
	return 0;
}

#endif
