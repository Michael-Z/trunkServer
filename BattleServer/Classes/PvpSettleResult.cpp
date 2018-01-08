#include "PvpSettleResult.h"
#include "CommonHelper.h"
#include "ModelHelper.h"
#include "PvpModel.h"
#include "UserModel.h"
#include "TeamModel.h"
#include "PvpRankModel.h"
#include "PvpTaskModel.h"
#include "Room.h"
#include "UserActionListener.h"
#include "GameDef.h"
#include "ConfGameSetting.h"
#include "BattleModels.h"
#include "Protocol.h"
#include "PvpProtocol.h"
#include "KxMemPool.h"
#include "CommOssHelper.h"
#include "KxCommManager.h"
#include "BattleDataCache.h"
#include "ItemDrop.h"
#include "PropUseHelper.h"
#include <stdlib.h>

#define _PSRMAX(a,b)            (((a) > (b)) ? (a) : (b))
#define _PSRMIN(a,b)            (((a) < (b)) ? (a) : (b))

CPvpSettleResult::CPvpSettleResult()
: m_nRoomType(0)
{
}

CPvpSettleResult::~CPvpSettleResult()
{
}

bool CPvpSettleResult::init(int roomType)
{
	m_nRoomType = roomType;
	return true;
}

void CPvpSettleResult::pvpSettleAccount(int winner, int loser, int result, int winerCryLv, int loserCryLv, int dt)
{
	if (m_nRoomType == PVPROOMTYPE_CHAMPIONSHIP)
	{
		overChampionPvp(winner, loser, result,winerCryLv,loserCryLv,dt);
	}
	else
	{
		overFairPvp(winner, loser, result, winerCryLv, loserCryLv, dt);
	}

    CModelHelper::addDayLiveness(winner, UNION_DAY_PVPLIVENESS);
    CModelHelper::addDayLiveness(loser, UNION_DAY_PVPLIVENESS);
}
	
int CPvpSettleResult::calcMMR(int myMMR, int otherMMR, int k, int result)
{
	float w = 0;
	float l = 0;
	int mmrChange = 0;
	//����ϵ��
	float fWinK = 1.0f, fDrawK = 0.5f, fLoseK = 0;
	//�����4, ��С��-4
	float fPower = _PSRMAX(_PSRMIN(4.0f, (otherMMR - myMMR) / 3000.0f), -4.0f);
	//����
	float e = 1.0f / (1 + pow(10.0f, fPower));
	int myDan = CCommonHelper::getUserMMRDan(myMMR);
	int otherDan = CCommonHelper::getUserMMRDan(otherMMR);
	float MMR_Kx = CCommonHelper::getDanMMR_Kx(myDan);
	float MMR_K = CCommonHelper::getDanMMR_K(myDan);

	w = myDan < otherDan ? 1 + ((otherDan - myDan) / 2.0f) : 1;
	l = myDan > otherDan ? 1 + ((myDan - otherDan) / 2.0f) : 1;

	if (k >= 0)
	{
		switch (result)
		{
		case -1:
			//ƽ��
			mmrChange = (int)ceil(w *(fDrawK - e) * MMR_K * MMR_Kx);
			break;
		case 0:
			//ʧ��
			mmrChange = (int)ceil(l *(fLoseK - e) * MMR_K * MMR_Kx);
			break;
		case 1:
			//ʤ��
			mmrChange = (int)ceil(w *(fWinK - e) * MMR_K * pow(MMR_Kx, abs(k)));
			break;
		}
	}
	else
	{
		switch (result)
		{
		case -1:
			//ƽ��
			mmrChange = (int)ceil(w *(fDrawK - e) * MMR_K * MMR_Kx);
			break;
		case 0:
			//ʧ��
			mmrChange = (int)ceil(l *(fLoseK - e) * MMR_K * pow(MMR_Kx, abs(k)));
			break;
		case 1:
			//ʤ��
			mmrChange = (int)ceil(w *(fWinK - e) * MMR_K * MMR_Kx);
			break;
		}
	}
	return _PSRMAX(_PSRMIN(mmrChange, 400), -400);
}

int CPvpSettleResult::calcIntegral(int myIntegral, int otherIntegral, int result)
{
	float w = 0;
	float l = 0;
	int integralChange = 0;
	//����ϵ��
	float fWinK = 1.0f, fDrawK = 0.5f, fLoseK = 0;
	//�����4, ��С��-4
	float fPower = _PSRMAX(_PSRMIN(4.0f, (otherIntegral - myIntegral) / 3000.0f), -4.0f);
	//����
	float e = 1.0f / (1 + pow(10.0f, fPower));
	int myDan = CCommonHelper::getUserIntegralDan(myIntegral);
	int otherDan = CCommonHelper::getUserIntegralDan(otherIntegral);
	float Arean_K = CCommonHelper::getDanArena_K(myDan);

	w = myDan < otherDan ? 1 + ((otherDan - myDan) / 2.0f) : 1;
	l = myDan > otherDan ? 1 + ((myDan - otherDan) / 2.0f) : 1;

	switch (result)
	{
	case -1:
		//ƽ��
		integralChange = (int)ceil(w * (fDrawK - e)*Arean_K);
		break;
	case 0:
		//ʧ��
		integralChange = (int)ceil(l * (fLoseK - e)*Arean_K);
		break;
	case 1:
		//ʤ��
		integralChange = (int)ceil(w * (fWinK - e)*Arean_K);
		break;
	}

	return _PSRMAX(_PSRMIN(integralChange, 100), -100);
}

