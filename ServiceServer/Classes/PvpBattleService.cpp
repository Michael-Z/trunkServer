#include "PvpBattleService.h"
#include "Protocol.h"
#include "PvpProtocol.h"
#include "GameUserManager.h"
#include "GameDef.h"
#include "PvpRankModel.h"
#include "ModelHelper.h"
#include "KxCommManager.h"
#include "CommOssHelper.h"
#include "PvpRankModel.h"
#include "ItemDrop.h"
#include "BufferData.h"
#include "BattleDataHelper.h"
#include "PvpSettleResult.h"
#include "PvpRobotHelper.h"
#include "ConfStage.h"
#include "ConfArena.h"
#include "PropUseHelper.h"

using namespace KxServer;

const static int RobotMinLv = 4;

void CPvpBattleService::processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    switch (subcmd)
    {
        // ���뷿��
    case CMD_PVP_ENTERROOM_CS:
        processPvpEnterRoomSSServices(uid, buffer, len, commun);
        break;
        // ս������
    case CMD_PVP_FINISH_SS:
        processPvpFinishSSServices(uid, buffer, len, commun);
        break;
        // ��������˷���
    case CMD_PVP_ENTERROBOTROOM_CS:
        processEnterRobotRoom(uid, buffer, len, commun);
        break;
        // �����˽���
    case CMD_PVP_FINISHROBOTEROOM_CS:
        processFinishRobotRoom(uid, buffer, len, commun);
        break;
    default:
        break;
    }
}

// �������PVP����
void CPvpBattleService::processPvpEnterRoomSSServices(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    do
    {
        // �������
        if (sizeof(PvpEnterRoomCS) > len)
        {
            break;
        }
        PvpEnterRoomCS *pEnterRoomCS = reinterpret_cast<PvpEnterRoomCS *>(buffer);
        int heroCount = pEnterRoomCS->heroCount;
        if (heroCount < 0 || heroCount > 7
            || sizeof(PvpEnterRoomCS) + sizeof(int)* heroCount != len)
        {
            KXLOGERROR("ProcessEnterRoom uid %d take %d heros", uid, heroCount);
            break;
        }

        //���뷿��ʱ��ȡ����
        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_RETURN_VOID(pGameUser);
        // ��ȡģ��
        CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
        CHECK_RETURN_VOID(pUserModel != NULL);
        CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(pGameUser->getModel(MODELTYPE_HERO));
        CHECK_RETURN_VOID(pHeroModel != NULL);
        CEquipModel *pEquipModel = dynamic_cast<CEquipModel*>(pGameUser->getModel(MODELTYPE_EQUIP));
        CHECK_RETURN_VOID(pEquipModel != NULL);
        CPvpModel *pPvpModel = dynamic_cast<CPvpModel*>(pGameUser->getModel(MODELTYPE_PVP));
        CHECK_RETURN_VOID(pPvpModel != NULL);

        if (!CModelHelper::haveSummoner(uid, pEnterRoomCS->summonerId))
        {
            // �ٻ�ʦδӵ��
            KXLOGERROR("ProcessEnterRoom uid %d summoner id not exist!", uid);
            break;
        }

        CBufferData bufferData;
        bufferData.init(10240);
        if (!bufferData.writeEmptyBuffer(sizeof(PvpEnterRoomSS)))
        {
            break;
        }

        PvpEnterRoomSS* enterSS = reinterpret_cast<PvpEnterRoomSS*>(bufferData.getBuffer());
        // ��ʼ��PvpEnterRoomSS
        pPvpModel->GetPvpField(PVP_FD_MMR, enterSS->mmr); // MMRֵ����������
        pPvpModel->GetPvpField(PVP_FD_INTEGRAL, enterSS->integral); // �������֣���������
        pPvpModel->GetPvpField(PVP_FD_DANCONTINUSWINTIMES, enterSS->continueWin);   // ��ʤ��������������
        pPvpModel->GetPvpField(PVPCOMM_FD_DAYWINTIMES, enterSS->dayWinTimes);   // ��ʤ��������ȡʤ��������
        enterSS->battleId = pEnterRoomCS->battleId;
        enterSS->roomType = pEnterRoomCS->roomType;

        // ��ʼ��PlayerData
        int playerOffset = bufferData.getDataLength();
        if (!bufferData.writeEmptyBuffer(sizeof(PlayerData)))
        {
            break;
        }
        PlayerData* playerData = reinterpret_cast<PlayerData*>(
            bufferData.getBuffer() + playerOffset);
        playerData->Camp = 0;
        playerData->Identity = pUserModel->getIdentity();
        playerData->MecenaryCount = 0;
        playerData->OuterBonusCount = 0;
        playerData->SoldierCount = heroCount;
        playerData->UserId = uid;
        pUserModel->GetUserFieldVal(USR_FD_USERLV, playerData->UserLv);
        std::string name = pUserModel->GetName();
        memset(playerData->UserName, 0, sizeof(playerData->UserName));
        strncpys(playerData->UserName, name.c_str(), strlen(name.c_str()));

        // ��ʼ���ٻ�ʦ��Ϣ
        int heroOffset = bufferData.getDataLength();
        if (!bufferData.writeEmptyBuffer(sizeof(HeroCardData)))
        {
            break;
        }
        HeroCardData* heroData = reinterpret_cast<HeroCardData*>(
            bufferData.getBuffer() + heroOffset);
        heroData->HeroId = pEnterRoomCS->summonerId;

        int initCount = 0;
        int *heroIds = reinterpret_cast<int*>(buffer + sizeof(PvpEnterRoomCS));
        //���Ӣ����Ϣ
        for (unsigned int i = 0; i < heroCount; ++i)
        {
            int heroId = heroIds[i];
            if (!CBattleDataHelper::soldierToBuffer(heroId, pEquipModel, pHeroModel, bufferData))
            {
                break;
            }
            ++initCount;
        }

        if (initCount != heroCount)
        {
            break;
        }

        // ����ҵ�ս����ʶ������ʱ���Ƴ�
        CModelHelper::persistPvpBattleKey(uid);

        // ת����Battle������
        CKxCommManager::getInstance()->forwardData(uid, CMD_PVP, CMD_PVP_ENTERROOM_SS,
            bufferData.getBuffer(), bufferData.getDataLength());
        return;
    } while (false);

    // ���ظ����ʧ�ܣ����޴�Э��
}

