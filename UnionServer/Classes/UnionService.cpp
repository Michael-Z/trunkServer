#include "UnionService.h"
#include "UnionManager.h"
#include "KxMemPool.h"
#include "GameUserManager.h"
#include "ConfUnion.h"
#include "ModelHelper.h"
#include "KxCommManager.h"
#include "UnionComm.h"
#include "UnionServer.h"
#include "MailHelper.h"
#include "RankModel.h"
#include "PropUseHelper.h"
#include "CommOssHelper.h"
#include "UnionHelper.h"

#include "Protocol.h"
#include "UnionProtocol.h"
#include "ServerProtocol.h"
#include "LoginProtocol.h"
#include "UserProtocol.h"

#include "UnionMercenaryModel.h"
#include "PersonMercenaryModel.h"
#include "MemoryOperation.h"

using namespace std;
using namespace KxServer;

#define MIN_TO_SECOND(M) M * 60

void CUnionService::processService(int subcmd, int uid, char *buffer, int len, IKxComm *commun)
{
	switch (subcmd)
	{
    case CMD_UNION_INFO_CS:
        processUnionInfoService(uid, buffer, len, commun);
		break;
    case CMD_UNION_MEMBERS_CS:
        processMembersService(uid, buffer, len, commun);
		break;
    case CMD_UNION_CREATE_CS:
        processCreateService(uid, buffer, len, commun);
        break;
    case CMD_UNION_APPLY_CS:
        processApplyService(uid, buffer, len, commun);
        break;
    case CMD_UNION_EXIT_CS:
        processExitService(uid, buffer, len, commun);
        break;
    case CMD_UNION_LOGLIST_CS:
        processLogListService(uid, buffer, len, commun);
        break;
    case CMD_UNION_AUDITLIST_CS:
        processAuditListService(uid, buffer, len, commun);
        break;
    case CMD_UNION_AUDIT_CS:
        processAuditService(uid, buffer, len, commun);
        break;
    case CMD_UNION_FUNCTION_CS:
        processFunctionService(uid, buffer, len, commun);
        break;
    case CMD_UNION_UNIONLIST_OUT_CS:
        processOutUnionListService(uid, buffer, len, commun);
        break;
    case CMD_UNION_SEARCH_CS:
        processSearchService(uid, buffer, len, commun);
        break;
    case CMD_UNION_EMBLEM_CS:
        processEmblemService(uid, buffer, len, commun);
        break;
    case CMD_UNION_NAME_CS:
        processUnionNameService(uid, buffer, len, commun);
        break;
    case CMD_UNION_NOTICE_CS:
        processUnionNoticeService(uid, buffer, len, commun);
        break;
    case CMD_UNION_WELFARE_CS:
        processWelfareService(uid, buffer, len, commun);
        break;
    case CMD_UNION_SETAUDIT_CS:
        processSetAuditService(uid, buffer, len, commun);
        break;
	case CMD_UNION_ALLMERCENARY_CS:
		processMercenaryService(uid, buffer, len, commun);
		break;
	case CMD_UNION_DISPATCH_CS:
		processDispatchMercenaryService(uid, buffer, len, commun);
		break;
	case CMD_UNION_RECALLMER_CS:
		processRecallMercenaryService(uid, buffer, len, commun);
		break;
	case CMD_UNION_SINGLEMER_CS:
		processSingleMercenaryService(uid, buffer, len, commun);
		break;
    case CMD_UNION_FORWARD_SS:
        processUnionForwardDataService(uid, buffer, len, commun);
        break;
	}
}

void CUnionService::processUnionInfoService(int uid, char *buffer, int len, IKxComm *commun)
{
    CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
    CHECK_RETURN_VOID(NULL != pGameUser);
    CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);

    int nUnionID = 0;
    CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));

    CUnion *pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
    if (NULL == pUnion)
    {
        //������Ƿ������˹���
        const map<int, int> &MapApplyUnion = pUserUnionModel->getApplyInfo();
        map<int, int>::const_iterator ator = MapApplyUnion.begin();
        for(; ator != MapApplyUnion.end(); ++ator)
        {
            if (CUnionManager::getInstane()->getUnion(ator->second))
            {
                //���ý�ɫ��ӵ�������
                CKxCommManager::getInstance()->addKxComm(uid, commun);
                break;
            }
        }

        nUnionID = 0;
        CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_INFO_SC
            , reinterpret_cast<char*>(&nUnionID), sizeof(int));
    }
    else
    {
        //���ý�ɫ��ӵ�������
		CKxCommManager::getInstance()->addKxComm(uid, commun);
        CUnionModel *pUnionModel = pUnion->getUnionModel();
		CHECK_RETURN_VOID(pUnionModel != NULL);
        // ��Ա�б�
        map<int, int> UnionMembers = pUnionModel->GetMembers();
        map<int, int>::iterator ator = UnionMembers.begin();
        for (; ator != UnionMembers.end();)
        {
            // ����
            if (CKxCommManager::getInstance()->getKxComm(ator->first))
            {
                ++ator;
            }
            else
            {
                UnionMembers.erase(ator++);
            }
        }

        CModelHelper::updateUserUnionInfo(uid);
        int nBuffSize = sizeof(UnionInfo)+sizeof(OnlineMemberInfo)*UnionMembers.size();
        char *buff = reinterpret_cast<char*>(kxMemMgrAlocate(nBuffSize));

        int nOffset = sizeof(UnionInfo);
        UnionInfo *pUnionInfoSC = reinterpret_cast<UnionInfo *>(buff);
        memset(pUnionInfoSC, 0, sizeof(UnionInfo));
        pUnionInfoSC->unionID = nUnionID;
        pUnionModel->GetUnionFieldVal(UNION_FIELD_LV, pUnionInfoSC->unionLv);
        pUnionModel->GetUnionFieldVal(UNION_FIELD_LIVENESS, pUnionInfoSC->liveness);
        CRankModel::getInstance()->GetRankData(UNION_RANK_TYPE, nUnionID, pUnionInfoSC->unionRank);
        pUnionModel->GetUnionFieldVal(UNION_FIELD_REPUTATION, pUnionInfoSC->reputation);
        pUnionModel->GetUnionFieldVal(UNION_FIELD_LIMITLV, pUnionInfoSC->limitLv);
        pUnionModel->GetUnionFieldVal(UNION_FIELD_EMBLEM, pUnionInfoSC->emblem);
        pUnionModel->GetUnionFieldVal(UNION_FIELD_AUTOAUDIT, pUnionInfoSC->isAutoAudit);
        pUnionModel->GetUnionFieldVal(UNION_FIELD_DANGEROUSTAG, pUnionInfoSC->dangerousTag);
        pUserUnionModel->getUserUnionValue(USER_UNION_WELFARE, pUnionInfoSC->welfareTag);
        pUnionInfoSC->pos = (char)pUnionModel->GetMemberPos(uid);
        pUnionInfoSC->memberCount = (char)pUnionModel->GetMembers().size();
        pUnionInfoSC->onlineMemberCount = (char)UnionMembers.size();
        strncpys(pUnionInfoSC->unionName, pUnionModel->GetUnionName().c_str(), strlen(pUnionModel->GetUnionName().c_str()));
        strncpys(pUnionInfoSC->notice, pUnionModel->GetUnionNotice().c_str(), strlen(pUnionModel->GetUnionNotice().c_str()));
        int nChairmanID = 0;
        pUnionModel->GetUnionFieldVal(UNION_FIELD_CHAIRMAN, nChairmanID);

        UserBasicInfo chairInfo;
        if (!CModelHelper::getUserInfo(nChairmanID, chairInfo))
        {
            KXLOGDEBUG("processUnionInfoService CModelHelper::getUserInfo is fail!%d", nChairmanID);
        }

        strncpys(pUnionInfoSC->chairmanName, chairInfo.name.c_str(), strlen(chairInfo.name.c_str()));
        pUnionInfoSC->chairIdentity = chairInfo.identity;

        OnlineMemberInfo *pOnlineMemberInfo = reinterpret_cast<OnlineMemberInfo*>(pUnionInfoSC + 1);
        std::map<int, int>::iterator onlineIter = UnionMembers.begin();
        for (; onlineIter != UnionMembers.end(); ++onlineIter)
        {
            CGameUser *pOnlineGameUser = CGameUserManager::getInstance()->getGameUser(onlineIter->first);
            if (NULL == pOnlineGameUser)
            {
                continue;
            }

            CUserModel *pUserModel = dynamic_cast<CUserModel*>(pOnlineGameUser->getModel(MODELTYPE_USER));
            pUserModel->GetUserFieldVal(USR_FD_USERLV, pOnlineMemberInfo->lv, true);
            pOnlineMemberInfo->userId = onlineIter->first;
            pOnlineMemberInfo->pos = onlineIter->second;
            memset(pOnlineMemberInfo->userName, 0, sizeof(pOnlineMemberInfo->userName));
            std::string name = pUserModel->GetName();
            strncpys(pOnlineMemberInfo->userName, name.c_str(), strlen(name.c_str()));
            pOnlineMemberInfo->identity = pUserModel->getIdentity();

            nOffset += sizeof(OnlineMemberInfo);
            pOnlineMemberInfo += 1;
        }

        //����·��
        CKxCommManager::getInstance()->setSessionRoute(uid, emRouteUnionKey, UNION_SERVER_ID);
        //��������
        CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_INFO_SC, buff, nOffset);
        kxMemMgrRecycle(buff, nBuffSize);
    }
}

