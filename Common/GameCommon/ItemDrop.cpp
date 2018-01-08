#include "ItemDrop.h"
#include "ConfHall.h"
#include "ConfGameSetting.h"
#include "GameDef.h"
#include "CommonHelper.h"
#include "RandGenerator.h"
#include "KxLog.h"
#include "ConfUnion.h"

using namespace std;

void CItemDrop::Drop(int id, std::vector<DropItemInfo> &vecDropItem, bool isExtra)
{
    const DropPropItem *pDropItem = queryConfDropProp(id);
    if (NULL == pDropItem)
    {
        LOG("DropPropItem is null : %d", id);
        return;
    }
    DropItemInfo Info;
    //���ҵ���
    for (std::vector<DropCurrencyData>::const_iterator iter = pDropItem->DropCurrencys.begin();
        iter != pDropItem->DropCurrencys.end(); ++iter)
    {
        Info.id = iter->CurrencyId;
        Info.num = g_RandGenerator.MakeRandNum(iter->LowerLimit, iter->UpperLimit);
        if (Info.num != 0)
        {
            vecDropItem.push_back(Info);
        }
    }

    //������Ƕ���ĲŻ���ж������, ��֤�ݹ�ֻ��һ��
    if (!isExtra)
    {
        for (std::vector<int>::const_iterator iter = pDropItem->ExtraDropRuleID.begin();
            iter != pDropItem->ExtraDropRuleID.end(); ++iter)
        {
            Drop(*iter, vecDropItem, true);
        }
    }

    int kinds = 1;
    int totalRate = 0;
    int randRate = 0;
    if (pDropItem->MeanwhileDropNum.size() >= 2)
    {
        kinds = g_RandGenerator.MakeRandNum(pDropItem->MeanwhileDropNum[0], pDropItem->MeanwhileDropNum[1]);
    }

    // ��Ҫ�����������ڵ������࣬�Ҳ��ظ���ֱ�ӵ�����Щ����
    if (kinds == static_cast<int>(pDropItem->DropIDs.size()) && pDropItem->IsRepeat == 0)
    {
        for (int i = 0; i < static_cast<int>(pDropItem->DropIDs.size()); ++i)
        {
            DropItemInfo dropItem;
            dropItem.id = pDropItem->DropIDs[i].DropID;
            dropItem.num = g_RandGenerator.MakeRandNum(
                pDropItem->DropIDs[i].DropNum[0], pDropItem->DropIDs[i].DropNum[1]);
            vecDropItem.push_back(dropItem);
        }
    }
    else
    {
        // �����ܸ���
        for (std::vector<DropIdData>::const_iterator iter = pDropItem->DropIDs.begin();
            iter != pDropItem->DropIDs.end(); ++iter)
        {
            const DropIdData &item = *iter;
            const PropItem *pPropConf = queryConfProp(item.DropID);
            if (NULL == pPropConf)
            {
                LOG("queryConfProp NULL : %d", item.DropID);
                return;
            }
            totalRate += static_cast<int>(item.DropRate * pPropConf->Ratio);
        }
        if (totalRate <= 0)
        {
            LOG("drop id %d totalRate is %d", id, totalRate);
            return;
        }

        // �����ظ��Ͳ��ظ����е���
        if (pDropItem->IsRepeat == 0)
        {
            // ������ظ�
            vector<DropIdData> dropDatas;
            dropDatas.assign(pDropItem->DropIDs.begin(), pDropItem->DropIDs.end());
            for (int i = 0; i < kinds; ++i)
            {
                int accumRate = 0;
                // �������
                randRate = rand() % totalRate;
                // �ж��������һ��
                for (vector<DropIdData>::iterator iterDrop = dropDatas.begin();
                    iterDrop != dropDatas.end(); ++iterDrop)
                {
                    // ǰ�����totalRateʱ�Ѿ��������ж�
                    int itemRate = static_cast<int>(iterDrop->DropRate * queryConfProp(iterDrop->DropID)->Ratio);
                    accumRate += itemRate;
                    if (accumRate >= randRate)
                    {
                        // �����˸���Ʒ
                        DropItemInfo dropItem;
                        dropItem.id = iterDrop->DropID;
                        dropItem.num = g_RandGenerator.MakeRandNum(
                            iterDrop->DropNum[0], iterDrop->DropNum[1]);
                        vecDropItem.push_back(dropItem);
                        // �������ʣ���������Ʒ�ӵ����б����Ƴ�
                        totalRate -= itemRate;
                        dropDatas.erase(iterDrop);
                        break;
                    }
                }

                // ����Ҫ������������ڵ��������������ʴ��������
                if (dropDatas.size() == 0 || totalRate <= 0)
                {
                    break;
                }
            }
        }
        else
        {
            // ��������ظ�
            for (int i = 0; i < kinds; ++i)
            {
                int accumRate = 0;
                // �������
                randRate = rand() % totalRate;
                // �ж��������һ��
                for (vector<DropIdData>::const_iterator iterDrop = pDropItem->DropIDs.begin();
                    iterDrop != pDropItem->DropIDs.end(); ++iterDrop)
                {
                    // ǰ�����totalRateʱ�Ѿ��������ж�
                    int itemRate = iterDrop->DropRate * queryConfProp(iterDrop->DropID)->Ratio;
                    accumRate += itemRate;
                    if (accumRate >= randRate)
                    {
                        // �����˸���Ʒ
                        DropItemInfo dropItem;
                        dropItem.id = iterDrop->DropID;
                        dropItem.num = g_RandGenerator.MakeRandNum(
                            iterDrop->DropNum[0], iterDrop->DropNum[1]);
                        vecDropItem.push_back(dropItem);
                        break;
                    }
                }
            }
        }
    }
}

