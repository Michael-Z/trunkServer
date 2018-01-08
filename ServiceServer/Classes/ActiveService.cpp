#include "ActiveService.h"
#include "ActiveHelper.h"
#include "ActiveProtocol.h"
#include "Protocol.h"
#include "ConfActive.h"
#include "GameUserManager.h"
#include "GameDef.h"
#include "PropUseHelper.h"
#include "Protocol.h"
#include "CommOssHelper.h"
#include "KxCommManager.h"
#include "ModelHelper.h"
#include "CommonHelper.h"
#include "ErrorCodeProtocol.h"

enum EActiveUIType
{
	eNormalUIType,			//��ͨ����
	eSevenDayUIType,		//7������
};

CActiveService::CActiveService()
{
}


CActiveService::~CActiveService()
{
}

void CActiveService::processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	switch (subcmd)
	{
	case CMD_ACTIVE_GET_CS:
		ProcessActiveGetService(uid, buffer, len, commun);
		break;
	case CMD_ACTIVE_SEVENDAY_CS:
		processSevenActiveGetService(uid, buffer, len, commun);
		break;
	case CMD_ACTIVE_BLUEDIMAND_CS:
		processQQActiveGetService(uid, buffer, len, commun);
		break;
	case CMD_EXCHANGE_GET_CS:
		processExChangeGetService(uid, buffer, len, commun);
		break;
	}
}

//�����̵깺��ҵ��
void CActiveService::ProcessActiveGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_DEBUG_VOID(buffer != NULL && len == sizeof(SActiveGet_CS));
	SActiveGet_CS *pMsg = (SActiveGet_CS *)buffer;
	//��ܱ�
	CConfActiveTime *pConfActiveTime = dynamic_cast<CConfActiveTime*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVE_TIME));
	CHECK_RETURN_DEBUG_VOID(pConfActiveTime != NULL);
	SConfActiveTime *pActiveTime = static_cast<SConfActiveTime *>(pConfActiveTime->getData(pMsg->sActiveID));
	CHECK_RETURN_DEBUG_VOID(pActiveTime != NULL);
	//����Ƿ���
	CHECK_RETURN_DEBUG_VOID(CActiveHelper::getInstance()->isUseFull(uid,pMsg->sActiveID));
	
	if (pActiveTime->nActiveType == SHOP_ACTIVE_TYPE)
	{
		ProcessActiveShopBuy(uid, pMsg->sActiveID, pMsg->cParamID,0, commun);
	}
	else if (pActiveTime->nActiveType == TASK_ACTIVE_TYPE)
	{
		ProcessActiveTaskGet(uid, pMsg->sActiveID, pMsg->cParamID,0,commun);
	}
	else if (pActiveTime->nActiveType == MONTHCARD_ACTIVE_TYPE)
	{
		processActiveMonthGet(uid, pMsg->sActiveID, pMsg->cParamID, commun);
	}
}

//����7����ȡ
void CActiveService::processSevenActiveGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_DEBUG_VOID(buffer != NULL && len == sizeof(SActiveGet_CS));
	SActiveGet_CS *pMsg = (SActiveGet_CS *)buffer;

	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(pGameUser != NULL);
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	CHECK_RETURN_VOID(pUserModel != NULL);
	int nCreatTime = 0;
	int nCurTime = static_cast<int>(time(NULL));
	pUserModel->GetUserFieldVal(USR_FD_CREATETIME, nCreatTime);
	int nCreateDay = CCommonHelper::getTotalDay(nCreatTime);
	int nCurDay = CCommonHelper::getTotalDay();
	int nDay = nCurDay - nCreateDay;

	//��ʼ��7��
	CConf7DayActive *pConf7DayActive = dynamic_cast<CConf7DayActive*>(
		CConfManager::getInstance()->getConf(CONF_7DAY_ACTIVE));
	CHECK_RETURN_VOID(pConf7DayActive != NULL);

	S7DayActive *pDayActive = static_cast<S7DayActive *>(pConf7DayActive->getData(nDay + 1));
	CHECK_RETURN_VOID(pDayActive != NULL);

	bool bFind = false;
	unsigned int nIndex = 0;
	for (; nIndex < pDayActive->VectActive.size(); nIndex++)
	{
		if (pMsg->sActiveID == pDayActive->VectActive[nIndex].nActiveID)
		{
			bFind = true;
			break;
		}
	}

	if (!bFind)
	{
		return;
	}

	if (pDayActive->VectActive[nIndex].nActiveType == SHOP_ACTIVE_TYPE)
	{
		ProcessActiveShopBuy(uid, pMsg->sActiveID, pMsg->cParamID,eSevenDayUIType, commun);
	}
	else if (pDayActive->VectActive[nIndex].nActiveType == TASK_ACTIVE_TYPE)
	{
		ProcessActiveTaskGet(uid, pMsg->sActiveID, pMsg->cParamID,eSevenDayUIType, commun);
	}

}

