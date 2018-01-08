#include "TowerTrialService.h"
#include "Protocol.h"
#include "TowerTestProtocol.h"
#include "ErrorCodeProtocol.h"

#include "CommStructs.h"
#include "GameDef.h"
#include "ItemDrop.h"
#include "GameUserManager.h"
#include "KxCommManager.h"
#include "ChallengeRoomManager.h"
#include "ConfHall.h"
#include "ConfGameSetting.h"

#include "CommOssHelper.h"
#include "ModelHelper.h"
#include "PropUseHelper.h"
#include "ChallengeHelper.h"
#include "RankModel.h"
#include "CommonHelper.h"
#include "BattleDataHelper.h"
#include "RandGenerator.h"

void CTowerTrialService::processService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
	case CMD_TOWER_FIGHTING_CS:
		processFighting(uid, buffer, len, commun);
		break;
    case CMD_TOWER_FINISH_CS:
    {
        CCommonHelper::encryptProtocolBuff(maincmd, subcmd, buffer, len);
		processFinish(uid, buffer, len, commun);
    }
		break;
	case CMD_TOWER_CHOSEOUTERBONUS_CS:
		processChoseOuterBonus(uid, buffer, len, commun);
		break;
	case CMD_TOWER_OPENTREASURE_CS:
		//����򿪱��䴦��, ��ʱ������
		//processOpenTreasure(uid, buffer, len, commun);
		break;
	case CMD_TOWER_ONEKEYFIGHTING_CS:
		//processOneKeyFighting(uid, buffer, len, commun);
		break;
	default:
		break;
	}
}

void CTowerTrialService::processFighting(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(len >= sizeof(ChallengeTeamInfo));
	
	CTowerTestModel *pTowerTestModel = dynamic_cast<CTowerTestModel*>(CModelHelper::getModel(uid, MODELTYPE_TOWERTEST));
    CHECK_RETURN_VOID(NULL != pTowerTestModel);

    int floor = pTowerTestModel->GetTowerTestField(TOWER_FD_FLOOR);
    int haveCrystal = pTowerTestModel->GetTowerTestField(TOWER_FD_CRYSTAL);

    CHECK_RETURN_VOID(CChallengeHelper::canChallengeTowerTest(uid, floor));

    const TowerFloorItem *pTowerFloorConf = queryConfTowerFloor(floor);
    if (NULL == pTowerFloorConf)
    {
        KXLOGDEBUG("%s user %d get tower config is NULL, floor %d!", __FUNCTION__, uid, floor);
        return;
    }

	// ��ö�����Ϣ
	ChallengeTeamInfo *pTeamInfo = reinterpret_cast<ChallengeTeamInfo*>(buffer);

	int nIndex = g_RandGenerator.MakeRandNum(1, pTowerFloorConf->StageID.size());
	int nStageID = pTowerFloorConf->StageID[nIndex - 1];
	pTowerTestModel->SetTowerTestField(TOWER_FD_STAGEID, nStageID);

    // ���췿������
    BattleRoomData room;
    room.battleType = EBATTLE_TOWERTEST;
	room.stageId = nStageID;
    room.stageLv = CChallengeHelper::getTowerTestStageLevel(uid, floor);
    room.summonerId = pTeamInfo->summonerId;
    room.ext1 = haveCrystal;    // ʣ��ˮ������
    room.outerBuffs = pTowerTestModel->GetOuterBonusList();
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
    // �ȷ�������ص�����
    bufferData.writeData(floor);

    if (!CBattleDataHelper::roomDataToBuffer(uid, room, bufferData))
    {
        return;
    }

	CKxCommManager::getInstance()->sendData(uid, CMD_TOWERTEST, CMD_TOWER_FIGHTING_SC, 
        bufferData.getBuffer(), bufferData.getDataLength());
}