void CPvpSettleResult::setWinnerPvpModel(int roomType, int uid, int changeMMR, int changeInte, int newRank)
{
	CPvpModel *model = dynamic_cast<CPvpModel*>(CModelHelper::getModel(uid, MODELTYPE_PVP));
	CPvpChestModel *pPvpChestModel = dynamic_cast<CPvpChestModel*>(CModelHelper::getModel(uid, MODELTYPE_PVPCHEST));
	if (NULL == model || NULL == pPvpChestModel)
	{
		return;
	}

	std::map<int, int> mapValues;
	int nActionID = ELA_PVP;
	model->GetPvpField(PVPCOMM_FD_DAYWINTIMES, mapValues[PVPCOMM_FD_DAYWINTIMES], true);
	model->GetPvpField(PVPCOMM_FD_DAYCONTINUSWINTIMES, mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES], true);
	model->GetPvpField(PVPCOMM_FD_DAYMAXCONITNUSWINTIMES, mapValues[PVPCOMM_FD_DAYMAXCONITNUSWINTIMES], true);
	model->GetPvpField(PVPCOMM_FD_DAYBATTLETIMES, mapValues[PVPCOMM_FD_DAYBATTLETIMES], true);

	if (roomType == PVPROOMTYPE_PVP)
	{
		//��ƽ����
		model->GetPvpField(PVP_FD_MMR, mapValues[PVP_FD_MMR]);
		model->GetPvpField(PVP_FD_INTEGRAL, mapValues[PVP_FD_INTEGRAL]);
		model->GetPvpField(PVP_FD_CONTINUSWINTIMES, mapValues[PVP_FD_CONTINUSWINTIMES]);
		model->GetPvpField(PVP_FD_DANCONTINUSWINTIMES, mapValues[PVP_FD_DANCONTINUSWINTIMES]);
		model->GetPvpField(PVP_FD_HISTORYHIGHESTRANK, mapValues[PVP_FD_HISTORYHIGHESTRANK]);
		model->GetPvpField(PVP_FD_HISTORYHIGHESTINTEGRAL, mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL]);
		model->GetPvpField(PVP_FD_TOTALWINTIMES, mapValues[PVP_FD_TOTALWINTIMES]);
		model->GetPvpField(PVP_FD_BATTLETIMES, mapValues[PVP_FD_BATTLETIMES]);
		model->GetPvpField(PVP_FD_HISTORYCONTINUSWINTIMES, mapValues[PVP_FD_HISTORYCONTINUSWINTIMES]);
		//MMR
		int beforeDan = CCommonHelper::getUserMMRDan(mapValues[PVP_FD_MMR]);
		mapValues[PVP_FD_MMR] += changeMMR;
		int afterDan = CCommonHelper::getUserMMRDan(mapValues[PVP_FD_MMR]);
		//��λ����ʤ��
		mapValues[PVP_FD_DANCONTINUSWINTIMES] = afterDan > beforeDan ?
			0 : mapValues[PVP_FD_DANCONTINUSWINTIMES] + 1;
		//��������ʤ��
		mapValues[PVP_FD_CONTINUSWINTIMES] = mapValues[PVP_FD_CONTINUSWINTIMES] < 0 ?
			0 : mapValues[PVP_FD_CONTINUSWINTIMES] + 1;
		//��ʷ�����ʤ����
		if (mapValues[PVP_FD_CONTINUSWINTIMES] > mapValues[PVP_FD_HISTORYCONTINUSWINTIMES])
		{
			mapValues[PVP_FD_HISTORYCONTINUSWINTIMES] = mapValues[PVP_FD_CONTINUSWINTIMES];
		}
		//��ս������
		mapValues[PVP_FD_BATTLETIMES] += 1;
		//��������
		mapValues[PVP_FD_INTEGRAL] += changeInte;
		//��ʷ�������
		if (newRank > 0 && (mapValues[PVP_FD_HISTORYHIGHESTRANK] > newRank
			|| mapValues[PVP_FD_HISTORYHIGHESTRANK] == 0))
		{
			mapValues[PVP_FD_HISTORYHIGHESTRANK] = newRank;
		}
		//��ʷ��߻���
		if (mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL] < mapValues[PVP_FD_INTEGRAL])
		{
			mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL] = mapValues[PVP_FD_INTEGRAL];
		}
		//��ʤ������
		mapValues[PVP_FD_TOTALWINTIMES] += 1;
	}
	else if (roomType == PVPROOMTYPE_CHAMPIONSHIP)
	{
		//������
		model->GetPvpField(CPN_FD_MMR, mapValues[CPN_FD_MMR]);
		model->GetPvpField(CPN_FD_INTEGRAL, mapValues[CPN_FD_INTEGRAL]);
		model->GetPvpField(CPN_FD_CONTINUSWINTIMES, mapValues[CPN_FD_CONTINUSWINTIMES]);
		model->GetPvpField(CPN_FD_DANCONTINUSWINTIMES, mapValues[CPN_FD_DANCONTINUSWINTIMES]);
		model->GetPvpField(CPN_FD_HISTORYHIGHESTRANK, mapValues[CPN_FD_HISTORYHIGHESTRANK]);
		model->GetPvpField(CPN_FD_HISTORYHIGHESTINTEGRAL, mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL]);
		model->GetPvpField(CPN_FD_TOTALWINTIMES, mapValues[CPN_FD_TOTALWINTIMES]);
		model->GetPvpField(CPN_FD_BATTLETIMES, mapValues[CPN_FD_BATTLETIMES]);
		model->GetPvpField(CPN_FD_HISTORYCONTINUSWINTIMES, mapValues[CPN_FD_HISTORYCONTINUSWINTIMES]);
		//MMR
		int beforeDan = CCommonHelper::getUserMMRDan(mapValues[CPN_FD_MMR]);
		mapValues[CPN_FD_MMR] += changeMMR;
		int afterDan = CCommonHelper::getUserMMRDan(mapValues[CPN_FD_MMR]);
		//��λ����ʤ��
		mapValues[CPN_FD_DANCONTINUSWINTIMES] = afterDan > beforeDan ?
			0 : mapValues[CPN_FD_DANCONTINUSWINTIMES] + 1;
		//��������ʤ��
		mapValues[CPN_FD_CONTINUSWINTIMES] = mapValues[CPN_FD_CONTINUSWINTIMES] < 0 ?
			0 : mapValues[CPN_FD_CONTINUSWINTIMES] + 1;
		//��ʷ�����ʤ����
		if (mapValues[CPN_FD_CONTINUSWINTIMES] > mapValues[CPN_FD_HISTORYCONTINUSWINTIMES])
		{
			mapValues[CPN_FD_HISTORYCONTINUSWINTIMES] = mapValues[CPN_FD_CONTINUSWINTIMES];
		}
		//��ս������
		mapValues[CPN_FD_BATTLETIMES] += 1;
		//��������
		mapValues[CPN_FD_INTEGRAL] += changeInte;
		//��ʷ�������
		if (newRank > 0 && (mapValues[CPN_FD_HISTORYHIGHESTRANK] > newRank
			|| mapValues[CPN_FD_HISTORYHIGHESTRANK] == 0))
		{
			mapValues[CPN_FD_HISTORYHIGHESTRANK] = newRank;
		}
		//��ʷ��߻���
		if (mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL] < mapValues[CPN_FD_INTEGRAL])
		{
			mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL] = mapValues[CPN_FD_INTEGRAL];
		}
		//��ʤ������
		mapValues[CPN_FD_TOTALWINTIMES] += 1;
		nActionID = ELA_CHAMPION;
	}
	//���һ����Ϸʱ���
	mapValues[PVPCOMM_FD_BATTLESTAMP] = (int)time(NULL);
	//��ʤ��
	mapValues[PVPCOMM_FD_DAYWINTIMES] += 1;
	//������ʤ��
	mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES] += 1;
	//�������ʤ
	if (mapValues[PVPCOMM_FD_DAYMAXCONITNUSWINTIMES] < mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES])
	{
		mapValues[PVPCOMM_FD_DAYMAXCONITNUSWINTIMES] = mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES];
	}
	//��ս������
	mapValues[PVPCOMM_FD_DAYBATTLETIMES] += 1;
	//��Ϊ�������ÿ���ȡ
	if (!CModelHelper::isPvpChestEmpty(uid))
	{
		mapValues[PVPCOMM_FD_CHESTSTATUS] = 1;
	}

	if (!model->SetPvpField(mapValues))
	{
		KXLOGERROR("set pvp field error! %s", __FUNCTION__);
	}

	//����PVP�����¼�
	ActionCommonInfo CommInfo;
	CommInfo.param1 = EPVP_WIN_BATTLE_FLAG;
	CommInfo.param2 = changeInte;
	CModelHelper::DispatchActionEvent(uid, nActionID, &CommInfo, sizeof(ActionCommonInfo));
}
	
