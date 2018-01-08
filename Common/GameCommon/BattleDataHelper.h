/*
    ս������ת��Ϊbuffer�ľ�̬��������
*/
#ifndef __BATTLE_DATA_HELPER__
#define __BATTLE_DATA_HELPER__

#include "HeroModel.h"
#include "EquipModel.h"
#include "BufferData.h"
#include "UnionMercenaryModel.h"

// ��ս������Ϣ����������
struct BattleRoomData
{
    int battleType;
    int stageId;
    int stageLv;
    int ext1;
    int ext2;
    int summonerId;
    int mecenaryId;
    std::vector<int> heroIds;
    std::vector<int> outerBuffs;
    std::vector<BuffData> buffs;

    BattleRoomData()
        : battleType(0)
        , stageId(0)
        , stageLv(0)
        , ext1(0)
        , ext2(0)
        , summonerId(0)
        , mecenaryId(0)
    {
    }
};

class CBattleDataHelper
{
public:
    // ���Ӷ����Ϣ
    static bool mercenaryToBuffer(CUserModel *pUserModel, UnionMercenaryInfo* info, CBufferData& bufferData);

    // ��������Ϣ
    static bool playerToBuffer(int uid, const BattleRoomData& roomData, CUserModel *pUserModel, CBufferData& bufferData);

    // ��һ��ʿ���ṹ��䵽�����buffer��
    // ����Ӣ��id��Ӣ��ģ�ͣ�װ��ģ�ͣ���������� + ����
    static bool soldierToBuffer(int heroId, CEquipModel* equipModel, CHeroModel* heroModel, CBufferData& bufferData);

    // ���뷿����Ϣ������
    static bool roomDataToBuffer(int uid, const BattleRoomData& roomData, CBufferData& bufferData);

    // �����û�id���ٻ�ʦid��ʿ��id��Ӷ��id���Զ���֤����װ���뷿���
    // static bool fillEnterRoomBuffer(int uid, int summonerId, int* heroIds, int heroCount, int mecenaryId, CBufferData& bufferData);
};

#endif //__ROOM_H__