void CItemDrop::UnionShopDrop(int id, std::map<int, int> &MapDropShop)
{
    const UnionShopDropData *pDropItem = queryConfUnionDropProp(id);
    CHECK_RETURN_DEBUG_VOID(pDropItem != NULL);
    int kinds = 1;
    int totalRate = 0;
    int randRate = 0;
    if (pDropItem->MeanwhileDropNum.size() >= 2)
    {
        kinds = g_RandGenerator.MakeRandNum(pDropItem->MeanwhileDropNum[0], pDropItem->MeanwhileDropNum[1]);
    }

    //�ܸ���
    for (std::vector<UnionDropIdData>::const_iterator iter = pDropItem->DropIDs.begin();
        iter != pDropItem->DropIDs.end(); ++iter)
    {
        const UnionDropIdData &item = *iter;
        const ShopGoodsConfigData *pPropConf = queryConfShopData(item.DropID);
        if (NULL == pPropConf)
        {
            LOG("Execute False On File %s Line %d : %d", __FILE__, __LINE__, item.DropID);
            continue;
        }
        totalRate += item.DropRate;
    }

    //����
    std::map<int, int> tempMap;
    std::map<int, int>::iterator ator;
    for (int i = 0; i < kinds; ++i)
    {
        //�Ƿ������һ��(�����ò����ظ���������ظ���Ʒʱ)
        bool bNext = false;
        do
        {
            bNext = false;
            int accumRate = 0;
            int critRate = 0;
            if (totalRate > 0)
                randRate = rand() % totalRate;

            for (std::vector<UnionDropIdData>::const_iterator iter = pDropItem->DropIDs.begin();
                iter != pDropItem->DropIDs.end(); ++iter)
            {
                const UnionDropIdData &item = *iter;
                const ShopGoodsConfigData *pPropConf = queryConfShopData(item.DropID);
                if (NULL == pPropConf)
                {
                    LOG("Execute False On File %s Line %d : %d", __FILE__, __LINE__, item.DropID);
                    continue;
                }

                accumRate += item.DropRate;
                if (randRate <= accumRate)
                {
                    ator = tempMap.find(item.DropID);
                    if (ator != tempMap.end())
                    {
                        // �ظ������һ��
                        bNext = true;
                        break;
                    }

                    int nNum = g_RandGenerator.MakeRandNum(item.DropNum[0], item.DropNum[1]);
                    tempMap[item.DropID] = nNum;
                    break;
                }
            }
        } while (bNext);
    }

    // ��ӵ��������
    MapDropShop.insert(tempMap.begin(), tempMap.end());
}

CardGambleItem * CItemDrop::RandHeroCard(int randType)
{
    const CardGambleSettingItem &cardGambleConf = queryConfCardGambleSetting();
    int totalRatio = 0;
    // �ȴ���ϡ�ж��б�
    std::map<int, int> rarityPool;
    for (std::map<int, Prob>::const_iterator iter = cardGambleConf.RareRatios.begin();
        iter != cardGambleConf.RareRatios.end(); ++iter)
    {
        if (randType == 0)
        {
            rarityPool[iter->first] = (int)(iter->second.Probability * iter->second.Ratio);
            totalRatio += rarityPool[iter->first];
        }
        else
        {
            if (iter->first >= RARITY_BLUE)
            {
                rarityPool[iter->first] = (int)(iter->second.Probability * iter->second.Ratio);
                totalRatio += rarityPool[iter->first];
            }
        }
    }
    //���ϡ�ж�
    int rarity = 0;
    int tempRatio = 0;
    int randRatio = 0;
    bool again = false;
    //ϡ�жȶ�Ӧ�����б�
    std::vector<CardGambleItem*> gambleCards;
    do
    {
        if (totalRatio <= 0)
        {
            KXLOGERROR("config is error!!! totalRatio %d", totalRatio);
            return NULL;
        }

        randRatio = rand() % totalRatio;
        for (std::map<int, int>::iterator iter = rarityPool.begin();
            iter != rarityPool.end(); ++iter)
        {
            tempRatio += iter->second;
            if (tempRatio > randRatio)
            {
                rarity = iter->first;
                break;
            }
        }

        again = !queryConfCardGamble(rarity, gambleCards);
        if (again)
        {
            LOG("card gamble haven't rarity=%d", rarity);
        }
    } while (again);

    // �����б��ܸ���
    totalRatio = 0;
    for (std::vector<CardGambleItem*>::iterator iter = gambleCards.begin();
        iter != gambleCards.end(); ++iter)
    {
        totalRatio += (*iter)->Rate * (*iter)->Ratio;
    }
    // ���һ�ſ�Ƭ
    if (totalRatio <= 0)
    {
        KXLOGERROR("config is error!!! totalRatio %d", totalRatio);
        return NULL;
    }
    randRatio = rand() % totalRatio;
    tempRatio = 0;

    for (std::vector<CardGambleItem*>::iterator iter = gambleCards.begin();
        iter != gambleCards.end(); ++iter)
    {
        tempRatio += (*iter)->Rate * (*iter)->Ratio;
        if (tempRatio >= randRatio)
        {
            return (*iter);
        }
    }

    return NULL;
}