void CUnionService::processMembersService(int uid, char *buffer, int len, IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(UnionMembersCS));

    UnionMembersCS *pUnionMembersCS = reinterpret_cast<UnionMembersCS*>(buffer);

    CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(pUnionMembersCS->unionID);
    CHECK_RETURN_VOID(NULL != pUnionModel);

    map<int, int> &mapMembers = pUnionModel->GetMembers();
    int nBufSize = mapMembers.size() * sizeof(UnionMembersInfo) + sizeof(UnionMembersSC);
    char* buf = reinterpret_cast<char*>(kxMemMgrAlocate(nBufSize));

    UnionMembersSC *pUnionMembersSC = reinterpret_cast<UnionMembersSC*>(buf);
    pUnionMembersSC->memberCount = 0;

    int nOffset = sizeof(UnionMembersSC);
    UserBasicInfo tempUserInfo;
    UserUnionBasicInfo tempUserUnionInfo;
    map<int, int>::iterator iter = mapMembers.begin();
    for (; iter != mapMembers.end(); iter++)
    {
        if (!CModelHelper::getUserInfo(iter->first, tempUserInfo)
            || !CModelHelper::getUserUnionInfo(iter->first, tempUserUnionInfo))
        {
            continue;
        }

        UnionMembersInfo *pUnionMembersInfo = reinterpret_cast<UnionMembersInfo*>(buf + nOffset);
        pUnionMembersInfo->userID = iter->first;
        pUnionMembersInfo->totalContrib = tempUserUnionInfo.totalContribution;
        pUnionMembersInfo->position = iter->second;
        pUnionMembersInfo->userLv = tempUserInfo.userLv;
        pUnionMembersInfo->todayLiveness = tempUserUnionInfo.todayStageLiveness + tempUserUnionInfo.todayPvpLiveness;
        pUnionMembersInfo->lastLoginTime = tempUserInfo.lastLoginTime;
        pUnionMembersInfo->identity = tempUserInfo.identity;
        memset(pUnionMembersInfo->userName, 0, sizeof(pUnionMembersInfo->userName));
        strncpys(pUnionMembersInfo->userName, tempUserInfo.name.c_str(), strlen(tempUserInfo.name.c_str()));

        pUnionMembersSC->memberCount++;
        // ƫ��һ����λ(UnionMembersInfo)
        nOffset += sizeof(UnionMembersInfo);
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_MEMBERS_SC, buf, nOffset);
    kxMemMgrRecycle(buf, nBufSize);
}

void CUnionService::processCreateService(int uid, char *buffer, int len, IKxComm *commun)
{
    UnionCreateSC unionCreateSC;
    unionCreateSC.result = 0;
    unionCreateSC.extend = UNION_EC_FAILD;
    
    do
    {
        CHECK_BREAK(len <= sizeof(UnionCreateCS) && len > 0);

        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
		if (NULL == pGameUser)
		{
			pGameUser = CGameUserManager::getInstance()->initGameUser(uid);
			CHECK_BREAK(NULL != pGameUser)
		}

        CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
		CHECK_BREAK(pUserModel != NULL);
		CHECK_BREAK(pUserUnionModel != NULL);
        UnionCreateCS *pUnionCreateCS = reinterpret_cast<UnionCreateCS*>(buffer);
        unionCreateSC.extend = CUnionHelper::checkCreate(uid, pUnionCreateCS->unionName);
        CHECK_BREAK(UNION_EC_SUCCESS == unionCreateSC.extend);
        unionCreateSC.extend = UNION_EC_FAILD;

        int nUnionID = 0;
        CHECK_BREAK(CUnionModel::getNewUnionId(UNION_SERVER_ID, nUnionID));

        //������Ҫ�����ñ���в���
        if (CUnionModel::hasUnion(UNION_SERVER_ID, nUnionID))
        {
            KXLOGERROR("error: new unionid %d had union!!", nUnionID);
            break;
        }

        CHECK_BREAK(CUnionModel::setUnionField(UNION_SERVER_ID, SRV_FD_UNIONREF, nUnionID));

        if (CUnionModel::addUnion(UNION_SERVER_ID, nUnionID))
        {
            // ��������
            if (CUnionManager::getInstane()->createUnion(nUnionID, uid, pUnionCreateCS->unionName))
            {
                const UnionItem &unionSetting = queryConfUnion();
                // �۳�����
                if (!CModelHelper::addGold(uid, -unionSetting.CostCoin))
                {
                    // ʧ�ܴ���
                    KXLOGERROR("userid %d deduct gold %d fail", uid, unionSetting.CostCoin);
                }

                // ����������Ϣ
                pUserUnionModel->removeAllApplyInfo();
                pUserUnionModel->setUserUnionValue(USER_UNION_ID, nUnionID);
                unionCreateSC.extend = nUnionID;
                unionCreateSC.result = UNION_EC_SUCCESS;
                CKxCommManager::getInstance()->addKxComm(uid, commun);
                //����·��
                CKxCommManager::getInstance()->setSessionRoute(uid, emRouteUnionKey, UNION_SERVER_ID);
                //ת������
                CKxCommManager::getInstance()->forwardData(uid, CMD_USER, CMD_USER_FORWARD_SS,
                    reinterpret_cast<char*>(&nUnionID), sizeof(int));
                // �ӵ����а�
                CRankModel::getInstance()->AddRankData(UNION_RANK_TYPE, nUnionID, 1);
            }
        }
    } while (false);

    //��������
    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_CREATE_SC, 
        reinterpret_cast<char*>(&unionCreateSC), sizeof(UnionCreateSC), commun);
}