//����QQ������ȡ
void CActiveService::processQQActiveGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_DEBUG_VOID(buffer != NULL && len == sizeof(SActiveGet_CS));
	SActiveGet_CS *pMsg = (SActiveGet_CS *)buffer;

	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_VOID(pGameUser != NULL);
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	CHECK_RETURN_VOID(pUserModel != NULL);
	CBlueDiamondModel *pBDModel = dynamic_cast<CBlueDiamondModel*>(CModelHelper::getModel(uid, MODELTYPE_BDACTIVE));
	CHECK_RETURN_VOID(pBDModel != NULL);

	//���Ӧʱ�ޱ�
	CConfBDActive *pBDActive = dynamic_cast<CConfBDActive*>(
		CConfManager::getInstance()->getConf(CONF_BLUEDIAMOND_ACTIVE));
	CHECK_RETURN_DEBUG_VOID(pBDActive != NULL);

	SBDActiveData *pActiveData = pBDActive->getDBActiveData(pMsg->sActiveID, pMsg->cParamID);
	CHECK_RETURN_DEBUG_VOID(pActiveData != NULL);

	int nIndex = 0;
	if (!pBDModel->getQQActiveValue(pMsg->sActiveID, pMsg->cParamID, nIndex))
	{
		KXLOGDEBUG("%s !pBDModel->getQQActiveValue(pMsg->sActiveID, pMsg->cParamID, nIndex)", __FUNCTION__);
		return;
	}

	if (nIndex != 0)
	{
		//�Ѿ���ȡ
		KXLOGDEBUG("%s nIndex != 0", __FUNCTION__);
		return;
	}

	//��ȡ��ɫ������Ϣ
	int nBDType = 0;
	int nBDLv = 0;
	pUserModel->GetUserFieldVal(USR_FD_QQVIPTYPE, nBDType);
	pUserModel->GetUserFieldVal(USR_FD_QQVIPLV, nBDLv);

	int nBlueType = nBDType & eBDNormalType;
	int nBlueYear = nBDType & eDBYearType;
	int nLuxurType = nBDType & eDBVipType;

	switch (pActiveData->nUserLimit)
	{
		case eBDLuxuryYType:
		{
			CHECK_RETURN_DEBUG_VOID(nLuxurType == eDBVipType && nBlueYear == eDBYearType && nBlueType == eBDNormalType);
		}
		break;
		case eBDLuxuryType:
		{
			CHECK_RETURN_DEBUG_VOID(nLuxurType == eDBVipType && nBlueType == eBDNormalType);
		}
		break;
		case eBDNYearUserType:
		{
			CHECK_RETURN_DEBUG_VOID(nBlueYear == eDBYearType && nBlueType == eBDNormalType);
		}
		break;
		case eBDNormalUserType:
		{
			CHECK_RETURN_DEBUG_VOID(nBlueType == eBDNormalType);
		}
		break;
		default:
		break;
	}

	switch (pActiveData->nActiveType)
	{
		case eBDActive_IncrLvType:
		{
			int Level = 0;
			pUserModel->GetUserFieldVal(USR_FD_USERLV, Level);
			CHECK_RETURN_DEBUG_VOID(pActiveData->nConditionParam <= Level);
		}
		break;

		case eBDActive_DailyType:
		{
			if (pActiveData->nConditionParam != 0)
			{
				CHECK_RETURN_DEBUG_VOID(pActiveData->nConditionParam == nBDLv);
			}
		}
		break;
	default:
		break;
	}

	std::vector<DropItemInfo> dropItem;
	DropItemInfo ItemInfo;

	for (int i = 0; i < pActiveData->vectGoodsID.size(); i++)
	{
		if (pActiveData->vectGoodsID[i] != 0)
		{
			ItemInfo.id = pActiveData->vectGoodsID[i];
			ItemInfo.num = pActiveData->vectGoodsNum[i];
			dropItem.push_back(ItemInfo);

			if (pActiveData->vectGoodsID[i] == RESOURCEID_DIAMOND)
			{
				CCommOssHelper::userDiamondGetOss(uid, pActiveData->vectGoodsNum[i], ACTIVE_DIAMOND_GET_REASON);
			}
		}
	}

	CPropUseHelper::getInstance()->AddItems(uid, dropItem, true, ACTIVE_GAIN_REASON);
	CHECK_RETURN_DEBUG_VOID(pBDModel->setQQActiveValue(pMsg->sActiveID,pMsg->cParamID, 1));

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = pMsg->sActiveID;
	GetSC.cParamID = pMsg->cParamID;

	CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_BLUEDIMAND_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	CCommOssHelper::UserActiveOss(uid, pMsg->sActiveID, pMsg->cParamID);
}