void CPvpSettleResult::setLoserPvpModel(int roomType, int uid, int changeMMR, int changeInte, int newRank)
{
	CPvpModel *model = dynamic_cast<CPvpModel*>(CModelHelper::getModel(uid, MODELTYPE_PVP));
	if (NULL == model)
	{
		return;
	}

	std::map<int, int> mapValues;
	int nActionID = ELA_PVP;
	model->GetPvpField(PVPCOMM_FD_DAYWINTIMES, mapValues[PVPCOMM_FD_DAYWINTIMES], true);
	model->GetPvpField(PVPCOMM_FD_DAYCONTINUSWINTIMES, mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES], true);
	model->GetPvpField(PVPCOMM_FD_DAYMAXCONITNUSWINTIMES, mapValues[PVPCOMM_FD_DAYMAXCONITNUSWINTIMES], true);
	model->GetPvpField(PVPCOMM_FD_DAYBATTLETIMES, mapValues[PVPCOMM_FD_DAYBATTLETIMES], true);

	if (roomType == PVPROOMTYPE_PVP)
	{
		model->GetPvpField(PVP_FD_MMR, mapValues[PVP_FD_MMR]);
		model->GetPvpField(PVP_FD_INTEGRAL, mapValues[PVP_FD_INTEGRAL]);
		model->GetPvpField(PVP_FD_CONTINUSWINTIMES, mapValues[PVP_FD_CONTINUSWINTIMES]);
		model->GetPvpField(PVP_FD_DANCONTINUSWINTIMES, mapValues[PVP_FD_DANCONTINUSWINTIMES]);

		model->GetPvpField(PVP_FD_HISTORYHIGHESTRANK, mapValues[PVP_FD_HISTORYHIGHESTRANK]);
		model->GetPvpField(PVP_FD_HISTORYHIGHESTINTEGRAL, mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL]);
		model->GetPvpField(PVP_FD_BATTLETIMES, mapValues[PVP_FD_BATTLETIMES]);

		//MMR, ��ʱ��ʽ�����changeMMRΪ����
		mapValues[PVP_FD_MMR] += changeMMR;
		if (mapValues[PVP_FD_MMR] < 0)
		{
			mapValues[PVP_FD_MMR] = 0;
		}
		//��������, ��ʱ��ʽ�����changeInteΪ����
		mapValues[PVP_FD_INTEGRAL] += changeInte;
		if (mapValues[PVP_FD_INTEGRAL] < 0)
		{
			mapValues[PVP_FD_INTEGRAL] = 0;
		}
		//��λ����ʤ��
		mapValues[PVP_FD_DANCONTINUSWINTIMES] = mapValues[PVP_FD_DANCONTINUSWINTIMES] > 0 ?
			0 : mapValues[PVP_FD_DANCONTINUSWINTIMES] - 1;
		//��������ʤ��
		mapValues[PVP_FD_CONTINUSWINTIMES] = mapValues[PVP_FD_CONTINUSWINTIMES] > 0 ?
			0 : mapValues[PVP_FD_CONTINUSWINTIMES] - 1;
		//��ս������
		mapValues[PVP_FD_BATTLETIMES] += 1;
	}
	else if (roomType == PVPROOMTYPE_CHAMPIONSHIP)
	{
		model->GetPvpField(CPN_FD_MMR, mapValues[CPN_FD_MMR]);
		model->GetPvpField(CPN_FD_INTEGRAL, mapValues[CPN_FD_INTEGRAL]);
		model->GetPvpField(CPN_FD_CONTINUSWINTIMES, mapValues[CPN_FD_CONTINUSWINTIMES]);
		model->GetPvpField(CPN_FD_DANCONTINUSWINTIMES, mapValues[CPN_FD_DANCONTINUSWINTIMES]);

		model->GetPvpField(CPN_FD_HISTORYHIGHESTRANK, mapValues[CPN_FD_HISTORYHIGHESTRANK]);
		model->GetPvpField(CPN_FD_HISTORYHIGHESTINTEGRAL, mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL]);
		model->GetPvpField(CPN_FD_BATTLETIMES, mapValues[CPN_FD_BATTLETIMES]);

		//MMR, ��ʱ��ʽ�����changeMMRΪ����
		mapValues[CPN_FD_MMR] += changeMMR;
		if (mapValues[CPN_FD_MMR] < 0)
		{
			mapValues[CPN_FD_MMR] = 0;
		}
		//��������, ��ʱ��ʽ�����changeInteΪ����
		mapValues[CPN_FD_INTEGRAL] += changeInte;
		if (mapValues[CPN_FD_INTEGRAL] < 0)
		{
			mapValues[CPN_FD_INTEGRAL] = 0;
		}
		//��λ����ʤ��
		mapValues[CPN_FD_DANCONTINUSWINTIMES] = mapValues[CPN_FD_DANCONTINUSWINTIMES] > 0 ?
			0 : mapValues[CPN_FD_DANCONTINUSWINTIMES] - 1;
		//��������ʤ��
		mapValues[CPN_FD_CONTINUSWINTIMES] = mapValues[CPN_FD_CONTINUSWINTIMES] > 0 ?
			0 : mapValues[CPN_FD_CONTINUSWINTIMES] - 1;
		//��ս������
		mapValues[CPN_FD_BATTLETIMES] += 1;
		nActionID = ELA_CHAMPION;
	}

	//���һ����Ϸʱ���
	mapValues[PVPCOMM_FD_BATTLESTAMP] = (int)time(NULL);
	//������ʤ��
	mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES] = 0;
	//��ս������
	mapValues[PVPCOMM_FD_DAYBATTLETIMES] += 1;

	if (!model->SetPvpField(mapValues))
	{
		KXLOGERROR("set pvp field error! %s", __FUNCTION__);
	}

	//����PVP�����¼�
	ActionCommonInfo CommInfo;
	CommInfo.param1 = EPVP_FAILED_BATTLE_FLAG;
	CommInfo.param2 = changeInte;
	CModelHelper::DispatchActionEvent(uid, nActionID, &CommInfo, sizeof(ActionCommonInfo));
}

