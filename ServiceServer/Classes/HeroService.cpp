#include "HeroService.h"
#include <vector>

#include "HeroModel.h"
#include "UserModel.h"
#include "BagModel.h"
#include "EquipModel.h"

#include "ConfGameSetting.h"
#include "ConfRole.h"

#include "KxMemPool.h"
#include "TimeCalcTool.h"
#include "KxLog.h"
#include "ServiceDef.h"

#include "ModelHelper.h"
#include "CommOssHelper.h"
#include "KxCommManager.h"
#include "GameDef.h"
#include "ItemDrop.h"
#include "UserActionListener.h"

#include "ErrorCodeProtocol.h"
#include "LoginProtocol.h"
#include "Protocol.h"
#include "NoticeProtocol.h"
#include "HeroProtocol.h"
#include "NoticeHelper.h"
#include "CommonHelper.h"

using namespace KxServer;
using namespace std;

#define LOTTERY_N_RESET 10        // �齱��N=10������
#define UPGRADE_MATERIAL_COUNT 6  // ����������
#define UPGRADE_LIMIT_LV 15       // �ȼ�����

// �����������
static const float ExpMultiple[4] = { 1.0f, 1.2f, 1.5f, 2.0f };

// �ٷֱȸ����Ƿ񴥷�
bool PercentProbablity(int percent)
{
    int nTotal = 100;
    int nRand = rand() % nTotal;
    if (nRand <= percent)
    {
        return true;
    }

    return false;
}

//���뵱ǰ�ȼ�, �͵�ǰ�ܾ���, ����֮������ȼ�
void convertExpToLevel(int &finalLv, int &remainExp, int limitLv)
{
	const SoldierLevelSettingItem *pUpgradeConf = queryConfSoldierLevelSetting(finalLv + 1);
	if (finalLv >= limitLv)
	{
		return;
	}

	while (remainExp >= pUpgradeConf->Exp)
	{
		remainExp -= pUpgradeConf->Exp;
		finalLv += 1;
		if (finalLv >= limitLv)
		{
			return;
		}
		pUpgradeConf = queryConfSoldierLevelSetting(finalLv + 1);
		CHECK_BREAK(NULL != pUpgradeConf);
	}
}

void CHeroService::processService(int subcmd, int uid, char *buffer, int len, IKxComm *commun)
{
    switch (subcmd)
    {
    case CMD_HERO_BUYCARD_CS:
        ProcessHeroBuy(uid, buffer, len, commun);
        break;
	case CMD_HERO_GEN_CS:
		ProcessHeroGen(uid, buffer, len, commun);
		break;
    case CMD_HERO_UPGRADE_CS:
        ProcessHeroUpgrade(uid, buffer, len, commun);
        break;
    case CMD_HERO_UPSTAR_CS:
        ProcessHeroUpgradeStar(uid, buffer, len, commun);
        break;
    case CMD_HERO_EQUIP_CS:
		ProcessHeroEquip(uid, buffer, len, commun);
        break;
    case CMD_HERO_ACTIVETALENT_CS:
        ProcessHeroTalent(uid, buffer, len, commun);
        break;
    default:
        break;
    }
}

