#include "HeroTrialService.h"
#include "HeroTestProtocol.h"
#include "StageProtocol.h"
#include "Protocol.h"

#include "GameDef.h"
#include "ConfHall.h"
#include "ConfGameSetting.h"
#include "ConfStage.h"
#include "CommStructs.h"
#include "GameUserManager.h"
#include "ModelHelper.h"
#include "CommOssHelper.h"
#include "ChallengeHelper.h"
#include "PropUseHelper.h"
#include "CommonHelper.h"
#include "KxCommManager.h"
#include "BattleDataHelper.h"

void CHeroTrialService::ProcessService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
	case CMD_HEROTEST_CHALLENGE_CS:
		ProcessChanllege(uid, buffer, len, commun);
	    break;
    case CMD_HEROTEST_FINISH_CS:
    {
        CCommonHelper::encryptProtocolBuff(maincmd, subcmd, buffer, len);
		ProcessFinish(uid, buffer, len, commun);
    }
	    break;
	default:
		break;
	}
}

void CHeroTrialService::ProcessChanllege(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len >= sizeof(HeroTestChallengeCS));
	HeroTestChallengeCS * pHeroTestCS = reinterpret_cast<HeroTestChallengeCS *>(buffer);
	int instanceId = pHeroTestCS->instanceId;
	int diff = pHeroTestCS->diff - 1;
	
    // �ܷ���ս
    CHECK_RETURN_VOID(CChallengeHelper::canChallengeHeroTest(uid, instanceId, diff));
    const HeroTestItem* pHeroTestConf = queryConfHeroTest(instanceId);
    if (NULL == pHeroTestConf)
    {
        KXLOGERROR("user %d instanceId %d NULL == pHeroTestConf", uid, instanceId);
        return;
    }

	// ������Ϣ
	ChallengeTeamInfo *pTeamInfo = reinterpret_cast<ChallengeTeamInfo*>(buffer + sizeof(HeroTestChallengeCS));

    // ���췿������
    BattleRoomData room;
    room.battleType = EBATTLE_CHAPTER;
    room.stageId = pHeroTestConf->Diff[diff].DiffID;
    room.stageLv = CChallengeHelper::getHeroTestStageLevel(uid, instanceId, diff);
    room.summonerId = pTeamInfo->summonerId;
    room.outerBuffs.push_back(pHeroTestConf->Occupation);
    for (int i = 0; i < 7; ++i)
    {
        if (pTeamInfo->heroIds[i] > 0)
        {
            room.heroIds.push_back(pTeamInfo->heroIds[i]);
        }
    }
	room.mecenaryId = pTeamInfo->mercenaryId;

    // ���ط������ݰ�
    CBufferData bufferData;
    bufferData.init(10240);
    if (!CBattleDataHelper::roomDataToBuffer(uid, room, bufferData))
    {
        return;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_HEROTEST, CMD_HEROTEST_CHALLENGE_SC,
        bufferData.getBuffer(), bufferData.getDataLength());
}

void CHeroTrialService::ProcessFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len >= sizeof(HeroTestFinishCS));
	HeroTestFinishCS *pHeroTestCS = reinterpret_cast<HeroTestFinishCS *>(buffer);
	int instanceId = pHeroTestCS->instanceId;
	// ǰ��lua table�Ǵ�1��ʼ, �����һ���Ѷ�, ����0��ʼ
	int diff = pHeroTestCS->diff - 1;

	//�ٴ���֤�Ƿ������ս
    CHECK_RETURN_VOID(CChallengeHelper::canChallengeHeroTest(uid, instanceId, diff));

	if (pHeroTestCS->result == CRESULT_LOSE)
	{
		HeroTestFinishSC sFinishSC;
		sFinishSC.instanceId = instanceId;
		sFinishSC.diff = pHeroTestCS->diff;
		sFinishSC.result = pHeroTestCS->result;

		CKxCommManager::getInstance()->sendData(uid, CMD_HEROTEST, CMD_HEROTEST_FINISH_SC,
			reinterpret_cast<char*>(&sFinishSC), sizeof(sFinishSC));
	}
	else if (pHeroTestCS->result == CRESULT_WIN)
	{
		// ս����Ϣ
		ChallengeBattleInfo *pBattleInfo = reinterpret_cast<ChallengeBattleInfo *>(pHeroTestCS + 1);
		const HeroTestItem* pHeroTestConf = queryConfHeroTest(instanceId);
		if (NULL == pHeroTestConf)
		{
			KXLOGERROR("%s user %d instanceId %d NULL == pHeroTestConf", __FUNCTION__, uid, instanceId);
			return;
		}
		
		int stageId = pHeroTestConf->Diff[diff].DiffID;
		const StageConfItem *pStageConf = queryConfStage(stageId);
		if (NULL == pStageConf)
		{
			KXLOGERROR("%s user %d NULL == pStageConf", __FUNCTION__, uid);
			return;
		}
		// ��鴦��Ӷ��
		if (pBattleInfo->mercenaryId != 0 &&
			CModelHelper::canMercenaryUse(uid, pBattleInfo->mercenaryId))
		{
			CModelHelper::addMercenaryUseList(uid, pBattleInfo->mercenaryId);
		}

		//��������
		std::vector<int> starReason;
		//���������Ʒ
		std::vector<DropItemInfo> dropItems;
		//������Ϣ
		int star = CChallengeHelper::getStageStar(uid, stageId, *pBattleInfo, starReason);
		//����id
		if (star > 0 && (int)pStageConf->ItemDrop.size() >= star)
		{
			//������Ʒ
            CItemDrop::Drop(pStageConf->ItemDrop[star - 1], dropItems);
		}

		CPropUseHelper::getInstance()->AddItems(uid, dropItems);

		int nSendLen = sizeof(HeroTestFinishSC)
			+sizeof(StageReward)
			+ sizeof(DropItemInfo)*dropItems.size();

		//�����ڴ�
		char *pSendBuffer = reinterpret_cast<char*>(KxServer::kxMemMgrAlocate(nSendLen));
		HeroTestFinishSC *pFinishSC = reinterpret_cast<HeroTestFinishSC *>(pSendBuffer);
		pFinishSC->instanceId = instanceId;
		pFinishSC->diff = pHeroTestCS->diff; 
		pFinishSC->result = CRESULT_WIN;

		StageReward *pStageReward = reinterpret_cast<StageReward*>(pFinishSC + 1);
		pStageReward->star = star;
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
		// ��¼��ս����
		CHeroTestModel *pHeroTestModel = dynamic_cast<CHeroTestModel*>(CModelHelper::getModel(uid, MODELTYPE_HEROTEST));
		if (NULL != pHeroTestModel)
		{
			pHeroTestModel->AddChallengeCount(instanceId, 1);
		}
		//����
		CKxCommManager::getInstance()->sendData(uid, CMD_HEROTEST, CMD_HEROTEST_FINISH_SC, pSendBuffer, nSendLen);
		//�ڴ����
		KxServer::kxMemMgrRecycle(pSendBuffer, nSendLen);

		CModelHelper::DispatchActionEvent(uid, ELA_HERO_TEST, &pHeroTestCS->diff, sizeof(int));
	}
}
