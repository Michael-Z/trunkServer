#include "UnionExpiditionService.h"
#include "Protocol.h"
#include "UnionExpiditionProtocol.h"
#include "KxCSComm.h"
#include "UnionManager.h"
#include "KxMemPool.h"
#include "GameUserManager.h"
#include "ConfUnion.h"
#include "KxCommManager.h"
#include "RandGenerator.h"
#include <algorithm>
#include "ChallengeHelper.h"
#include "UnionHelper.h"
#include "UnionServer.h"
#include "ErrorCodeProtocol.h"
#include "ModelHelper.h"
#include "CommOssHelper.h"
#include "CommonHelper.h"
#include "BattleDataHelper.h"

using namespace std;
using namespace KxServer;

int CUnionExpiditionService::m_RankFreshTime = 0;

void CUnionExpiditionService::processService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
		case CMD_UNIONEXPIDITION_INFO_CS:
		{
			processExpiditionInfo(uid, buffer, len, commun);
		}
		break;
		case CMD_UNIONEXPIDITION_MAPSET_CS:
		{
			processExpiditionMapChoose(uid, buffer, len, commun);
		}
		break;
		case CMD_UNIONEXPIDITION_STAGESTART_CS:
		{
			processExpiditionFightStart(uid, buffer, len, commun);
		}
		break;
		case CMD_UNIONEXPIDITION_STAGEFINISH_CS:
		{
            CCommonHelper::encryptProtocolBuff(maincmd, subcmd, buffer, len);
			processExpiditionFightFinish(uid, buffer, len, commun);
		}
		break;
		case CMD_UNIONEXPIDITION_DAMAGERANK_CS:
		{
			processExpiditionDamageRank(uid, buffer, len, commun);
		}
		break;
		case CMD_UNIONEXPIDITION_STAGEINFO_CS:
		{
			processExpiditionStageInfo(uid, buffer, len, commun);
		}
		break;

		default:
		break;
	}
}