void CTowerTrialService::processFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len >= sizeof(TowerFinishCS));

	TowerFinishCS *pFinishCS = reinterpret_cast<TowerFinishCS*>(buffer);

	CTowerTestModel *pTowerTestModel = dynamic_cast<CTowerTestModel*>(CModelHelper::getModel(uid, MODELTYPE_TOWERTEST));
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
    CHECK_RETURN_VOID(NULL != pTowerTestModel && NULL != pUserModel);

	//����ģ���޸�
	std::map<int, int> towerData;
	towerData[TOWER_FD_CRYSTAL] = 0;
	towerData[TOWER_FD_FLOOR] = 0;
	towerData[TOWER_FD_STAGEID] = 0;
	
    CHECK_RETURN_VOID(pTowerTestModel->GetTowerTestField(towerData));
	CHECK_RETURN_VOID(towerData[TOWER_FD_STAGEID] != 0);

	int floor = towerData[TOWER_FD_FLOOR];
	// �ٴμ���Ƿ������ս
   //CHECK_RETURN_VOID(CChallengeHelper::canChallengeTowerTest(uid, floor));

	const TowerFloorItem *pTowerFloorConf = queryConfTowerFloor(floor);
	if (NULL == pTowerFloorConf)
	{
		KXLOGDEBUG("%s user %d get tower config is NULL!", __FUNCTION__, uid);
		return;
	}

	// ��Ӧ�Ĺؿ�id(�����Ѿ�����vector��С)
	if (pFinishCS->result == CRESULT_WIN)
	{
		//��������
		ChallengeBattleInfo *pBattleInfo = reinterpret_cast<ChallengeBattleInfo*>(pFinishCS+1);
		const TowerTestSettingItem * pTowerSettingConf = queryConfTowerSetting();
		CHECK_RETURN_VOID(NULL != pTowerSettingConf)
		
		//�¼���Ϊ�����¼� 
		//ע��, ���ﱾӦΪ����״̬, �����߻���Ϊ���챦��, û�б���״̬, ֱ����һ��!
		towerData[TOWER_FD_FLOOR] = towerData[TOWER_FD_FLOOR] + 1;

		// ��鴦��Ӷ��
		if (pBattleInfo->mercenaryId != 0 &&
			CModelHelper::canMercenaryUse(uid, pBattleInfo->mercenaryId))
		{
			CModelHelper::addMercenaryUseList(uid, pBattleInfo->mercenaryId);
		}
		
		//���������Ʒ
		std::vector<DropItemInfo> dropItems;
		int nSendLen = sizeof(TowerFinishSC);

		//��Ʒ����
		CItemDrop::Drop(pTowerFloorConf->Drop, dropItems);
		CPropUseHelper::getInstance()->AddItems(uid, dropItems);
		nSendLen = nSendLen + sizeof(TreasureReward) +dropItems.size() * sizeof(DropItemInfo);
	
		char *pSendBuffer = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLen));
		TowerFinishSC *pFinishSC = reinterpret_cast<TowerFinishSC*>(pSendBuffer);
		pFinishSC->floor = floor;
		pFinishSC->result = CRESULT_WIN;

		TreasureReward *pTreasureReward = reinterpret_cast<TreasureReward *>(pFinishSC + 1);
		pTreasureReward->count = dropItems.size();

		DropItemInfo *pPropsInfo = reinterpret_cast<DropItemInfo *>(pTreasureReward + 1);
		for (size_t i = 0; i < dropItems.size(); ++i)
		{
			memcpy(pPropsInfo, &dropItems[i], sizeof(DropItemInfo));
			pPropsInfo += 1;
		}


		CKxCommManager::getInstance()->sendData(uid, CMD_TOWERTEST, CMD_TOWER_FINISH_SC, pSendBuffer, nSendLen);
		KxServer::kxMemMgrRecycle(pSendBuffer, nSendLen);
		//�޸��������
		towerData[TOWER_FD_CRYSTAL] = pTowerSettingConf->FirstCrystal;
		towerData[TOWER_FD_STAGEID] = 0; //���ùؿ�ID

		if (!pTowerTestModel->SetTowerTestField(towerData))
		{
			KXLOGERROR("%s user %d add tower model error!", __FUNCTION__, uid);
		}

		//������ҪУ���Ƿ����������ŵ�ʱ�䷶Χ��
		//if ((int)time(NULL) < towerData[TOWER_FD_TIMESTAMP])
		{
			//��ӵ����а���
			CRankModel::getInstance()->AddRankData(TOWER_RANK_TYPE, uid, towerData[TOWER_FD_FLOOR]);
		}

        CModelHelper::DispatchActionEvent(uid, ELA_TOWER_TEST_FLOOR, &floor, sizeof(int));
	}
	else if (pFinishCS->result == CRESULT_LOSE)
	{
		TowerFinishSC finishSC;
		finishSC.floor = floor;
		finishSC.result = CRESULT_LOSE;
		CKxCommManager::getInstance()->sendData(uid, CMD_TOWERTEST, CMD_TOWER_FINISH_SC,
			reinterpret_cast<char *>(&finishSC), sizeof(finishSC));
	}
	else
	{
		// ȡ���ݲ�����
	}
}