void CHeroService::ProcessHeroBuy(int uid, char *buffer, int len, IKxComm *commun)
{
    CHECK_RETURN_VOID(len == sizeof(HeroBuyCardCS));
	HeroBuyCardCS *pHeroBuyCard = reinterpret_cast<HeroBuyCardCS*>(buffer);

	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
    CHECK_RETURN_VOID(NULL != pUserModel && NULL != pHeroModel);

    // ��Ҫ������ʯ
	int nHaveDiamond = 0;
	int nCostDiamond = 0;
    if (!pUserModel->GetUserFieldVal(USR_FD_DIAMOND, nHaveDiamond))
    {
        KXLOGERROR("uid %d ProcessHeroBuy1 GetUserFieldVal fail!", uid);
        return;
    }

	// ��ȡ���Ŀ�Ƭ�б�
	std::vector<CardGambleItem*> gambleCards;
	// �������
	const CardGambleSettingItem &cardGambleConf = queryConfCardGambleSetting();
    CardGambleItem *pCardGambleItem = NULL;

	int OssPayFor = 0;
    int OssCost = 0;
	int nInfo = 0;
    int nLotteryCount = LOTTERY_1 == pHeroBuyCard->buyType ? 1 : 10;
    // ����(�齱)����,��������
    if (LOTTERY_1 == pHeroBuyCard->buyType)
    {
		// �״γ鿨��Ƭ����
		CardGambleItem firstCard;

		time_t nowStamp = time(NULL);
		int nCostFlashcard = 0;
        int nFreeHeroTimes = 0;

		std::map<int, int> usrValues;
		usrValues[USR_FD_FLASHCARD] = 0;
		usrValues[USR_FD_EXTRAHEROTIMES] = 0;
		usrValues[USR_FD_EXTRAHERTOTALTIMES] = 0;
		usrValues[USR_FD_FREEHEROTIMES] = 0;

		if (!pUserModel->GetUserFieldVal(usrValues))
        {
            KXLOGERROR("uid %d ProcessHeroBuy2 GetUserFieldVal fail!", uid);
            return;
        }
		nFreeHeroTimes = usrValues[USR_FD_FREEHEROTIMES];

		// ����ÿ�����
		if (nFreeHeroTimes <= 0)
		{
			if (usrValues[USR_FD_FLASHCARD] > 0)
			{
				// ����г鿨ȯ
				nCostFlashcard = 1;
			}
			else
			{
				nCostDiamond = cardGambleConf.DiamondCardGamblePrice;
				// ��ʯ����
				if (nCostDiamond > nHaveDiamond)
				{
					ErrorCodeData CodeData;
					CodeData.nCode = ERROR_HERO_BAGNOTENOUGH;
					CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
						(char*)&CodeData, sizeof(CodeData));
					return;
				}
			}
		}
		//�׳�
		if (usrValues[USR_FD_EXTRAHERTOTALTIMES] == 0 && cardGambleConf.FirstDrawCard.size() >= 2)
		{
			usrValues[USR_FD_EXTRAHEROTIMES] += 1;
			firstCard.ID = cardGambleConf.FirstDrawCard[0];
			firstCard.Star = cardGambleConf.FirstDrawCard[1];
            firstCard.Rare = 0;
            firstCard.Rate = 0;
            firstCard.Ratio = 0;
			pCardGambleItem = &firstCard;
		}
		else if (LOTTERY_N_RESET <= usrValues[USR_FD_EXTRAHEROTIMES] + 1)
		{
			// ע�����ݿ��¼������һ��, ���� + 1
			// ������Ϊ��10��, ����
			usrValues[USR_FD_EXTRAHEROTIMES] = 0;
			pCardGambleItem = CItemDrop::RandHeroCard(1);
		}
		else
		{
			usrValues[USR_FD_EXTRAHEROTIMES] += 1;
			pCardGambleItem = CItemDrop::RandHeroCard(0);
		}

        if (NULL == pCardGambleItem)
        {
			KXLOGERROR("uid %d ProcessHeroBuy4 pCardGambleItem is nil TotalTimes %d", uid, usrValues[USR_FD_EXTRAHERTOTALTIMES]);
            return;
        }
        gambleCards.push_back(pCardGambleItem);
		
		usrValues[USR_FD_EXTRAHERTOTALTIMES] += 1;

		if (!pUserModel->SetUserFieldVal(USR_FD_EXTRAHEROTIMES, usrValues[USR_FD_EXTRAHEROTIMES])
			|| !pUserModel->SetUserFieldVal(USR_FD_EXTRAHERTOTALTIMES, usrValues[USR_FD_EXTRAHERTOTALTIMES]))
        {
            KXLOGERROR("uid %d ProcessHeroBuy5 nLotteryTotalTimes %d nLotteryTimes %d",
				uid, usrValues[USR_FD_EXTRAHEROTIMES], usrValues[USR_FD_EXTRAHERTOTALTIMES]);
            return;
        }

		//�۳�����, �����, ��鿨ȯ, �������ʯ
		if (nFreeHeroTimes > 0)
		{
			if (!pUserModel->SetUserFieldVal(USR_FD_FREEHEROTIMES, nFreeHeroTimes - 1))
			{
				KXLOGERROR("uid %d ProcessHeroBuy3 SetUserFieldVal fail", uid);
				return;
			}
		}
		else if (nCostFlashcard > 0)
		{
			CModelHelper::addFlashCard(uid, -1 * nCostFlashcard);
		}
		else if (nCostDiamond > 0)
		{
			CModelHelper::addDiamond(uid, -1 * nCostDiamond);
		}

		OssPayFor = SINGLE_FLOOP_CARD_TYPE;
		OssCost = nCostDiamond;
		nInfo = pCardGambleItem->ID;
    }
    else if (LOTTERY_10 == pHeroBuyCard->buyType)
    {
		int nHaveFlashcard10 = 0;
		int nCostFlashcard10 = 0;

        if (!pUserModel->GetUserFieldVal(USR_FD_FLASHCARD10, nHaveFlashcard10))
        {
            KXLOGERROR("uid %d ProcessHeroBuy6 get field fail!", uid);
            return;
        }
		if (nHaveFlashcard10 > 0)
		{
			nCostFlashcard10 = 1;
		}
		else
		{
			// 10��۸�
			nCostDiamond = cardGambleConf.DiamondCardGamble10Price;
			CHECK_RETURN_VOID(nHaveDiamond >= nCostDiamond);
		}

        // A1��ʽ9��,A2��ʽ1��->�ص��������Ͽ�Ƭ
        for (int i = 0; i < 9; i++)
        {
            pCardGambleItem = CItemDrop::RandHeroCard(0);
            CHECK_RETURN_VOID(NULL != pCardGambleItem);
            gambleCards.push_back(pCardGambleItem);
        }

        pCardGambleItem = CItemDrop::RandHeroCard(1);
        CHECK_RETURN_VOID(NULL != pCardGambleItem);
        gambleCards.push_back(pCardGambleItem);
		// ����һ��
		if (nLotteryCount != (int)gambleCards.size())
		{
			KXLOGERROR("uid %d gamblesize not equal to nLotteryCount %d", uid, nLotteryCount);
			return;
		}
		// �۳�����
		if (nCostFlashcard10 > 0)
		{
			CModelHelper::addFlashCard10(uid, -1 * nCostFlashcard10);
		}
		else if (nCostDiamond > 0)
		{
			CModelHelper::addDiamond(uid, -1 * nCostDiamond);
		}

		OssPayFor = TEN_FLOOP_CARD_TYPE;
		OssCost = nCostDiamond;
    }
	// �鿨�¼�,�鿨����
	CModelHelper::DispatchActionEvent(uid, ELA_DRAW_CARD, &nLotteryCount, sizeof(int));
	CCommOssHelper::userDiamondPayOss(uid, OssCost, OssPayFor, 1, nInfo);

    // �㲥��Ʒ�ʿ�Ƭ
    map<int, CardGambleItem*> qualityCard;
    // ��ӿ�Ƭ
	int nSendLen = sizeof(HeroBuyCardSC)+sizeof(HeroBuyCardInfo)* nLotteryCount;
    char *pSendData = static_cast<char*>(kxMemMgrAlocate(nSendLen));

	HeroBuyCardSC *pHeroBuyCardSC = reinterpret_cast<HeroBuyCardSC*>(pSendData);
    pHeroBuyCardSC->cardCount = 0;

	HeroBuyCardInfo *heroInfo = reinterpret_cast<HeroBuyCardInfo*>(pHeroBuyCardSC + 1);
	for (vector<CardGambleItem*>::iterator iter = gambleCards.begin(); 
		iter != gambleCards.end(); ++iter)
    {
		CardGambleItem *pCardGambleItem = *iter;
        if (pCardGambleItem->Rare >= RARITY_GOLDEN && !CModelHelper::isHeroCompletion(uid, pCardGambleItem->ID))
        {
            qualityCard[pCardGambleItem->ID] = pCardGambleItem;
        }

		if (CModelHelper::addHero(uid, pCardGambleItem->ID, pCardGambleItem->Star, 1))
		{
			pHeroBuyCardSC->cardCount += 1;
			heroInfo->heroId = pCardGambleItem->ID;
			heroInfo->heroStar = pCardGambleItem->Star;
			heroInfo += 1;
			CCommOssHelper::userGoodsGainOss(uid, FLOPCARD_GAIN_REASON,pCardGambleItem->ID, 1);
		}
    }
    // �·���Ϣ
    CKxCommManager::getInstance()->sendData(uid, CMD_HERO, CMD_HERO_BUYCARD_SC, pSendData, nSendLen);
	kxMemMgrRecycle(pSendData, nSendLen);

    // �㲥ȫ���鿨
    if (!qualityCard.empty())
    {
        int len = sizeof(NoticeSC)+qualityCard.size()*
            CNoticeHelper::getStringIntIntInfoLen(pUserModel->GetName().length() + 1);
        char *buff = reinterpret_cast<char*>(KxServer::kxMemMgrAlocate(len));
        NoticeSC *pNoticeSC = reinterpret_cast<NoticeSC*>(buff);
        pNoticeSC->noticeCount = 0;

        int nOffset = sizeof(NoticeSC);
        map<int, CardGambleItem*>::iterator iter = qualityCard.begin();
        for (; iter != qualityCard.end(); ++iter)
        {
            nOffset += CNoticeHelper::getStringIntIntNoticeInfo(buff + nOffset,
                iter->second->Rare == RARITY_GOLDEN ? NOTICE_USER_DRAWCARD_GOLD : NOTICE_USER_DRAWCARD_PURPLE,
                pUserModel->GetName(), iter->second->ID, iter->second->Star);
            ++pNoticeSC->noticeCount;
        }

        CNoticeHelper::noticeAll(uid, buff, nOffset, commun);
        KxServer::kxMemMgrRecycle(buff, len);
    }
}