void CUnionService::processApplyService(int uid, char *buffer, int len, IKxComm *commun)
{
    EUnionErrorCode errorCode = UNION_EC_FAILD;

    CHECK_RETURN_VOID(len == sizeof(UnionApplyCS));
    UnionApplyCS *pUnionApplyCS = reinterpret_cast<UnionApplyCS*>(buffer);

    do
    {
        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
		// �����ǵ�һ�ε�¼���������
        if (NULL == pGameUser)
        {
            pGameUser = CGameUserManager::getInstance()->initGameUser(uid);
            CHECK_BREAK(NULL != pGameUser)
        }

        CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(pUnionApplyCS->unionID);
        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
        CHECK_BREAK(NULL != pUnionModel && NULL != pUserUnionModel);
        
        CModelHelper::updateUserUnionInfo(uid);
        // �Ƿ��ܹ�����
        errorCode = CUnionHelper::checkApply(uid, pUnionApplyCS->unionID);
        CHECK_BREAK(UNION_EC_SUCCESS == errorCode);
        errorCode = UNION_EC_FAILD;
        // �Ƿ��Զ�ͨ������
        int nIsAuto = 0;
        CHECK_BREAK(pUnionModel->GetUnionFieldVal(UNION_FIELD_AUTOAUDIT, nIsAuto));

        if (nIsAuto > 0)
        {
            if (pUserUnionModel->setUserUnionValue(USER_UNION_ID, pUnionApplyCS->unionID)
                && pUnionModel->AddMember(uid, UNION_POS_NORMAL)
                && pUserUnionModel->removeAllApplyInfo())
            {
                errorCode = UNION_EC_AUTO_AUDIT;
            }
        }
        else
        {
            int nLiveTime = int(time(NULL)) + MIN_TO_SECOND(queryConfUnion().AuditTime);
            if (pUserUnionModel->addApplyInfo(pUnionApplyCS->unionID, nLiveTime)
                && pUnionModel->AddAudit(uid, nLiveTime)
                && pUserUnionModel->addUserUnionValue(USER_UNION_DAYAPPLAYCOUNT))
            {
                errorCode = UNION_EC_SUCCESS;

                CUnionHelper::sendMessageTips(pUnionModel, UNION_MESSAGE_AUDIT);
            }
        }

        if (UNION_EC_AUTO_AUDIT == errorCode || UNION_EC_SUCCESS == errorCode)
        {
            CKxCommManager::getInstance()->addKxComm(uid, commun);

            if (UNION_EC_AUTO_AUDIT == errorCode)
            {
                CKxCommManager::getInstance()->forwardData(uid, CMD_USER, CMD_USER_FORWARD_SS,
                    reinterpret_cast<char*>(&pUnionApplyCS->unionID), sizeof(int), commun);
                // �Զ�ͨ�����·������¼��Ϣ
                CUnionHelper::sendOwnUnionInfo(uid, pUnionApplyCS->unionID);
                // ֪ͨ�������˼��빫��
                CUnionHelper::noticeUnion(pUnionApplyCS->unionID, 
                    NOTICE_UNION_NEWMEMBER, CModelHelper::getUserName(uid));
            }
        }
    } while (0);

    // �·���Ϣ
    UnionApplySC unionApplySC;
    unionApplySC.errorCode = errorCode;
    unionApplySC.unionID = pUnionApplyCS->unionID;
    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_APPLY_SC
        , reinterpret_cast<char*>(&unionApplySC), sizeof(UnionApplySC), commun);
}

void CUnionService::processExitService(int uid, char *buffer, int len, IKxComm *commun)
{
    UnionExitSC unionExitSC;
    unionExitSC.timeStamp = 0;

    int nUnionID = 0;

    do
    {
        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_BREAK(NULL != pGameUser);

        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));

        CHECK_BREAK(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));

        CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(nUnionID);
        CHECK_BREAK(NULL != pUnionModel);

        // ������IDȥ��&�´���������ʱ��
        int nTime = -1; 
        // ��������һ���˳����Ὣ��ɢ����,�����ǻ᳤��ӹ����Ա�б����Ƴ�
        if (pUnionModel->GetMembers().size() <= 1)
        {
            nTime = CUnionHelper::quitUnion(uid);
            CHECK_BREAK(nTime > 0 && CUnionManager::getInstane()->deleteUnion(nUnionID));
        }
        else
        {
            // ����ǻ᳤�˻�,�������һ���˲�����
            int pos = pUnionModel->GetMemberPos(uid);
            if (UNION_POS_CHAIRMAN == pos)
            {
                break;
            }

            nTime = CUnionHelper::quitUnion(uid);
            // �����¼��б�
            CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
			CHECK_RETURN_VOID(pUserModel != NULL);
            CUnionHelper::addUnionLog(nUnionID, UNION_FUNC_EXIT, pUserModel->GetName());
        }

        unionExitSC.timeStamp = nTime;
    } while (0);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_EXIT_SC
        , reinterpret_cast<char*>(&unionExitSC), sizeof(UnionExitSC));

    //����˻�ɹ�
    if (unionExitSC.timeStamp > 0)
    {
        // ֪ͨ���������˳�����
        CUnionHelper::noticeUnion(nUnionID,
            NOTICE_UNION_QUIT, CModelHelper::getUserName(uid));
        //ɾ������
        CKxCommManager::getInstance()->delKxComm(uid);
        //�˻��//·������Ϊ0
        CKxCommManager::getInstance()->setSessionRoute(uid, emRouteUnionKey, 0);
    }
}

void CUnionService::processLogListService(int uid, char *buffer, int len, IKxComm *commun)
{
    CUnionModel *pUnionModel = CUnionHelper::getUnionModel(uid);
    CHECK_RETURN_VOID(NULL != pUnionModel);

    int nBuffSize = sizeof(UnionLogListSC) + pUnionModel->GetLogs().size() * sizeof(UnionLogInfo);
    char *buff = reinterpret_cast<char*>(kxMemMgrAlocate(nBuffSize));
    UnionLogListSC *pUnionLogListSC = reinterpret_cast<UnionLogListSC *>(buff);
    pUnionLogListSC->logCount = pUnionModel->GetLogs().size();

    UnionLogInfo *pUnionLogInfo = reinterpret_cast<UnionLogInfo*>(pUnionLogListSC + 1);
    map<int, UnionLogInfo> &mapLogInfo = pUnionModel->GetLogs();
    map<int, UnionLogInfo>::reverse_iterator iter = mapLogInfo.rbegin();
    for (; iter != mapLogInfo.rend(); ++iter)
    {
        memset(pUnionLogInfo, 0, sizeof(UnionLogInfo));
        memcpy(pUnionLogInfo, &iter->second, sizeof(UnionLogInfo));
        // ƫ��һ����λ(UnionLogInfo)
        pUnionLogInfo += 1;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_LOGLIST_SC, buff, nBuffSize);
    kxMemMgrRecycle(buff, nBuffSize);
}

void CUnionService::processAuditListService(int uid, char *buffer, int len, IKxComm *commun)
{
    CUnionModel *pUnionModel = CUnionHelper::getUnionModel(uid);
    CHECK_RETURN_VOID(NULL != pUnionModel);

    int nBuffSize = sizeof(UnionAuditListSC) + pUnionModel->GetAudits().size() * sizeof(AuditsInfo);
    char *buff = reinterpret_cast<char*>(kxMemMgrAlocate(nBuffSize));
    UnionAuditListSC *pUnionAduitListSC = reinterpret_cast<UnionAuditListSC *>(buff);
    pUnionAduitListSC->auditsCount = 0;
    // ƫ�Ƶ�λ(UnionAduitListSC)
    int nOffset = sizeof(UnionAuditListSC);

    AuditsInfo *pAuditsInfo = reinterpret_cast<AuditsInfo*>(buff + nOffset);

    int nCurTime = int(time(NULL));
    UserBasicInfo info;
    map<int, int> mapAuditInfo = pUnionModel->GetAudits();
    map<int, int>::iterator iter = mapAuditInfo.begin();
    for (; iter != mapAuditInfo.end(); iter++)
    {
        if (nCurTime >= iter->second) // �������ʱ��ɾ�����·�
        {
            pUnionModel->RemoveAudit(iter->first);
            continue;
        }

        if (!CModelHelper::getUserInfo(iter->first, info))
        {
            continue;
        }

        memset(pAuditsInfo, 0, sizeof(AuditsInfo));
        pAuditsInfo->userID = iter->first;
        pAuditsInfo->userLv = info.userLv;
        pAuditsInfo->identity = info.identity;
        strncpys(pAuditsInfo->userName, info.name.c_str(), strlen(info.name.c_str()));

        pUnionAduitListSC->auditsCount++;
        nOffset += sizeof(AuditsInfo);
        pAuditsInfo = reinterpret_cast<AuditsInfo*>(buff + nOffset);
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_AUDITLIST_SC, buff, nOffset);
    kxMemMgrRecycle(buff, nBuffSize);
}