void CTowerTrialService::processChoseOuterBonus(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
 //   CHECK_RETURN_VOID(len == sizeof(TowerChoseOuterBonusCS));
	//TowerChoseOuterBonusCS * pOuterBonusCS = reinterpret_cast<TowerChoseOuterBonusCS *>(buffer);

	//CTowerTestModel *pTowerTestModel = dynamic_cast<CTowerTestModel*>(CModelHelper::getModel(uid, MODELTYPE_TOWERTEST));
 //   CHECK_RETURN_VOID(NULL != pTowerTestModel);

	//// ������Ϣ
	//std::map<int, int> towerValues;
	//towerValues[TOWER_FD_FLOOR] = 0;
	//towerValues[TOWER_FD_FLOORSTATE] = 0;
	//towerValues[TOWER_FD_EVENTPARAM] = 0;
	//towerValues[TOWER_FD_STARS] = 0;
	//if (!pTowerTestModel->GetTowerTestField(towerValues))
	//{
	//	KXLOGERROR("%s user %d get tower model error!", __FUNCTION__, uid);
	//	return;
	//}

	//if (towerValues[TOWER_FD_FLOORSTATE] != FLOORSTATE_OUTERBONUS)
	//{
	//	KXLOGERROR("%s user %d floorState != FLOORSTATE_OUTERBONUS", __FUNCTION__, uid);
	//	return;
	//}
	////���ص�Э��
	//TowerChoseOuterBonusSC outerBonusSC;
	//memset(&outerBonusSC, 0, sizeof(outerBonusSC));
	////�������е�buffId�����
	//const TowerBuffItem *pTowerBuffConf = queryConfTowerBuff(towerValues[TOWER_FD_EVENTPARAM]);
	//if (NULL == pTowerBuffConf)
	//{
	//	//bug!!!!���ʲô����??
	//	KXLOGERROR("%s user %d NULL == pTowerBuffConf", __FUNCTION__, uid);
	//	return;
	//}

	//for (std::vector<TowerBuffInfo>::const_iterator iter = pTowerBuffConf->Buff.begin();
	//	iter != pTowerBuffConf->Buff.end(); ++iter)
	//{
	//	if (pOuterBonusCS->outerBonus1 != 0 && iter->BuffID == pOuterBonusCS->outerBonus1)
	//	{
	//		outerBonusSC.outerBonus1 = iter->BuffID;
	//		outerBonusSC.costStars += iter->Cost;
	//	}
	//	if (pOuterBonusCS->outerBonus2 != 0 && iter->BuffID == pOuterBonusCS->outerBonus2)
	//	{

	//		outerBonusSC.outerBonus2 = iter->BuffID;
	//		outerBonusSC.costStars += iter->Cost;
	//	}
	//	if (pOuterBonusCS->outerBonus3 != 0 && iter->BuffID == pOuterBonusCS->outerBonus3)
	//	{
	//		outerBonusSC.outerBonus3 = iter->BuffID;
	//		outerBonusSC.costStars += iter->Cost;
	//	}
	//}
	////��������
	//if (outerBonusSC.costStars > 0)
	//{
	//	//�¸�¥��
	//	int haveStar = pTowerTestModel->GetTowerTestField(TOWER_FD_STARS);
	//	if (haveStar < outerBonusSC.costStars)
	//	{
	//		//���ǲ�����
	//		KXLOGDEBUG("%s haveStar < outerBonusSC.costStars", __FUNCTION__);
	//		ErrorCodeData CodeData;
	//		CodeData.nCode = ERROR_BATTLE_STARTERROR;
	//		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
	//		return;
	//	}

	//	haveStar -= outerBonusSC.costStars;
	//	towerValues[TOWER_FD_STARS] = haveStar;

	//	std::vector<int> outerBonusVec;
	//	if (outerBonusSC.outerBonus1 != 0)
	//	{
	//		outerBonusVec.push_back(outerBonusSC.outerBonus1);
	//	}

	//	if (outerBonusSC.outerBonus2 != 0)
	//	{
	//		outerBonusVec.push_back(outerBonusSC.outerBonus2);
	//	}

	//	if (outerBonusSC.outerBonus3 != 0)
	//	{
	//		outerBonusVec.push_back(outerBonusSC.outerBonus3);
	//	}
	//	// �������
	//	if (!pTowerTestModel->AddOuterBonus(outerBonusVec))
	//	{
	//		KXLOGERROR("%s user %d add tower test outer bonus error!", __FUNCTION__, uid);
	//	}
	//}
	////�¸�¥��
	//towerValues[TOWER_FD_FLOOR] += 1;
	//towerValues[TOWER_FD_FLOORSTATE] = FLOORSTATE_FIGHTING;
	//if (!pTowerTestModel->SetTowerTestField(towerValues))
	//{
	//	KXLOGERROR("%s user %d add tower test outer bonus error!", __FUNCTION__, uid);
	//}
	//
	//CKxCommManager::getInstance()->sendData(uid, CMD_TOWERTEST, CMD_TOWER_CHOSEOUTERBONUS_SC,
	//	reinterpret_cast<char *>(&outerBonusSC), sizeof(outerBonusSC));
}

