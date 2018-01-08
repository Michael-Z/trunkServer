#include "ChapterService.h"
#include "CommStructs.h"
#include "Protocol.h"
#include "StageProtocol.h"
#include "GameUserManager.h"
#include "ChallengeRoom.h"
#include "ChallengeRoomManager.h"
#include "GameDef.h"
#include "ConfHall.h"
#include "ConfStage.h"
#include "ConfGameSetting.h"
#include "KxCommManager.h"

#include "PropUseHelper.h"
#include "ModelHelper.h"
#include "PropUseHelper.h"
#include "CommonHelper.h"
#include "CommOssHelper.h"
#include "ChallengeHelper.h"
#include "TimeCalcTool.h"
#include "KxMemPool.h"
#include "ErrorCodeProtocol.h"
#include "BattleDataHelper.h"

using namespace KxServer;

void CChapterService::processService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
	case CMD_STAGE_CHALLENGE_CS:
		processChallenge(uid, buffer, len, commun);
		break;
    case CMD_STAGE_FINISH_CS:
    {
        CCommonHelper::encryptProtocolBuff(maincmd, subcmd, buffer, len);
		processFinish(uid, buffer, len, commun);
    }
		break;
	case CMD_STAGE_SWEEP_CS:
		processSweep(uid, buffer, len, commun);
		break;
	case CMD_STAGE_STRATEGY_CS:
		processStrategy(uid, buffer, len, commun);
		break;
	case CMD_STAGE_CHAPTERAWARD_CS:
		processChapterReward(uid, buffer, len, commun);
		break;
	case CMD_STAGE_BUYTIMES_CS:
		processBuyTimes(uid, buffer, len, commun);
		break;
	default:
		break;
	}
}

void CChapterService::processChallenge(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(len >= sizeof(StageChallengeCS));
	StageChallengeCS *pChallengeCS = reinterpret_cast<StageChallengeCS*>(buffer);

	const StageConfItem *pStageConf = queryConfStage(pChallengeCS->stageId);
	if (NULL == pStageConf)
	{
		KXLOGDEBUG("%s NULL == pStageConf", __FUNCTION__);
		return;
	}

    CHECK_RETURN_VOID(CChallengeHelper::canChallengeStage(uid, pChallengeCS->chapterId, pChallengeCS->stageId));

	// ��ö�����Ϣ
	ChallengeTeamInfo *pTeamInfo = reinterpret_cast<ChallengeTeamInfo*>(buffer + sizeof(StageChallengeCS));

    // ���췿������
    BattleRoomData room;
    room.battleType = EBATTLE_CHAPTER;
    room.stageId = pChallengeCS->stageId;
    room.stageLv = CChallengeHelper::getChapterStageLevel(pChallengeCS->chapterId, pChallengeCS->stageId);
    room.summonerId = pTeamInfo->summonerId;
    for (int i = 0; i < 7; ++i)
    {
        if (pTeamInfo->heroIds[i] > 0)
        {
            room.heroIds.push_back(pTeamInfo->heroIds[i]);
        }
    }
	room.mecenaryId = pTeamInfo->mercenaryId;

    int result = 1;
    // ���ط������ݰ�
    CBufferData bufferData;
    bufferData.init(10240);
    // �ȷ�ؿ���ս����Ϣ
    bufferData.writeData(result);

    if (!CBattleDataHelper::roomDataToBuffer(uid, room, bufferData))
    {
        return;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_CHALLENGE_SC,
        bufferData.getBuffer(), bufferData.getDataLength());
}

