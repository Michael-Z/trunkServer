/*
* ͨѶЭ�� ��Ҫ����������Ͱ�ͷ
*
*/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum NetMainCMD
{
    CMD_LOGIN = 1,      // ��¼
    CMD_USER,		    // �û�����
    CMD_SUMMONER,	    // �ٻ�ʦ
    CMD_HERO,		    // Ӣ��
    CMD_BAG,		    // ����
    CMD_STAGE,		    // pve����
    CMD_PVP,		    // pvpƥ��
    CMD_BATTLE,		    // pvpս��
    CMD_TEAM,		    // ����
    CMD_TASK,           // ����
    CMD_ACHIEVEMENT,    // �ɾ�
    CMD_GUIDE,		    // ����
    CMD_UNION,          // ����
    CMD_MAIL,           // �ʼ�
    CMD_INSTANCE,	    // �����
    CMD_GOLDTEST,	    // �������
    CMD_HEROTEST,	    // Ӣ������	
    CMD_TOWERTEST,	    // ��¥����
    CMD_UNIONTASK,      // ��������
    CMD_MATCH,		    // ƥ��Э��
	CMD_PVPCHEST,	    // pvp����
    CMD_SHOP,           // �̵�
    CMD_RANK,		    // ���а�
    CMD_ACTIVE,		    // �
    CMD_ERRORCODE,	    // ������
    CMD_PAY,            // ��ֵ
	CMD_UNIONEXPIDITION,// ����Զ��
    CMD_NOTICE,         // ֪ͨ
    CMD_CHAT,           // ����
    CMD_LOOK,           // �鿴����
	CMD_GM = 50,		// GM������
};

inline int MakeCommand(int main, int sub)
{
    return (main << 16) | (sub & 0x0000ffff);
}

struct Head
{
    int length;
    int cmd;
    int id;

    inline void MakeCommand(int main, int sub)
    {
        cmd = (main << 16) | (sub & 0x0000ffff);
    }

    inline int SubCommand()
    {
        return cmd & 0x0000ffff;
    }

    inline int MainCommand()
    {
        return cmd >> 16;
    }

    inline void* data()
    {
        return this + 1;
    }
};

#endif