void CHeroService::ProcessHeroGen(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
	CHECK_RETURN_VOID(NULL != buffer && len == sizeof(HeroGenCS));

	HeroGenCS *pHeroGenCS = reinterpret_cast<HeroGenCS*>(buffer);
	int heroId = pHeroGenCS->heroId;

	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
	CHECK_RETURN_VOID(NULL != pHeroModel);

	DBHeroAttr heroAttr;
	if (!pHeroModel->GetHero(heroId, heroAttr))
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_NOTEXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
			(char*)&CodeData, sizeof(CodeData));
		return;
	}
	//����������
	if (CModelHelper::isHeroCompletion(uid, heroId))
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_COMPLETE;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
			(char*)&CodeData, sizeof(CodeData));
		return;
	}

	const SoldierUpRateItem *pHeroGenConf = queryConfSoldierUpRateItem(heroId);
	CHECK_RETURN_VOID(NULL != pHeroGenConf);
	const SoldierStarSettingItem *pHeroFirstConf = queryConfSoldierStarSetting(pHeroGenConf->DefaultStar);
	CHECK_RETURN_VOID(NULL != pHeroFirstConf);
	if (heroAttr.fragment < pHeroFirstConf->TurnCardCount)
	{
		// ��Ƭ����
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_FRAGNOTENOUGH;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
			(char*)&CodeData, sizeof(CodeData));
		return;
	}
	
	heroAttr.level = 1;
	heroAttr.star = pHeroGenConf->DefaultStar;
	heroAttr.fragment -= pHeroFirstConf->TurnCardCount;

	if (!pHeroModel->UpdateHero(heroId, heroAttr))
	{
		KXLOGERROR("ProcessHeroGen update hero info error! uid %d, heroId %d", uid, heroId);
		return;
	}

	LoginHeroInfo heroInfo;
	heroInfo.heroId = heroId;
	heroInfo.heroFrag = heroAttr.fragment;
	heroInfo.heroLv = heroAttr.level;
	heroInfo.heroStar = heroAttr.star;
	heroInfo.heroExp = heroAttr.exp;
    memcpy(heroInfo.heroTalent, heroAttr.talent, sizeof(heroInfo.heroTalent));
	for (size_t i = 0; i < 6; ++i)
	{
		heroInfo.equip[i] = heroAttr.equipId[i+1];
	}
	
	CKxCommManager::getInstance()->sendData(uid, CMD_HERO, CMD_HERO_GEN_SC,
		reinterpret_cast<char*>(&heroInfo), sizeof(heroInfo));
}

