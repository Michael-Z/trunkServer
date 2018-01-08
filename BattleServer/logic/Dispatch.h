#ifndef __DISPATCH_H__
#define __DISPATCH_H__

#include <list>
#include "ConfStage.h"

enum EDispatchCondition
{
	DC_Non,								  // ������
	DC_PCHeroDistance,					  // ����Ӣ��ĳ����Χ�ڣ������������Ƿ�����ҵ�ʿ��
	DC_PCHeroHP,						  // ����Ӣ�۵�Ѫ��С�ڵ��ڶ��ٰٷֱ�
	DC_CrystalLv,						  // ��ҵ�ˮ�����Ｘ��
	DC_SoldierCount,					  // ��ҷ��ڳ��ϻ�����ʿ�����������Ｘ��
	DC_SoldierMaxStar,					  // ����ڳ��ϵĻ�����ʿ��������Ǽ����Ｘ��
	DC_DispatchSoldierCount,			  // ����Ѿ��ɷ���ʿ�������ﵽһ������
	DC_DeadSoldierCount,				  // ����ɷ���ʿ�������������ﵽһ������
	DC_PCDeadSoldierCount,				  // �����ɷ���ʿ�������������ﵽһ������
	DC_DispatchSoldierType,				  // ����ڳ���ʿ���Ƿ���ָ�����͵ı��֣�ָ������Ϊint��������Ӧʿ���Ŀ�Ƭ���Բ���ֵ
	DC_DispatchSoldierID				  // ����ڳ���ʿ���Ƿ���ָ��ID�ı��֣�ָ��IDΪint��������Ӧʿ��ID
};

class CBattleHelper;
class CRole;
class CDispatch
{
public:
	CDispatch();
	~CDispatch();

	// ��ʼ��(�ؿ�ID,���ID)
	bool init(int stageID, int playerID, int enemyID, CBattleHelper *battleHelper);

	void logicUpdate(float dt);
	// �����ж�
	bool checkCondition(const int& conditionType, const int& conditionVal);
	// ����
	void execute(const SDispatchInfo &info);

private:
	// ����Ӣ��ĳ����Χ�ڣ������������Ƿ�����ҵ�ʿ��
	bool checkPCHeroDistance(const int& conditionVal);
	// ����Ӣ�۵�Ѫ��С�ڵ��ڶ��ٰٷֱ�
	bool checkPCHeroHP(const int& conditionVal);
	// ��ҵ�ˮ�����Ｘ��
	bool checkCrystalLv(const int& conditionVal);
	// ��ҷ��ڳ��ϻ�����ʿ�����������Ｘ��
	bool checkSoldierCount(const int& conditionVal);
	// ����ڳ��ϵĻ�����ʿ��������Ǽ����Ｘ��
	bool checkSoldierMaxStar(const int& conditionVal);
	// ����Ѿ��ɷ���ʿ�������ﵽһ������(ע:������)
	bool checkDispatchSoldierCount(const int& conditionVal);
	// ����ɷ���ʿ�������������ﵽһ������
	bool checkDeadSoldierCount(const int& conditionVal);
	// �����ɷ���ʿ�������������ﵽһ������
	bool checkPCDeadSoldierCount(const int& conditionVal);
	// ����ڳ���ʿ���Ƿ���ָ�����͵ı��֣�ָ������Ϊint��������Ӧʿ���Ŀ�Ƭ���Բ���ֵ
	bool checkDispatchSoldierType(const int& conditionVal);
	// ����ڳ���ʿ���Ƿ���ָ��ID�ı��֣�ָ��IDΪint��������Ӧʿ��ID
	bool checkDispatchSoldierID(const int& conditionVal);

	// ����ر��б�
	void processCloseList(const int &conditionID);
	// �������б�
	void processOpenList(const int &conditionID);
	// ��������Ϣ�б�
	void processDispatchList(const int &conditionID);

    // ��ȡĳ��ʿ������Ϣ
    bool getSoldierInfo(CRole *role, int &confID, int &star);
    // �ж�ʿ������
    bool isSoldierType(int confid, int star, int type);

private:
    int							m_nPlayerCamp;			// �����Ӫ
    int							m_nEnemyCamp;			// ������Ӫ

    std::list<CDispatchItem*>	m_listDispatchOpen;		// ����ʱ�������б�
    std::list<CDispatchItem*>	m_listDispatchClose;	// ����ʱ�رյ��б�
    std::list<CDispatchItem*>	m_listDispatchCache;	// ���������б�
    std::list<SDispatchInfo>	m_listDispatchInfo;		// ������Ϣ�б�

    const StageConfItem			*m_ConfStageItem;		// �ؿ�����
    CBattleHelper				*m_pBattleHelper;		// ս������
};

#endif