//����һ����ȡ
void CActiveService::processExChangeGetService(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_DEBUG_VOID(buffer != NULL && len == sizeof(SActiveGet_CS));
	SActiveGet_CS *pMsg = (SActiveGet_CS *)buffer;

	CConfExChangeActive *pConfActiveExChange = dynamic_cast<CConfExChangeActive*>(
		CConfManager::getInstance()->getConf(CONF_EXCHANGE_ACTIVE));
	CHECK_RETURN_DEBUG_VOID(pConfActiveExChange != NULL);
	SConfExChangeActiveData* pActiveExChangeData = pConfActiveExChange->getConfExChangeData(pMsg->sActiveID, pMsg->cParamID);
	CHECK_RETURN_DEBUG_VOID(pActiveExChangeData != NULL);
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_DEBUG_VOID(NULL != pGameUser);
	CUserActiveModel *pUserActiveModel = (CUserActiveModel *)pGameUser->getModel(MODELTYPE_ACTIVE);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel != NULL);
	CUserModel *pUserModel = (CUserModel *)pGameUser->getModel(MODELTYPE_USER);
	CHECK_RETURN_DEBUG_VOID(pUserModel != NULL);
	CBagModel *pBagModel = (CBagModel *)pGameUser->getModel(MODELTYPE_BAG);
	CHECK_RETURN_DEBUG_VOID(pBagModel != NULL);

	//��鹺������Ƿ��㹻
	int nCurTimes = 0;
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetTaskActiveIndex(pMsg->sActiveID, pMsg->cParamID, nCurTimes));
	CHECK_RETURN_DEBUG_VOID(nCurTimes < pActiveExChangeData->nTimes);

	//��鱳������Ʒ�Ƿ��㹻
	for (unsigned int i = 0; i < pActiveExChangeData->VecGoods[0].size(); i++)
	{
		int nCurNum = 0;
		CHECK_RETURN_DEBUG_VOID(pBagModel->GetItem(pActiveExChangeData->VecGoods[0][i], nCurNum, true));
		CHECK_RETURN_DEBUG_VOID(nCurNum >= pActiveExChangeData->VecGoods[1][i]);
	}

	//����㹻��ɾ��������Ʒ
	for (unsigned int i = 0; i < pActiveExChangeData->VecGoods[0].size(); i++)
	{
		CModelHelper::removeItem(uid, pActiveExChangeData->VecGoods[0][i], pActiveExChangeData->VecGoods[1][i]);
	}

	//�����Ʒ
	//���������Ʒ
	std::vector<DropItemInfo> dropItems;
	for (unsigned int i = 0; i < pActiveExChangeData->VecRewardGoods[0].size(); i++)
	{
		DropItemInfo Info;
		Info.id		= pActiveExChangeData->VecRewardGoods[0][i];
		Info.num	= pActiveExChangeData->VecRewardGoods[1][i];
		dropItems.push_back(Info);
	}
	CPropUseHelper::getInstance()->AddItems(uid, dropItems, true, ACTIVE_GAIN_REASON);

	//�޸Ĺ������
	int nInterval = CActiveHelper::getInstance()->getActiveTimeInterval(uid, pMsg->sActiveID);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->SetTaskActiveIndex(pMsg->sActiveID,nInterval,pMsg->cParamID, nCurTimes + 1));

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = pMsg->sActiveID;
	GetSC.cParamID = pMsg->cParamID;

	CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_EXCHANGE_GET_SC,
		reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	CCommOssHelper::UserActiveOss(uid, pMsg->sActiveID, pMsg->cParamID);
}