void CHeroService::ProcessHeroEquip(int uid, char *buffer, int len, IKxComm *commun)
{
    CHECK_RETURN_VOID(NULL != buffer && len >= sizeof(HeroEquipCS));

    // Model
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
	CBagModel *pBagModel = dynamic_cast<CBagModel*>(CModelHelper::getModel(uid, MODELTYPE_BAG));
	CEquipModel *pEquipModel = dynamic_cast<CEquipModel*>(CModelHelper::getModel(uid, MODELTYPE_EQUIP));
    if (NULL == pUserModel || NULL == pHeroModel || NULL == pBagModel || NULL == pEquipModel)
    {
        return;
    }

    HeroEquipCS *pHeroEquipCS = reinterpret_cast<HeroEquipCS*>(buffer);
    CHECK_RETURN_VOID(len == (sizeof(HeroEquipCS) + pHeroEquipCS->equipCnt*sizeof(HeroEquipInfo)));

	int heroId = pHeroEquipCS->heroId;
    DBHeroAttr heroAttr;
    // ��Ӣ���Ƿ����
	if (!pHeroModel->GetHero(heroId, heroAttr))
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_NOTEXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, 
			(char*)&CodeData, sizeof(CodeData));
		return;
	}

	const SoldierConfItem *pSoldierConf = queryConfSoldier(heroId, heroAttr.star);
	CHECK_RETURN_VOID(pSoldierConf);

    std::map<int, int> successEquips;
    HeroEquipInfo *pEquipInfo = reinterpret_cast<HeroEquipInfo*>(pHeroEquipCS + 1);
    for (int i = 0; i < pHeroEquipCS->equipCnt; i++)
    {
		
		int equipId = pEquipInfo->equipId;
		int equipPart = pEquipInfo->equipPos;
		int equipConfId = 0;
		if (equipPart < WEAPON || equipPart > TREASURE)
		{
			continue;
		}
		// װ��, �鿴�Ƿ����
		if (equipId != 0)
		{
			// װ���ڱ����д������ǻ�û����
			if (!pBagModel->GetItem(equipId, equipConfId))
			{
				ErrorCodeData CodeData;
				CodeData.nCode = ERROR_BAG_EQUIPNOTEXIST;
				CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
					(char*)&CodeData, sizeof(CodeData));
				continue;
			}

			// װ��λ���ж�&�ж�װ���Ƿ���ͬ&�ȼ��ж�&ְҵ�ж�
			const EquipmentItem *pEquipmentConf = queryConfEquipment(equipConfId);
			CHECK_RETURN_VOID(pEquipmentConf);

			vector<int>::const_iterator iterVoc = find(pEquipmentConf->Vocation.begin(), 
				pEquipmentConf->Vocation.end(), pSoldierConf->Common.Vocation);
	
			if (pEquipInfo->equipPos + 1 == pEquipmentConf->Parts
				&& heroAttr.equipId[equipPart] != equipId
				&& heroAttr.level >= pEquipmentConf->Level
				&& iterVoc != pEquipmentConf->Vocation.end())
			{
				continue;
			}

			if (!CModelHelper::removeExistEquipFromBag(uid, equipId))
			{
				continue;
			}
		}

		if (heroAttr.equipId[equipPart] != 0)
		{
			// �滻װ��,����װ���Żر���
			// ���µ��Ƴ�����
			if (!CModelHelper::addExistEquipToBag(uid, heroAttr.equipId[equipPart]))
			{
				continue;
			}
		}
		// ������װ��
		heroAttr.equipId[equipPart] = equipId;
		if (!pHeroModel->UpdateHero(heroId, heroAttr))
		{
			continue;
		}
		// �����¼�, װ����λ
		CModelHelper::DispatchActionEvent(uid, ELA_HERO_EQUIP, &equipPart, sizeof(int));
		successEquips[equipPart] = equipId;
		// next
        pEquipInfo += 1;
    }

    int nSendLen = sizeof(HeroEquipSC)+successEquips.size() * sizeof(HeroEquipInfo);
    char *pSendData = static_cast<char*>(kxMemMgrAlocate(nSendLen));

    HeroEquipSC *pHeroEquipSC = reinterpret_cast<HeroEquipSC *>(pSendData);
    pHeroEquipSC->heroId = pHeroEquipCS->heroId;
    pHeroEquipSC->equipCnt = successEquips.size();
  
	HeroEquipInfo *pEquipId = reinterpret_cast<HeroEquipInfo*>(pHeroEquipSC + 1);
	for (std::map<int, int> ::iterator iter = successEquips.begin(); 
		iter != successEquips.end(); ++iter)
    {
		pEquipId->equipPos = iter->first;
        pEquipId->equipId = iter->second;
        pEquipId += 1;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_HERO, CMD_HERO_EQUIP_SC,
        pSendData, nSendLen);

	kxMemMgrRecycle(pSendData, nSendLen);
}