void CChapterService::processFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(buffer != NULL &&(len == sizeof(StageFinishCS)+sizeof(ChallengeBattleInfo)))

	StageFinishCS *pFinishCS = reinterpret_cast<StageFinishCS*>(buffer);
	int chapterId = pFinishCS->chapterId;
	int stageId = pFinishCS->stageId;
	int result = pFinishCS->result;

	const StageConfItem *pStageConf = queryConfStage(stageId);
	if (NULL == pStageConf)
	{
		KXLOGERROR("%s user %d NULL == pStageConf", __FUNCTION__, uid);
		return;
	}
	//�ٴ���֤�Ƿ������ս
	CHECK_RETURN_VOID(CChallengeHelper::canChallengeStage(uid, chapterId, stageId));

	// ս����Ϣ
	ChallengeBattleInfo *pBattleInfo = reinterpret_cast<ChallengeBattleInfo *>(pFinishCS + 1);
	if (result == CRESULT_LOSE)
	{
		StageFinishSC finishSC;
		finishSC.chapterId = chapterId;
		finishSC.stageId = stageId;
		finishSC.result = CRESULT_LOSE;

		CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_FINISH_SC,
			reinterpret_cast<char*>(&finishSC), sizeof(finishSC));

		SOssPveLogData LogData;
		LogData.nStageID = stageId;
		LogData.bWin = true;
		LogData.UnLockStageID = 0;
		LogData.nCryStalLevel = pBattleInfo->nCrystalLv;
		memcpy(LogData.nHeroID, pBattleInfo->heroIds, sizeof(pBattleInfo->heroIds));
		LogData.nStartNum = 0;
		LogData.nSummonerID = pBattleInfo->summonerId;
		LogData.nTime = pBattleInfo->timeCost;

		CCommOssHelper::StageLogicOss(uid,LogData);
	}
	else if (result == CRESULT_WIN)
	{
		// ��鴦��Ӷ��
		if (pBattleInfo->mercenaryId != 0 &&
			CModelHelper::canMercenaryUse(uid, pBattleInfo->mercenaryId))
		{
			CModelHelper::addMercenaryUseList(uid, pBattleInfo->mercenaryId);
		}

		std::vector<int> starReason;
		// ����id
		int dropId = 0;
		//������Ϣ
		int finalStar = CChallengeHelper::getStageStar(uid, stageId, *pBattleInfo, starReason);
		//���������Ʒ
		std::vector<DropItemInfo> dropItems;
		//�����½�
		std::vector<int> unlockStages;
		//�Ƿ��״�ͨ��
		if (CModelHelper::isFirstChllange(uid, chapterId, stageId) && pStageConf->FirstItemDrop > 0)
		{
			dropId = pStageConf->FirstItemDrop;
		}
		else
		{
			dropId = pStageConf->ItemDrop[finalStar - 1];
		}
		//������Ʒ
		CItemDrop::Drop(dropId, dropItems);
		//ֻ����һ��, ���ɶ�Ķ���
		CPropUseHelper::getInstance()->AddItems(uid, dropItems, true, WORDSTAGE_GAIN_REASON);
		// �����ؿ�, �Ѿ�������
		CModelHelper::finishChapterStage(uid, chapterId, stageId, STAGESTATUS_UNLOCK + finalStar, unlockStages);
		//�����ڴ�
		int nSendLen = sizeof(StageFinishSC) + sizeof(StageReward) + sizeof(DropItemInfo)*dropItems.size();	
		char *pSendBuffer = reinterpret_cast<char*>(KxServer::kxMemMgrAlocate(nSendLen));
		// �ؿ�������Ϣ
		StageFinishSC *pFinishSC = reinterpret_cast<StageFinishSC *>(pSendBuffer);
		pFinishSC->chapterId = chapterId;
		pFinishSC->stageId = stageId;
		pFinishSC->result = CRESULT_WIN;

		StageReward *pStageReward = reinterpret_cast<StageReward*>(pFinishSC + 1);
		pStageReward->star = finalStar;
		pStageReward->star2Reason = starReason[0];
		pStageReward->star3Reason = starReason[1];
		pStageReward->rewardCount = dropItems.size();
		//�������Ʒ��Ϣ
		DropItemInfo *pPropInfo = reinterpret_cast<DropItemInfo *>(pStageReward + 1);
		for (std::vector<DropItemInfo>::iterator iter = dropItems.begin();
			iter != dropItems.end(); ++iter)
		{
			memcpy(pPropInfo, &(*iter), sizeof(DropItemInfo));
			pPropInfo += 1;
		}
		//����
		CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_FINISH_SC,
			pSendBuffer, nSendLen);
		//�ڴ����
		KxServer::kxMemMgrRecycle(pSendBuffer, nSendLen);
		// ͨ������
		ActionCommonInfo passStage;
		passStage.param1 = stageId;
		passStage.param2 = 1;
		CModelHelper::DispatchActionEvent(uid, ELA_PASS_STAGE, &passStage, sizeof(ActionCommonInfo));
		CModelHelper::DispatchActionEvent(uid, ELA_STAGE_STAR, NULL, 0);
	
		if (unlockStages.size() > 0)
		{
			SOssPveLogData LogData;
			LogData.nStageID = stageId;
			LogData.bWin = true;
			LogData.UnLockStageID = unlockStages[0];
			LogData.nCryStalLevel = pBattleInfo->nCrystalLv;
			memcpy(LogData.nHeroID, pBattleInfo->heroIds, sizeof(pBattleInfo->heroIds));
			LogData.nStartNum = finalStar;
			LogData.nSummonerID = pBattleInfo->summonerId;
			LogData.nTime = pBattleInfo->timeCost;
			CCommOssHelper::StageLogicOss(uid,LogData);
		}
	}
	else
	{
		// ȡ��, ��ʱû���߼�
	}
}