void CPvpSettleResult::setDrawPvpModel(int roomType, int uid, int changeMMR, int changeInte, int newRank)
{
	CPvpModel *model = dynamic_cast<CPvpModel*>(CModelHelper::getModel(uid, MODELTYPE_PVP));
	if (NULL == model)
	{
		return;
	}

	std::map<int, int> mapValues;
	int nActionID = ELA_PVP;
	model->GetPvpField(PVPCOMM_FD_DAYWINTIMES, mapValues[PVPCOMM_FD_DAYWINTIMES]);
	model->GetPvpField(PVPCOMM_FD_DAYBATTLETIMES, mapValues[PVPCOMM_FD_DAYBATTLETIMES]);

	if (roomType == PVPROOMTYPE_PVP)
	{
		//��ƽ����
		model->GetPvpField(PVP_FD_MMR, mapValues[PVP_FD_MMR]);
		model->GetPvpField(PVP_FD_INTEGRAL, mapValues[PVP_FD_INTEGRAL]);
		model->GetPvpField(PVP_FD_HISTORYHIGHESTRANK, mapValues[PVP_FD_HISTORYHIGHESTRANK]);
		model->GetPvpField(PVP_FD_HISTORYHIGHESTINTEGRAL, mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL]);
		model->GetPvpField(PVP_FD_TOTALWINTIMES, mapValues[PVP_FD_TOTALWINTIMES]);
		model->GetPvpField(PVP_FD_BATTLETIMES, mapValues[PVP_FD_BATTLETIMES]);
		//���ּ���
		mapValues[PVP_FD_MMR] += changeMMR;
		mapValues[PVP_FD_INTEGRAL] += changeInte;
		//��ʷ��߻���
		if (mapValues[PVP_FD_INTEGRAL] > mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL])
		{
			mapValues[PVP_FD_HISTORYHIGHESTINTEGRAL] = mapValues[PVP_FD_INTEGRAL];
		}
		//��������ʤ��
		mapValues[PVP_FD_DANCONTINUSWINTIMES] = 0;
		mapValues[PVP_FD_CONTINUSWINTIMES] = 0;
		//��ս������
		mapValues[PVP_FD_BATTLETIMES] += 1;
		//��ʷ�������
		if (newRank > 0 && (mapValues[PVP_FD_HISTORYHIGHESTRANK] > newRank
			|| mapValues[PVP_FD_HISTORYHIGHESTRANK] == 0))
		{
			mapValues[PVP_FD_HISTORYHIGHESTRANK] = newRank;
		}
	}
	else if (roomType == PVPROOMTYPE_CHAMPIONSHIP)
	{
		//������
		model->GetPvpField(CPN_FD_MMR, mapValues[CPN_FD_MMR]);
		model->GetPvpField(CPN_FD_INTEGRAL, mapValues[CPN_FD_INTEGRAL]);
		model->GetPvpField(CPN_FD_HISTORYHIGHESTRANK, mapValues[CPN_FD_HISTORYHIGHESTRANK]);
		model->GetPvpField(CPN_FD_HISTORYHIGHESTINTEGRAL, mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL]);
		model->GetPvpField(CPN_FD_TOTALWINTIMES, mapValues[CPN_FD_TOTALWINTIMES]);
		model->GetPvpField(CPN_FD_BATTLETIMES, mapValues[CPN_FD_BATTLETIMES]);
		//���ּ���
		mapValues[CPN_FD_MMR] += changeMMR;
		mapValues[CPN_FD_INTEGRAL] += changeInte;
		//��ʷ��߻���
		if (mapValues[CPN_FD_INTEGRAL] > mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL])
		{
			mapValues[CPN_FD_HISTORYHIGHESTINTEGRAL] = mapValues[CPN_FD_INTEGRAL];
		}
		//��������ʤ��
		mapValues[CPN_FD_DANCONTINUSWINTIMES] = 0;
		mapValues[CPN_FD_CONTINUSWINTIMES] = 0;
		//��ս������
		mapValues[CPN_FD_BATTLETIMES] += 1;
		//��ʷ�������
		if (newRank > 0 && (mapValues[CPN_FD_HISTORYHIGHESTRANK] > newRank
			|| mapValues[CPN_FD_HISTORYHIGHESTRANK] == 0))
		{
			mapValues[CPN_FD_HISTORYHIGHESTRANK] = newRank;
		}
		nActionID = ELA_CHAMPION;
	}
	//���һ����Ϸʱ���
	mapValues[PVPCOMM_FD_BATTLESTAMP] = (int)time(NULL);
	//��ս������
	mapValues[PVPCOMM_FD_DAYBATTLETIMES] += 1;
	//������ʤ��
	mapValues[PVPCOMM_FD_DAYCONTINUSWINTIMES] = 0;

	if (!model->SetPvpField(mapValues))
	{
		KXLOGERROR("set pvp field error! %s", __FUNCTION__);
	}
	//����PVP�����¼�
	ActionCommonInfo CommInfo;
	CommInfo.param1 = EPVP_DRAW_BATTLE_FLAG;
	CommInfo.param2 = changeInte;
	CModelHelper::DispatchActionEvent(model->GetUid(), nActionID, &CommInfo, sizeof(ActionCommonInfo));
}