//�����̵��������
void CActiveService::ProcessActiveShopBuy(int uid, int sActiveID, int nGiftID, int nActiveType, KxServer::IKxComm *commun)
{
	CConfActiveShop *pConfActiveShop = dynamic_cast<CConfActiveShop*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVE_SHOP));
	CHECK_RETURN_DEBUG_VOID(pConfActiveShop != NULL);
	SConfActiveShopData* pActiveShopData = pConfActiveShop->GetActiveShopData(sActiveID,nGiftID);
	CHECK_RETURN_DEBUG_VOID(pActiveShopData != NULL);
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_DEBUG_VOID(NULL != pGameUser);
	CUserActiveModel *pUserActiveModel = (CUserActiveModel *)pGameUser->getModel(MODELTYPE_ACTIVE);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel != NULL);
	CUserModel *pUserModel = (CUserModel *)pGameUser->getModel(MODELTYPE_USER);
	CHECK_RETURN_DEBUG_VOID(pUserModel != NULL);

	int nTimes = 0;
	int nMoney = 0;
	int nField = 0;
	int nRealNeed = (pActiveShopData->nPrice * pActiveShopData->nSaleRate) / 100;
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetShopActiveValue(sActiveID, nGiftID, nTimes));
	CHECK_RETURN_DEBUG_VOID(nTimes < pActiveShopData->nMaxBuyTimes)

	switch (pActiveShopData->nGoldType)
	{
		case BUY_GOLD_TYPE:					//���
		{
			nField = USR_FD_GOLD;
		}
		break;
		case BUY_DIAMOND_TYPE:				//��ʯ
		{
			nField = USR_FD_DIAMOND;
		}
		break;
		case BUY_PVP_TYPE:				//PVP��
		{
			nField = USR_FD_PVPCOIN;
		}
		break;
		case BUY_TOWER_TYPE:					//������
		{
			nField = USR_FD_TOWERCOIN;
		}
		break;
		default:
			return;
			break;
	}

	pUserModel->GetUserFieldVal(nField, nMoney, true);
	CHECK_RETURN_DEBUG_VOID(nMoney >= nRealNeed);
	//�۳�
	switch (pActiveShopData->nGoldType)
	{
		case BUY_GOLD_TYPE:					//���
		{
			CModelHelper::addGold(uid, -1 * nRealNeed);
		}
		break;
		case BUY_DIAMOND_TYPE:				//��ʯ
		{
			CModelHelper::addDiamond(uid, -1 * nRealNeed);
		}
		break;
		case BUY_PVP_TYPE:				//PVP��
		{
			CModelHelper::addPvpCoin(uid, -1 * nRealNeed);
		}
		break;
		case BUY_TOWER_TYPE:					//������
		{
			CModelHelper::addTowerCoin(uid, -1 * nRealNeed);
		}
		break;
		default:
		return;
		break;
	}

	if (pActiveShopData->nGoldType == BUY_DIAMOND_TYPE)
	{
		CCommOssHelper::userDiamondPayOss(uid, nRealNeed, NONE_PAY_TYPE,1);
	}

	int nInterval = CActiveHelper::getInstance()->getActiveTimeInterval(uid, sActiveID);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->SetShopActiveValue(sActiveID,nInterval,nGiftID, nTimes + 1));

	std::vector<DropItemInfo> dropItem;
	for (int i = 0; i < 4; i++)
	{
		if (pActiveShopData->nGoodsID[i] != 0)
		{
			DropItemInfo ItemInfo;
			ItemInfo.id = pActiveShopData->nGoodsID[i];
			ItemInfo.num = pActiveShopData->nGoodsNum[i];
			dropItem.push_back(ItemInfo);
		}
	}
	CPropUseHelper::getInstance()->AddItems(uid, dropItem, true, ACTIVE_GAIN_REASON);

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = sActiveID;
	GetSC.cParamID = nGiftID;

	if (nActiveType == eNormalUIType)
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_GET_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}
	else
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_SEVENDAY_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}

	CCommOssHelper::UserActiveOss(uid, sActiveID, nGiftID);
}