void CUnionService::processAuditService(int uid, char *buffer, int len, IKxComm *commun)
{
    EUnionErrorCode result = UNION_EC_FAILD;
    std::list<int> agreeList;
    std::list<int> refuseList;
    int nUnionID = 0;

    do
    {
        CHECK_BREAK(len >= sizeof(UnionAuditCS));

        UnionAuditCS *pUnionAuditCS = reinterpret_cast<UnionAuditCS*>(buffer);
        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_BREAK(NULL != pGameUser);
        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
		CHECK_RETURN_VOID(pUserUnionModel != NULL);
        
        CHECK_BREAK(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));
        CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(nUnionID);
        CHECK_BREAK(NULL != pUnionModel);

        int nUnionLv = 0;
        CHECK_BREAK(pUnionModel->GetUnionFieldVal(UNION_FIELD_LV, nUnionLv));

        int position = pUnionModel->GetMemberPos(uid);
        // ���᳤����û�����Ȩ��
        if (position < UNION_POS_VICE_CHAIRMAN)
        {
            result = UNION_EC_NO_AUDIT_POWER;
            break;
        }

        const UnionLevelItem *pUnionItem = queryConfUnionLevel(nUnionLv);
        CHECK_BREAK(NULL != pUnionItem);

        int nCurMemberCount = (int)pUnionModel->GetMembers().size();
        if (pUnionAuditCS->isAgree > 0 && nCurMemberCount >= pUnionItem->MemberLimit)
        {
            result = UNION_EC_MEMBER_FULL;
            break;
        }
        
        if (pUnionAuditCS->auditCount > 0)
        {
            int *pUserId = reinterpret_cast<int*>(pUnionAuditCS + 1);
            for (int i = 0; i < pUnionAuditCS->auditCount; ++i)
            {
                result = CUnionHelper::auditUser(nUnionID, *pUserId, pUnionAuditCS->isAgree > 0);
                if (UNION_EC_PASS_AUDIT == result)
                {
                    agreeList.push_back(*pUserId);
                    ++nCurMemberCount;
                    if (nCurMemberCount >= pUnionItem->MemberLimit)
                    {
                        break;
                    }
                }
                else if (UNION_EC_REFUSE_AUDIT == result
                    || UNION_EC_AUDIT_OVERTIME == result
                    || UNION_EC_OWN_UNION == result
                    || UNION_EC_ALREADY_DO == result)
                {
                    refuseList.push_back(*pUserId);
                }

                pUserId += 1;
            }
        }

        result = UNION_EC_SUCCESS;
    } while (0);

    int dataLen = sizeof(UnionAuditSC)+(agreeList.size() + refuseList.size()) * sizeof(int);
    char *buff = reinterpret_cast<char*>(kxMemMgrAlocate(dataLen));
    UnionAuditSC *pUnionAuditSC = reinterpret_cast<UnionAuditSC*>(buff);
    pUnionAuditSC->result = result;
    pUnionAuditSC->agreeCount = agreeList.size();
    pUnionAuditSC->refuseCount = refuseList.size();

    int *pUserId = reinterpret_cast<int*>(pUnionAuditSC + 1);
    if (!agreeList.empty())
    {
        std::list<int>::iterator iterAgree = agreeList.begin();
        for (; iterAgree != agreeList.end(); ++iterAgree)
        {
            *pUserId = *iterAgree;
            pUserId += 1;

            CKxCommManager::getInstance()->forwardData(*iterAgree, CMD_USER, CMD_USER_FORWARD_SS,
                reinterpret_cast<char*>(&nUnionID), sizeof(int));
            // ��ͨ����˵�������ҷ��͹�����Ϣ
            CUnionHelper::sendOwnUnionInfo(*iterAgree, nUnionID);

            // ֪ͨ�������˼��빫��
            CUnionHelper::noticeUnion(nUnionID,
                NOTICE_UNION_NEWMEMBER, CModelHelper::getUserName(*iterAgree));
        }
    }

    if (!refuseList.empty())
    {
        std::list<int>::iterator iterRefuse = refuseList.begin();
        for (; iterRefuse != refuseList.end(); ++iterRefuse)
        {
            *pUserId = *iterRefuse;
            pUserId += 1;
        }
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_AUDIT_SC, buff, dataLen);
    kxMemMgrRecycle(buff, dataLen);
}

void CUnionService::processFunctionService(int uid, char *buffer, int len, IKxComm *commun)
{
    UnionFunctionSC unionFuncSC;
    unionFuncSC.result = 0;

    do 
    {
        CHECK_BREAK(len == sizeof(UnionFunctionCS));

        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_BREAK(NULL != pGameUser);

        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
		CHECK_RETURN_VOID(pUserUnionModel != NULL);
        int nUnionId = 0;
        pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionId);
        CUnionModel *pInitiativeUnion = CUnionManager::getInstane()->getUnionModel(nUnionId);
        CHECK_BREAK(NULL != pInitiativeUnion);

        UnionFunctionCS *pUnionFunctionCS = reinterpret_cast<UnionFunctionCS*>(buffer);
        // �Ƿ��ڹ���
        CHECK_BREAK(pInitiativeUnion->MemberExist(pUnionFunctionCS->userID));

        bool bRlt = false;
        int nInitiativePos = pInitiativeUnion->GetMemberPos(uid);
        int nPassivePos = pInitiativeUnion->GetMemberPos(pUnionFunctionCS->userID);
        if (UNION_FUNC_KICK == pUnionFunctionCS->funcType)
        {
            if (CUnionHelper::canKick(nInitiativePos, nPassivePos))
            {
                if (CUnionHelper::quitUnion(pUnionFunctionCS->userID) > 0)
                {
                    // ֪ͨ���������˳�����
                    CUnionHelper::noticeUnion(nUnionId,
                        NOTICE_UNION_QUIT, CModelHelper::getUserName(pUnionFunctionCS->userID));
                    bRlt = true;
                }
            }
        }
        else if (UNION_FUNC_RELIEVE == pUnionFunctionCS->funcType)
        {
            if (CUnionHelper::canRelieve(nInitiativePos, nPassivePos))
            {
                if (pInitiativeUnion->SetMemberPos(pUnionFunctionCS->userID, UNION_POS_NORMAL))
                {
                    nPassivePos = UNION_POS_NORMAL;
                    bRlt = true;
                }
            }
        }
        else if (UNION_FUNC_TRANSFER == pUnionFunctionCS->funcType)
        {
            if (CUnionHelper::canTransfer(nInitiativePos, nPassivePos))
            {
                if (pInitiativeUnion->SetMemberPos(uid, UNION_POS_NORMAL)
                    && pInitiativeUnion->SetMemberPos(pUnionFunctionCS->userID, nInitiativePos))
                {
                    nPassivePos = nInitiativePos;
                    // �᳤ת��Ȩ��,��Ҫ���û᳤ID
                    bRlt = true;
                    if (UNION_POS_CHAIRMAN == nInitiativePos)
                    {
                        if (!pInitiativeUnion->SetUnionFieldVal(UNION_FIELD_CHAIRMAN, pUnionFunctionCS->userID))
                        {
                            bRlt = false;
                        }
                        else
                        {
                            // �᳤תְ֪ͨ
                            CUnionHelper::noticeUnion(nUnionId, NOTICE_UNION_TRASFER,
                                CModelHelper::getUserName(uid), CModelHelper::getUserName(pUnionFunctionCS->userID));
                        }
                    }
                }
            }
        }
        else if (UNION_FUNC_APPOINT == pUnionFunctionCS->funcType)
        {
            int nUnionLv = 0;
            pInitiativeUnion->GetUnionFieldVal(UNION_FIELD_LV, nUnionLv);
            const UnionLevelItem *pUnionItem = queryConfUnionLevel(nUnionLv);
            if (NULL == pUnionItem)
            {
                break;
            }

            // ���᳤�����ж�
            int nViceChairman = 0;
            map<int, int>& mapMembers = pInitiativeUnion->GetMembers();
            map<int, int>::iterator iter = mapMembers.begin();
            for (; iter != mapMembers.end(); iter++)
            {
                if (UNION_POS_VICE_CHAIRMAN == iter->second)
                {
                    nViceChairman++;
                }
            }

            CHECK_BREAK(nViceChairman < pUnionItem->ViceChairmanNum);

            if (CUnionHelper::canAppoint(nInitiativePos, nPassivePos))
            {
                if (pInitiativeUnion->SetMemberPos(pUnionFunctionCS->userID, UNION_POS_VICE_CHAIRMAN))
                {
                    // �᳤�������᳤
                    CUnionHelper::noticeUnion(nUnionId, NOTICE_UNION_APPOINT,
                        CModelHelper::getUserName(uid), CModelHelper::getUserName(pUnionFunctionCS->userID));
                    nPassivePos = UNION_POS_VICE_CHAIRMAN;
                    bRlt = true;
                }
            }
        }
        else if (UNION_FUNC_RESIGN == pUnionFunctionCS->funcType)
        {
            map<int, int>& mapMembers = pInitiativeUnion->GetMembers();
            map<int, int>::iterator iter = mapMembers.find(uid);
            if (iter != mapMembers.end())
            {
                // ֻ�и��᳤�д�ְ����
                if (UNION_POS_VICE_CHAIRMAN == iter->second)
                {
                    if (pInitiativeUnion->SetMemberPos(uid, UNION_POS_NORMAL))
                    {
                        bRlt = true;
                    }
                }
            }
        }

        if (bRlt)        
        {
            CUserModel *pInitiativeUser = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
            CUnionHelper::addUnionLog(nUnionId, pUnionFunctionCS->funcType, 
                pInitiativeUser->GetName(), CModelHelper::getUserName(pUnionFunctionCS->userID));

            // ������������Ϣ֪ͨ
            if (UNION_FUNC_RESIGN != pUnionFunctionCS->funcType)
            {
                CUnionHelper::sendBeFunctionInfo(pUnionFunctionCS->userID, nUnionId, pUnionFunctionCS->funcType);
                // ���߳����˷��ʼ�
                if (UNION_FUNC_KICK == pUnionFunctionCS->funcType)
                {
                    CMailHelper::sendUnionMail(pUnionFunctionCS->userID, MAIL_CONFID_UNIONKICK, pInitiativeUnion->GetUnionName());
                }
            }

            unionFuncSC.funcType = pUnionFunctionCS->funcType;
            unionFuncSC.userID = pUnionFunctionCS->userID;
            unionFuncSC.result = 1;
        }
    } while (0);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_FUNCTION_SC
        , reinterpret_cast<char*>(&unionFuncSC), sizeof(UnionFunctionSC));
}

