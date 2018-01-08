#ifndef __SessionHelper_H__
#define __SessionHelper_H__
#include "ConnectModule.h"
#include "NetworkManager.h"
#include "SessionClient.h"
#include "SessionServer.h"
#include "GameInterface.h"


using namespace KxServer;
using namespace ServerProtocol;


class CSessionHelper
{
public:
	CSessionHelper();
	~CSessionHelper();

	// Ϊ�û�����Ȩ��
	static void ServerSubSetPermission(char* buffer, unsigned int len, IKxComm *target, Head* head);
	// �Ͽ��û�,����̨������Ϣ
	static void ServerSubDisconnect(char* buffer, unsigned int len, IKxComm *target, Head* head, CConnectModule* connectModule);
	// ����ĳ����ҵ�·��ֵ
	static void ServerSubSetRoute(char* buffer, unsigned int len, Head* head);
	// ��������
	static void ServerSubTransmit(char* buffer, unsigned int len, Head* head);
	// �㲥���������
	static void ServerSubBoardCast(char* buffer, unsigned int len);
	// ��֤����
	static void ServerSubInit(char* buffer, unsigned int len, CConnectModule* connectModule);
	// ��ѯ��ǰ���������
	static void ServerSubQueryLoad(char* buffer, unsigned int len, Head* head);
    // ת���������ڲ�����
    static void ServerSubForward(char* buffer, unsigned int len, Head* head);
private:

};

#endif