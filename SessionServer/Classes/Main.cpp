/*
 * SessionServer ��Ҫ����ά���Ϳͻ���֮�������
 * 1.�ͻ������ӶϿ�ʱ���͵���Ϸ������
 * 2.�����ѵ�½�û�+�����οͣ����û��Ϸ��Խ�����֤
 * 3.���տͻ�����Ϣת����ָ���ĺ��
 * 4.���յ������Ϣת����ָ����ǰ��
 * 5.�յ����ָ��ǿ�ƹر�ָ����ǰ��
 * 6.UserId ��Ӧ CommunicationId �Ĺ�ϣ
 * 
 * 2013-10-30 By ��ү
 */
#include "KXServer.h"
#include "SessionServer.h"

/* ѡ��:
	-g	��
	-si ������id
	-ip �󶨵�ip��ַ
	-p �󶨶˿�
	-sg ����������
	-sc ����������
*/

int main(int argc, char ** argv) 
{
    CSessionServer* server = CSessionServer::getInstance();
	if (server->initServerInfo(argc, argv))
	{
		server->startServer();
	}
    CSessionServer::destroy();
    return 0;
}