void CUnionService::processOutUnionListService(int uid, char *buffer, int len, IKxComm *commun)
{
    list<UnionListInfo> unionInfoList;

    do
    {
        CHECK_BREAK(len == sizeof(UnionListOutCS));
        
        const vector<int> vecUnionSort = CUnionManager::getInstane()->getUnionSort();

        UnionListOutCS *pUnionListOutCS = (UnionListOutCS *)(buffer);
        CHECK_BREAK(pUnionListOutCS->orignNum > 0 && pUnionListOutCS->orignNum <= (int)vecUnionSort.size());
        unsigned int i = pUnionListOutCS->orignNum - 1;

        UnionListInfo info;
        for (; i < vecUnionSort.size(); ++i)
        {
            CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(vecUnionSort[i]);
            if (NULL == pUnionModel)
            {
                continue;
            }

            if (SEARCH_UNION_NO_FULL == pUnionListOutCS->type)
            {
                // �Ƿ���Ա�ж�
                int nUnionLv = 0;
                pUnionModel->GetUnionFieldVal(UNION_FIELD_LV, nUnionLv);
                const UnionLevelItem *pUnionItem = queryConfUnionLevel(nUnionLv);
                if (NULL == pUnionItem || pUnionItem->MemberLimit <= (int)pUnionModel->GetMembers().size())
                {
                    continue;
                }
            }

            CUnionHelper::fillUnionInfo(pUnionModel, info);
            unionInfoList.push_back(info);

            if ((int)unionInfoList.size() >= pUnionListOutCS->count)
            {
                break;
            }
        }

    } while (0);

    int nBuffSize = sizeof(UnionSearchSC)+unionInfoList.size() * sizeof(UnionListInfo);
    char *buff = reinterpret_cast<char*>(kxMemMgrAlocate(nBuffSize));
    UnionSearchSC *pUnionSearchSC = reinterpret_cast<UnionSearchSC*>(buff);
    pUnionSearchSC->unionCount = unionInfoList.size();

    UnionListInfo *pUnionInfo = reinterpret_cast<UnionListInfo *>(pUnionSearchSC + 1);
    list<UnionListInfo>::iterator iter = unionInfoList.begin();
    for (; iter != unionInfoList.end(); iter++)
    {
        memcpy(pUnionInfo, &(*iter), sizeof(UnionListInfo));

        // ƫ��һ����λ(UnionListInfo)
        pUnionInfo += 1;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_UNIONLIST_OUT_SC, buff, nBuffSize, commun);
    kxMemMgrRecycle(buff, nBuffSize);
}

void CUnionService::processSearchService(int uid, char *buffer, int len, IKxComm *commun)
{
    list<UnionListInfo> searchList;

    do
    {
        if (len > sizeof(UnionSearchCS) || len <= 0)
        {
            break;
        }

        UnionSearchCS *pUnionSearchCS = reinterpret_cast<UnionSearchCS*>(buffer);
        CHECK_BREAK("" != pUnionSearchCS->searchName);
        
        // ��������
        CUnionHelper::searchUnion(pUnionSearchCS->searchName, searchList);

    } while (0);

    int nBuffSize = sizeof(UnionSearchSC)+searchList.size() * sizeof(UnionListInfo);
    char *buff = reinterpret_cast<char*>(kxMemMgrAlocate(nBuffSize));
    UnionSearchSC *pUnionSearchSC = reinterpret_cast<UnionSearchSC*>(buff);
    pUnionSearchSC->unionCount = searchList.size();

    UnionListInfo *pUnionInfo = reinterpret_cast<UnionListInfo *>(pUnionSearchSC + 1);
    list<UnionListInfo>::iterator iter = searchList.begin();
    for (; iter != searchList.end(); iter++)
    {
        memcpy(pUnionInfo, &(*iter), sizeof(UnionListInfo));

        // ƫ��һ����λ(UnionListInfo)
        pUnionInfo += 1;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_SEARCH_SC, buff, nBuffSize, commun);
    kxMemMgrRecycle(buff, nBuffSize);
}

void CUnionService::processEmblemService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    UnionEmblemSC unionEmblemSC;
    unionEmblemSC.emblemId = 0;

    do
    {
        CHECK_BREAK(len == sizeof(UnionEmblemCS));

        UnionEmblemCS *pUnionEmblemCS = reinterpret_cast<UnionEmblemCS*>(buffer);
        // �ж����ñ����Ƿ���
        CHECK_BREAK(hasUnionBadge(pUnionEmblemCS->emblemId));

        CUnionModel *pUnionModel = CUnionHelper::getUnionModel(uid);
        CHECK_BREAK(NULL != pUnionModel);

        int nPos = pUnionModel->GetMemberPos(uid);
        CHECK_BREAK(UNION_POS_CHAIRMAN == nPos);

        int nEmblemId = 0;
        CHECK_BREAK(pUnionModel->GetUnionFieldVal(UNION_FIELD_EMBLEM, nEmblemId));
        
        CHECK_BREAK(pUnionEmblemCS->emblemId != nEmblemId);

        if (pUnionModel->SetUnionFieldVal(UNION_FIELD_EMBLEM, pUnionEmblemCS->emblemId))
        {
            unionEmblemSC.emblemId = pUnionEmblemCS->emblemId;
        }

    } while (false);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_EMBLEM_SC,
        reinterpret_cast<char*>(&unionEmblemSC), sizeof(UnionEmblemSC));
}

void CUnionService::processUnionNameService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    UnionNameSC unionNameSC;
    unionNameSC.result = 0;

    do
    {
        CHECK_BREAK(len == sizeof(UnionNameCS));

        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_BREAK(NULL != pGameUser);

        CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
        CHECK_BREAK(NULL != pUserUnionModel && NULL != pUserModel);

        int nUnionID = 0, nDiamond = 0;
        CHECK_BREAK(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID)
            && pUserModel->GetUserFieldVal(USR_FD_DIAMOND, nDiamond, true));

        CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(nUnionID);
        CHECK_BREAK(NULL != pUserUnionModel);

        CHECK_BREAK(UNION_POS_CHAIRMAN == pUnionModel->GetMemberPos(uid));

        UnionNameCS *pUnionNameCS = reinterpret_cast<UnionNameCS*>(buffer);
        CHECK_BREAK(pUnionNameCS->unionName != pUnionModel->GetUnionName());

        // �����Ƿ�
        int nCostDiamond = queryConfUnion().ChangeNameCost;
        CHECK_BREAK(nDiamond >= nCostDiamond);
        // ����ͬ��
        CHECK_BREAK(UNION_EC_SUCCESS == CUnionHelper::checkUnionName(pUnionNameCS->unionName));

        // �۳�����
        CModelHelper::addDiamond(uid, -nCostDiamond);
		CCommOssHelper::userDiamondPayOss(uid,nCostDiamond,0,0);

        if (pUnionModel->SetUnionName(pUnionNameCS->unionName))
        {
            unionNameSC.result = 1;
        }
    } while (false);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_NAME_SC,
        reinterpret_cast<char*>(&unionNameSC), sizeof(UnionNameSC));
}