//����������������ȡ
void CActiveService::ProcessActiveTaskGet(int uid, int sActiveID, int nActiveTaskID, int nActiveType, KxServer::IKxComm *commun)
{
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_DEBUG_VOID(NULL != pGameUser);
	CUserActiveModel *pUserActiveModel = (CUserActiveModel *)pGameUser->getModel(MODELTYPE_ACTIVE);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel != NULL);
	CUserModel *pUserModel = (CUserModel *)pGameUser->getModel(MODELTYPE_USER);
	CHECK_RETURN_DEBUG_VOID(pUserModel != NULL)
	CConfActiveTask *pConfActiveTask = dynamic_cast<CConfActiveTask*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVE_TASK));
	CHECK_RETURN_DEBUG_VOID(pConfActiveTask != NULL);
	SConfActiveTaskData *pActiveTaskData = pConfActiveTask->GetTaskActiveTaskData(sActiveID, nActiveTaskID);
	CHECK_RETURN_DEBUG_VOID(pActiveTaskData != NULL);

	if (pActiveTaskData->nFinishCondition == ELA_DAILY_ENERGY)
	{
		processTimesActiveTaskGet(uid, sActiveID, nActiveTaskID, nActiveType, commun);
		return;
	}
	else if (pActiveTaskData->nFinishCondition == ELA_OLTIME)
	{
		processOLTimeActiveTaskGet(uid, sActiveID, nActiveTaskID, nActiveType, commun);
		return;
	}

	int nIndex = 0;
	int nValue = 0;
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetTaskActiveIndex(sActiveID, nActiveTaskID, nIndex));
	CHECK_RETURN_DEBUG_VOID(nIndex == 0);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetTaskActiveValue(sActiveID, nActiveTaskID, nValue, true));
	CHECK_RETURN_DEBUG_VOID(nValue >= pActiveTaskData->nConditionParam[0]);

	std::vector<DropItemInfo> dropItem;
	DropItemInfo ItemInfo;

	if (pActiveTaskData->nRewardDimand != 0)
	{
		ItemInfo.id = RESOURCEID_DIAMOND;
		ItemInfo.num = pActiveTaskData->nRewardDimand;
		dropItem.push_back(ItemInfo);
		//���ȡשʯ
		CCommOssHelper::userDiamondGetOss(uid, pActiveTaskData->nRewardDimand,ACTIVE_DIAMOND_GET_REASON);
	}

	if (pActiveTaskData->nRewardEnergy != 0)
	{
		ItemInfo.id = RESOURCEID_ENERGY;
		ItemInfo.num = pActiveTaskData->nRewardEnergy;
		dropItem.push_back(ItemInfo);
	}

	if (pActiveTaskData->nRewardGold != 0)
	{
		ItemInfo.id = RESOURCEID_GOLD;
		ItemInfo.num = pActiveTaskData->nRewardGold;
		dropItem.push_back(ItemInfo);
	}

	for (int i = 0; i < 4; i++)
	{
		if (pActiveTaskData->nRewardGoodsID[i] != 0)
		{
			ItemInfo.id = pActiveTaskData->nRewardGoodsID[i];
			ItemInfo.num = pActiveTaskData->nRewardGoodsNum[i];
			dropItem.push_back(ItemInfo);

			if(pActiveTaskData->nRewardGoodsID[i] == RESOURCEID_DIAMOND)
			{
				CCommOssHelper::userDiamondGetOss(uid, pActiveTaskData->nRewardGoodsNum[i], ACTIVE_DIAMOND_GET_REASON);
			}
		}
	}

	CPropUseHelper::getInstance()->AddItems(uid, dropItem, true, ACTIVE_GAIN_REASON);
	int nInterval = CActiveHelper::getInstance()->getActiveTimeInterval(uid, sActiveID);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->SetTaskActiveIndex(sActiveID,nInterval,nActiveTaskID, 1));

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = sActiveID;
	GetSC.cParamID = nActiveTaskID;

	if (nActiveType == eNormalUIType)
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_GET_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}
	else
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_SEVENDAY_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}

	if (pActiveTaskData->nActiveTaskType == ACTIVE_TASK_GROWUP_TYPE)
	{
		CModelHelper::DispatchActionEvent(uid, ELA_FINISHACTIVETASK, &pActiveTaskData->nActiveTaskType, sizeof(pActiveTaskData->nActiveTaskType));
	}

	CCommOssHelper::UserActiveOss(uid, sActiveID, nActiveTaskID);
}