void CChapterService::processSweep(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	//CHECK_RETURN_VOID(buffer != NULL && (len == sizeof(StageSweepCS)));
	//StageSweepCS *pSweepCS = reinterpret_cast<StageSweepCS*>(buffer);
	//int chapterId = pSweepCS->chapterId;
	//int stageId = pSweepCS->stageId;
	//int sweepTimes = pSweepCS->times;

	//const ChapterConfItem *pChapterConf = queryConfChapter(chapterId);
	//const StageConfItem *pStageConf = queryConfStage(stageId);
	//if (NULL == pStageConf || NULL == pChapterConf)
	//{
	//	KXLOGDEBUG("%s user %d NULL == pChapterConf or NULL == pStageConf !", __FUNCTION__, uid, chapterId, stageId);
	//	return;
	//}
	////��֤�Ƿ����ɨ��
	//if (!CChallengeHelper::canChallengeStage(uid, chapterId, stageId, sweepTimes))
	//{
	//	return;
	//}
	//// �Ƿ����ɨ��, ��ѯ���ݿ�ؿ���Ϣ
	//if (CModelHelper::getChapterStageState(uid, chapterId, stageId) < STAGESTATUS_STAR3)
	//{
	//	KXLOGERROR("%s user %d get stage %d state < STAGESTATUS_STAR3 when sweep!", __FUNCTION__, uid, stageId);
	//	return;
	//}

 //	int nSendLen = 0;
 //	char * pSendBuffer = reinterpret_cast<char*>(kxMemMgrAlocate(4096));
 //	StageSweepSC *pSweepSC = reinterpret_cast<StageSweepSC *>(pSendBuffer);
	//pSweepSC->chapterId = pSweepCS->chapterId;
	//pSweepSC->stageId = pSweepCS->stageId;
 //	pSweepSC->times = pSweepCS->times;
 //	nSendLen += sizeof(StageSweepSC);
	////3�ǵ���id
	//int dropId = pStageConf->ItemDrop[2];
	//for (int i = 0; i < pSweepCS->times; ++i)
	//{	
	//	//���������Ʒ
	//	std::vector<DropItemInfo> dropItems;
	//	CItemDrop::Drop(dropId, dropItems);
	//	//ֻ����һ��, ���ɶ�Ķ���
	//	CPropUseHelper::getInstance()->AddItems(uid, dropItems);

	//	StageReward *pStageReward = reinterpret_cast<StageReward*>(pSendBuffer + nSendLen);
	//	pStageReward->star = 3;
	//	pStageReward->rewardCount = dropItems.size();
	//	pStageReward->star2Reason = 0;
	//	pStageReward->star3Reason = 0;

	//	nSendLen += sizeof(StageReward);
	//	//�������Ʒ��Ϣ
	//	DropItemInfo *pPropInfo = reinterpret_cast<DropItemInfo *>(pStageReward + 1);
	//	for (std::vector<DropItemInfo>::iterator iter = dropItems.begin();
	//		iter != dropItems.end(); ++iter)
	//	{
	//		memcpy(pPropInfo, &(*iter), sizeof(DropItemInfo));
	//		nSendLen += sizeof(DropItemInfo);
	//		pPropInfo += 1;
	//	}
	//}
	////��������
	////int needEnergy = CCommonHelper::getStageNeedEnergy(chapterId, stageId);
	////CModelHelper::addEnergy(uid, -1 * needEnergy * sweepTimes);
 //       
	//if (pChapterConf->Type == CHAPTERTYPE_ELITE)
	//{
	//	//��ս����
	//	CModelHelper::addEliteUseTimes(uid, pSweepCS->chapterId, pSweepCS->stageId, pSweepCS->times);
	//}
	//	
	//CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_SWEEP_SC, pSendBuffer, nSendLen);
 //	//�����ڴ��
 //	kxMemMgrRecycle(pSendBuffer, 4096);

	//ActionCommonInfo passStage;
	//passStage.param1 = pSweepCS->stageId;
	//passStage.param2 = pSweepCS->times;
	//CModelHelper::DispatchActionEvent(uid, ELA_PASS_STAGE, &passStage, sizeof(ActionCommonInfo));
	//CCommOssHelper::StageSweepOss(uid, pSweepCS->stageId, pSweepCS->times);
}

