#ifndef  __REDISASYNCSTORER__
#define  __REDISASYNCSTORER_
#include "hiredis/async.h"
#include "hiredis/hiredis.h"
#include "KxCommInterfaces.h"
#include <string>

using namespace  KxServer;

class CRedisAsyncStorer : public IKxComm
{
public:
	CRedisAsyncStorer();
	~CRedisAsyncStorer();


	// ��ʼ��
	virtual bool init();

	//���ӵ�Redis
	bool Connect(std::string ip, unsigned int port, std::string password);

	// ��������
	virtual int sendData(const char* buffer, unsigned int len);

	// ��������
	virtual int recvData(char* buffer, unsigned int len);

	// �رո�ͨѶ����
	virtual void close();

	// ��ȡsocket fd��
	virtual KXCOMMID getCommId();
	
	// ���յ�����ʱ�����Ļص�����IKxCommPoller����0
	virtual int onRecv();

	// ���ݿɱ�����ʱ�����Ļص�����IKxCommPoller����
	virtual int onSend();

	redisAsyncContext* getRedisAsyncContext();

private:
	redisAsyncContext*  m_Context;			//Redis���������Ķ���
	redisReply*			m_pReply;           //Redis����صĶ���

	unsigned int		m_Port;				//���ӵĶ˿�
	std::string			m_Ip;				//���ӵ�IP
	std::string			m_pssward;			//����
	KXCOMMID			m_sock;				//���ӵ�fd
};



#endif