// ����PVP����
void CPvpBattleService::processPvpFinishSSServices(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    // ���˷Ƿ���
    if (sizeof(PvpFinishSS) > len)
    {
        return;
    }
    PvpFinishSS *pFinishSS = reinterpret_cast<PvpFinishSS *>(buffer);
    if (pFinishSS->itemCount < 0 || pFinishSS->itemCount > 10
        || sizeof(PvpFinishSS) + pFinishSS->itemCount * sizeof(DropItemInfo) != len)
    {
        return;
    }

    // ����б���ˢ����Ҫ�·�
    CModelHelper::refreshPvpChest(uid);

    CPvpRankModel *pPvpRankModel = CPvpRankModel::getInstance();
    // ��������б仯
    if (pFinishSS->integral != 0)
    {
        // ���а��޸ľ�������
        pPvpRankModel->AddPvpRank(uid, pFinishSS->newintegral);
    }
    int rank = 0;
    pPvpRankModel->GetPvpRank(uid, rank);

    switch (pFinishSS->result)
    {
        // ʤ���Ĵ���
    case CHALLENGE_WON:
        do 
        {
            std::vector<DropItemInfo> vecDropItems;
            DropItemInfo* item = reinterpret_cast<DropItemInfo*>(buffer + sizeof(PvpFinishSS));
            for (int i = 0; i < pFinishSS->itemCount; ++i)
            {
                vecDropItems.push_back(*item);
                ++item;
            }
            if (!CPropUseHelper::getInstance()->AddItems(uid, vecDropItems, true, PVPDROP_GAIN_REASON))
            {
                KXLOGERROR("processPvpFinishSSServices %d addItems faile", uid);
            }
			else
			{
				for (unsigned int i = 0; i < vecDropItems.size(); i++)
				{
					if (item->id == RESOURCEID_DIAMOND)
					{
						CCommOssHelper::userDiamondGetOss(uid, vecDropItems[i].num, PVP_DIAMOND_GET_REASON);
					}
				}
			}

            CPvpSettleResult::setWinnerPvpModel(pFinishSS->roomType, uid, pFinishSS->mmr, pFinishSS->integral, rank);
			// ע���¹���:
			// �����½ڽ���, ���½ڸ������pvp�ȼ����н���
			CModelHelper::checkChapterUnlock(uid, pFinishSS->newintegral);
        } while (false);
        break;
        // ʧ�ܵĴ���
    case CHALLENGE_FAILED:
        CPvpSettleResult::setLoserPvpModel(pFinishSS->roomType, uid, pFinishSS->mmr, pFinishSS->integral, rank);
        break;
        // ƽ�ֵĴ���
    case CHALLENGE_DRAW:
        CPvpSettleResult::setDrawPvpModel(pFinishSS->roomType, uid, pFinishSS->mmr, pFinishSS->integral, rank);
        break;
    case CHALLENGE_CANCEL:
        return;
    default:
        break;
    }

    // ���ӹ����Ծ
    CModelHelper::addDayLiveness(uid, UNION_DAY_PVPLIVENESS);

    // ��¼����������Ϣ
	SOssPvpLogData LogData;
	LogData.bWin = (pFinishSS->result == CHALLENGE_WON);
	LogData.nCryStalLevel = pFinishSS->crylv;
	memcpy(LogData.nHeroID, pFinishSS->heroid, sizeof(LogData.nHeroID));
	LogData.nOpponentSummer = pFinishSS->enemysummonerid;
	LogData.nPvpNum = pFinishSS->newintegral;
	LogData.nPvpType = pFinishSS->roomType;
	LogData.nSummonerID = pFinishSS->summonerid;
	LogData.nTime = pFinishSS->dt;
    CCommOssHelper::PvpLogicOss(uid,LogData);
}