void CUnionService::processUnionNoticeService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    UnionNoticeSC unionNoticeSC;
    unionNoticeSC.result = 0;

    do
    {
        CHECK_BREAK(len >= 0 && len <= sizeof(UnionNoticeCS));

        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_BREAK(NULL != pGameUser);

        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
        CHECK_BREAK(NULL != pUserUnionModel);

        int nUnionID = 0;
        CHECK_BREAK(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID));

        CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(nUnionID);
        CHECK_BREAK(pUnionModel && pUnionModel->GetMemberPos(uid) > UNION_POS_NORMAL);

        UnionNoticeCS *pUnionNoticeCS = reinterpret_cast<UnionNoticeCS*>(buffer);
        if (pUnionModel->SetUnionNotice(pUnionNoticeCS->notice))
        {
            // ֪ͨ������¹���
            CUnionHelper::noticeUnion(nUnionID, NOTICE_UNION_NOTICEUPDATE);
            unionNoticeSC.result = 1;
        }
    } while (false);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_NOTICE_SC,
        reinterpret_cast<char*>(&unionNoticeSC), sizeof(UnionNoticeSC));
}

void CUnionService::processWelfareService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    UnionWelfareSC unionWelfareSC;
    unionWelfareSC.welfareType = -1;
    unionWelfareSC.itemCount = 0;

    do
    {
        CHECK_BREAK(len == sizeof(UnionWelfareCS));

        UnionWelfareCS *pUnionWelfareCS = reinterpret_cast<UnionWelfareCS*>(buffer);

        CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
        CHECK_BREAK(NULL != pGameUser);

        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
        CHECK_BREAK(NULL != pUserUnionModel);

        CModelHelper::updateUserUnionInfo(uid);

        int nUnionID = 0, nWelfare = 0;
        CHECK_BREAK(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID)
            && pUserUnionModel->getUserUnionValue(USER_UNION_WELFARE, nWelfare));

        CUnionModel *pUnionModel = CUnionManager::getInstane()->getUnionModel(nUnionID);
        CHECK_BREAK(NULL != pUserUnionModel);

        int nUnionLv = 0, nLiveness = 0;
        CHECK_BREAK(pUnionModel->GetUnionFieldVal(UNION_FIELD_ORIGINLV, nUnionLv)
            && pUnionModel->GetUnionFieldVal(UNION_FIELD_LIVENESS, nLiveness));

        const UnionLevelItem* pUnionLevelItem = queryConfUnionLevel(nUnionLv);
        if (NULL == pUnionLevelItem)
        {
            KXLOGERROR("config can't find unionLv %d", nUnionLv);
            break;
        }

        int dropId = 0, nNeedLiveness = 0;
        if (pUnionWelfareCS->welfareType == UNION_WELFARE_ACTIVEBOX)
        {
            nNeedLiveness = pUnionLevelItem->ActiveReward;
            dropId = pUnionLevelItem->RewardID;
        }
        else if (pUnionWelfareCS->welfareType == UNION_WELFARE_SUPTERACTIVEBOX)
        {
            nNeedLiveness = pUnionLevelItem->ActiveSReward;
            dropId = pUnionLevelItem->SRewardID;
        }
        else
        {
            break;
        }

        // �ж��Ƿ񹻻�Ծ��
        CHECK_BREAK(nNeedLiveness <= nLiveness);
        // �ж��Ƿ���ȡ��
        CHECK_BREAK(1 != (nWelfare >> pUnionWelfareCS->welfareType & 0x1));
        // ������ȡ
        CHECK_BREAK(pUserUnionModel->setUserUnionValue(USER_UNION_WELFARE, 
            nWelfare | (0x1 << pUnionWelfareCS->welfareType)));

        unionWelfareSC.welfareType = pUnionWelfareCS->welfareType;
        
        UnionWelfareSS sUnionWelfareSS;
        sUnionWelfareSS.welfareType = pUnionWelfareCS->welfareType;
        sUnionWelfareSS.dropId = dropId;

        // ת����ServiceServer��ӱ������·�
        CKxCommManager::getInstance()->forwardData(uid, CMD_UNION, CMD_UNION_WELFARE_SS,
            reinterpret_cast<char*>(&sUnionWelfareSS), sizeof(UnionWelfareSS));

        return;
    } while (false);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_WELFARE_SC,
        reinterpret_cast<char*>(&unionWelfareSC), sizeof(UnionWelfareSC));
}

void CUnionService::processSetAuditService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    UnionSetAuditSC unionSetAuditSC;
    unionSetAuditSC.result = 0;

    do 
    {
        CHECK_BREAK(len == sizeof(UnionSetAuditCS));

        CUnionModel *pUnionModel = CUnionHelper::getUnionModel(uid);
        CHECK_BREAK(NULL != pUnionModel);

        int nLimitLv = 0, nIsAuto = 0;
        CHECK_BREAK(pUnionModel->GetUnionFieldVal(UNION_FIELD_LIMITLV, nLimitLv)
            && pUnionModel->SetUnionFieldVal(UNION_FIELD_AUTOAUDIT, nIsAuto));

        CHECK_BREAK(pUnionModel->GetMemberPos(uid) > UNION_POS_NORMAL);

        UnionSetAuditCS *pUnionSetAuditCS = reinterpret_cast<UnionSetAuditCS*>(buffer);
        if (pUnionSetAuditCS->limitLv > 0 && pUnionSetAuditCS->limitLv != nLimitLv)
        {
            CHECK_BREAK(pUnionModel->SetUnionFieldVal(UNION_FIELD_LIMITLV, pUnionSetAuditCS->limitLv));
        }
        
        if (pUnionSetAuditCS->isAuto != nIsAuto)
        {
            CHECK_BREAK(pUnionModel->SetUnionFieldVal(UNION_FIELD_AUTOAUDIT, pUnionSetAuditCS->isAuto));
        }

        unionSetAuditSC.limitLv = pUnionSetAuditCS->limitLv;
        unionSetAuditSC.isAuto = pUnionSetAuditCS->isAuto;
        unionSetAuditSC.result = 1;
    } while (false);

    CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_SETAUDIT_SC, 
        reinterpret_cast<char*>(&unionSetAuditSC), sizeof(UnionSetAuditSC));
}