void CTowerTrialService::processOpenTreasure(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
 //   CHECK_RETURN_VOID(len == sizeof(TowerOpenTreasureCS));
	//TowerOpenTreasureCS *pOpenTreasureCS = reinterpret_cast<TowerOpenTreasureCS *>(buffer);

	//CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	//CTowerTestModel *pTowerTestModel = dynamic_cast<CTowerTestModel*>(CModelHelper::getModel(uid, MODELTYPE_TOWERTEST));
 //   CHECK_RETURN_VOID(NULL != pUserModel && NULL != pTowerTestModel);

	//std::map<int, int> mapValues;
	//mapValues[TOWER_FD_FLOOR] = 0;
	//mapValues[TOWER_FD_FLOORSTATE] = 0;
	//mapValues[TOWER_FD_EVENTPARAM] = 0;

 //   CHECK_RETURN_VOID(pTowerTestModel->GetTowerTestField(mapValues));

	//int floor = mapValues[TOWER_FD_FLOOR];
	//int floorState = mapValues[TOWER_FD_FLOORSTATE];
	//int eventParam = mapValues[TOWER_FD_EVENTPARAM];
	//if (floorState != FLOORSTATE_TREASURE)
	//{
	//	KXLOGDEBUG("%s user %d floorState != FLOORSTATE_TREASURE", __FUNCTION__, uid);
	//	return;
	//}

	//TowerOpenTreasureSC openTreasureSC;
	//if (pOpenTreasureCS->operate == 0)
	//{
	//	//�¸�¥��
	//	mapValues[TOWER_FD_FLOOR] = floor + 1;
	//	mapValues[TOWER_FD_FLOORSTATE] = FLOORSTATE_FIGHTING;
	//	mapValues[TOWER_FD_EVENTPARAM] = 1;
	//	if (!pTowerTestModel->SetTowerTestField(mapValues))
	//	{
	//		KXLOGERROR("%s user %d set tower test model data error!", __FUNCTION__, uid);
	//	}

	//	openTreasureSC.diamond = 0;
	//	openTreasureSC.count = 0;

	//	CKxCommManager::getInstance()->sendData(uid, CMD_TOWERTEST, CMD_TOWER_OPENTREASURE_SC,
	//		reinterpret_cast<char *>(&openTreasureSC), sizeof(openTreasureSC));
	//}
	//else
	//{
	//	//Ŀǰ�߻���ֻ�ܳ�һ��
	//	//�ٳ�һ��
	//	const TowerFloorItem *pTowerFloorItem = queryConfTowerFloor(floor);
	//	if (NULL == pTowerFloorItem)
	//	{
	//		KXLOGDEBUG("%s NULL == pTreasureItem", __FUNCTION__);
	//		return;
	//	}
	//	//��ʯ�Ƿ��㹻,������ʯ
	//	const IncreasePayItem * pPayment = queryConfIncreasePay(eventParam);
	//	if (NULL == pPayment || pPayment->TowerTreasureCost <= 0)
 //       {
 //           return;
 //       }

	//	int diamond = 0;
	//	int needDiamond = pPayment->TowerTreasureCost;
	//	pUserModel->GetUserFieldVal(USR_FD_DIAMOND, diamond);
	//	if (diamond < needDiamond)
	//	{
	//		KXLOGERROR("%s user %d test test OpenTreasure diamond not enough!", __FUNCTION__, uid);
	//		return;
	//	}
	//	else
 //       {
 //           //���ӹ������
 //           eventParam += 1;
	//		if (!pTowerTestModel->SetTowerTestField(TOWER_FD_EVENTPARAM, eventParam))
	//		{
	//			KXLOGERROR("%s user %d add tower test open treasure times error!", __FUNCTION__, uid);
	//		}
	//		if (!CModelHelper::addDiamond(uid, -1 * needDiamond))
	//		{
	//			KXLOGERROR("%s user %d cost diamond error!", __FUNCTION__, uid);
	//		}
	//	}

	//	std::vector<DropItemInfo> dropItems;
 //       CItemDrop::Drop(pTowerFloorItem->Drop, dropItems);
	//	CPropUseHelper::getInstance()->AddItems(uid, dropItems);

	//	int nSendLen = sizeof(TowerOpenTreasureSC)+dropItems.size()*sizeof(DropItemInfo);
	//	char *pSendBuffer = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLen));

	//	TowerOpenTreasureSC *pTreasuerSC = reinterpret_cast<TowerOpenTreasureSC *>(pSendBuffer);
	//	pTreasuerSC->diamond = needDiamond;
	//	pTreasuerSC->count = dropItems.size();

	//	DropItemInfo *pPropsInfo = reinterpret_cast<DropItemInfo *>(pTreasuerSC + 1);
	//	for (size_t i = 0; i < dropItems.size(); ++i)
	//	{
	//		memcpy(pPropsInfo, &dropItems[i], sizeof(DropItemInfo));
	//		pPropsInfo += 1;
	//	}

	//	CKxCommManager::getInstance()->sendData(uid, CMD_TOWERTEST, CMD_TOWER_OPENTREASURE_SC,
	//		pSendBuffer, nSendLen);

	//	KxServer::kxMemMgrRecycle(pSendBuffer, nSendLen);
	//	//����齱
	//	CCommOssHelper::userDiamondPayOss(uid, needDiamond, 0,1);
	//}
}

void CTowerTrialService::processOneKeyFighting(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{

}
