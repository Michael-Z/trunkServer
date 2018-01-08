#ifndef __SERVER_PROTOCOL_H__
#define __SERVER_PROTOCOL_H__

enum
{
    emRouteLoginKey = 1,                    //��½��֤Key
    emRouteUnionKey,                        //������֤Key
	emRouteBattleKey,                       //ս����֤Key
    emRouteChatKey,                         //������֤Key
};

namespace ServerProtocol
{
	// �������ڲ������� ���� ������ҵ���������ظ�
	enum ServerMainCmd
	{
        ServerMain,
	};

	enum ServerSubCmd
	{
        // �û����
        ServerSubSetPermission,		// ����Ȩ��-SProSetPermission����
        ServerSubDisconnect,		// �Ͽ��û�
        ServerSubPlayerOffLine,     // �û��ѵ���
        ServerSubSetRoute,          // ����·��ID�Լ���Ӧ��ֵ-SProSetRoute����
        ServerSubTransmit,          // ��������
        ServerSubBoardCast,         // �������û���������
        ServerSubInit,              // ������Route+Ȩ��+����-SProInit����

        // �������ڲ����
        ServerSubQueryLoad = 100,   // ��ѯ��ǰ������û���
        ServerSubResponseLoad,      // ���ص�ǰ������û���-SProLoad����
        ServerSubCloseServer,       // �رշ�����
        ServerSubForward,           // �������ڲ�ת������
	};

#pragma pack(1)
	// �������ڲ�ͨ�ð�ͷ
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

    //����ĳ���û���Ȩ������
    struct SProSetPermission
    {
        int     Permission;
    };

    //����ĳ����ҵ�·������
    struct SProSetRoute
    {
        int nRouteKey;
        int nRoteValue;         //·��ֵ
    };

	//ServerSubDisconnect
	struct SProDisConect
	{
		int nDisConectuid;		//���Ͽ����û�ID
	};

    //ServerSubInit
    struct SProInitSCC
    {
        unsigned int nGuestId;
        unsigned int nRouteKey;
        unsigned int nRouteValue;
        unsigned int nPermssion;			//Ȩ��ID

        SProInitSCC()
        {
            nGuestId = 0;
            nRouteKey = 0;
            nRouteValue = 0;
            nPermssion = 0;
        }
    };

    //ServerSubQueryLoad
    struct SPServerLoad
    {
        unsigned int ClientNum;             //�ͻ���������
    };

#pragma pack()
}

#endif