void CHeroService::ProcessHeroUpgrade(int uid, char *buffer, int len, IKxComm *commun)
{
	CHECK_RETURN_VOID(NULL != buffer && len >= sizeof(HeroUpgradeCS));
    // Model
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
	CBagModel *pBagModel = dynamic_cast<CBagModel*>(CModelHelper::getModel(uid, MODELTYPE_BAG));
    CHECK_RETURN_VOID(pUserModel && pHeroModel && pBagModel);
	// ��������
    HeroUpgradeCS *pUpgradeCS = reinterpret_cast<HeroUpgradeCS*>(buffer);
    if (pUpgradeCS->itemCount > UPGRADE_MATERIAL_COUNT || pUpgradeCS->itemCount < 0)
    {
        return;
    }
    CHECK_RETURN_VOID(len == (sizeof(HeroUpgradeCS) + pUpgradeCS->itemCount*sizeof(int)));

	int nCurGold = 0, nUserLv = 0;
	CHECK_RETURN_VOID(pUserModel->GetUserFieldVal(USR_FD_GOLD, nCurGold));
	CHECK_RETURN_VOID(pUserModel->GetUserFieldVal(USR_FD_USERLV, nUserLv));

	int heroId = pUpgradeCS->heroId;
    DBHeroAttr heroAttr;
	if (!pHeroModel->GetHero(heroId, heroAttr) 
		|| !CModelHelper::isHeroCompletion(uid, heroId))
    {
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_NOTEXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, (char*)&CodeData, sizeof(CodeData));
		return;
    }

	// ��ȡ���ܾ���ֵ
	int totalExp = 0;
	std::vector<int> upgradeItems;
	int *pItemID = reinterpret_cast<int*>(pUpgradeCS + 1);
	for (int i = 0; i < pUpgradeCS->itemCount; i++)
	{
		int itemId = *pItemID;
		upgradeItems.push_back(itemId);

		int itemCount = 0;
		CHECK_RETURN_VOID(pBagModel->GetItem(*pItemID, itemCount, true) && itemCount > 0);

		const PropItem* pPropItem = queryConfProp(itemId);
		if (NULL == pPropItem)
		{
			KXLOGDEBUG("hero upgrade NULL == pPropItem itemId %d", itemId);
			return;
		}
		// ������
        if (PROPTYPE_EXPBOOK == pPropItem->Type)
		{
			// ʹ�þ�����, Ӣ�۵ȼ����ܳ����ٻ�ʦ�ȼ�, ����С��15��
			if (heroAttr.level >= 15 && heroAttr.level >= nUserLv )
			{
				ErrorCodeData CodeData;
				CodeData.nCode = ERROR_HERO_LEVELLIMIT;
				CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
					(char*)&CodeData, sizeof(CodeData));
				return;
			}
			if (pPropItem->TypeParam.size() >= 1)
			{
				totalExp += pPropItem->TypeParam[0];
			}
			// ʹ�þ��鿨�¼�,����ID (���͵Ŀ�����û��ʹ�õ���Ʒ, �������Ʒ���Ϸ��Ļ�)
			CModelHelper::DispatchActionEvent(uid, ELA_USE_EXP_BOOK, &itemId, sizeof(int));
		}
		else
		{
			KXLOGERROR("item isn't SKILLBOOK & EXPCARD !!");
			return;
		}
		pItemID += 1;
	}

	// Ҫ������Ƭ�����ñ���Ϣ
	const SoldierLevelSettingItem *pLevelSetting = queryConfSoldierLevelSetting(heroAttr.level);
	const SoldierStarSettingItem *pStarSetting = queryConfSoldierStarSetting(heroAttr.star);
	CHECK_RETURN_VOID(NULL != pLevelSetting && NULL != pStarSetting);
	// ����Ƿ�����
	int nCostGold = pLevelSetting->LvUpCost * pUpgradeCS->itemCount;
	CHECK_RETURN_VOID(nCurGold >= nCostGold);
	
	// ���鱶��
	const CardGambleSettingItem &cardGambleSetting = queryConfCardGambleSetting();
	int nRegion = randRateIndex(cardGambleSetting.SoldierLvUpScuccessRate);
	if (nRegion > 0)
	{
		totalExp = static_cast<int>(ExpMultiple[nRegion] * totalExp);
	}

	int nCurLv = heroAttr.level;
	int nCurExp = heroAttr.exp + totalExp;
	// �ȼ�����
	convertExpToLevel(nCurLv, nCurExp, pStarSetting->TopLevel);
	heroAttr.exp = nCurExp;
	bool bSendLvEvent = false;
	bool bSendSkEvent = false;
	// ���о����������
	if (nCurLv != heroAttr.level)
	{
		if (nUserLv <= 15 && nCurLv >= 15)
		{
			heroAttr.level = 15;
			heroAttr.exp = 0;
		}
		else if (nUserLv > 15 && nCurLv >= nUserLv)
		{
			heroAttr.level = nUserLv;
			heroAttr.exp = 0;
		}
		else
		{
			heroAttr.level = nCurLv;
		}
		bSendLvEvent = true;
		// Ӣ�������¼�
		CModelHelper::DispatchActionEvent(uid, ELA_HERO_LEVEL_UP, &heroAttr.level, sizeof(int));
		CCommOssHelper::UserCardLvUpOss(uid, heroId);
	}

	CHECK_RETURN_VOID(pHeroModel->UpdateHero(heroId, heroAttr));
	// �Ƴ���Ʒ
	for (std::vector<int>::iterator iter = upgradeItems.begin();
		iter != upgradeItems.end(); ++iter)
	{
		CHECK_BREAK(CModelHelper::removeItem(uid, *iter, 1));
	}
	// �۷�
	CModelHelper::addGold(uid, -1 * nCostGold);

	HeroUpgradeSC sHeroUpgradeSC;
    sHeroUpgradeSC.ret = 1;
    sHeroUpgradeSC.addExp = (int)totalExp;
    sHeroUpgradeSC.moneyType = MONEY_GOLD;
    sHeroUpgradeSC.money = nCostGold;
    sHeroUpgradeSC.multiple = nRegion;

	if (bSendLvEvent)
	{
		CModelHelper::DispatchActionEvent(uid, ELA_OWNLEVELHERO, 0, NULL);
	}

	CKxCommManager::getInstance()->sendData(uid, CMD_HERO, CMD_HERO_UPGRADE_SC, 
        reinterpret_cast<char*>(&sHeroUpgradeSC), sizeof(HeroUpgradeSC));
}