//�����¿���е��¿�������ȡ
void CActiveService::processActiveMonthGet(int uid, int sActiveID, int nMonthCardID, KxServer::IKxComm *commun)
{
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_DEBUG_VOID(NULL != pGameUser);
	CUserActiveModel *pUserActiveModel = (CUserActiveModel *)pGameUser->getModel(MODELTYPE_ACTIVE);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel != NULL);
	CUserModel *pUserModel = (CUserModel *)pGameUser->getModel(MODELTYPE_USER);
	CHECK_RETURN_DEBUG_VOID(pUserModel != NULL);
	const SConfActiveTime *pConfActiveTime = queryActiveTimeData(sActiveID);
	CHECK_RETURN_DEBUG_VOID(pConfActiveTime != NULL);
	const SConfMonthActiveData *pMonthData = queryMonthActiveData(sActiveID, nMonthCardID);
	CHECK_RETURN_DEBUG_VOID(pMonthData != NULL);

	int nCurTime = static_cast<int>(time(NULL));
	int nMonthStamp = 0;
	CUserActiveModel::getMonthCardValue(uid,pMonthData->nPid, nMonthStamp);
	if (nMonthStamp == 0)
	{
		//û�й������ÿ�
		ErrorCodeData CodeData; //����쳣 ���ʹ�����
		CodeData.nCode = ERROR_MONTHCARD_NOTEXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
	}

	//�ж����ղ�����ȡ
	time_t tMonthStamp = (time_t)nMonthStamp;
	int nMonthStampDay = CCommonHelper::getTotalDay(tMonthStamp);
	int nCurDay = CCommonHelper::getTotalDay();
	if (nCurDay == nMonthStampDay)
	{
		//�¿����ղ�����ȡ
		ErrorCodeData CodeData; //����쳣 ���ʹ�����
		CodeData.nCode = ERROR_CURDAY_CANTGET;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
	}

	//�ж�ʱ���¿��Ĺ������
	if (pMonthData->nDayLimit > 0)
	{
		if (nMonthStamp + pMonthData->nDayLimit * 24*3600 < nCurTime)
		{
			//�¿�����
			ErrorCodeData CodeData; //����쳣 ���ʹ�����
			CodeData.nCode = ERROR_MONTHCAR_OUTDATE;
			CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
			return;
		}
	}

	//�ж�������û����ȡ
	int nRwardTime = 0;
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetTaskActiveValue(sActiveID,nMonthCardID, nRwardTime, true));
	if (nRwardTime != 0)
	{
		int nRewardDay = CCommonHelper::getTotalDay(nRwardTime);
		if (nRewardDay >= nCurDay)
		{
			//�Ѿ���ȡ����
			return;
		}
	}

	//��ӻ���
	CHECK_RETURN_DEBUG_VOID(CModelHelper::addDiamond(uid, pMonthData->nDiamondReward));
	//���ȡשʯ
	CCommOssHelper::userDiamondGetOss(uid, pMonthData->nDiamondReward, ACTIVE_DIAMOND_GET_REASON);
	//���û�����ȡʱ��
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->SetTaskActiveValue(sActiveID,0, nMonthCardID, nCurTime));

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = sActiveID;
	GetSC.cParamID = nMonthCardID;
	CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_GET_SC,
		reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	CCommOssHelper::UserActiveOss(uid, sActiveID, nMonthCardID);
}


