/*
 * ս������
 * 
 * 1.��ȡ�ⲿ�����ݣ��û���Ϣ��������Ϣ�ȵ�
 * 2.�ڿͻ��˼���ս��������ʾ���ݣ�����ʼ��
 * 3.����BattleHelper��EventManager��һϵ�ж��󣬲���ʼ��
 * 4.ս��ָ�������
 *
 * 2014-12-18 By ��ү
 */
#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "KxCSComm.h"
#include "BattleHelper.h"
#include "EventManager.h"

class CPvpRoom;
class CRoomModel;
class CBattlePlayerModel;
class CBattleHelper;
class CBattleLayer : public Node
{
public:
    CBattleLayer();
    ~CBattleLayer();

public:
    
	bool init(CPvpRoom *room, CEventManager<int> *manager);

    void update(float dt);

    void setTickSpeed(int frame);

    float getTickDelta();

    void logicUpdate(float delta);

    void onResponse(void *data, int len);

    CBattleHelper *getBattleHelper();

private:

    float                     m_fDelta;
    float                     m_fTickDelta;
	CPvpRoom*			      m_pRoom;
    CRoomModel *              m_pRoomModel;
    CBattleHelper *           m_pHelper;
    CBattlePlayerModel *      m_pBlueModel;
    CBattlePlayerModel *      m_pRedModel;
};

#endif