void CHeroService::ProcessHeroUpgradeStar(int uid, char *buffer, int len, IKxComm *commun)
{
    CHECK_RETURN_VOID(NULL != buffer && len == sizeof(HeroUpStarCS));

	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
	CBagModel *pBagModel = dynamic_cast<CBagModel*>(CModelHelper::getModel(uid, MODELTYPE_BAG));
	CUserModel *pUserModel = dynamic_cast<CUserModel*>(CModelHelper::getModel(uid, MODELTYPE_USER));
    CHECK_RETURN_VOID(pHeroModel && pBagModel && pUserModel);

    HeroUpStarCS *pHeroUpStarCS = reinterpret_cast<HeroUpStarCS*>(buffer);
   
	int heroId = pHeroUpStarCS->heroId;
	DBHeroAttr heroAttr;
	if (!pHeroModel->GetHero(heroId, heroAttr))
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_NOTEXIST;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, 
			(char*)&CodeData, sizeof(CodeData));
		return;
	}

	// Ӣ������(����)
	const SoldierUpRateItem *pSoldierUpRate = queryConfSoldierUpRateItem(heroId);
	CHECK_RETURN_VOID(NULL != pSoldierUpRate);
	if (heroAttr.star >= pSoldierUpRate->TopStar)
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_MAXSTART;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
			(char*)&CodeData, sizeof(CodeData));
		return;
	}
	
	// ����һ��, ��Ҫ�ȼ�, ���
	int nextStar = heroAttr.star + 1;
	const SoldierStarSettingItem *pSoldierStarConf = queryConfSoldierStarSetting(nextStar);
    if (NULL == pSoldierStarConf)
    {
        KXLOGERROR("ProcessHeroUpgradeStar SoldierStarSettingItem is nil heroId %d nextStar %d", heroId, nextStar);
        return;
    }
	if (heroAttr.level < pSoldierStarConf->UpStarLevel)
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_LEVELNOTENOUGH;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, 
			(char*)&CodeData, sizeof(CodeData));
		return;
	}

    int nGold = 0;
    pUserModel->GetUserFieldVal(USR_FD_GOLD, nGold);
    if (nGold < pSoldierStarConf->UpStarCost)
    {
        ErrorCodeData CodeData;
        CodeData.nCode = ERROR_GOLD_NOTENOUGH;
        CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL, 
			(char*)&CodeData, sizeof(CodeData));
        return;
    }
	// ������Ҫ����Ƭ
	if (heroAttr.fragment < pSoldierStarConf->UpStarCount)
	{
		ErrorCodeData CodeData;
		CodeData.nCode = ERROR_HERO_FRAGNOTENOUGH;
		CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
			(char*)&CodeData, sizeof(CodeData));
		return;
	}

	heroAttr.star += 1;
	heroAttr.fragment -= pSoldierStarConf->UpStarCount;
	
	// �ȼ�Ӣ��ת��ʣ����Ƭ
	if (heroAttr.star == pSoldierUpRate->TopStar)
	{
		//ת�ɽ��
		const CardGambleSettingItem& pTurnRateConf = queryConfCardGambleSetting();
		int gold = heroAttr.fragment * pTurnRateConf.exchangeRatio;
        if (CModelHelper::addGold(uid, gold))
		{
			heroAttr.fragment = 0;
		}
	}
	// ���µȼ�����Ƭ
    if (!pHeroModel->UpdateHero(heroId, heroAttr))
    {
        KXLOGERROR("ProcessHeroUpgradeStar UpdateHero fail heroId %d", heroId);
        return;
    }
	// �۳����
    if (!CModelHelper::addGold(uid, -1 * pSoldierStarConf->UpStarCost))
    {
        KXLOGERROR("ProcessHeroUpgradeStar addGold fail uid %d", uid);
    }
	
	HeroUpStarSC heroUpStarSC;
	heroUpStarSC.heroId = heroId;
	heroUpStarSC.newStar = nextStar;
	CKxCommManager::getInstance()->sendData(uid, CMD_HERO, CMD_HERO_UPSTAR_SC,
		reinterpret_cast<char*>(&heroUpStarSC), sizeof(HeroUpStarSC));

	// Ӣ�������¼�,Ӣ���Ǽ�
	CModelHelper::DispatchActionEvent(uid, ELA_HERO_UPGRADE_STAR, &heroUpStarSC.newStar, sizeof(int));
	CModelHelper::DispatchActionEvent(uid, ELA_HAVE_HERO_X_STAR, NULL,0);
	CCommOssHelper::UserCardUpStartOss(uid, pHeroUpStarCS->heroId);
}

