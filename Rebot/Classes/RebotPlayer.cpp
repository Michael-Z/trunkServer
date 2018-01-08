#include "RebotPlayer.h"
#include "LoginProtocol.h"
#include "ShopProtocol.h"
#include "Protocol.h"
#include "KXServer.h"
#include "RebotComm.h"
#include "RebotOpConfig.h"
#include "HeroProtocol.h"
#include "StageProtocol.h"
#include "BagProtocol.h"
#include "PvpProtocol.h"

using namespace KxServer;
using namespace std;


CRebotPlayer::CRebotPlayer()
{
}


CRebotPlayer::~CRebotPlayer()
{
}

bool CRebotPlayer::init(int nUid)							//��ʼ��
{
	m_nUid = nUid;
	m_bFight = false;
	m_bLoginOK = false;
	m_bSendLogin = false;
	m_bSendMatching = false;
	m_pKxComm = NULL;
	m_bBattleID = 0;			//ս��ID
	//û��ս�������ͷ�ս��ָ��
	vector<SOpData> &VectOpData = CRebotOpConfig::getInstance()->GetRebotOp();
	m_VectOpData.assign(VectOpData.begin(), VectOpData.end());

	return true;
}

bool CRebotPlayer::SetKxComm(KxServer::IKxComm* pKxComm)		//����ͨѶ�˿�
{
	m_pKxComm = pKxComm;
	return true;
}

bool CRebotPlayer::PlayerOperator()								//��Ҳ���
{
	if (!m_bLoginOK)
	{
		PlayerCheckLogin();
		return true;
	}

	int nCurTime = (int)time(NULL);
	if (m_VectOpData.size() != 0)
	{
		vector<SOpData>::iterator ator = m_VectOpData.begin();
		SOpData *pData = &(*ator);
		if (!m_bSendMatching)
		{
			PlayerSendRequest(pData);
			if (pData->nMainCmd == 20 && pData->nSubCmd == 1)
			{
				if (m_bBattleID == 0)
				{
					m_bSendMatching = true;
				}
			}

			if (pData->nMainCmd == 8 && pData->nSubCmd == 1)
			{
				return true;
			}

			m_VectOpData.erase(ator);
		}
	}

	return true;
}

void CRebotPlayer::encryptBuffer(int mainCmd, int subCmd, char* buff, int len)
{
	if (CMD_STAGE == mainCmd && CMD_STAGE_FINISH_CS == subCmd)
	{
		int start = 0;
		while (start < len / 2)
		{
			char *num1 = buff + start;
			char *num2 = buff + (len - start - 1);

			int t = *num1 ^ MakeCommand(mainCmd, subCmd);
			*num1 = *num2 ^ MakeCommand(mainCmd, subCmd);
			*num2 = t;

			start += 1;
		}
	}
}

//�����������
bool CRebotPlayer::ProcessRecvData(int mainCmd, int nSubCmd, char *pszContext, int nLen)
{
	switch (mainCmd)
	{
	case CMD_LOGIN:
		ProcessLoginData(nSubCmd, pszContext, nLen);
		break;

	case CMD_USER:
		break;

	case CMD_SUMMONER:
		break;

	case CMD_HERO:
		break;

	case CMD_BAG:
		break;
	case CMD_STAGE:		    // pve����
	case CMD_PVP:		    // pvpƥ��
	{
		processPvpResultData(nSubCmd);
	}
	case CMD_BATTLE:		// pvpս��
		break;
	case CMD_TEAM:
		break;

	case CMD_TASK:
		break;

	case CMD_ACHIEVEMENT:
		break;

	case CMD_GUIDE:
		break;

	case CMD_MAIL:
		break;
	case CMD_MATCH:
		ProcessPvpMatchData(nSubCmd, pszContext, nLen);
		break;
	case CMD_PVPCHEST:
		break;

	case CMD_INSTANCE:
		break;

	case CMD_GOLDTEST:
		break;

	case CMD_HEROTEST:
		break;

	case CMD_TOWERTEST:
		break;

	case CMD_SHOP:
		break;
	case CMD_RANK:
		break;
	case CMD_ACTIVE:
		break;
	case CMD_PAY:
		break;
	default:
		break;
	}

	return true;
}