//������Զ����Ϣ�·�
void CUnionExpiditionService::processExpiditionInfo(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(CModelHelper::getModel(uid, MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);
	CUnion *pUnion = CUnionHelper::getUserUnion(uid);
	CHECK_RETURN_VOID(pUnion != NULL);

    CUnionExpiditionModel *pUnionExpModel = pUnion->getExpiditionModel();
    CHECK_RETURN_VOID(pUnionExpModel != NULL);
	// �Ƿ�����Զ��
	int worldId = 0;
	int mapId = 0;
	CHECK_RETURN_VOID(pUnionExpModel->getExpiditionFieldValue(UNION_EXPIDITION_WORLDID, worldId));
	CHECK_RETURN_VOID(pUnionExpModel->getExpiditionFieldValue(UNION_EXPIDITION_MAPID, mapId));
	// Զ����ͼ�����е�ͼ��bossѪ��, index-bossHp
    std::vector<SExpiditionIndexInfo> expStageInfo;
    std::set<int> &finishIndexs = pUnionExpModel->getAllFinishIndex();
	if (worldId != 0 && mapId != 0)
	{
		SExpeditionMapData *pMapConf = queryConfExpiditionMapData(mapId);
		CHECK_RETURN_VOID(NULL != pMapConf);
		// У��Զ���Ƿ����
		CUnionHelper::finishExpiditionWar(pUnionExpModel);
		
		// ��ʼ�ؿ���δ��ɵ�
		for (VecInt::iterator iter = pMapConf->vecStartStages.begin();
			iter != pMapConf->vecStartStages.end(); ++iter)
		{
			int index = *iter;
            CHECK_CONTINUE(index >= 0 && index < MAX_MAP_INDEX);

			if (finishIndexs.find(index) == finishIndexs.end())
			{
				SExpiditionIndexInfo indexInfo;
				indexInfo.nIndex = index;
				if (!pUnionExpModel->getCurExpiditionBossHp(indexInfo.nIndex, indexInfo.nBossHp))
                {
                    SExpeditonIndexData &indexConf = pMapConf->stageList[index - 1];
					indexInfo.nBossHp = indexConf.nBossHp;
				}
				expStageInfo.push_back(indexInfo);
			}
		}

		for (std::set<int>::iterator iter = finishIndexs.begin();
			iter != finishIndexs.end(); ++iter)
		{
			int index = *iter;
            CHECK_CONTINUE(index >= 0 && index < MAX_MAP_INDEX);

			// ǰ����ɺ�����ؿ�
			SExpeditonIndexData &indexConf = pMapConf->stageList[index - 1];
			for (VecInt::iterator unlockIter = indexConf.unlockIndexs.begin();
				unlockIter != indexConf.unlockIndexs.end(); ++unlockIter)
			{
				//  ��δ���
				if (finishIndexs.find(*unlockIter) == finishIndexs.end())
				{
					SExpiditionIndexInfo indexInfo;
					indexInfo.nIndex = *unlockIter;
					if (!pUnionExpModel->getCurExpiditionBossHp(indexInfo.nIndex, indexInfo.nBossHp))
					{
						indexInfo.nBossHp = indexConf.nBossHp;
					}
					expStageInfo.push_back(indexInfo);
				}
			}
		}
	}

	int value = 0;
    int nSendLength = sizeof(SUnionExpiditionInfo)+sizeof(SExpiditionIndexInfo)*(expStageInfo.size() + finishIndexs.size());
	char *pSendData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLength));
	// ����Զ����Ϣ
	SUnionExpiditionInfo *pExpeditionInfo = reinterpret_cast<SUnionExpiditionInfo*>(pSendData);
	pExpeditionInfo->nWorldID = worldId;
	pUserUnionModel->getUserUnionValue(USER_UNION_EXPIDITION_TIME, value);
	pExpeditionInfo->cExpiditonTimes = (unsigned char)value;
	pUnionExpModel->getExpiditionFieldValue(UNION_EXPIDITION_MAPID, pExpeditionInfo->nMapID);
	pUnionExpModel->getExpiditionFieldValue(UNION_EXPIDITION_FINISHTIME, pExpeditionInfo->nFightFinshTime);
	pUnionExpModel->getExpiditionFieldValue(UNION_EXPIDITION_COLDFINISHTIME, pExpeditionInfo->nFightColdStartTime);
	pUnionExpModel->getExpiditionFieldValue(UNION_EXPIDITION_REWARDSENDTIME, pExpeditionInfo->nRewardSendTime);
	// �Ƿ������ȡ
	vector<int> vecReward = pUserUnionModel->getUserExpiditionValue();
	pExpeditionInfo->cRewardFlag = vecReward.size() > 0 ? 1 : 0;
    pExpeditionInfo->cNum = expStageInfo.size() + finishIndexs.size();
	// ÿ���ؿ�bossѪ��
	SExpiditionIndexInfo *pIndexInfo = reinterpret_cast<SExpiditionIndexInfo *>(pExpeditionInfo + 1);
	for (std::vector<SExpiditionIndexInfo>::iterator iter = expStageInfo.begin(); 
		iter != expStageInfo.end(); ++iter)
	{
        memcpy(pIndexInfo, &(*iter), sizeof(SExpiditionIndexInfo));
		pIndexInfo += 1;
	}

    // ����˵Ĺؿ�
    for (std::set<int>::iterator iterFinish = finishIndexs.begin();
        iterFinish != finishIndexs.end(); ++iterFinish)
    {
        pIndexInfo->nIndex = *iterFinish;
        pIndexInfo->nBossHp = 0;
        pIndexInfo += 1;
    }

	CKxCommManager::getInstance()->sendData(uid, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_INFO_SC,
		pSendData, nSendLength);
	KxServer::kxMemMgrRecycle(pSendData, nSendLength);
}