void CPvpSettleResult::overFairPvp(int winner, int loser, int result, int winerCryLv, int loserCryLv, int dt)
{
	CPvpRankModel *pPvpRankModel = CPvpRankModel::getInstance();
	// ƽ��Ҳ��winner loser, ֻ�ǽ�����, ��������庬��
	CPvpModel *pWinnerPvpModel = dynamic_cast<CPvpModel*>(CModelHelper::getModel(winner, MODELTYPE_PVP));
	CPvpModel *pLoserPvpModel = dynamic_cast<CPvpModel*>(CModelHelper::getModel(loser, MODELTYPE_PVP));
	// pvp setting
	CConfPvpSetting *pPvpSetting = dynamic_cast<CConfPvpSetting*>(
		CConfManager::getInstance()->getConf(CONF_PVP_SETTING));

	//��������ֵ��ֲ�
	int nWinnerGNum = 0, nWinnerGDval = 0;
	//���Ե�MMR
	int nWinnerMMR = 0, nLoserMMR = 0;
	//����
	int nWinnerIntegral = 0, nLoserIntegral = 0;
	//�仯ϵ��(��ʤ����)
	int nKW = 0, nKL = 0;

	pWinnerPvpModel->GetPvpField(PVP_FD_MMR, nWinnerMMR);
	pWinnerPvpModel->GetPvpField(PVP_FD_INTEGRAL, nWinnerIntegral);
	pWinnerPvpModel->GetPvpField(PVP_FD_DANCONTINUSWINTIMES, nKW);

	pLoserPvpModel->GetPvpField(PVP_FD_MMR, nLoserMMR);
	pLoserPvpModel->GetPvpField(PVP_FD_INTEGRAL, nLoserIntegral);
	pLoserPvpModel->GetPvpField(PVP_FD_DANCONTINUSWINTIMES, nKL);

	//����MMR
	int nWChangeMMR = 0;
	int nLChangeMMR = 0;
	int nWChangeIntegral = 0;
	int nLChangeIntegral = 0;
	if (result == CHALLENGE_DRAW)
	{
		nWChangeMMR = calcMMR(nWinnerMMR, nLoserMMR, nKW, -1);
		nLChangeMMR = calcMMR(nLoserMMR, nWinnerMMR, nKL, -1);
	}
	else
	{
		//����MMR
		nWChangeMMR = calcMMR(nWinnerMMR, nLoserMMR, nKW, 1);
		nLChangeMMR = calcMMR(nLoserMMR, nWinnerMMR, nKL, 0);
		//�������
		nWChangeIntegral = calcIntegral(nWinnerIntegral, nLoserIntegral, 1);
		//���ִ���һ�����ֲſ۳�
		if (nLoserIntegral > pPvpSetting->m_PvpSetting.CriticalPoint)
		{
			nLChangeIntegral = calcIntegral(nLoserIntegral, nWinnerIntegral, 0);
		}
	}

	//��ø�������, �������������ֵ
	int nRankNow = 0;
	std::vector<DropItemInfo> vecDropItems;
	//ʤ����ս���������ж��ٻ���
	nWinnerIntegral += nWChangeIntegral;
	if (nWinnerIntegral < 0)
	{
		nWinnerIntegral = 0;
	}

	pPvpRankModel->AddPvpRank(winner, nWinnerIntegral);
	pPvpRankModel->GetPvpRank(winner, nRankNow);

	if (result == CHALLENGE_DRAW)
	{
		setDrawPvpModel(m_nRoomType, winner, nWChangeMMR, nWChangeIntegral, nRankNow);
	}
	else
	{
		//����pvpʤ�����������
		setWinnerPvpModel(m_nRoomType, winner, nWChangeMMR, nWChangeIntegral, nRankNow);

		int winnerDropId = CModelHelper::getWinDropId(winner);
		if (winnerDropId > 0)
		{
			CItemDrop::Drop(winnerDropId, vecDropItems);
			CPropUseHelper::getInstance()->AddItems(winner, vecDropItems);
		}
	}

	int nLSendLen = sizeof(PvpResultSC);
	char *pLoserData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nLSendLen));
	int nWSendLen = sizeof(PvpResultSC)+sizeof(DropItemInfo)*vecDropItems.size();
	char *pWinnerData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nWSendLen));
	//�ɹ��ߵ�����
	PvpResultSC * pResultSC = reinterpret_cast<PvpResultSC*>(pWinnerData);
	pResultSC->roomType = m_nRoomType;
	pResultSC->result = result == CHALLENGE_DRAW ? CHALLENGE_DRAW : CHALLENGE_WON;
	pResultSC->rankNow = nRankNow;
    pResultSC->integral = nWinnerIntegral;
	pResultSC->itemCount = vecDropItems.size();
	
	DropItemInfo *item = reinterpret_cast<DropItemInfo*>(pResultSC + 1);
	for (std::vector<DropItemInfo>::iterator iter = vecDropItems.begin();
		iter != vecDropItems.end(); ++iter)
	{
		memcpy(item, &(*iter), sizeof(DropItemInfo));
		iter += 1;
	}
    // �������ж�����Ƿ�����
    //CBattleDataCache::getInstance()->pushBackBattleData(winner, *pResultSC);
	//ʧ��������������
	nRankNow = 0;
	// ʧ���߽�����ж��ٻ���
	nLoserIntegral += nLChangeIntegral;
	if (nLoserIntegral < 0)
	{
		nLoserIntegral = 0;
	}

	pPvpRankModel->AddPvpRank(loser, nLoserIntegral);
	pPvpRankModel->GetPvpRank(loser, nRankNow);

	if (result == CHALLENGE_DRAW)
	{
		setDrawPvpModel(m_nRoomType, loser, nLChangeMMR, nLChangeIntegral, nRankNow);
	}
	else
	{
		//����pvpʧ�����������
		setLoserPvpModel(m_nRoomType, loser, nLChangeMMR, nLChangeIntegral, nRankNow);
	}

	pResultSC = reinterpret_cast<PvpResultSC*>(pLoserData);
	pResultSC->roomType = m_nRoomType;
	pResultSC->result = result == CHALLENGE_DRAW ? CHALLENGE_DRAW : CHALLENGE_FAILED;
	pResultSC->rankNow = nRankNow;
    pResultSC->integral = nLoserIntegral;
	pResultSC->itemCount = 0;
    // �������ж�����Ƿ�����
    //CBattleDataCache::getInstance()->pushBackBattleData(loser, *pResultSC);

	//���͸����
	CKxCommManager::getInstance()->sendData(winner, CMD_PVP, CMD_PVP_RESULT_SC, pWinnerData, nWSendLen);
	CKxCommManager::getInstance()->sendData(loser, CMD_PVP, CMD_PVP_RESULT_SC, pLoserData, nLSendLen);
	//�����ڴ�
	KxServer::kxMemMgrRecycle(pWinnerData, nWSendLen);
	KxServer::kxMemMgrRecycle(pLoserData, nLSendLen);

	//ͳ����Ϣ
	CTeamModel *pWinTeamModel = dynamic_cast<CTeamModel*>(CModelHelper::getModel(winner, MODELTYPE_TEAM));
	CHECK_RETURN_VOID(pWinTeamModel != NULL);

	TeamInfo WinTeam;
	if (!pWinTeamModel->GetTeamInfo(ETT_SPORTE, WinTeam))
	{
		return;
	}

	CTeamModel *pLostTeamModel = dynamic_cast<CTeamModel*>(CModelHelper::getModel(loser, MODELTYPE_TEAM));
	CHECK_RETURN_VOID(pLostTeamModel != NULL);

	TeamInfo LostTeam;
	if (!pLostTeamModel->GetTeamInfo(ETT_SPORTE, LostTeam))
	{
		return;
	}

	//CCommOssHelper::PvpLogicOss(winner, true, nWinnerIntegral, LostTeam.summonerId, m_nRoomType,winerCryLv,dt);
	//CCommOssHelper::PvpLogicOss(loser, false, nLoserIntegral, WinTeam.summonerId,m_nRoomType,loserCryLv,dt);
}