void CRebotPlayer::PlayerCheckLogin()								//��ҵ�½
{
	if (m_bSendLogin)
	{
		return;
	}

	int nLen = sizeof(Head)+sizeof(LoginCS);
	char *pSendData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nLen));

	Head *phead = (Head*)(pSendData);

	phead->MakeCommand(CMD_LOGIN, CMD_LOGIN_CHECK_TEST_CS);
	phead->id = m_nUid;
	phead->length = nLen;

	LoginCS *pLoginData = (LoginCS*)(phead + 1);
	pLoginData->userid = m_nUid;
	m_pKxComm->sendData(pSendData, nLen);
	KxServer::kxMemMgrRecycle(pSendData, nLen);
	m_bSendLogin = true;
	int nCurTime = (int)time(NULL);
	KXLOGDEBUG("LoginStart!! uid:%d Time: %d", m_nUid,nCurTime);
}

void CRebotPlayer::PlayerLogin()									//��ҵ�½
{
	int nLen = sizeof(Head)+sizeof(LoginCS);
	char *pSendData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nLen));

	Head *phead = (Head*)(pSendData);

	phead->MakeCommand(CMD_LOGIN, CMD_LOGIN_CS);
	phead->id = m_nUid;
	phead->length = nLen;

	LoginCS *pLoginData = (LoginCS*)(phead + 1);
	pLoginData->userid = m_nUid;
	m_pKxComm->sendData(pSendData, nLen);
	KxServer::kxMemMgrRecycle(pSendData, nLen);
}

void CRebotPlayer::PlayerSendRequest(SOpData *pData)				//��ҷ�������
{
	int nTotalLen = 0;
	for (int i = 0; i < pData->VectParm.size(); i++)
	{
		SOpParamData &Param = pData->VectParm[i];

		if (Param.nParamType == emRebotParamInt)
		{
			nTotalLen += sizeof(int);
		}
		else
		{
			nTotalLen += Param.nLen;
		}
	}

	nTotalLen += sizeof(Head);

	char *pSendData = reinterpret_cast<char *>(KxServer::kxMemMgrAlocate(nTotalLen));
	Head *phead = (Head*)(pSendData);

	phead->MakeCommand(pData->nMainCmd,pData->nSubCmd);
	phead->id = m_nUid;
	phead->length = nTotalLen;

	char *pContext = (char*)(phead + 1);
	int nOffset = 0;
	for (int i = 0; i < pData->VectParm.size(); i++)
	{
		SOpParamData &Param = pData->VectParm[i];

		if (Param.nParamType == emRebotParamInt)
		{
			int nValue = 0;
			if (Param.nLen == emRebotUidSParam)
			{
				nValue = m_nUid;
			}
			else if (Param.nLen == emRebotBattleSParam)
			{
				nValue = m_bBattleID;
			}
			else
			{
				nValue = atoi(Param.szParam.c_str());
			}

			memcpy(pContext + nOffset, &nValue, sizeof(int));
			nOffset += sizeof(int);
		}
		else
		{
			memcpy(pContext + nOffset, Param.szParam.c_str(), Param.nLen);
			nOffset += Param.nLen;
		}
	}

	encryptBuffer(pData->nMainCmd, pData->nSubCmd, pSendData + sizeof(Head), nTotalLen - sizeof(Head));
	m_pKxComm->sendData(pSendData,nTotalLen);
	KxServer::kxMemMgrRecycle(pSendData,nTotalLen);

}