// ������������PVP����
void CPvpBattleService::processEnterRobotRoom(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    // ���ȼ��
    CHECK_RETURN_VOID(len >= sizeof(PvpEnterRobotRoomCS));
    PvpEnterRobotRoomCS* pTeamInfo = reinterpret_cast<PvpEnterRobotRoomCS*>(buffer);
    CHECK_RETURN_VOID(pTeamInfo->heroCount > 0 && pTeamInfo->heroCount <= 7);
    CHECK_RETURN_VOID(pTeamInfo->heroCount * sizeof(int) + sizeof(PvpEnterRobotRoomCS) == len);

    //���뷿��ʱ��ˢ������
    CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
    CHECK_RETURN_VOID(pGameUser != NULL);

    int robotId = 0;
    // ������, ���battleIdKey, �Ƿ���battleId��robotId, ���򷵻�
    if (!CRobotHelper::canChallengeRobot(uid, robotId))
    {
        KXLOGERROR("processEnterRobotRoom %d can not enter robot room", uid);
        return;
    }

    const SArenaRobotItem *pRobotConf = queryConfArenaRobotItem(robotId);
    const ArenaRobotNameItem *pRobotNameConf = randConfRobotName();
    CHECK_RETURN_VOID(NULL != pRobotConf && NULL != pRobotNameConf);
    if (pRobotConf->stageID.size() < 2)
    {
        KXLOGERROR("%d get robot config error, robotid=%d, stageID is empty!", uid, robotId);
        return;
    }

    if (pRobotConf->robotAP.empty())
    {
        KXLOGERROR("%d get robot config error, robotid=%d, robotAP is empty!", uid, robotId);
        return;
    }

    if (pRobotConf->robotLevel.empty())
    {
        KXLOGERROR("%d get robot config error, robotid=%d, robotLevel is empty!", uid, robotId);
        return;
    }

    const StageConfItem *pStageConf = queryConfStage(pRobotConf->stageID[0]);
    CHECK_RETURN_VOID(NULL != pStageConf);

    // ע��, �����˵Ļ���, ��ʼʱ���͵Ļ��ָ�����Ļ��ֶ��ǵ������, ��ֵ��ͬ, ����Ҫ�����ݿ��ֶ�������.
    CPvpModel *pPvpModel = dynamic_cast<CPvpModel*>(CModelHelper::getModel(uid, MODELTYPE_PVP));
    CHECK_RETURN_VOID(NULL != pPvpModel);
    CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
    CHECK_RETURN_VOID(NULL != pUserModel);

    int intergal = 0;
    int userLevel = 0;
    pPvpModel->GetPvpField(PVP_FD_INTEGRAL, intergal);
    pUserModel->GetUserFieldVal(USR_FD_USERLV, userLevel);
    int index = rand() % pRobotConf->robotAP.size();
    intergal += pRobotConf->robotAP[index];
    index = rand() % pRobotConf->robotLevel.size();
    userLevel += pRobotConf->robotLevel[index];
    if (userLevel < RobotMinLv)
    {
        userLevel = RobotMinLv;
    }

    // ���췿������
    BattleRoomData room;
    room.battleType = EBATTLE_PVPROBOT;
    room.stageId = pRobotConf->stageID[0];
    room.stageLv = pRobotConf->stageID[1];
    room.summonerId = pTeamInfo->summonerId;
    int * heroIds = reinterpret_cast<int*>(buffer + sizeof(PvpEnterRobotRoomCS));
    for (int i = 0; i < pTeamInfo->heroCount; ++i)
    {
        if (heroIds[i] > 0)
        {
            room.heroIds.push_back(heroIds[i]);
        }
    }

    // ���ط������ݰ�
    CBufferData bufferData;
    bufferData.init(10240);
    if (!CBattleDataHelper::roomDataToBuffer(uid, room, bufferData))
    {
        return;
    }
    CKxCommManager::getInstance()->sendData( uid, CMD_PVP, CMD_PVP_ROBOTROOMDATA_SC,
        bufferData.getBuffer(), bufferData.getDataLength());

    // ���Ͷ�����Ϣ
    PvpEnterRoomSC enterRoomData;
    enterRoomData.result = 1;
    //Ĭ����ɫ��
    enterRoomData.camp = 1;
    //������uidΪ0
    enterRoomData.uid = 0;
    enterRoomData.usrLv = userLevel;
    //������bossId
    enterRoomData.summonerId = pStageConf->Boss;
    enterRoomData.integral = intergal;
    enterRoomData.progress = 100;
    enterRoomData.extend = 0;
    memset(enterRoomData.usrName, 0, sizeof(enterRoomData.usrName));
    memcpy(enterRoomData.usrName, pRobotNameConf->robotName.c_str(),
        strlen(pRobotNameConf->robotName.c_str()));

    CKxCommManager::getInstance()->sendData(
        uid, CMD_PVP, CMD_PVP_ENTERROOM_SC,
        reinterpret_cast<char*>(&enterRoomData), sizeof(enterRoomData));
}

