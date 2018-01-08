#ifndef __CHAT_HELPER_H__
#define __CHAT_HELPER_H__

#include <string>
#include "ConnectModule.h"
#include "NetworkManager.h"
#include "ChatClient.h"
#include "ChatServer.h"
#include "ServerProtocol.h"

class CChatHelper
{
public:
    CChatHelper();
    ~CChatHelper();

	// Ϊ�û�����Ȩ��
    static void ServerSubSetPermission(char* buffer, unsigned int len, KxServer::IKxComm *target, ServerProtocol::Head* head);
	// �Ͽ��û�,����̨������Ϣ
    static void ServerSubDisconnect(char* buffer, unsigned int len, KxServer::IKxComm *target, ServerProtocol::Head* head, CConnectModule* connectModule);
	// ����ĳ����ҵ�·��ֵ
    static void ServerSubSetRoute(char* buffer, unsigned int len, ServerProtocol::Head* head);
	// ��������
    static void ServerSubTransmit(char* buffer, unsigned int len, ServerProtocol::Head* head);
	// �㲥���������
	static void ServerSubBoardCast(char* buffer, unsigned int len);
	// ��֤����
	static void ServerSubInit(char* buffer, unsigned int len, CConnectModule* connectModule);
	// ��ѯ��ǰ���������
    static void ServerSubQueryLoad(char* buffer, unsigned int len, ServerProtocol::Head* head);
    // ת���������ڲ�����
    static void ServerSubForward(char* buffer, unsigned int len, ServerProtocol::Head* head);
    // �����Ƿ�Ϸ�
    static bool isContentLegal(int chatType, const std::string& str);
    // ����Ƿ�Ϸ�
    static bool isPlayerLegal(int uid);
private:

};

#endif