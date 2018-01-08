#ifndef __TARGET_SEARCHER_H__
#define __TARGET_SEARCHER_H__

#include <vector>

enum SearchSequence
{
    Positive_Sequence,          //�������ߵ�ʿ��ʹ�������� soldier[0].x < soldier[1].x <��...
    Inverted_Sequence,          //������ұߵ�ʿ��ʹ�÷����� soldier[0].x > soldier[1].x �� ...
};

enum SearchSortOrder
{
    SortOrder_None,             //������
    SortOrder_Blood,            //��Ѫ������
    SortOrder_X,                //��x������
	SortOrder_RND,              //���
    SortOrder_BigHatred,        //��޴Ӵ�С����
    SortOrder_SmallHatred,      //��޴�С��������
};

enum SearchType
{
    SearchType_None = 0,        //��
    SearchType_Attackable,      //�ɷ��幥��
    SearchType_Aoeable,         //�ɷ�aoe����
    SearchType_Buffable,        //�ɷ��buff����
};

enum SearchOrderType
{
    SearchOrder_Blood,          //Ѫ������
    SearchOrder_XCoord,         //x������
};

enum SearchRangeType
{
    SearchRange_Far = -2,       //Զ�̷�Χ
    SearchRange_Close = -1,     //��ս��Χ
    SearchRange_All = 0,        //����
};

enum SearchDirection
{
    SearchDirection_None = 0,   //��
    SearchDirection_OneWay,     //��������
    SearchDirection_BothWay,    //˫������
};

enum SearchListType
{
    SearchListType_All = 0,             //����
    SearchListType_Enemy,               //�о�
    SearchListType_Friend,              //�Ѿ�
    SearchListType_Self,                //�Լ�
    SearchListType_EnemySummoner,       //�з��ٻ�ʦ
    SearchListType_FriendSummoner,      //�����ٻ�ʦ
    SearchListType_FirstEnemy,          //ָ����Χ���뼺������ĵ�һ������
    SearchListType_NeedTreastFriend,    //������Ҫ����
    SearchListType_NeedTreastAll,       //ȫ��Ӫ������Ҫ����
    //SearchListType_FirstFriend,       //ָ����Χ���뼺������ĵ�һ���Ѿ�
};

class CRole;
class CGameObject;
class CBattleHelper;
class SearchConfItem;
class CTargetSearcher
{
public:
    // �ж�Ŀ���Ƿ��ڷ�Χ��
    static bool isTargetInRange(CRole *target, float minX, float maxX);
    // ���ҷ�Χ��Ŀ�꼯��
    static bool searchTargets(std::vector<CRole*> &objList, float minX, float maxX, int &minIdx, int &maxIdx, int dir);
    // �������Ŀ��
    static CRole *getClosestTarget(std::vector<CRole*> &objList, CRole *origin);
    // �����б�
    static void sortList(std::vector<CRole*> &objList, int sortorder);
    // ����Ŀ��
    static bool searchTargetsWithId(int searchid, CGameObject* searcher, std::vector<CRole *> *targets);
};

class CSearchCondition
{
public:
    // ��������
    static bool checkConditionAllAndPush(CRole *target, const SearchConfItem *conf, std::vector<CRole *> *targets);
    static bool checkConditionAll(CRole *target, const SearchConfItem *conf);
    // ��ɫ�Ƿ��������Ƿ��ܱ�����
    static bool checkDeath(CRole *target, bool canSearch);
    // ngΪ�Ƿ�ȡ��, ���º���ͬ��
	static bool checkIdentify(CRole *target, const std::vector<int> &identify);
    // Ѫ���ٷֱ�
    static bool checkBlood(CRole *target, bool ng, float percent);
    // ְҵ
    static bool checkVocation(CRole *target, bool ng, const std::vector<int> &vocations);
    // �Ա�
    static bool checkSex(CRole *target, bool ng, int sex);
    // �Ƿ��ս
    static bool checkFireRange(CRole * target, int rangeType);
    // ����
    static bool checkRace(CRole *target, bool ng, int race);
    // ��ɫ�Ǽ��ȼ�
    static bool checkStarLevel(CRole *target, bool ng, int starLevel);
    // ��������, ��ɫ����һ����������
    static bool checkType(CRole *target, int type);
    // �Ƿ�ӵ��ָ��buff����
    static bool checkBuff(CRole *target, bool ng, const std::vector<int> &buffIds);
    // �ǲ���ĳָ���Ľ�ɫid
    static bool checkRoleId(CRole *target, bool ng, const std::vector<int> &roleIds);
    // �Ƿ��Ƿ���״̬��
    static bool checkIsInState(CRole *target, bool ng, int state);
    // ȥ���Լ�
    static bool checkCutMe(std::vector<CRole *> &targets, CGameObject *searcher);
    // ����������
    static bool checkLimitMaxAndSort(std::vector<CRole *> &targets, CGameObject * searcher, int max, int sortOrder);
};

#endif //__TARGET_SEARCHER_H__