// ��������˽���
void CPvpBattleService::processFinishRobotRoom(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(PvpFinishRobotRoomCS));
    PvpFinishRobotRoomCS *pFinishCS = reinterpret_cast<PvpFinishRobotRoomCS*>(buffer);

    //���battleIdKey, �Ƿ���battleId��robotId, ���򷵻�
    int robotId = 0;
    if (!CRobotHelper::canChallengeRobot(uid, robotId))
    {
        return;
    }

    // ����б���ˢ����Ҫ�·�
    CModelHelper::refreshPvpChest(uid);

    CPvpModel *pPvpModel = dynamic_cast<CPvpModel*>(CModelHelper::getModel(uid, MODELTYPE_PVP));
    CPvpRankModel *pPvpRankModel = CPvpRankModel::getInstance();
    CHECK_RETURN_VOID(NULL != pPvpModel && NULL != pPvpRankModel);
    //ɾ��battleIdKey
    pPvpModel->DeletePvpBattleKey();

    //���ؽ���
    std::vector<DropItemInfo> vecDropItems;
    if (pFinishCS->wonOrFailed == CRESULT_LOSE)
    {
        CRobotHelper::setModelLoseRobot(uid, robotId);
    }
    else if (pFinishCS->wonOrFailed == CRESULT_WIN)
    {
        CRobotHelper::setModelWinRobot(uid, robotId);

        // ʤ������
        int winnerDropId = CModelHelper::getWinDropId(uid);
        if (winnerDropId > 0)
        {
            CItemDrop::Drop(winnerDropId, vecDropItems);
			CPropUseHelper::getInstance()->AddItems(uid, vecDropItems, true, PVPDROP_GAIN_REASON);
        }
    }

    // ���ӹ����Ծ
    CModelHelper::addDayLiveness(uid, UNION_DAY_PVPLIVENESS);

    int nSendLen = sizeof(PvpResultSC)+vecDropItems.size() * sizeof(DropItemInfo);
    char *pSendData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLen));
    PvpResultSC * pResultSC = reinterpret_cast<PvpResultSC*>(pSendData);
    pResultSC->roomType = PVPROOMTYPE_ROBOT;
    pResultSC->result = pFinishCS->wonOrFailed;
    pResultSC->itemCount = vecDropItems.size();

    DropItemInfo *pDropItemInfo = reinterpret_cast<DropItemInfo*>(pSendData + sizeof(PvpResultSC));
    for (size_t i = 0; i < vecDropItems.size(); ++i)
    {
        memcpy(pDropItemInfo, &(vecDropItems[i]), sizeof(DropItemInfo));
        pDropItemInfo += 1;
    }

    pPvpModel->GetPvpField(PVP_FD_INTEGRAL, pResultSC->integral);
    pPvpRankModel->GetPvpRank(uid, pResultSC->rankNow);
    //���͸����
    CKxCommManager::getInstance()->sendData(uid, CMD_PVP, CMD_PVP_RESULT_SC, pSendData, nSendLen);
    //�����ڴ�
    KxServer::kxMemMgrRecycle(pSendData, nSendLen);
}
