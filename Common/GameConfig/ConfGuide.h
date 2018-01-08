#ifndef __CONF_GUIDE_H__
#define __CONF_GUIDE_H__

#include "ConfManager.h"

enum CameraMoveType
{
    CameraMovePosition,
    CameraMoveMySummoner,
    CameraMoveEnemySummoner,
};

//��ͷ
class CameraConfItem
{
public:
	int									ID;						//��ͷID	
	int									MoveX;				    //��ͷ���ƾ���
    int                                 MoveType;               //0�����ƶ���1�ƶ��������ٻ�ʦ��2�ƶ����з��ٻ�ʦ
	int									MoveTime;				//��ͷ����ʱ��
	int									Scale;					//�������
	int									ScaleTime;				//����ʱ��
	int									Time;					//��ͷ������ʱ��
	int									NextCamera;				//������ͷID
};

//��������
struct GuideCondition
{
	int									Type;					//����
	VecInt								Param;					//����
};

//������
class GuideConfItem
{
public:
	int									ID;						//����ID
	int									Listen;					//������Ϣ
	VecInt								Nexts;					//�¸�����
    VecInt                              Closes;                 //Ҫ�رյ�����
	std::vector<GuideCondition>			StartCondition;			//��ʼ����
	std::vector<GuideCondition>			SkipCondition;			//��������
};

//����������
class GuideStepConfItem
{
public:
	int									GuideID;				//����ID
	int									StepID;					//����ID
	int									ButtonID;				//��ťID
	int									CameraID;				//��ͷID
	int									IsLock;					//�Ƿ�����
	int									IsPause;				//�Ƿ���ͣ
	int									IsHideUI;				//�Ƿ�����UI
	int									TipsContent;			//��ʾ����
	int									DialogContent;			//�Ի�������
	int									HeadName;				//��ɫ����
	int									EffectType;				//��Ч����
	int									EffectTime;				//��Чʱ��
	int									EffectParam;			//��Ч����
	int									TotalTime;				//����ʱ��
    int                                 EndType;                //��������
    int                                 Anchor;                 //ͳ��ê��
	VecInt								HighlightPos;			//����λ��
	VecInt								TipsPos;				//��ʾλ��
	VecInt								DialogPos;				//�Ի���λ��
	VecInt								ShowButton;				//������ť
	GuideCondition						EndCondition;			//��������
    std::string							HighlightRes;			//������Դ
    std::string							HighlightAni;			//����������ǩ
    std::string							TipsRes;				//��ʾ����Դ
    std::string							TipsAni;				//��ʾ�򶯻���ǩ
	std::string							DialogRes;				//�Ի�����Դ
    std::string							DialogAni;				//�Ի��򶯻���ǩ
	std::string							HeadRes;				//ͷ����Դ
    std::string							HeadTag;				//ͷ���ǩ
    std::string                         ShowCSB;				//��������
	std::string							ShowTag;				//����������ǩ
	std::string							BgRes;					//������Դ
	std::string							BgTag;					//���ű�ǩ
};

//UI�ڵ���
class UINodeConfItem
{
public:
	int									NodeID;					//�ڵ�ID
	int									UIID;					//����ID
	std::string							NodePath;				//�ڵ�·��
};

//UI״̬��
class UIStatusConfItem
{
public:
	int									UIID;					//����ID
	int									ButtonLockCount;		//��ť����
	int									NodeID;					//���Ŷ���
	std::string							CSB;					//���Ŷ���
};

struct GuideSoliderInfo
{
    int                                 SoliderId;              //ʿ��ID
    int                                 SoliderLevel;           //ʿ���ȼ�
    int                                 SoliderStar;            //ʿ���Ǽ�
};

// ս������
class GuideBattleConfItem
{
public:
    int                                 StageId;
    int                                 HeroId;
    int                                 HeroLv;
    std::vector<GuideSoliderInfo>       Soliders;
};

//��ͷ����
class CConfCamera : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

//��������
class CConfGuide : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

//�����������
class CConfGuideStep : public CConfBase
{
public:
    virtual ~CConfGuideStep();
	virtual bool LoadCSV(const std::string& str);

	GuideStepConfItem* getStep(int guideID, int stepID)
	{
		std::map<int, std::map<int, GuideStepConfItem*> >::iterator iter = m_mapGuideSteps.find(guideID);
		if (iter != m_mapGuideSteps.end())
		{
			std::map<int, GuideStepConfItem*>::iterator iterItem = iter->second.find(stepID);
			if (iterItem != iter->second.end())
			{
				return iterItem->second;
			}
		}
		return NULL;
	}

	std::map<int, GuideStepConfItem*>* getSteps(int guideID)
	{
		std::map<int, std::map<int, GuideStepConfItem*> >::iterator iter = m_mapGuideSteps.find(guideID);
		if (iter != m_mapGuideSteps.end())
		{
			return &(iter->second);
		}
		return NULL;
	}
private:
	std::map<int, std::map<int, GuideStepConfItem*> > m_mapGuideSteps;
};

//����UI�ڵ����
class CConfUINode : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

//����UI״̬����
class CConfUIStatus : public CConfBase
{
public:
    virtual ~CConfUIStatus();
	virtual bool LoadCSV(const std::string& str);

	UIStatusConfItem* getItem(int ui, int count)
	{
		std::map<int, std::map<int, UIStatusConfItem*> >::iterator iter = m_mapUIStatusItems.find(ui);
		if (iter != m_mapUIStatusItems.end())
		{
			std::map<int, UIStatusConfItem*>::iterator iterItem = iter->second.find(count);
			if (iterItem != iter->second.end())
			{
				return iterItem->second;
			}
		}
		return NULL;
	}

	std::map<int, std::map<int, UIStatusConfItem*> >& getItems()
	{
		return m_mapUIStatusItems;
	}

private:
	std::map<int, std::map<int, UIStatusConfItem*> > m_mapUIStatusItems;
};

class CConfGuideBattle : public CConfBase
{
public:
    virtual bool LoadCSV(const std::string& str);
    const GuideBattleConfItem* getConfItem() { return &m_confItem; }

private:
    GuideBattleConfItem m_confItem;
};

//��ѯ��������
inline const GuideStepConfItem* queryConfGuideStep(int guideID, int stepID)
{
	CConfGuideStep* conf = dynamic_cast<CConfGuideStep*>(CConfManager::getInstance()->getConf(CONF_GUIDE_STEP));
	return static_cast<GuideStepConfItem*>(conf->getStep(guideID, stepID));
}

//��ѯUI״̬
inline const UIStatusConfItem* queryConfUIStatus(int uiID, int btnCount)
{
	CConfUIStatus* conf = dynamic_cast<CConfUIStatus*>(CConfManager::getInstance()->getConf(CONF_UI_STATUS));
	return static_cast<UIStatusConfItem*>(conf->getItem(uiID, btnCount));
}

#endif