void CUnionService::processMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	//���ݹ���ID��UID��ȡ����Ҫ������ģ��
	CGameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(NULL != pGameUser);
	int nUnionID = 0;
	CUserUnionModel* pUserUnionModel = dynamic_cast<CUserUnionModel *>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);
	pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID);
	CHECK_RETURN_VOID(nUnionID > 0);
	CUnion* pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
    CHECK_RETURN_VOID(NULL != pUnion);

	CUnionMercenaryModel* pUnionMercenaryModel = pUnion->getMercenaryModel();
	CPersonMercenaryModel* pPersonMercenaryModel = dynamic_cast<CPersonMercenaryModel *>(pGameUser->getModel(MODELTYPE_MERCENARY));
    CHECK_RETURN_VOID(NULL != pUnionMercenaryModel && NULL != pPersonMercenaryModel);

	//������ģ����ȡ������
	map<int, UnionMercenaryInfo> mapUnionMercenaryInfo = pUnionMercenaryModel->GeUniontDispatchMercenaryInfo();
	map<int, int> mapPersonDispatchMercenaryInfo = pPersonMercenaryModel->GetPersonDispatchMercenaryInfo();
	set<int> setPersonEmpoloyedMercenaryInfo = pPersonMercenaryModel->GetPersonEmployedMercenaryInfo();

	int buffersize = sizeof(int)+mapUnionMercenaryInfo.size() * 48 + sizeof(int)+mapPersonDispatchMercenaryInfo.size()*sizeof(int)* 4 + sizeof(int)+setPersonEmpoloyedMercenaryInfo.size()*sizeof(int);
	CMemoryOperation cMemOp(buffersize);
	
	//��乫��Ӷ���б����ݣ�ֻ�·�ǰ����Ҫչʾ�Ĳ���
	cMemOp.WriteData((int)mapUnionMercenaryInfo.size());
	for (map<int, UnionMercenaryInfo>::iterator mapit = mapUnionMercenaryInfo.begin(); mapit != mapUnionMercenaryInfo.end(); ++ mapit)
	{
		char gUserName[32] = {};
		string strUserName = CModelHelper::getUserName(mapit->second.nUid);
		memcpy(gUserName, strUserName.c_str(), strUserName.length());
		cMemOp.WriteData(gUserName, 32);
		cMemOp.WriteData(mapit->first);
		cMemOp.WriteData(mapit->second.stSoldierCardData.SoldierId);
		cMemOp.WriteData(mapit->second.stSoldierCardData.SoldierLv);
		cMemOp.WriteData(mapit->second.stSoldierCardData.SoldierStar);
	}

	//������Ӷ����Ϣ
	cMemOp.WriteData((int)mapPersonDispatchMercenaryInfo.size());
	for (map<int, int>::iterator mapit = mapPersonDispatchMercenaryInfo.begin(); mapit != mapPersonDispatchMercenaryInfo.end(); ++mapit)
	{
		cMemOp.WriteData(mapit->first);
		cMemOp.WriteData(mapit->second);
		map<int, UnionMercenaryInfo>::iterator temp = mapUnionMercenaryInfo.find(mapit->second);
		if (temp != mapUnionMercenaryInfo.end())
		{
			pUnionMercenaryModel->UpdateSingleMercenary(mapit->second);
			cMemOp.WriteData(temp->second.nDispatchTime);
			UnionMercenaryInfo *pUnionMercenary = pUnionMercenaryModel->GetSingleMercenaryInfo(mapit->second);
			cMemOp.WriteData(CUnionHelper::mercenaryPrize(*pUnionMercenary));
		}
	}

	//�����˹�Ӷ����Ӷ���б���Ϣ
	cMemOp.WriteData((int)setPersonEmpoloyedMercenaryInfo.size());
	for (set<int>::iterator setit = setPersonEmpoloyedMercenaryInfo.begin(); setit != setPersonEmpoloyedMercenaryInfo.end(); ++setit)
	{
		cMemOp.WriteData(*setit);
	}

	CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_MERCENARY_SC, cMemOp.GetBuffer(), cMemOp.GetDataLenth(), commun);
}

void CUnionService::processDispatchMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len == 2 * sizeof(int));
	//���ݹ���ID��UID��ȡ����Ҫ������ģ��
	CGameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(pGameUser);
	int nUnionID = 0;
	CUserUnionModel* pUserUnionModel = dynamic_cast<CUserUnionModel *>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);
    pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID);
	CHECK_RETURN_VOID(nUnionID > 0);
	CUnion* pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
    CHECK_RETURN_VOID(NULL != pUnion);
	CUnionMercenaryModel* pUnionMercenaryModel = pUnion->getMercenaryModel();
	CPersonMercenaryModel* pPersonMercenaryModel = dynamic_cast<CPersonMercenaryModel *>(pGameUser->getModel(MODELTYPE_MERCENARY));
    CHECK_RETURN_VOID(NULL != pUnionMercenaryModel && NULL != pPersonMercenaryModel);

	//���Ӣ�ۿ�Ƭ��Ϣ
	int nHeroID = *reinterpret_cast<int *>(buffer);
	int field = *reinterpret_cast<int *>(buffer + 4);
	map<int, int>& mapPersonMercenary =  pPersonMercenaryModel->GetPersonDispatchMercenaryInfo();
	map<int, UnionMercenaryInfo>& mapUnionDispatchMercenaryInfo = pUnionMercenaryModel->GeUniontDispatchMercenaryInfo();
	for (map<int, int>::iterator mapit = mapPersonMercenary.begin(); mapit != mapPersonMercenary.end(); ++mapit)
	{
		map<int, UnionMercenaryInfo>::iterator tempMapit = mapUnionDispatchMercenaryInfo.find(mapit->second);
		if (tempMapit != mapUnionDispatchMercenaryInfo.end())
		{
			CHECK_RETURN_VOID(tempMapit->second.stSoldierCardData.SoldierId != nHeroID);
		}
	}

	CHECK_RETURN_VOID(mapPersonMercenary.find(field) == mapPersonMercenary.end());
    CHeroModel* pHeroModel = dynamic_cast<CHeroModel *>(pGameUser->getModel(MODELTYPE_HERO));
    CEquipModel *pEquipModel = dynamic_cast<CEquipModel *>(pGameUser->getModel(MODELTYPE_EQUIP));
    CHECK_RETURN_VOID(NULL != pHeroModel && NULL != pEquipModel);
	DBHeroAttr stHeroAttr;
	CHECK_RETURN_VOID(pHeroModel->GetHero(nHeroID, stHeroAttr, true));

	//���Ӣ�ۿ�Ƭ��װ����Ϣ
	SEquipInfo *pgEquipInfo[6];
	int nEquipmentCount = 0;
	for (int i = 0; i < 6; i++)
	{
		if (0 == stHeroAttr.equipId[i + 1])
		{
			pgEquipInfo[i] = NULL;
		}
		else
		{
			pgEquipInfo[i] = pEquipModel->GetEquipData(stHeroAttr.equipId[i + 1], true);
			nEquipmentCount++;
		}
	}

	//��乫��Ӷ��ģ������Ҫ�Ľṹ��
	UnionMercenaryInfo stUnionMercenaryInfo;
	stUnionMercenaryInfo.nUid = uid;
	stUnionMercenaryInfo.nDispatchTime = (int)time(NULL);
	stUnionMercenaryInfo.nRecallTime = 0;
	stUnionMercenaryInfo.nEmployedTimes = 0;
	stUnionMercenaryInfo.stSoldierCardData.SoldierId = nHeroID;
	stUnionMercenaryInfo.stSoldierCardData.SoldierLv = stHeroAttr.level;
	stUnionMercenaryInfo.stSoldierCardData.SoldierStar = stHeroAttr.star;
	stUnionMercenaryInfo.stSoldierCardData.SoldierExp = stHeroAttr.exp;
    memcpy(stUnionMercenaryInfo.stSoldierCardData.SoldierTalent, stHeroAttr.talent, sizeof(stHeroAttr.talent));
	stUnionMercenaryInfo.stSoldierCardData.EquipCnt = nEquipmentCount;
	for (int i = 0; i < 6; i++)
	{
		if (NULL == pgEquipInfo[i])
		{
			memset(&(stUnionMercenaryInfo.stEquipments[i]), 0, sizeof(SoldierEquip));
		}
		else
		{
			stUnionMercenaryInfo.stEquipments[i].confId = pgEquipInfo[i]->nItemID;
			for (int j = 0; j < 8; ++j)
			{
				stUnionMercenaryInfo.stEquipments[i].cEffectID[j] = pgEquipInfo[i]->cEffectID[j];
				stUnionMercenaryInfo.stEquipments[i].sEffectValue[j] = pgEquipInfo[i]->sEffectValue[j];
			}
		}
	}

	//��ǲӶ��
	int nMercenaryID = pUnionMercenaryModel->DispatchMercenary(stUnionMercenaryInfo);
	if (!pPersonMercenaryModel->DispatchMercenary(nMercenaryID, field))
	{
		KXLOGDEBUG("PersonMercenary dispatch error %d, %d", nMercenaryID, field);
		pUnionMercenaryModel->DelMercenary(nMercenaryID);
		return;
	}

	//��ȡ�������
	char cName[32] = {};
	string strUserName = CModelHelper::getUserName(uid);
	memcpy(cName, strUserName.c_str(), strUserName.length());

	//��䷢�͸��ͻ��˵�����
	CMemoryOperation cMemOp(sizeof(int)* 7 + 32);
	cMemOp.WriteData(UNION_MESSAGE_MERCENARYDISPATCH);
	cMemOp.WriteData(uid);
	cMemOp.WriteData(field);
	cMemOp.WriteData(cName, 32);
	cMemOp.WriteData(nMercenaryID);
	cMemOp.WriteData(nHeroID);
	cMemOp.WriteData(stUnionMercenaryInfo.stSoldierCardData.SoldierLv);
	cMemOp.WriteData(stUnionMercenaryInfo.stSoldierCardData.SoldierStar);

	//��ǲ�ɹ����㲥��������ȫ���������
	vector<int> vUnionMembers;
	pUnion->getUnionModel()->GetMembers(vUnionMembers);
	CKxCommManager::getInstance()->broadcast(vUnionMembers,
		MakeCommand(CMD_UNION, CMD_UNION_MESSAGE_SC),
		cMemOp.GetBuffer(), cMemOp.GetDataLenth());
}

