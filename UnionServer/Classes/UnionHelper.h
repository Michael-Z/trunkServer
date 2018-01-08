#include "PersonMercenaryModel.h"
#ifndef _UNION_HELPER_H__
#define _UNION_HELPER_H__

#include <vector>
#include <list>
#include "ServiceDef.h"
#include "UnionMercenaryModel.h"
#include "PersonMercenaryModel.h"
#include "UnionShopModel.h"

class CUnionExpiditionModel;
class CUnionModel;
class CUnion;
struct SExpeditonWorldItem;
struct UnionListInfo;
struct OwnUnionInfo;
struct BuffData;

class CUnionHelper
{
public:
	//���������������ͼ
	static int getRandomMapID(const SExpeditonWorldItem *pExpiditionWorldData);
	//У���ͼ�����Ƿ�Ϸ�,����Ϸ�����ȡ�ؿ�ID
	static bool checkIndexData(CUnionExpiditionModel *pModel, int nIndex, int &nStageID, int &nStageLv);
	//��ȡ��ɫ��פվBUFF
	static bool getExpiditionBuff(CUnionExpiditionModel *pModel, std::vector<BuffData> &Buff);
	//��ս�����趨
	static bool finishExpiditionWar(CUnionExpiditionModel *pModel, bool bWin = false);
	//ˢ�¹����̵���Ʒ
	static bool freshUnionShopData(CUnionShopModel *pModel, int nUnionDropID);
    // ��ӹ����Ծ��
    static bool addUnionLiveness(int unionId, int liveness);
    // �������
    static bool addUnionReputation(int unionId, int reputation);
    // ͨ����������ȼ�
    static int calcUnionLv(int curLv, int curReputation);

    // ��ⴴ������(������ش�����)
    static int checkCreate(int uid, std::string unionName);
    // ��⹫�������Ƿ�Ϸ�(������ش�����)
    static int checkUnionName(std::string unionName);
    // ����ܷ����빫��
    static EUnionErrorCode checkApply(int uid, int unionId);
	
	// �����ҹ������, ���û�з���NULL
	static CUnion *getUserUnion(int uid);
    // �ܷ��߳�����
    static bool canKick(int initiativePos, int passivePos);
    // �ܷ���
    static bool canRelieve(int initiativePos, int passivePos);
    // �ܷ�ת��Ȩ��
    static bool canTransfer(int initiativePos, int passivePos);
    // �ܷ�����
    static bool canAppoint(int initiativePos, int passivePos);

    // ������������(ID)
    static void searchUnion(std::string searchName, std::list<UnionListInfo> &searchList);
    // ��ȡ������Ϣ
    static void fillUnionInfo(CUnionModel *pUnionModel, UnionListInfo &info);
    // �˳�����,�����´����빫��ʱ���
    static int quitUnion(int uid);
    // ������
    static EUnionErrorCode auditUser(int nUnionID, int uid, bool agree);
    // �����־
    static bool addUnionLog(int nUnionId, int eventType, std::string nameOne, std::string nameTwo = "", int extend = 0);
	// Ӷ������
	static int mercenaryPrize(UnionMercenaryInfo& info);
    // ���͹�����Ϣ(�����ͨ�����Զ�ͨ��)
    static void sendOwnUnionInfo(int uid, int unionId);
    // ���ͱ�������Ϣ(����ְ���ܾ�ͨ����)
    static void sendBeFunctionInfo(int uid, int unionId, int funcType);
    // ������ʾ��Ϣ(���콱�������)
    static void sendMessageTips(CUnionModel *pUnionModel, int tipsType);
    // ͨ�����id��ȡ���ڹ���ģ��
    static CUnionModel *getUnionModel(int uid);
    // ����¼������Ϣ
    static bool fillOwnUnionInfo(int uid, int unionId, OwnUnionInfo &ownUnionInfo);

    /////////////////////////////////// ����֪ͨ ///////////////////////////////////////
    static void noticeUnionMembers(int unionId, char *buff, int len);
    // ֪ͨ�����Ա
    static void noticeUnion(int unionId, int noticeId);
    // ֪ͨ�����Ա
    static void noticeUnion(int unionId, int noticeId, std::string szVal);
    // ֪ͨ�����Ա
    static void noticeUnion(int unionId, int noticeId, std::string szValOne, std::string szValTwo);
};

#endif //_UNION_HELPER_H__

