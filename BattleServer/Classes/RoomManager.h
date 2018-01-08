#ifndef __ROOMMANAGER_H__
#define __ROOMMANAGER_H__

#include <map>
#include <vector>

class IRoom;
class CPvpRoom;
class CPvpRobotRoom;
class CRoomManager
{
private:
    CRoomManager(void);
    ~CRoomManager(void);

public:

    static CRoomManager *getInstance();
    static void destroy();

	//ָ��ս��id�Ƿ��з���
	bool haveRoomWithUid(int uid);
	//ָ��ս��id
	bool haveRoomWithBattleId(int battleId);
	//���뷿��
	CPvpRoom *enterRoom(int uid, int battleId, int roomType);
	//ͨ��ս��id��÷���
	IRoom* getRoomWithBattleId(int battleId);
	//ͨ��uid��÷���
	IRoom* getRoomWithUid(int uid);
	//�Ƴ�����
	void removeWithBattleId(int battleId);

private:

    int                      m_nRoomId;			//ȫ�ַ���id
    static CRoomManager*     m_pInstance;
	std::map<int, IRoom*>    m_PVPRoomMap;		//<battleId, IRoom *>
	std::map<int, int>		 m_UsersBattleId;	//<userId, battleId>
};

#endif 