void CUnionService::processRecallMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(int));
	//��ȡ����Ҫ������ģ��
	int nMercenaryID = *reinterpret_cast<int *>(buffer);
	CGameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
    CHECK_RETURN_VOID(NULL != pGameUser);

	CUserUnionModel* pUserUnionModel = dynamic_cast<CUserUnionModel *>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);
    int nUnionID = 0;
	pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID);
    CHECK_RETURN_VOID(nUnionID > 0);
	CUnion* pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
    CHECK_RETURN_VOID(NULL != pUnion);
	CUnionMercenaryModel* pUnionMercenaryModel = pUnion->getMercenaryModel();
	CPersonMercenaryModel* pPersonMercenaryModel = dynamic_cast<CPersonMercenaryModel *>(pGameUser->getModel(MODELTYPE_MERCENARY));
    CHECK_RETURN_VOID(NULL != pUnionMercenaryModel && NULL != pPersonMercenaryModel);

	//����ģ�ͻ�ȡ���ݣ��ٻ�
	pUnionMercenaryModel->UpdateSingleMercenary(nMercenaryID);
	map<int, UnionMercenaryInfo> mapUnionMercenaryInfo = pUnionMercenaryModel->GeUniontDispatchMercenaryInfo();
	map<int, UnionMercenaryInfo>::iterator mapIt = mapUnionMercenaryInfo.find(nMercenaryID);
	if (mapIt != mapUnionMercenaryInfo.end())
	{
		int gold = CUnionHelper::mercenaryPrize(mapIt->second);
		int field = pPersonMercenaryModel->GetMercenaryField(nMercenaryID);
		CHECK_RETURN_VOID(field);

		//��ǲʱ�����ٻ�ʱ�����24Сʱ�ſ����ٻء��ٻسɹ���㲥�����й���������ң�ʧ����ֻ�����ٻص����
		if (time(NULL) - mapIt->second.nDispatchTime > 86400)
		{
			CHECK_RETURN_DEBUG_VOID(pUnionMercenaryModel->RecallMercenary(nMercenaryID));
			CHECK_RETURN_DEBUG_VOID(pPersonMercenaryModel->RecallMercenary(nMercenaryID));
			CHECK_RETURN_DEBUG_VOID(CModelHelper::addGold(uid, gold));

			CMemoryOperation cMemOp(sizeof(int)* 5);
			cMemOp.WriteData(UNION_MESSAGE_MERCENARYRecall);
			cMemOp.WriteData(nMercenaryID);
			cMemOp.WriteData(gold);
			cMemOp.WriteData(uid);
			cMemOp.WriteData(field);

			vector<int> vUnionMembers;
			pUnion->getUnionModel()->GetMembers(vUnionMembers);
			CKxCommManager::getInstance()->broadcast(vUnionMembers,
				MakeCommand(CMD_UNION, CMD_UNION_MESSAGE_SC),
				cMemOp.GetBuffer(), cMemOp.GetDataLenth());
		}
		else
		{
			CMemoryOperation cMemOp(sizeof(int)* 2);
			cMemOp.WriteData(field);
			cMemOp.WriteData(gold);
			CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_RECALL_SC, cMemOp.GetBuffer(), cMemOp.GetDataLenth(), commun);
		}
	}
}

void CUnionService::processSingleMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(int));
	//��ȡ��Ҫ������ģ��
	int nMercenaryID = *reinterpret_cast<int *>(buffer);
	CGameUser* pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(pGameUser);
	int nUnionID = 0;
	CUserUnionModel* pUserUnionModel = dynamic_cast<CUserUnionModel *>(pGameUser->getModel(MODELTYPE_USERUNION));
    CHECK_RETURN_VOID(NULL != pUserUnionModel);
	pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionID);
	CHECK_RETURN_VOID(nUnionID);
	CUnion* pUnion = CUnionManager::getInstane()->getUnion(nUnionID);
    CHECK_RETURN_VOID(NULL != pUnion);
	CUnionMercenaryModel* pUnionMercenaryModel = pUnion->getMercenaryModel();
    CHECK_RETURN_VOID(NULL != pUnionMercenaryModel);

    //��ȡ��Ӷ������ϸ��Ϣ���·�����������
    UnionMercenaryInfo* pUnionMercenaryInfo = pUnionMercenaryModel->GetSingleMercenaryInfo(nMercenaryID);
	if (NULL != pUnionMercenaryInfo)
	{
		CMemoryOperation cMemOp(sizeof(SoldierCardData)+sizeof(SoldierEquip)* 6 + sizeof(int));
		cMemOp.WriteData(nMercenaryID);
		cMemOp.WriteData(pUnionMercenaryInfo->stSoldierCardData);
		cMemOp.WriteData(pUnionMercenaryInfo->stEquipments[0]);
		cMemOp.WriteData(pUnionMercenaryInfo->stEquipments[1]);
		cMemOp.WriteData(pUnionMercenaryInfo->stEquipments[2]);
		cMemOp.WriteData(pUnionMercenaryInfo->stEquipments[3]);
		cMemOp.WriteData(pUnionMercenaryInfo->stEquipments[4]);
		cMemOp.WriteData(pUnionMercenaryInfo->stEquipments[5]);

		CKxCommManager::getInstance()->sendData(uid, CMD_UNION, CMD_UNION_SINGELMER_SC, cMemOp.GetBuffer(), cMemOp.GetDataLenth(), commun);
	}
}

void CUnionService::processUnionForwardDataService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(UnionForwardSS));

    CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
    CHECK_RETURN_VOID(NULL != pGameUser);

    UnionForwardSS *pUnionForwardSS = reinterpret_cast<UnionForwardSS*>(buffer);
    if (UNION_FORWARD_PVPLIVENESS == pUnionForwardSS->type || UNION_FORWARD_STAGELIVENESS == pUnionForwardSS->type)
    {
        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
        int nUnionId = 0;
        CHECK_RETURN_VOID(pUserUnionModel->getUserUnionValue(USER_UNION_ID, nUnionId));
        if (UNION_FORWARD_PVPLIVENESS == pUnionForwardSS->type)
        {
            pUserUnionModel->alterUserUnionValueAdd(USER_UNION_DAYPVPLIVENESS, pUnionForwardSS->val);
        }
        else
        {
            pUserUnionModel->alterUserUnionValueAdd(USER_UNION_DAYSTAGELIVENESS, pUnionForwardSS->val);
        }

        CUnionHelper::addUnionLiveness(nUnionId, pUnionForwardSS->val);
    }
    else if (UNION_FORWARD_USERNAME == pUnionForwardSS->type)
    {
        CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
        pUserModel->AlterUserName(pUnionForwardSS->szExtend);
    }
    else if (UNION_FORWARD_USERLV == pUnionForwardSS->type)
    {
        CUserModel *pUserModel = dynamic_cast<CUserModel*>(pGameUser->getModel(MODELTYPE_USER));
        pUserModel->AlterUserFieldVal(USR_FD_USERLV, pUnionForwardSS->val);
    }
    else if (UNION_FORWARD_CONTRIBUTION == pUnionForwardSS->type)
    {
        CUserUnionModel *pUserUnionModel = dynamic_cast<CUserUnionModel*>(pGameUser->getModel(MODELTYPE_USERUNION));
        pUserUnionModel->alterUserUnionValueAdd(USER_UNION_CONTRIBUTION, pUnionForwardSS->val);
    }
}