void CChapterService::processStrategy(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(len == sizeof(StageStrategyCS))
	StageStrategyCS *pStrategyCS = reinterpret_cast<StageStrategyCS*>(buffer);
	int stageId = pStrategyCS->stageId;
	CGameUser *usrInfo = CGameUserManager::getInstance()->getGameUser(uid);
	CPassTeamModel *pPassTeamModel = CGameUserManager::getInstance()->getPassTeamModel();
	std::map<int, PassTeamData> passTeams;
	// ������ж���
	pPassTeamModel->GetPassTeam(stageId, passTeams);
	// ���ͳ���
	int sendLen = sizeof(StageStrategySC);
	for (std::map<int, PassTeamData>::iterator iter = passTeams.begin();
		iter != passTeams.end(); ++iter)
	{
		sendLen += iter->second.len;
	}

	char *pSendData = reinterpret_cast<char *>(kxMemMgrAlocate(sendLen));
	StageStrategySC *pStrategySC = reinterpret_cast<StageStrategySC *>(pSendData);
	pStrategySC->stageId = stageId;
	pStrategySC->teamCount = passTeams.size();

	char *pPassTeam = reinterpret_cast<char *>(pStrategySC + 1);
	for (std::map<int, PassTeamData>::iterator iter = passTeams.begin();
		iter != passTeams.end(); ++iter)
	{
		memcpy(pPassTeam, iter->second.data, iter->second.len);
		pPassTeam += iter->second.len;
	}

	CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_STRATEGY_SC,
		pSendData, sendLen);
}