//����������ʱ��������ȡ
void CActiveService::processTimesActiveTaskGet(int uid, int sActiveID, int nActiveTaskID, int nActiveType, KxServer::IKxComm *commun)
{
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_DEBUG_VOID(NULL != pGameUser);
	CUserActiveModel *pUserActiveModel = (CUserActiveModel *)pGameUser->getModel(MODELTYPE_ACTIVE);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel != NULL);
	CUserModel *pUserModel = (CUserModel *)pGameUser->getModel(MODELTYPE_USER);
	CHECK_RETURN_DEBUG_VOID(pUserModel != NULL);
	CConfActiveTask *pConfActiveTask = dynamic_cast<CConfActiveTask*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVE_TASK));
	CHECK_RETURN_DEBUG_VOID(pConfActiveTask != NULL);
	SConfActiveTaskData *pActiveTaskData = pConfActiveTask->GetTaskActiveTaskData(sActiveID, nActiveTaskID);
	CHECK_RETURN_DEBUG_VOID(pActiveTaskData != NULL);
	CHECK_RETURN_DEBUG_VOID(pActiveTaskData->nFinishCondition == ELA_DAILY_ENERGY);

	//�ж�ʱ���Ƿ�ﵽ
	time_t nCurStamp = time(NULL);
	tm curTm;
	curTm = *(localtime(&nCurStamp));
	int nCurHour = curTm.tm_hour % 24;
	CHECK_RETURN_DEBUG_VOID(nCurHour >= pActiveTaskData->nConditionParam[0]);
	int nIndex = 0;
	if (!pUserActiveModel->GetTaskActiveIndex(sActiveID, nActiveTaskID, nIndex))
	{
		KXLOGDEBUG("%s !pUserActiveModel->GetTaskActiveIndex(sActiveID, nActiveTaskID, nIndex)", __FUNCTION__);
		return;
	}

	if (nIndex != 0)
	{
		//�Ѿ���ȡ
		KXLOGDEBUG("%s nIndex != 0", __FUNCTION__);
		return;
	}

	std::vector<DropItemInfo> dropItem;
	DropItemInfo ItemInfo;
	if (pActiveTaskData->nRewardDimand != 0)
	{
		ItemInfo.id = RESOURCEID_DIAMOND;
		ItemInfo.num = pActiveTaskData->nRewardDimand;
		dropItem.push_back(ItemInfo);
		//���ȡשʯ
		CCommOssHelper::userDiamondGetOss(uid, ACTIVE_DIAMOND_GET_REASON, pActiveTaskData->nRewardDimand);
	}

	if (pActiveTaskData->nRewardEnergy != 0)
	{
		ItemInfo.id = RESOURCEID_ENERGY;
		ItemInfo.num = pActiveTaskData->nRewardEnergy;
		dropItem.push_back(ItemInfo);
	}

	if (pActiveTaskData->nRewardGold != 0)
	{
		ItemInfo.id = RESOURCEID_GOLD;
		ItemInfo.num = pActiveTaskData->nRewardGold;
		dropItem.push_back(ItemInfo);
	}

	for (int i = 0; i < 4; i++)
	{
		if (pActiveTaskData->nRewardGoodsID[i] != 0)
		{
			ItemInfo.id = pActiveTaskData->nRewardGoodsID[i];
			ItemInfo.num = pActiveTaskData->nRewardGoodsNum[i];
			dropItem.push_back(ItemInfo);
			if (pActiveTaskData->nRewardGoodsID[i] == RESOURCEID_DIAMOND)
			{
				CCommOssHelper::userDiamondGetOss(uid, pActiveTaskData->nRewardGoodsNum[i], ACTIVE_DIAMOND_GET_REASON);
			}
		}
	}

	CPropUseHelper::getInstance()->AddItems(uid, dropItem, true, ACTIVE_GAIN_REASON);
	int nInterval = CActiveHelper::getInstance()->getActiveTimeInterval(uid, sActiveID);
	if (!pUserActiveModel->SetTaskActiveIndex(sActiveID,nInterval,nActiveTaskID, 1))
	{
		KXLOGDEBUG("%s !pUserActiveModel->SetTaskActiveIndex(sActiveID, nActiveTaskID,1)", __FUNCTION__);
		return;
	}

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = sActiveID;
	GetSC.cParamID = nActiveTaskID;

	if (nActiveType == eNormalUIType)
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_GET_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}
	else
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_SEVENDAY_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}

	if (pActiveTaskData->nActiveTaskType == ACTIVE_TASK_GROWUP_TYPE)
	{
		CModelHelper::DispatchActionEvent(uid, ELA_FINISHACTIVETASK, &pActiveTaskData->nActiveTaskType, sizeof(pActiveTaskData->nActiveTaskType));
	}

	CCommOssHelper::UserActiveOss(uid, sActiveID, nActiveTaskID);
}

