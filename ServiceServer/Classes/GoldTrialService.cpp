#include "GoldTrialService.h"
#include "GameDef.h"
#include "CommOssHelper.h"
#include "ChallengeHelper.h"
#include "ModelHelper.h"
#include "KxCommManager.h"
#include "GoldTestProtocol.h"
#include "ErrorCodeProtocol.h"
#include "Protocol.h"
#include "ConfStage.h"
#include "GameUserManager.h"
#include "CommonHelper.h"
#include "BattleDataHelper.h"

void CGoldTrialService::ProcessService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
	case CMD_GOLDTEST_CHALLENGE_CS:
		ProcessChallenge(uid, buffer, len, commun);
		break;
    case CMD_GOLDTEST_FINISH_CS:
    {
        CCommonHelper::encryptProtocolBuff(maincmd, subcmd, buffer, len);
		ProcessFinish(uid, buffer, len, commun);
    }
		break;
	case CMD_GOLDTEST_REWARDCHEST_CS:
		ProcessRewardChest(uid, buffer, len, commun);
		break;
	default:
		break;
	}
}

void CGoldTrialService::ProcessChallenge(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len >= sizeof(ChallengeTeamInfo));
	int stageId = 0, stageLv = 0;
	if (!CChallengeHelper::canChallengeGoldTest(uid, stageId, stageLv))
    {
        return;
    }

	ChallengeTeamInfo *pTeamInfo = reinterpret_cast<ChallengeTeamInfo*>(buffer);

    // ���췿������
    BattleRoomData room;
    room.battleType = EBATTLE_GOLDTEST;
    room.stageId = stageId;
    room.stageLv = stageLv;
    room.summonerId = pTeamInfo->summonerId;
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

    CKxCommManager::getInstance()->sendData(uid, CMD_GOLDTEST, CMD_GOLDTEST_CHALLENGE_SC,
        bufferData.getBuffer(), bufferData.getDataLength());
}

void CGoldTrialService::ProcessFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(GoldTestFinishCS)+sizeof(ChallengeTeamInfo));
	GoldTestFinishCS *pFinishCS = reinterpret_cast<GoldTestFinishCS*>(buffer);
    ChallengeTeamInfo *pBattleInfo = reinterpret_cast<ChallengeTeamInfo*>(pFinishCS + 1);
	// Ӣ��ģ��
	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
	CHECK_RETURN_VOID(NULL != pHeroModel);
	// ���ڼ�
	time_t nowStamp = time(NULL);
	tm *pCurTm = localtime(&nowStamp);
	int wday = pCurTm->tm_wday == 0 ? 7 : pCurTm->tm_wday;
	const GoldTestConfItem *pGoldTestConf = queryConfGoldTest(wday);
	if (NULL == pGoldTestConf)
	{
		KXLOGERROR("user %d config is NULL!!", uid);
		return;
    }

    CHECK_RETURN_VOID(pFinishCS->damage >= 0);

	// ��鴦��Ӷ��
	if (pBattleInfo->mercenaryId != 0 &&
		CModelHelper::canMercenaryUse(uid, pBattleInfo->mercenaryId))
	{
		CModelHelper::addMercenaryUseList(uid, pBattleInfo->mercenaryId);
	}
    
    int nTotalLevel = 0;
    for (int i = 0; i < 7; ++i)
    {
        int heroId = pBattleInfo->heroIds[i];
        DBHeroAttr dbHeroInfo;
        if (heroId != 0 && pHeroModel->GetHero(heroId, dbHeroInfo))
        {
            nTotalLevel += dbHeroInfo.level;
        }
    }

    // �����˺��͵ȼ�����
    int nDamageReward = 0, nLevelReward = 0;
    if (pGoldTestConf->Param.size() >= 2)
    {
        nDamageReward = static_cast<int>(pFinishCS->damage * pGoldTestConf->Param[0]);
        nLevelReward = static_cast<int>(nTotalLevel * pGoldTestConf->Param[1]);
    }

    GoldTestFinishSC sFinishSC;
    sFinishSC.damage = pFinishCS->damage;
    sFinishSC.damageReward = nDamageReward;
    sFinishSC.levelReward = nLevelReward;

    // �����ս�˺��ʹ���
    CModelHelper::setGoldTestData(uid, pFinishCS->damage);
    // ��ӽ��
    CModelHelper::addGold(uid, sFinishSC.damageReward + sFinishSC.levelReward);
    // ��ɽ������
    CModelHelper::DispatchActionEvent(uid, ELA_GOLD_TEST, NULL, 0);

    CKxCommManager::getInstance()->sendData(uid, CMD_GOLDTEST, CMD_GOLDTEST_FINISH_SC,
        reinterpret_cast<char *>(&sFinishSC), sizeof(GoldTestFinishSC));

    CCommOssHelper::UserSpStageOss(uid, pGoldTestConf->Stage, GOLD_CHALLENGLE_TYPE, true);
}

void CGoldTrialService::ProcessRewardChest(int uid, char * buffer, int len, KxServer::IKxComm * commun)
{
    CHECK_RETURN_VOID(len == sizeof(GoldTestRewardChestCS));

	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
    CHECK_RETURN_VOID(NULL != pGameUser);
	
	CGoldTestModel *pGoldTestModel = dynamic_cast<CGoldTestModel*>(pGameUser->getModel(MODELTYPE_GOLDTEST));
    CHECK_RETURN_VOID(NULL != pGoldTestModel);

	GoldTestRewardChestCS *pGetChestCS = reinterpret_cast<GoldTestRewardChestCS *>(buffer);
	// �����������
	int goldReward = 0;		
	// ��ȡ�ı���
	std::vector<int> chests;
	// һ����ȡ
    if (0 == pGetChestCS->chestFlag) 
    {
        // ��ȡ����ȡ�ı���
        CConfGoldTestChest *conf = dynamic_cast<CConfGoldTestChest *>(
			CConfManager::getInstance()->getConf(CONF_GOLD_TEST_CHEST));
        CHECK_RETURN_VOID(NULL != conf);

        std::map<int, void *> &mapItem = conf->getDatas();

		for (std::map<int, void *>::iterator iter = mapItem.begin();
			iter != mapItem.end(); ++iter)
        {
			GoldTestChestConfItem *pChestConf = static_cast<GoldTestChestConfItem*>(iter->second);
			// �Ƿ������ȡ
            if (CChallengeHelper::isEnoughGoldTestChest(uid, iter->first))
            {
                goldReward += pChestConf->Gold;
				chests.push_back(iter->first);
            }
        }
    }
    else
    {
		// �Ƿ������ȡ
		CHECK_RETURN_VOID(CChallengeHelper::isEnoughGoldTestChest(uid, pGetChestCS->chestFlag));

        const GoldTestChestConfItem *pGoldTestChestConfItem = queryConfGoldTestChest(pGetChestCS->chestFlag);
        CHECK_RETURN_VOID(NULL != pGoldTestChestConfItem);

        goldReward = pGoldTestChestConfItem->Gold;
		chests.push_back(pGetChestCS->chestFlag);
    }
	
    // ��ӽ��
    CModelHelper::addGold(uid, goldReward);
    // ������ȡ״̬
	int chestFlag = CModelHelper::markGoldTestChest(uid, chests);

	GoldTestReardChestSC sRewardChest;
	sRewardChest.chestFlag = chestFlag;
	sRewardChest.goldReward = goldReward;

	CKxCommManager::getInstance()->sendData(uid, CMD_GOLDTEST, CMD_GOLDTEST_REWARDCHEST_SC, 
		reinterpret_cast<char *>(&sRewardChest), sizeof(sRewardChest));
}