void CChapterService::processChapterReward(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(len == sizeof(StageChapterAwardCS));
	StageChapterAwardCS *pChapterReward = reinterpret_cast<StageChapterAwardCS*>(buffer);
	int chapterId = pChapterReward->chapterId;

	CGameUser *pUserInfo = CGameUserManager::getInstance()->getGameUser(uid);
	CStageModel *pStageModel = dynamic_cast<CStageModel*>(pUserInfo->getModel(MODELTYPE_STAGE));
	const ChapterConfItem *pChapterConf = queryConfChapter(chapterId);

    CHECK_RETURN_DEBUG_VOID(NULL != pStageModel && NULL != pChapterConf);

	// �����½�״̬Ϊ����ȡ
	if (pChapterConf->Type == CHAPTERTYPE_ELITE)
	{
		return;
		if (pStageModel->GetEliteChapterStatus(chapterId) == CHAPTERSTATUS_AWARD)
		{
			KXLOGERROR("user=%d elite chapter=%d award was got!", uid, chapterId);
			return;
		}
	}
	else
	{
		if (pChapterReward->cIndex <= 0 || pChapterReward->cIndex > MAX_CHAPTER_BOX_INDEX)
		{
			//�Ƿ���ȡ
			return;
		}

		int nStatus = 0;
		pStageModel->getChapterBoxStatus(chapterId, pChapterReward->cIndex, nStatus);
		if (nStatus == CHAPTERBOX_GET)
		{
			//�Ѿ���ȡ
			return;
		}
	}

	// ���������������, ����н�����ȡ
	int curStar = CModelHelper::getChapterRewardStar(uid, chapterId);
	int nNeedStar = pChapterConf->Reward[pChapterReward->cIndex - 1].Star;

	if (curStar >= nNeedStar)
	{
		std::vector<DropItemInfo> newItems;
		//ͨ���½ڽ����б�, ���ʵ�ʵ���Ʒ
		for (std::vector<AwardInfo>::const_iterator iter = pChapterConf->Reward[pChapterReward->cIndex - 1].StarAward.begin();
			iter != pChapterConf->Reward[pChapterReward->cIndex - 1].StarAward.end(); ++iter)
		{
			DropItemInfo dropItem;
			dropItem.id = iter->ID;
			dropItem.num = iter->Num;
			newItems.push_back(dropItem);
		}

		CPropUseHelper::getInstance()->AddItems(uid, newItems, true, WORDSTAGEBOX_GAIN_REASON);

		int nSendLen = sizeof(StageChapterAwardSC)
			+sizeof(DropItemInfo)* newItems.size();
		char *pSendBuffer = reinterpret_cast<char*>(kxMemMgrAlocate(nSendLen));

		StageChapterAwardSC *pAwardSC = reinterpret_cast<StageChapterAwardSC*>(pSendBuffer);
		pAwardSC->chapterId = chapterId;
		pAwardSC->itemCount = newItems.size();
		// ������Ʒ
		DropItemInfo *pPropItem = reinterpret_cast<DropItemInfo *>(pAwardSC + 1);
		for (std::vector<DropItemInfo>::iterator iter = newItems.begin();
			iter != newItems.end(); ++iter)
		{
			memcpy(pPropItem, &(*iter), sizeof(DropItemInfo));
			pPropItem += 1;
		}

		// �����½�״̬Ϊ����ȡ
		if (pChapterConf->Type == CHAPTERTYPE_ELITE)
		{
			if (!pStageModel->SetEliteChapterStatus(chapterId, CHAPTERSTATUS_AWARD))
			{
				KXLOGERROR("set elite chapter award status error !");
			}
		}
		else
		{
			if (!pStageModel->setChapterBoxStatus(chapterId, pChapterReward->cIndex, CHAPTERBOX_GET))
			{
				KXLOGERROR("set chapter award status error !");
			}
		}
		
		CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_CHAPTERAWARD_SC,
			pSendBuffer, nSendLen);
		kxMemMgrRecycle(pSendBuffer, nSendLen);
	}
	else
	{
		KXLOGERROR("user %d get reward item, but not enough stars !!!", uid);
	}
}