//��������ʱ���Ļ����
void CActiveService::processOLTimeActiveTaskGet(int uid, int sActiveID, int nActiveTaskID, int nActiveType, KxServer::IKxComm *commun)
{
	CGameUser *pGameUser = CGameUserManager::getInstance()->getGameUser(uid);
	CHECK_RETURN_DEBUG_VOID(pGameUser != NULL);
	CUserActiveModel *pUserActiveModel = (CUserActiveModel *)pGameUser->getModel(MODELTYPE_ACTIVE);
	CUserModel *pUserModel = (CUserModel *)pGameUser->getModel(MODELTYPE_USER);
	CHECK_RETURN_DEBUG_VOID(pUserModel != NULL)

	CConfActiveTask *pConfActiveTask = dynamic_cast<CConfActiveTask*>(
		CConfManager::getInstance()->getConf(CONF_ACTIVE_TASK));
	CHECK_RETURN_DEBUG_VOID(pConfActiveTask != NULL);
	SConfActiveTaskData *pActiveTaskData = pConfActiveTask->GetTaskActiveTaskData(sActiveID, nActiveTaskID);
	CHECK_RETURN_DEBUG_VOID(pActiveTaskData != NULL)
	CHECK_RETURN_DEBUG_VOID(pActiveTaskData->nFinishCondition == ELA_OLTIME)
	CHECK_RETURN_DEBUG_VOID(pActiveTaskData->nActiveTaskType != ACTIVE_TASK_GROWUP_TYPE)
	
	int nIndex = 0;
	int nValue = 0;
	int nLoginTime = 0;
	CHECK_RETURN_DEBUG_VOID(pUserModel->GetUserFieldVal(USR_FD_LOGINTIME, nLoginTime));
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetTaskActiveIndex(sActiveID, nActiveTaskID, nIndex));
	CHECK_RETURN_DEBUG_VOID(nIndex == 0);
	CHECK_RETURN_DEBUG_VOID(pUserActiveModel->GetTaskActiveValue(sActiveID, nActiveTaskID, nValue, true));
	nValue = nValue + static_cast<int>(time(NULL)) - nLoginTime;
	CHECK_RETURN_DEBUG_VOID(nValue >= pActiveTaskData->nConditionParam[0]);

	std::vector<DropItemInfo> dropItem;
	DropItemInfo ItemInfo;
	if (pActiveTaskData->nRewardDimand != 0)
	{
		ItemInfo.id = RESOURCEID_DIAMOND;
		ItemInfo.num = pActiveTaskData->nRewardDimand;
		dropItem.push_back(ItemInfo);
		//���ȡשʯ
		CCommOssHelper::userDiamondGetOss(uid, ACTIVE_DIAMOND_GET_REASON, pActiveTaskData->nRewardDimand);
	}

	if (pActiveTaskData->nRewardEnergy != 0)
	{
		ItemInfo.id = RESOURCEID_ENERGY;
		ItemInfo.num = pActiveTaskData->nRewardEnergy;
		dropItem.push_back(ItemInfo);
	}

	if (pActiveTaskData->nRewardGold != 0)
	{
		ItemInfo.id = RESOURCEID_GOLD;
		ItemInfo.num = pActiveTaskData->nRewardGold;
		dropItem.push_back(ItemInfo);
	}

	for (int i = 0; i < 4; i++)
	{
		if (pActiveTaskData->nRewardGoodsID[i] != 0)
		{
			ItemInfo.id = pActiveTaskData->nRewardGoodsID[i];
			ItemInfo.num = pActiveTaskData->nRewardGoodsNum[i];
			dropItem.push_back(ItemInfo);
			if (pActiveTaskData->nRewardGoodsID[i] == RESOURCEID_DIAMOND)
			{
				CCommOssHelper::userDiamondGetOss(uid, pActiveTaskData->nRewardGoodsNum[i], ACTIVE_DIAMOND_GET_REASON);
			}
		}
	}

	CPropUseHelper::getInstance()->AddItems(uid, dropItem, true, ACTIVE_GAIN_REASON);
	int nInterval = CActiveHelper::getInstance()->getActiveTimeInterval(uid, sActiveID);
	if (!pUserActiveModel->SetTaskActiveIndex(sActiveID,nInterval,nActiveTaskID, 1))
	{
		KXLOGDEBUG("%s !pUserActiveModel->SetTaskActiveIndex(sActiveID, nActiveTaskID,1)", __FUNCTION__);
		return;
	}

	SActiveGet_SC GetSC;
	GetSC.cFlag = 1;
	GetSC.sActiveID = sActiveID;
	GetSC.cParamID = nActiveTaskID;

	if (nActiveType == eNormalUIType)
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_GET_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}
	else
	{
		CKxCommManager::getInstance()->sendData(uid, CMD_ACTIVE, CMD_ACTIVE_SEVENDAY_SC,
			reinterpret_cast<char *>(&GetSC), sizeof(GetSC));
	}

	if (pActiveTaskData->nActiveTaskType == ACTIVE_TASK_GROWUP_TYPE)
	{
		CModelHelper::DispatchActionEvent(uid, ELA_FINISHACTIVETASK, &pActiveTaskData->nActiveTaskType, sizeof(pActiveTaskData->nActiveTaskType));
	}

	CCommOssHelper::UserActiveOss(uid, sActiveID, nActiveTaskID);
}