void CHeroService::ProcessHeroTalent(int uid, char *buffer, int len, KxServer::IKxComm *commun)
{
    CHECK_RETURN_VOID(NULL != buffer && len == sizeof(HeroTalentCSC));
	HeroTalentCSC *pHeroTalentCS = reinterpret_cast<HeroTalentCSC*>(buffer);

	int heroId = pHeroTalentCS->heroId;

	CHeroModel *pHeroModel = dynamic_cast<CHeroModel*>(CModelHelper::getModel(uid, MODELTYPE_HERO));
	CHECK_RETURN_VOID(NULL != pHeroModel);

	DBHeroAttr heroAttr;
	if (!pHeroModel->GetHero(heroId, heroAttr))
	{
        ErrorCodeData CodeData;
        CodeData.nCode = ERROR_HERO_NOTEXIST;
        CKxCommManager::getInstance()->sendData(uid, CMD_ERRORCODE, ERRORCODE_PROTOCOL,
            (char*)&CodeData, sizeof(CodeData));
        return;
    }

    const SoldierConfItem* pSoldierConfItem = queryConfSoldier(heroId, heroAttr.star);
    if (NULL == pSoldierConfItem)
    {
        KXLOGERROR("error:can't find soilder id%d star%d config!!!", heroId, heroAttr.star);
        return;
    }

    // �����츳��ְҵ�츳��4���츳
    const STalentArrangeData *pRaceTalentArrange = CCommonHelper::getRaceTalentArrange(pSoldierConfItem->Common.Race);
    const STalentArrangeData *pVocationTalentArrange = CCommonHelper::getVocationTalentArrange(pSoldierConfItem->Common.Vocation);
    if (NULL == pRaceTalentArrange || NULL == pVocationTalentArrange || 
        pRaceTalentArrange->FloorTalent.size() != (TALENT_COUNT / 2) ||
        pVocationTalentArrange->FloorTalent.size() != (TALENT_COUNT / 2))
    {
        KXLOGERROR("ProcessHeroTalent getTalentArrange fail race %d vocation %d", 
            pSoldierConfItem->Common.Race, pSoldierConfItem->Common.Vocation);
        return;
    }

    //�ж����õ��츳�Ƿ���ȷ(�츳��С�ڵ����Ǽ�+���ù�����ϵ���)
    int nCheckCount = 0;
    bool bCheckBreak = false;
    for (int i = 0; i < TALENT_COUNT / 2; ++i)  // �����츳
    {
        unsigned char talentIndex = pHeroTalentCS->talent[i];
        CHECK_RETURN_VOID(talentIndex >= 0);
        if (talentIndex == 0)
        {
            bCheckBreak = true;
        }
        else if (talentIndex > 0)
        {
            CHECK_RETURN_VOID(!bCheckBreak && talentIndex <= pRaceTalentArrange->FloorTalent[i].size());
            nCheckCount++;
            pHeroTalentCS->talent[i] = static_cast<unsigned char>(pRaceTalentArrange->FloorTalent[i][talentIndex - 1]);
        }
    }

    bCheckBreak = false;
    for (int j = 0; j < TALENT_COUNT / 2; ++j)   // ְҵ�츳
    {
        unsigned char talentIndex = pHeroTalentCS->talent[j + TALENT_COUNT / 2];
        CHECK_RETURN_VOID(talentIndex >= 0);
        if (talentIndex == 0)
        {
            bCheckBreak = true;
        }
        else if (talentIndex > 0)
        {
            CHECK_RETURN_VOID(!bCheckBreak && talentIndex <= pVocationTalentArrange->FloorTalent[j].size());
            nCheckCount++;
            pHeroTalentCS->talent[j + TALENT_COUNT / 2] = static_cast<unsigned char>(pVocationTalentArrange->FloorTalent[j][talentIndex - 1]);
        }
    }

    if (nCheckCount > heroAttr.star)
    {
        //�츳�����
        KXLOGERROR("ProcessHeroTalent hero talent point not enough! uid %d, heroId %d, nCheckCount%d, star%d",
            uid, heroId, nCheckCount, heroAttr.star);
        return;
    }

    memcpy(heroAttr.talent, pHeroTalentCS->talent, sizeof(heroAttr.talent));
    if (!pHeroModel->UpdateHero(heroId, heroAttr))
    {
        KXLOGERROR("ProcessHeroTalent update hero info error! uid %d, heroId %d", uid, heroId);
        return;
    }

    CKxCommManager::getInstance()->sendData(uid, CMD_HERO, CMD_HERO_ACTIVETALENT_SC,
        buffer, sizeof(HeroTalentCSC));
}