void CPvpSettleResult::overChampionPvp(int winner, int loser, int result, int winerCryLv, int loserCryLv, int dt)
{	//��ˢ�������������
	CModelHelper::updatePvpInfo(winner);
	CModelHelper::updatePvpInfo(loser);

	CPvpRankModel *pPvpRankModel = CPvpRankModel::getInstance();
	// ƽ��Ҳ��winner loser, ֻ�ǽ�����, ��������庬��
	CPvpModel *pWinnerPvpModel = dynamic_cast<CPvpModel*>(CModelHelper::getModel(winner, MODELTYPE_PVP));
	CPvpModel *pLoserPvpModel = dynamic_cast<CPvpModel*>(CModelHelper::getModel(loser, MODELTYPE_PVP));

	CConfPvpSetting *pPvpSetting = dynamic_cast<CConfPvpSetting*>(
		CConfManager::getInstance()->getConf(CONF_PVP_SETTING));

	CConfPvpGradingChange *pPvpGChangeConf = dynamic_cast<CConfPvpGradingChange*>(
		CConfManager::getInstance()->getConf(CONF_PVP_GRADING_CHANGE));

	//��������ֵ��ֲ�
	int nWinnerGNum = 0, nWinnerGDval = 0;
	//��������ֵ��ֲ�
	int nLoserGNum = 0, nLoserGDval = 0;
	//���Ե�MMR
	int nWinnerMMR = 0, nLoserMMR = 0;
	//����
	int nWinnerIntegral = 0, nLoserIntegral = 0;
	//�仯ϵ��(��ʤ����)
	int nKW = 0, nKL = 0;

	pWinnerPvpModel->GetPvpField(CPN_FD_MMR, nWinnerMMR);
	pWinnerPvpModel->GetPvpField(CPN_FD_INTEGRAL, nWinnerIntegral);
	pWinnerPvpModel->GetPvpField(CPN_FD_DANCONTINUSWINTIMES, nKW);

	pLoserPvpModel->GetPvpField(CPN_FD_MMR, nLoserMMR);
	pLoserPvpModel->GetPvpField(CPN_FD_INTEGRAL, nLoserIntegral);
	pLoserPvpModel->GetPvpField(CPN_FD_DANCONTINUSWINTIMES, nKL);

	//����MMR
	int nWChangeMMR = 0;
	int nLChangeMMR = 0;
	int nWChangeIntegral = 0;
	int nLChangeIntegral = 0;
	if (result == CHALLENGE_DRAW)
	{
		nWChangeMMR = calcMMR(nWinnerMMR, nLoserMMR, nKW, -1);
		nLChangeMMR = calcMMR(nLoserMMR, nWinnerMMR, nKL, -1);
	}
	else
	{
		//����MMR
		nWChangeMMR = calcMMR(nWinnerMMR, nLoserMMR, nKW, 1);
		nLChangeMMR = calcMMR(nLoserMMR, nWinnerMMR, nKL, 0);
		//�������
		nWChangeIntegral = calcIntegral(nWinnerIntegral, nLoserIntegral, 1);
		nLChangeIntegral = calcIntegral(nLoserIntegral, nWinnerIntegral, 0);
	}

	//��ø�������, �������������ֵ
	int nRankNow = 0;
	// ���������û����
	pWinnerPvpModel->GetPvpField(CPN_FD_GRADINGNUM, nWinnerGNum);
	pWinnerPvpModel->GetPvpField(CPN_FD_GRADINGDVAL, nWinnerGDval);
	if (nWinnerGNum < pPvpSetting->m_PvpSetting.GradingNum)
	{
		nWinnerGNum += 1;
		nWinnerGDval += nWChangeMMR;
		pWinnerPvpModel->SetPvpField(CPN_FD_GRADINGNUM, nWinnerGNum);
		pWinnerPvpModel->SetPvpField(CPN_FD_GRADINGDVAL, nWinnerGDval);
		// ����������
		if (nWinnerGNum == pPvpSetting->m_PvpSetting.GradingNum)
		{
			std::vector<PvpGradingChange> &changeConf = pPvpGChangeConf->GetGradingChange();
			for (std::vector<PvpGradingChange>::iterator iter = changeConf.begin();
				iter != changeConf.end(); ++iter)
			{
				if (nWinnerGDval > iter->ChangeMMRLower
					&& nWinnerGDval < iter->ChangeMMRUpper)
				{
					nWChangeIntegral += iter->ChangePoint;
				}
			}
		}
	}
	//ʤ����ս���������ж��ٻ���
	nWinnerIntegral += nWChangeIntegral;
	if (nWinnerIntegral < 0)
	{
		nWinnerIntegral = 0;
	}
	// 5�����ϵ�Ϊ������֮��
	if (nWinnerGNum >= pPvpSetting->m_PvpSetting.GradingNum)
	{
		//��������
		pPvpRankModel->addChampionRank(winner, nWinnerIntegral);
		pPvpRankModel->getChampionRank(winner, nRankNow);
	}

	if (result == CHALLENGE_DRAW)
	{
		setDrawPvpModel(m_nRoomType, winner, nWChangeMMR, nWChangeIntegral, nRankNow);
	}
	else
	{
		//����pvpʤ�����������
		setWinnerPvpModel(m_nRoomType, winner, nWChangeMMR, nWChangeIntegral, nRankNow);
	}

	int nSendLen = sizeof(PvpResultSC);
	char *pWinnerData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLen));
	char *pLoserData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nSendLen));

	//�ɹ��ߵ�����
	PvpResultSC * pResultSC = reinterpret_cast<PvpResultSC*>(pWinnerData);
	pResultSC->roomType = m_nRoomType;
	pResultSC->result = result == CHALLENGE_DRAW ? CHALLENGE_DRAW : CHALLENGE_WON;
	pResultSC->rankNow = nRankNow;
	pResultSC->integral = nWinnerIntegral;
	pResultSC->itemCount = 0;
    // �������ж�����Ƿ�����
    //CBattleDataCache::getInstance()->pushBackBattleData(winner, *pResultSC);

	//ʧ��������������
	nRankNow = 0;
	// ���������û����
	pLoserPvpModel->GetPvpField(CPN_FD_GRADINGNUM, nLoserGNum);
	pLoserPvpModel->GetPvpField(CPN_FD_GRADINGDVAL, nLoserGDval);
	if (nLoserGNum < pPvpSetting->m_PvpSetting.GradingNum)
	{
		nLoserGNum += 1;
		nLoserGDval += nLChangeMMR;
		pLoserPvpModel->SetPvpField(CPN_FD_GRADINGNUM, nLoserGNum);
		pLoserPvpModel->SetPvpField(CPN_FD_GRADINGDVAL, nLoserGDval);
		// ����������
		if (nLoserGNum == pPvpSetting->m_PvpSetting.GradingNum)
		{
			std::vector<PvpGradingChange> &changeConf = pPvpGChangeConf->GetGradingChange();
			for (std::vector<PvpGradingChange>::iterator iter = changeConf.begin();
				iter != changeConf.end(); ++iter)
			{
				if (nLoserGDval > iter->ChangeMMRLower
					&& nLoserGDval < iter->ChangeMMRUpper)
				{
					nLChangeIntegral += iter->ChangePoint;
				}
			}
		}
	}
	// ʧ���߽�����ж��ٻ���
	nLoserIntegral += nLChangeIntegral;
	if (nLoserIntegral < 0)
	{
		nLoserIntegral = 0;
	}
	// 5�����ϵ�Ϊ������֮��
	if (nLoserGNum >= pPvpSetting->m_PvpSetting.GradingNum)
	{
		//��������
		pPvpRankModel->addChampionRank(loser, nLoserIntegral);
		pPvpRankModel->getChampionRank(loser, nRankNow);
	}

	if (result == CHALLENGE_DRAW)
	{
		setDrawPvpModel(m_nRoomType, loser, nLChangeMMR, nLChangeIntegral, nRankNow);
	}
	else
	{
		//����pvpʧ�����������
		setLoserPvpModel(m_nRoomType, loser, nLChangeMMR, nLChangeIntegral, nRankNow);
	}

	pResultSC = reinterpret_cast<PvpResultSC*>(pLoserData);
	pResultSC->roomType = m_nRoomType;
	pResultSC->result = result == CHALLENGE_DRAW ? CHALLENGE_DRAW : CHALLENGE_FAILED;
	pResultSC->rankNow = nRankNow;
    pResultSC->integral = nLoserIntegral;
	pResultSC->itemCount = 0;
    // �������ж�����Ƿ�����
    //CBattleDataCache::getInstance()->pushBackBattleData(loser, *pResultSC);
	//���͸����
	CKxCommManager::getInstance()->sendData(winner, CMD_PVP, CMD_PVP_RESULT_SC, pWinnerData, nSendLen);
	CKxCommManager::getInstance()->sendData(loser, CMD_PVP, CMD_PVP_RESULT_SC, pLoserData, nSendLen);
	//�����ڴ�
	KxServer::kxMemMgrRecycle(pWinnerData, nSendLen);
	KxServer::kxMemMgrRecycle(pLoserData, nSendLen);

	//ͳ����Ϣ
	CTeamModel *pWinTeamModel = dynamic_cast<CTeamModel*>(CModelHelper::getModel(winner, MODELTYPE_TEAM));
	CHECK_RETURN_VOID(pWinTeamModel != NULL);

	TeamInfo WinTeam;
	if (!pWinTeamModel->GetTeamInfo(ETT_SPORTE, WinTeam))
	{
		return;
	}

	CTeamModel *pLostTeamModel = dynamic_cast<CTeamModel*>(CModelHelper::getModel(loser, MODELTYPE_TEAM));
	CHECK_RETURN_VOID(pLostTeamModel != NULL);

	TeamInfo LostTeam;
	if (!pLostTeamModel->GetTeamInfo(ETT_SPORTE, LostTeam))
	{
		return;
	}

	//CCommOssHelper::PvpLogicOss(winner, true, nWinnerIntegral, LostTeam.summonerId, m_nRoomType, winerCryLv, dt);
	//CCommOssHelper::PvpLogicOss(loser, false, nLoserIntegral, WinTeam.summonerId, m_nRoomType, loserCryLv, dt);
}
