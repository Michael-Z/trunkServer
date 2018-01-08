#include "BattleScene.h" 
#include "Room.h"
#include "Player.h"

#include "RoleComm.h"
#include "Hero.h"
#include "CallSoldier.h"
#include "BattleModels.h"
#include "KxMemPool.h"

#include "Protocol.h"
#include "BattleProtocol.h"

CBattleLayer::CBattleLayer()
: m_fDelta(0.0f)
, m_fTickDelta(0.0f)
, m_pRoom(NULL)
, m_pHelper(NULL)
{
}

CBattleLayer::~CBattleLayer()
{
    SAFE_DELETE(m_pRoomModel);
    SAFE_DELETE(m_pHelper);
}

bool CBattleLayer::init(CPvpRoom *room, CEventManager<int> *manager)
{
	// �����ʼ��
	m_pRoom = room;
    m_pHelper = new CBattleHelper;
    m_pRoomModel = new CRoomModel();

    do 
    {
        // �������˫��������ģ��
        if (!m_pRoomModel->initPVPRoomBylayer(room->getStageId(),
            room->getMasterPlayer(), room->getOtherPlayer()))
        {
            KXLOGERROR("CRoomModel initByRoomData false!!");
            break;
        }

        m_pBlueModel = m_pRoomModel->getMasterModel();
        m_pRedModel = m_pRoomModel->getOtherModel();

        if (m_pBlueModel == NULL || m_pRedModel == NULL)
        {
            KXLOGDEBUG("m_pBlueModel %p or %p m_pRedModel is NULL!!", m_pBlueModel, m_pRedModel);
            break;
        }

        if (!m_pHelper->init(m_pRoomModel, m_pBlueModel, m_pRedModel, manager, this))
        {
            KXLOGDEBUG("m_pHelper init false !!");
            break;
        }
        setTickSpeed(10);
        return true;

    } while (true);
    SAFE_DELETE(m_pRoomModel);
    SAFE_DELETE(m_pHelper);
    return false;
}

void CBattleLayer::update(float dt)
{
    // ����Ҫ����ǰ�˸���debug, ���ٵ�����
    m_fDelta += dt;
    while (m_fDelta >= m_fTickDelta)
    {
        logicUpdate(m_fTickDelta);
        m_fDelta -= m_fTickDelta;
    }
}

void CBattleLayer::setTickSpeed(int frame)
{
    m_fTickDelta = 1.0f / frame;
}

float CBattleLayer::getTickDelta()
{
    return m_fTickDelta;
}

void CBattleLayer::logicUpdate(float delta)
{
	if (m_pHelper->IsBattleOver)
	{
		return;
	}

	// ���ս������ ���� �����Ϸ�����Ὣm_pBattle->IsBattleOver����Ϊtrue
	if (m_pHelper->checkBattleOver())
	{
		int result = m_pHelper->getRoom()->getSettleAccountModel()->getChallengeResult();
		if (result == CHALLENGE_WON)
		{
			m_pRoom->setChallengeResult(CHALLENGE_WON);
			m_pRoom->setWinnerId(m_pBlueModel->getUserId());
			m_pRoom->setLoserId(m_pRedModel->getUserId());
		}
		else if (result == CHALLENGE_FAILED)
		{
			m_pRoom->setChallengeResult(CHALLENGE_FAILED);
			m_pRoom->setWinnerId(m_pRedModel->getUserId());
			m_pRoom->setLoserId(m_pBlueModel->getUserId());
		}
		else
		{
			m_pRoom->setChallengeResult(CHALLENGE_DRAW);
			m_pRoom->setWinnerId(m_pBlueModel->getUserId());
			m_pRoom->setLoserId(m_pRedModel->getUserId());
		}
	}
	else
	{
		// ����ս���߼�
		m_pHelper->processBattle(delta);
	} 
}

void CBattleLayer::onResponse(void *data, int len)
{
    // ������Ϸ���������ս������
    Head* head = reinterpret_cast<Head*>(data);
    switch (head->SubCommand())
    {
    case CMD_BAT_PVPCOMMANDSC:
        {
            BattleCommandInfo* info = reinterpret_cast<BattleCommandInfo*>(head+1);
            if (NULL != m_pHelper)
            {
                if (!m_pHelper->insertBattleCommand(*info))
				{
                    // �����־
                }
            }
        }
        break;

    case CMD_BAT_PVPENDSC:
        break;

    default:
        break;
    }
}

CBattleHelper *CBattleLayer::getBattleHelper()
{
    return m_pHelper;
}