//������Զ����ͼѡ��
void CUnionExpiditionService::processExpiditionMapChoose(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(buffer != NULL && len == sizeof(UnionExpiditionMapSetCS));
	UnionExpiditionMapSetCS *pUnionChooseCS = reinterpret_cast<UnionExpiditionMapSetCS*>(buffer);
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(NULL != pGameUser);
	CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);

	int nUnionID = 0;
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));
	CUnion *pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
	CHECK_RETURN_VOID(pUnion != NULL);
	CUnionExpiditionModel *pUnionExpiditionModel = pUnion->getExpiditionModel();
    CUnionModel *pUnionModel = pUnion->getUnionModel();
    CExpiditonRankModel *pRankModel = pUnion->getExpiditionRankModel();
    CHECK_RETURN_VOID(NULL != pUnionExpiditionModel && NULL != pUnionModel && NULL != pRankModel);

	int nRewardMapID = pUnion->getRewardMapID();
	if (nRewardMapID != 0)
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_UNION_EXPIDITION_REARDERROR;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
	}

	int nUnionLv = 0;
	CHECK_RETURN_VOID(pUnionModel->GetUnionFieldVal(UNION_FIELD_LV, nUnionLv));
	CHECK_RETURN_VOID(pUnionModel->GetMemberPos(uid) > 0);

	int nCurMapID = 0;
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_MAPID, nCurMapID));
	if (nCurMapID != 0)
	{
		//У��Զ���Ƿ����
		CUnionHelper::finishExpiditionWar(pUnionExpiditionModel);
	}
	const SExpeditonWorldItem *pExpiditionWorldData = queryConfExpiditionWordData(pUnionChooseCS->nWorldID);
	CHECK_RETURN_VOID(pExpiditionWorldData != NULL);
	//�����ȼ��ж��Ƿ���������
	CHECK_RETURN_VOID(nUnionLv >= pExpiditionWorldData->nUnLockLv);
	int nCurTime = time(NULL);
	int nTmpTime = 0;
	//�Ƿ���ս��״̬
	nTmpTime = 0;
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_FINISHTIME, nTmpTime));
	CHECK_RETURN_VOID(nTmpTime < nCurTime);
	//��ȴʱ���Ƿ���������
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_COLDFINISHTIME, nTmpTime));
	//�����Ϣʱ��������������뽻ս�׶�
	int nFinishFightTime = 0;
	if (nCurTime >= nTmpTime)
	{
		//����ս������
		nFinishFightTime = nCurTime + pExpiditionWorldData->nFightTime;
	}
	else
	{
		nFinishFightTime = nTmpTime + pExpiditionWorldData->nFightTime;
	}

	CHECK_RETURN_VOID(pUnionExpiditionModel->setExpiditinoFieldVale(UNION_EXPIDITION_FINISHTIME, nFinishFightTime));
	//����ս����ͼ��
	//���ָ��һ����ͼID
	int nMapID = CUnionHelper::getRandomMapID(pExpiditionWorldData);
	CHECK_RETURN_VOID(nMapID > 0);
	CHECK_RETURN_VOID(pUnionExpiditionModel->setExpiditinoFieldVale(UNION_EXPIDITION_MAPID, nMapID));
	CHECK_RETURN_VOID(pUnionExpiditionModel->setExpiditinoFieldVale(UNION_EXPIDITION_WORLDID, pUnionChooseCS->nWorldID));
	CHECK_RETURN_VOID(pUnionExpiditionModel->setExpiditinoFieldVale(UNION_EXPIDITION_FINISHFLAG, 0));
	pRankModel->clearDamageRankData();
	pRankModel->setMapID(nMapID);
	//�㲥�����й������߳�Ա
	UnionExpiditionMapSetSC MapSetSC;
	MapSetSC.nWorldID = pUnionChooseCS->nWorldID;
	MapSetSC.nMapID = nMapID;
	MapSetSC.nExpiditionFinishTime = nFinishFightTime;
	map<int, int>& MapMember = pUnionModel->GetMembers();
	for (map<int, int>::iterator ator = MapMember.begin(); ator != MapMember.end(); ++ator)
	{
		CKxCommManager::getInstance()->sendData(ator->first, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_MAPSET_SC, (char*)&MapSetSC, sizeof(MapSetSC));
	}
}
//������Զ���ַ���ʼ
void CUnionExpiditionService::processExpiditionFightStart(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(buffer != NULL && len == sizeof(UnionExpiditonStageStartCS));
	UnionExpiditonStageStartCS *pMsg = reinterpret_cast<UnionExpiditonStageStartCS*>(buffer);
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(NULL != pGameUser);
	CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);
	// ����service������װ������Ӣ��
	pGameUser->refreshModels();

	int nUnionID = 0;
	int nExpiditionTimes = 0;
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_EXPIDITION_TIME, nExpiditionTimes));
	//У���ɫ�����Ƿ��㹻
	const UnionItem& UnionSetData = queryConfUnion();
	CHECK_RETURN_VOID(nExpiditionTimes < UnionSetData.nExpiditionTimes);
	CUnion *pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
	CHECK_RETURN_VOID(pUnion != NULL);
	CUnionExpiditionModel *pUnionExpiditionModel = pUnion->getExpiditionModel();
	CHECK_RETURN_VOID(pUnionExpiditionModel != NULL);

	int nTempTime = 0;
	int nWorldID = 0;
	int nFlag = 0;
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_FINISHFLAG, nFlag));
	//У��Զ���Ƿ����
	if (CUnionHelper::finishExpiditionWar(pUnionExpiditionModel) || nFlag == 1)
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_UNION_EXPIDITION_OVER;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
	}

	//��ȴʱ���Ƿ���������
    CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_COLDFINISHTIME, nTempTime));
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_WORLDID, nWorldID));
	const SExpeditonWorldItem *pExpiditionWorldData = queryConfExpiditionWordData(nWorldID);
	CHECK_RETURN_VOID(pExpiditionWorldData != NULL);
	//������Ϣ�׶β��ܴ�
    CHECK_RETURN_VOID(nTempTime <= (int)time(NULL));

	//��������Ƿ��Ѿ����
	set<int>& SetIndex = pUnionExpiditionModel->getAllFinishIndex();
	set<int>::iterator iter = SetIndex.find(pMsg->nIndex);
	if (iter != SetIndex.end())
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_UNION_EXPIDITION_STAGEPASS;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
	}

    // ���췿������
    BattleRoomData room;
    room.battleType = EBATTLE_UNIONEXPEDITION;
    room.summonerId = pMsg->summonerId;
    room.mecenaryId = pMsg->mercenaryId;

	//У���ͼѡ���Ƿ�Ϸ�
    CHECK_RETURN_VOID(CUnionHelper::checkIndexData(pUnionExpiditionModel, pMsg->nIndex, room.stageId, room.stageLv));
	
	int nCurMapID = 0;
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_MAPID, nCurMapID));
	SExpeditionMapData *pMapData = queryConfExpiditionMapData(nCurMapID);
    CHECK_RETURN_VOID(NULL != pMapData);
	//У���Ƿ�Ϊ���չؿ�
	if (pMsg->nIndex == pMapData->nTotal)
	{
		//����פվBUff
        CUnionHelper::getExpiditionBuff(pUnionExpiditionModel, room.buffs);
	}

	for (int i = 0; i < 7; ++i)
	{
		if (pMsg->heroIds[i] > 0)
		{
            room.heroIds.push_back(pMsg->heroIds[i]);
		}
	}

    // ���ط������ݰ�
    CBufferData bufferData;
    bufferData.init(10240);
    if (!CBattleDataHelper::roomDataToBuffer(uid, room, bufferData))
    {
        return;
    }

    CHECK_RETURN_VOID(pUserUnionModel->setUserUnionValue(USER_UNION_INDEX, pMsg->nIndex));

    CKxCommManager::getInstance()->sendData(uid, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_STAGESTART_SC,
        bufferData.getBuffer(), bufferData.getDataLength());

}
//������Զ���ַ�����
void CUnionExpiditionService::processExpiditionFightFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(buffer != NULL && len == sizeof(UnionExpiditionStageFinishCS));
	UnionExpiditionStageFinishCS *pFinishCS = reinterpret_cast<UnionExpiditionStageFinishCS*>(buffer);
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(NULL != pGameUser);
	CHECK_RETURN_VOID(pFinishCS->nDamage >= 0);
	CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(pGameUser->getModel(MODELTYPE_HERO));
	CSummonModel *pSummerModel = dynamic_cast<CSummonModel*>(pGameUser->getModel(MODELTYPE_SUMMONER));
	CHECK_RETURN_VOID(pUserUnionModel != NULL && pHeroModel != NULL && pSummerModel != NULL);
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
	CHECK_RETURN_VOID(pUserModel != NULL);

	//У��ͻ��˷��͵����ݺϷ���
	for (int i = 0; i < 7; ++i)
	{
		if (pFinishCS->nHeroID[i] > 0)
		{
			CHECK_RETURN_VOID(pHeroModel->ExsitHero(pFinishCS->nHeroID[i]));
		}
	}
	CHECK_RETURN_VOID(pSummerModel->HaveSummoner(pFinishCS->nSummerID));

	int nUnionID = 0;
	int nIndex = 0;
	int nExpiditionTimes = 0;
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_INDEX, nIndex));
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_EXPIDITION_TIME, nExpiditionTimes));
	CHECK_RETURN_VOID(nIndex <= MAX_MAP_INDEX && nIndex > 0);
	const UnionItem& UnionSetData = queryConfUnion();
	CHECK_RETURN_VOID(nExpiditionTimes < UnionSetData.nExpiditionTimes);
	CUnion *pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
	CHECK_RETURN_VOID(pUnion != NULL);
	CUnionExpiditionModel *pUnionExpiditionModel = pUnion->getExpiditionModel();
	CHECK_RETURN_VOID(pUnionExpiditionModel != NULL);
	CExpiditonRankModel *pUnionExpiditionRankModel = pUnion->getExpiditionRankModel();
	CHECK_RETURN_VOID(pUnionExpiditionRankModel != NULL);
	CUnionShopModel *pUnionShopModel = pUnion->getUnionShopModel();
	CHECK_RETURN_VOID(pUnionShopModel != NULL);
	// ��鴦��Ӷ��
	if (pFinishCS->mercenaryId != 0)
	{
        CHECK_RETURN_VOID(CModelHelper::canMercenaryUse(uid, pFinishCS->mercenaryId));
        CModelHelper::addMercenaryUseList(uid, pFinishCS->mercenaryId);
	}

	int nPassFlag = 0;
	pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_FINISHFLAG, nPassFlag);
	bool bFinishFlag = CUnionHelper::finishExpiditionWar(pUnionExpiditionModel);
	//�ж��Ƿ�ʱ�䵽/�Ƿ�ͨ��
	if (nPassFlag == 1 || bFinishFlag)
	{
		//ͨ��
		if (nPassFlag)
		{
			//�˺����а�����ͬ��
			SDamageRankShowData Data;
			pUnionExpiditionRankModel->getDamageData(uid, Data);
			SExpiditionRankDBData DBData;
			int nTotalDamage = pFinishCS->nDamage + Data.nDamage;
			DBData.nSummerID = pFinishCS->nSummerID;
			memcpy(DBData.nHeroID, pFinishCS->nHeroID, sizeof(DBData.nHeroID));
			memcpy(DBData.nStartID, pFinishCS->nHeroStart, sizeof(DBData.nStartID));
			memcpy(DBData.szName, pUserModel->GetName().c_str(), pUserModel->GetName().length() + 1);
			int nValue = 0;
			pUserModel->GetUserFieldVal(USR_FD_QQVIPTYPE,nValue);
			DBData.cBDType = nValue;
			pUserModel->GetUserFieldVal(USR_FD_QQVIPLV, nValue);
			DBData.cBDLev = nValue;
			pUnionExpiditionRankModel->addDamageData(uid, nTotalDamage, DBData);
		}

		UnionExpiditionStageFinishSC sFinishSC;
		sFinishSC.nIndex = nIndex;
		CKxCommManager::getInstance()->sendData(uid, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_STAGEFINISH_SC,
			reinterpret_cast<char *>(&sFinishSC), sizeof(UnionExpiditionStageFinishSC));
		CHECK_RETURN_VOID(pUserUnionModel->setUserUnionValue(USER_UNION_INDEX, 0));
		CHECK_RETURN_VOID(pUserUnionModel->setUserUnionValue(USER_UNION_EXPIDITION_TIME, nExpiditionTimes + 1));
		//CCommOssHelper::UserSpStageOss(uid, pMapData->indexData[nIndex - 1].nStageID, UNIONEXPIDITION_TYPE, true);
		return;
	}

	int nCurMapID = 0;
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_MAPID, nCurMapID));
	SExpeditionMapData *pMapData = queryConfExpiditionMapData(nCurMapID);
	CHECK_RETURN_VOID(pMapData != NULL);
	//�ؿ��ǲ������չؿ�
	bool bLastStage = false;
	unsigned char  cPass = 0;
	if (nIndex == pMapData->nTotal)
	{
		bLastStage = true;
	}

	//�ж��ùؿ��Ƿ�ͨ��
	set<int>& SetIndex = pUnionExpiditionModel->getAllFinishIndex();
	set<int>::iterator iter = SetIndex.find(nIndex);
	if (iter == SetIndex.end())
	{
		//����BOSSʣ��Ѫ��
		int nBossHp;
		if (!pUnionExpiditionModel->getCurExpiditionBossHp(nIndex, nBossHp))
		{
			//û�б�ʾ��BOSSû�б��˺���
			nBossHp = pMapData->stageList[nIndex - 1].nBossHp;
		}

		SUnionStageShowData ShowData;
		bool bAdd = false;
		if (!pUnionExpiditionModel->getStageIndexShowData(nIndex, ShowData))
		{
			bAdd = true;
		}
		else
		{
			if (ShowData.nDamage < pFinishCS->nDamage)
			{
				bAdd = true;
			}
		}

		if (bAdd)
		{
			memset(&ShowData, 0, sizeof(ShowData));
			int nValue = 0;
			pUserModel->GetUserFieldVal(USR_FD_USERLV, nValue);
			ShowData.cLevel = nValue;
			pUserModel->GetUserFieldVal(USR_FD_HEADICON, nValue);
			ShowData.nHeadID = nValue;
			ShowData.nDamage = pFinishCS->nDamage;
			memcpy(ShowData.szName, pUserModel->GetName().c_str(), pUserModel->GetName().length() + 1);
			pUnionExpiditionModel->setStageIndexShowData(nIndex, ShowData);
		}
		
		nBossHp = nBossHp - pFinishCS->nDamage;
		if (nBossHp <= 0)
		{
			cPass = 1;
			if (!bLastStage)
			{
				CHECK_RETURN_VOID(pUnionExpiditionModel->clearCurExpiditionBossHp(nIndex));
				CHECK_RETURN_VOID(pUnionExpiditionModel->addExpiditionFinishIndex(nIndex));
			}
			else
			{
				//���йؿ�ȫ��ͨ��
				pUnion->setRewardMapID(nCurMapID);
				CUnionHelper::finishExpiditionWar(pUnionExpiditionModel, true);
				//���ö�ʱ����10���Ӻ������������
				KxTimerManager *pTimerManager = CUnionServer::getInstance()->getTimerManager();
				int nReardTimes = time(NULL) + UnionSetData.nRewardSendTime;
				CHECK_RETURN_VOID(pTimerManager->addTimer(pUnion, UnionSetData.nRewardSendTime));
				CHECK_RETURN_VOID(pUnionExpiditionModel->setExpiditinoFieldVale(UNION_EXPIDITION_REWARDSENDTIME,nReardTimes));
                // �������
                CUnionHelper::addUnionReputation(nUnionID, pMapData->nUnionPrestige);
				
				pUnionShopModel->clearUnionShop();
			}
			
		}
		else
		{
			CHECK_RETURN_VOID(pUnionExpiditionModel->setCurExpiditionBossHp(nIndex, nBossHp));
		}
	}

	//�˺����а�����ͬ��
	SDamageRankShowData Data;
	pUnionExpiditionRankModel->getDamageData(uid, Data);
	SExpiditionRankDBData DBData;
	int nTotalDamage = pFinishCS->nDamage + Data.nDamage;
	DBData.nSummerID = pFinishCS->nSummerID;
	memcpy(DBData.nHeroID, pFinishCS->nHeroID, sizeof(DBData.nHeroID));
	memcpy(DBData.nStartID, pFinishCS->nHeroStart, sizeof(DBData.nStartID));
	memcpy(DBData.szName, pUserModel->GetName().c_str(), pUserModel->GetName().length()+1);
	int nValue = 0;
	pUserModel->GetUserFieldVal(USR_FD_QQVIPTYPE, nValue);
	DBData.cBDType = nValue;
	pUserModel->GetUserFieldVal(USR_FD_QQVIPLV, nValue);
	DBData.cBDLev = nValue;
	pUnionExpiditionRankModel->addDamageData(uid, nTotalDamage, DBData);

	CHECK_RETURN_VOID(pUserUnionModel->setUserUnionValue(USER_UNION_INDEX,0));
	CHECK_RETURN_VOID(pUserUnionModel->setUserUnionValue(USER_UNION_EXPIDITION_TIME,nExpiditionTimes+1));
	UnionExpiditionStageFinishSC sFinishSC;
	sFinishSC.nIndex = nIndex;

	if (cPass == 1)
	{
		CUnionModel *pUnionModel = pUnion->getUnionModel();
		map<int, int>& MapMember = pUnionModel->GetMembers();
		for (map<int, int>::iterator ator = MapMember.begin(); ator != MapMember.end(); ++ator)
		{
			CKxCommManager::getInstance()->sendData(ator->first, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_INDEX_PASS_SC,
				reinterpret_cast<char *>(&sFinishSC), sizeof(UnionExpiditionStageFinishSC));
		}
	}

	CKxCommManager::getInstance()->sendData(uid, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_STAGEFINISH_SC,
		reinterpret_cast<char *>(&sFinishSC), sizeof(UnionExpiditionStageFinishSC));
	CCommOssHelper::UserSpStageOss(uid, pMapData->stageList[nIndex - 1].nStageID, UNIONEXPIDITION_TYPE, true);

}
//�������˺����а�
void CUnionExpiditionService::processExpiditionDamageRank(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(NULL != pGameUser);
	CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);

	int nUnionID = 0;
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));
	CUnion *pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
	CHECK_RETURN_VOID(pUnion != NULL);
	CExpiditonRankModel *pExpiditionRankModel = pUnion->getExpiditionRankModel();
	CHECK_RETURN_VOID(pExpiditionRankModel != NULL);
	int nCurMapID = pExpiditionRankModel->getMapID();
	int nCurTime = time(NULL);

	if (m_RankFreshTime == 0 || nCurTime > m_RankFreshTime)
	{
		pExpiditionRankModel->Refresh();
		m_RankFreshTime = nCurTime + 300;
	}
	
	map<int, SDamageRankShowData>& ShowMap = pExpiditionRankModel->getShowDamageRankData();
	map<int, SDamageRankShowData>::iterator ator;
	int nSendLength = sizeof(UnionExpiditionDamageRankSC)+(ShowMap.size() + 1) * sizeof(SDamageRankShowData); // ���㷢�͵���󳤶�
	char *pSendData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLength));
	int nOffset = 0;
	UnionExpiditionDamageRankSC *pDamageRankSC = reinterpret_cast<UnionExpiditionDamageRankSC *>(pSendData);
	pDamageRankSC->nMapID = nCurMapID;
	pDamageRankSC->nNum = ShowMap.size();
	pDamageRankSC->nMyIndex = 0;
	nOffset += sizeof(UnionExpiditionDamageRankSC);

	SDamageSendRankShowData *pRankShowData = reinterpret_cast<SDamageSendRankShowData *>(pDamageRankSC + 1);
	for (ator = ShowMap.begin(); ator != ShowMap.end(); ++ator)
	{
		memcpy(pRankShowData, &ator->second, sizeof(SDamageSendRankShowData));
		pRankShowData++;
		nOffset += sizeof(SDamageSendRankShowData);
	}

	ator = ShowMap.find(uid);
	if (ator == ShowMap.end())
	{
		SDamageRankShowData ShowData;
		if (pExpiditionRankModel->getDamageData(uid, ShowData))
		{
			pDamageRankSC->nNum = ShowMap.size() + 1;
			memcpy(pRankShowData, &ShowData, sizeof(SDamageSendRankShowData));
			nOffset += sizeof(SDamageSendRankShowData);
			pDamageRankSC->nMyIndex = ShowData.nIndex;
		}
	}
	else
	{
		pDamageRankSC->nMyIndex = ator->second.nIndex;
	}

	CKxCommManager::getInstance()->sendData(uid, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_DAMAGERANK_SC,
		pSendData,nOffset);
	KxServer::kxMemMgrRecycle(pSendData, nSendLength);
}

