#include "RedisAsyncStorer.h"

void connectCallback(const redisAsyncContext *c, int status) {
	if (status != REDIS_OK) {
		printf("Error: %s\n", c->errstr);
		return;
	}
	printf("Connected...\n");
}

void getCallback(redisAsyncContext *c, void *r, void *privdata) 
{
	time_t t = time(NULL);
	tm *tt = localtime(&t);
	printf("����ʱ��%d:%d", tt->tm_min, tt->tm_sec);
}

CRedisAsyncStorer::CRedisAsyncStorer()
{
	m_Context = NULL;
	m_pReply  = NULL;
	m_pssward = "";
	m_sock	  = KXINVALID_COMMID;
}

CRedisAsyncStorer::~CRedisAsyncStorer()
{
}

// ��ʼ��
bool CRedisAsyncStorer::init()
{
	return true;
}

//���ӵ�Redis������
bool CRedisAsyncStorer::Connect(std::string ip, unsigned int port, std::string password)
{
	if ((ip.empty()) && (port > 65535))//��������Ϸ���
	{
		return false;
	}
	else
	{
		if (NULL != m_Context)//֮ǰ�Ƿ�������
		{
			redisAsyncDisconnect(m_Context);//�������ͷ�
		}

		m_Context = redisAsyncConnect(ip.c_str(), port);//�첽���ӣ���ʱ������socket�������ӵ��˷�����
		if (m_Context->err)//�����򷵻�
		{
			return false;
		}
		else
		{
			m_sock = m_Context->c.fd; //����fd
			m_PollType = KXPOLLTYPE_IN | KXPOLLTYPE_OUT;			//���ù��ĵ��¼�����,ת����KXPOLL_TYPE��������ӽ�poller
		}


		
		redisAsyncSetConnectCallback(m_Context, connectCallback);

		
		for (int i = 0; i < 1000000; i++)
		{
			int status = redisAsyncCommand(m_Context, NULL, NULL, "zadd za %d 686", i, i);
			if (REDIS_OK != status)
			{
				return false;
			}
		}

		redisAsyncCommand(m_Context, getCallback, NULL, "set 10000 188");

		if ("" != password)
		{
			int status;
			status = redisAsyncCommand(m_Context, NULL, NULL, "auth %s", password.c_str());
			if (REDIS_OK != status)
			{
				return false;
			}
			return true;
		}


		return true;
	}
}

// ��������
int CRedisAsyncStorer::sendData(const char* buffer, unsigned int len)
{
	return 1;
}

// ��������
int CRedisAsyncStorer::recvData(char* buffer, unsigned int len)
{
	return 1;
}

// �رո�ͨѶ����
void CRedisAsyncStorer::close()
{

}

// ��ȡͨѶID��ͨ����socket fd��
KXCOMMID CRedisAsyncStorer::getCommId()
{
	return m_sock;
}

// ���յ�����ʱ�����Ļص�����IKxCommPoller����
// -1 ��ʾ����
// 0 ��ʾ��ȷ
// ����0 ��ʾ���ݿ���δ������
int CRedisAsyncStorer::onRecv()
{
	redisAsyncHandleRead(m_Context);
	return 1;
}

// ���ݿɱ�����ʱ�����Ļص�����IKxCommPoller����
// -1 ��ʾ����
// 0 ��ʾ��ȷ
// ����0 ��ʾ���ݿ���δ������
int CRedisAsyncStorer::onSend()
{
	redisAsyncHandleWrite(m_Context);
	return 1;
}

redisAsyncContext* CRedisAsyncStorer::getRedisAsyncContext()
{
	return m_Context;
}