void CChapterService::processBuyTimes(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	//CGameUser *pUserInfo = CGameUserManager::getInstance()->getGameUser(uid);
	//CStageModel *pStageModel = dynamic_cast<CStageModel*>(pUserInfo->getModel(MODELTYPE_STAGE));
	//CUserModel *pUserModel = dynamic_cast<CUserModel*>(pUserInfo->getModel(MODELTYPE_USER));

	//StageBuyTimesCS *pBuyTimesCS = reinterpret_cast<StageBuyTimesCS *>(buffer);
	//int chapterId = pBuyTimesCS->chapterId;
	//int stageId = pBuyTimesCS->stageId;

	//// ������ս������Ϣ
	//CModelHelper::updateEliteInfo(uid, chapterId, stageId);

 //   const ChapterConfItem *pChapterConf = queryConfChapter(chapterId);
 //   CHECK_RETURN_DEBUG_VOID(NULL != pChapterConf);

	//int haveDiamond = 0;
	//int costDiamond = 0;
 //   CHECK_RETURN_DEBUG_VOID(pUserModel->GetUserFieldVal(USR_FD_DIAMOND, haveDiamond));
	//// ��þ�Ӣ�ؿ���Ϣ
	//DBEliteStageInfo info;
 //   CHECK_RETURN_DEBUG_VOID(pStageModel->GetEliteInfo(stageId, info));
	////����������ܹ���,�����������
 //   CHECK_RETURN_DEBUG_VOID(info.challengeTimes >= pChapterConf->ChallengeTimes);
	//const IncreasePayItem *pPayConf = queryConfIncreasePay(info.buyTimes + 1);
	//if (pPayConf == NULL || pPayConf->ChallengeCost == 0)
	//{
	//	//��������������
	//	ErrorCodeData CodeData;
	//	CodeData.nCode = ERROR_ELTLEBUYTIMES_NONENOUGH;
	//	CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
	//	return;
	//}

	////��Ҫ���ѵ���ʯ
	//costDiamond = pPayConf->ChallengeCost;
	//// �ȿ�Ǯ
 //   CHECK_RETURN_DEBUG_VOID(haveDiamond >= costDiamond);
	//CHECK_RETURN_DEBUG_VOID(CModelHelper::addDiamond(uid, -1 * costDiamond));
	//CCommOssHelper::userDiamondPayOss(uid, costDiamond, DIAMONDRESET_ELITE_TYPE, stageId);

	//time_t nowStamp = time(NULL);
 //   info.challengeTimes = 0;
	//if (info.buyTimes == 0)
	//{
	//	info.buyRecoverStamp = CTimeCalcTool::nextTimeStamp(nowStamp, 0, 0);
	//}
	//info.buyTimes += 1;

 //   CHECK_RETURN_DEBUG_VOID(pStageModel->SetEliteInfo(stageId, info));

	//StageBuyTimesSC buyTimeSC;
	//buyTimeSC.chapterId = chapterId;
	//buyTimeSC.stageId = stageId;
 //   buyTimeSC.addTimes = pChapterConf->ChallengeTimes;

	//CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_BUYTIMES_SC,
	//	reinterpret_cast<char *>(&buyTimeSC), sizeof(buyTimeSC));
}

void CChapterService::processBuyChapter(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(buffer != NULL && len >= sizeof(StageBuyChapterCS));

	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	CHECK_RETURN_VOID(pUserModel != NULL);

	StageBuyChapterCS *pChapterCS = reinterpret_cast<StageBuyChapterCS*>(buffer);
	int chapterId = pChapterCS->chapterId;
	const ChapterConfItem *pChapterConf = queryConfChapter(chapterId);
	if (pChapterConf == NULL)
	{
		KXLOGERROR("uid %d unlock not exsit chapterid = %d", uid, chapterId);
		return;
	}
	//ǰһ���Ƿ����
	if (!CModelHelper::isChapterUnlock(uid, pChapterConf->ExInfo.PrevID))
	{
		KXLOGERROR("uid %d prev chapter id=%d not unlock, unlock prev chapter first!", uid, pChapterConf->ExInfo.PrevID);
		return;
	}
	if (CModelHelper::isChapterUnlock(uid, chapterId))
	{
		KXLOGERROR("uid %d chapter id=%d unlock already!", uid, pChapterConf->ExInfo.PrevID);
		return;
	}
	//��ʯ�Ƿ��㹻
	int haveDiamond = 0;
	if (!pUserModel->GetUserFieldVal(USR_FD_DIAMOND, haveDiamond))
	{
		KXLOGERROR("uid %d get user field error when unlock chapter!", uid, haveDiamond);
		return;
	}
	if (haveDiamond < pChapterConf->UnlockDiamond)
	{
		KXLOGERROR("uid %d unlock chapter but diamond not enough!", uid, haveDiamond);
		return;
	}
	//�����ؿ�
	std::vector<int> unlockChapter;
	unlockChapter.push_back(chapterId);
	CModelHelper::unlockChapter(uid, unlockChapter);
	//�۳���ʯ
	if (!CModelHelper::addDiamond(uid, -1 * pChapterConf->UnlockDiamond))
	{
		KXLOGERROR("uid %d cost diamond error when unlock chapter!", uid);
		return;
	}

	StageBuyChapterSC chapterSC;
	chapterSC.chapterId = chapterId;
	chapterSC.diamond = pChapterConf->UnlockDiamond;
	
	CKxCommManager::getInstance()->sendData(uid, CMD_STAGE, CMD_STAGE_BUYCHPATER_CS,
		reinterpret_cast<char*>(&chapterSC), sizeof(chapterSC));
}