//������Զ���ؿ���Ϣ
void CUnionExpiditionService::processExpiditionStageInfo(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(buffer != NULL && len == sizeof(UnionExpiditionStageInfoCS));
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(NULL != pGameUser);
	CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);

	int nUnionID = 0;
	CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));
	CUnion *pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
	CHECK_RETURN_VOID(pUnion != NULL);
	CUnionExpiditionModel *pUnionExpiditionModel = pUnion->getExpiditionModel();
	CHECK_RETURN_VOID(pUnionExpiditionModel != NULL);
	int nCurMapID = 0;
	CHECK_RETURN_VOID(pUnionExpiditionModel->getExpiditionFieldValue(UNION_EXPIDITION_MAPID, nCurMapID));
	SExpeditionMapData *pMapData = queryConfExpiditionMapData(nCurMapID);
	CHECK_RETURN_VOID(pMapData != NULL);
	UnionExpiditionStageInfoCS *pMsg = reinterpret_cast<UnionExpiditionStageInfoCS*>(buffer);
	CHECK_RETURN_VOID(pMsg->nIndex <= MAX_MAP_INDEX);
	if (CUnionHelper::finishExpiditionWar(pUnionExpiditionModel))
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_UNION_EXPIDITION_OVER;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
	}

	set<int>& SetIndex = pUnionExpiditionModel->getAllFinishIndex();
	set<int>::iterator iter = SetIndex.find(pMsg->nIndex);

	UnionExpiditionStageInfoSC StageInfoSC;
	memset(&StageInfoSC, 0, sizeof(StageInfoSC));
	if (!pUnionExpiditionModel->getCurExpiditionBossHp(pMsg->nIndex, StageInfoSC.nBossHp))
	{
		if (iter == SetIndex.end())
		{
            // �ؿ��ж�,���ñ����15��
            int nIndex = pMsg->nIndex - 1;
            CHECK_RETURN_VOID(nIndex >= 0 && nIndex < MAX_MAP_INDEX);
            StageInfoSC.nBossHp = pMapData->stageList[nIndex].nBossHp;
		}
		else
		{
			StageInfoSC.nBossHp = 0;
		}
	}

	SUnionStageShowData ShowData;
	pUnionExpiditionModel->getStageIndexShowData(pMsg->nIndex, ShowData);
	StageInfoSC.cLevel = ShowData.cLevel;
	StageInfoSC.nDamage = ShowData.nDamage;
	StageInfoSC.nHeadID = ShowData.nHeadID;
	memcpy(StageInfoSC.szName, ShowData.szName, sizeof(ShowData.szName));

	CKxCommManager::getInstance()->sendData(uid, CMD_UNIONEXPIDITION, CMD_UNIONEXPIDITION_STAGEINFO_SC,(char*)&StageInfoSC,sizeof(StageInfoSC));
}