//�����½���ݽ���
bool CRebotPlayer::ProcessLoginData(int nSubCmd, char *pszContext, int nLen)
{
	switch (nSubCmd)
	{
		case CMD_LOGIN_FINISH_SC:		//��¼���
		{
			m_bLoginOK = true;
			int nCurTime = (int)time(NULL);
			KXLOGDEBUG("LoginFinish!! uid:%d,nCurTime:%d",m_nUid,nCurTime);
		}
		break;
		case CMD_LOGIN_SC:				//��������·�
		case CMD_LOGIN_USERMODEL_SC:	//���ģ������
		case CMD_LOGIN_BAGMODEL_SC:		//����ģ������
		case CMD_LOGIN_EQUIPMODEL_SC:	//װ��ģ������
		case CMD_LOGIN_SUMMONMODEL_SC:	//�ٻ�ʦģ������
		case CMD_LOGIN_HEROMODEL_SC:		//Ӣ��ģ������
		case CMD_LOGIN_STAGEMODEL_SC:	//�ؿ�ģ������
		case CMD_LOGIN_TEAMMODEL_SC:     //����ģ������
		case CMD_LOGIN_TASKMODEL_SC:     //����ģ������
		case CMD_LOGIN_ACHIEVEMODEL_SC:  //�ɾ�ģ������
		case CMD_LOGIN_GUIDEMODEL_SC:	//����ģ������
		case CMD_LOGIN_UNIONMODEL_SC:	//����ģ������
		case CMD_LOGIN_MAILMODEL_SC:	//�ʼ�ģ������
		case CMD_LOGIN_INSTANCEMODEL_SC:	//�����ģ��
		case CMD_LOGIN_GOLDTESTMODEL_SC: //�����������ģ��
		case CMD_LOGIN_HEROTESTMODEL_SC:	//Ӣ����������ģ��
		case CMD_LOGIN_TOWERTESTMODEL_SC: //������������
		case CMD_LOGIN_PVPMODEL_SC:		//pvpģ������
		case CMD_LOGIN_SHOPMODEL_SC:		//�̵�ģ������
		case CMD_LOGIN_ACTIVEMODEL_SC:	//�ģ������
		case CMD_LOGIN_BAN_SC:			//��������ͣӦ��
		case CMD_LOGIN_RECHANGE_SC:		//��������ɫ������Ӧ��
		case CMD_LOGIN_TICK_SC:			//��������ɫ��TӦ��
		{

		}
		break;
		case CMD_LOGIN_CHECK_SC:		//��֤Ӧ��
		{
			PlayerLogin();
		}
		break;
		default:
			break;
	}

	return true;
}

//����PVPս��ƥ��ʱ��ս��ID
bool CRebotPlayer::ProcessPvpMatchData(int nSubCmd, char *pszContext, int nLen)
{
	switch (nSubCmd)
	{
	case CMD_MATCH_MATCHING_SC:		//����ƥ��
	case CMD_MATCH_REMATCH_SC:		//����ƥ��
	break;
	case CMD_MATCH_MATCHSUCCESS_SC:	//�ɹ�ƥ�䵽��Ϣ
		{
			if (nLen != sizeof(MatchMatchSuccessSC) || pszContext == NULL)
			{
				return false;
			}

			MatchMatchSuccessSC *pMsg = (MatchMatchSuccessSC*)pszContext;

			m_bBattleID = pMsg->battleId;
			m_bSendMatching = false;
		}
	break;
	case CMD_MATCH_CANCEL_SC:		//ȡ��ƥ��
		{
			m_bBattleID = 0;
		}
	break;
	break;
	default:
	break;
	}

	return true;
}


//�������PVPս������
bool CRebotPlayer::processPvpResultData(int nSubCmd)
{
	int nCurTime = (int)time(NULL);
	switch (nSubCmd)
	{
	case CMD_PVP_RESULT_SC:
	{
		KXLOGDEBUG("PvpFinish!! uid:%d,nTime:%d", m_nUid,nCurTime);
		m_VectOpData.clear();			//����PVP���꣬�������������
	}
	default:
		break;
	}

	return true;
}