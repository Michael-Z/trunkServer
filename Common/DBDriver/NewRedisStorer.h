#ifndef __REDISSTORER_H__
#define __REDISSTORER_H__

#include <string>
#include <map>
#include <set>
#include <vector>
#include "hiredis.h"

#include "IStorer.h"

enum NEWSTORERESULT
{
	SR_EXCEPTION = -2,
	SR_FAIL		 = -1,
	SR_SUCCESS	 = 0,
};

class CNewRedisStorer : public IStorer
{
public:
    CNewRedisStorer();
    ~CNewRedisStorer();

public:
	// ����ָ��redis������, ʹ������ʽ����
	virtual int connect(const char* ip, int port, const char* psw);
	// ����֮ǰ��ip�˿�����redis������
    virtual int reconnect();
	// AUTH psw, ��������
	virtual int password(const char* psw);
	// ���redis����
    virtual redisContext* getRedisContext();

    // ����append��ʽ��ȡ���ݣ�����֮��ִ�е�redisָ��ᱻ�����ڱ���
    virtual int beginAppend();
    // append��ʽ�������endAppend������appendģʽ����ȡ����
    virtual int endAppend(std::vector<redisReply*>& results);
    // �ڷ�append��ʽ�»�ȡ����
    virtual redisReply* getReply();

    //*********************Key����*********************
    // DEL key, ɾ��KEY
    int delKey(const char* key);
    // EXSIT key, �ж�Key�Ƿ����
    int existKey(const char* key);
    // EXPIREAT key timestamp, ָ��Key��ĳ��ʱ���(time())����
    int expireAt(const char* key, int timestamp);
    // EXPIRE key seconds, ָ��Key�೤ʱ��֮�����
    int expire(const char* key, int seconds);
    // TTL key, ��ȡKey���ڵ�ʣ��ʱ�䣬reply����integer
    int ttl(const char* key);
	// PERSIST key, ȡ��key�Ĺ���ʱ�䣬reply����integer
    int persist(const char* key);

    //*********************�ַ�������**********************
	// GET key, reply�����ַ���
    int getString(const char* key);
	// SET key value, reply����ok
    int setString(const char* key, const char* value, unsigned int len);
    int setString(const char* key, const char* value);
    int setString(const char* key, int value);
	// SETRANGE key offset value valuelen
	int setStringRange(const char* key, int offset, char* value, int valuelen);
	// GETRANGE key offset offset+len
	int getStringRange(const char* key, int offset, int len);
	// INCRBY key value 
    int incrString(const char* key, int value = 1);

	//*********************Hash��������*********************
    // ���������ֶΣ�HMSET key field value [field value ...]��reply�����ַ���
    int setHash(const char* key, std::map<int, int> &mapset);
    int setHash(const char* key, std::map<std::string, std::string> &mapset);
	int setHash(const char* key, std::map<int, std::string> &mapset);
    int setHash(const char* key, std::map<std::string, int> &mapset);
    // ������ȡ�ֶΣ�HMGET key field [field ...]
    // reply����REDIS_REPLY_ARRAY����field1 value1 filed2 value2��ʽ����
    int getHash(const char* key, std::map<int, int> &mapget);
    int getHash(const char* key, std::map<std::string, std::string> &mapget);
    int getHash(const char* key, std::map<int, std::string> &mapget);
    int getHash(const char* key, std::map<std::string, int> &mapget);
    // ��ȡ�����ֶΣ�HGETALL key
    // reply����REDIS_REPLY_ARRAY����field1 value1 filed2 value2��ʽ����
    int getHash(const char* key);
    // ��ȡָ���ֶΣ�HGET key field
    int getHashByField(const char* key, const char* field);
    int getHashByField(const char* key, int field);
    // ����ָ���ֶΣ�HSET key filed value
    int setHashByField(const char* key, int field, int value);
    int setHashByField(const char* key, int field, char* value);
    int setHashByField(const char* key, int field, char* value, int len);
    int setHashByField(const char* key, const char* field, int value);
    int setHashByField(const char* key, const char* field, const char* value);
    int setHashByField(const char* key, const char* field, char* value, int len);
    // ɾ��ָ���ֶΣ�HDEL key filed
    int delHashByField(const char* key, const char* field);
    int delHashByField(const char* key, int field);
    // HEXISTS key field, �ж��Ƿ���field
    int hashFieldExist(const char* key, const char* field);
    int hashFieldExist(const char* key, int field);
    // HINCRBY key field value ������Ӧhash field
    int increHashByField(const char* key, const char* field, int value);
    int increHashByField(const char* key, int field, int value);
    // HLEN key, ����hashԪ�ظ���
    int hashLen(const char* key);

    //*********************Set��������*********************
	// SADD key value, ���һ��Ԫ��
    int setAdd(const char* key, const char* value);
    int setAdd(const char* key, const char* value, unsigned int len);
	// SREM key value, �Ƴ���ӦԪ��
    int removeSetMember(const char* key, const char* value);
    int removeSetMember(const char* key, const char* value, unsigned int len);
	// SMEMBERS key, �������Ԫ��
    int getSetMembers(const char* key);
	// SISMEMBER key value, �鿴�Ƿ���value
    int isSetMember(const char* key, const char* value);
    int isSetMember(const char* key, const char* value, unsigned int len);
	// SMOVE srckey destkey value, ��value��srckey���Ƶ�destkey��
    int setMove(const char* srckey, const char* destkey, const char* value);
	int setMove(const char* srckey, const char* destkey, const char* value, unsigned int len);

    //*********************ZSet��������*********************
	// ZADD key score member, ��key�����һ����¼
    int zadd(const char* key, int member, int score);
	// ZCOUNT key, ���minscore��maxscoreΪ0, ����zset��Ա����, ���򷵻ط�Χ�ڵĳ�Ա����
	int zcount(const char* key, int minscore = 0, int maxscore = 0);
	// ZRANK key member, ����score����, ��С��������, score��С����0
	int zrank(const char* key, int member);
	// ZREVRANK key member, ����score����, �Ӵ�С����, score�������0
	int zrevrank(const char* key, int member);
	// ZSCORE key member, ����member��scoreֵ
	int zscore(const char* key, int member);
	// ZRANGE key start stop, ����score��С��������, ȡ�����������ڳ�Ա
    int zrange(const char* key, int start, int stop);
	// ZREVRANGE key start stop, ����score�Ӵ�С����, ȡ�����������ڳ�Ա -1�������һ��
	int zrevrange(const char* key, int start, int stop);
	// ZREMRANGEBYRANK key start stop, ͨ�������Ƴ���Ա, -1�������һ��
    int zremovebyrank(const char* key, int start = 0, int stop = -1);
	// ZREMRANGEBYSCORE key minscore maxscore, �Ƴ�score�����ڵĳ�Ա
	int zremrangebyscore(const char* key, int minscore, int maxscore);

    //*********************List��������*********************
    // LPUSH key value, ��һ��������ֵ�����ڱ�ͷ
    int listPush(const char* key, char *value, int len);
    // LPUSH key value, ����intԪ��
    int listPush(const char* key, int value);
	// LPOP key, non-block ������������pop�׸�Ԫ�أ�reply���ص�һ��Ԫ�ص�ֵ
    int listPop(const char* key);
    // LRANGE key start stop, ���������ڵĳ�Ա
    int listRange(const char* key, int start = 0, int stop = -1);
    // LINDEX key index, ��ö�Ӧ��Ա, 0�����׸�Ԫ�أ�1����ڶ���Ԫ�أ�-1�������һ��Ԫ�أ� -2�������ڶ���Ԫ��
    int listIndex(const char* key, int index);
	// count > 0 : �ӱ�ͷ��ʼ���β�������Ƴ��� value ��ȵ�Ԫ�أ�����Ϊ count
	// count < 0 : �ӱ�β��ʼ���ͷ�������Ƴ��� value ��ȵ�Ԫ�أ�����Ϊ count �ľ���ֵ��
	// count = 0 : �Ƴ����������� value ��ȵ�ֵ��
    int listRemove(const char* key, int count, const char* value);
    int listRemove(const char* key, int count, int value);

private:
    // �Զ����ݲ���ģʽִ��ָ��
    int executeCommand(const char *format, ...);

private:

	bool					m_bIsAppendMode;	//�Ƿ���appendģʽ
    redisContext*			m_pContext;         //Redis���������Ķ���
    redisReply*				m_pReply;           //Redis�������
	int						m_Port;				//�˿�
	int						m_AppendCount;		//append�������
    std::string				m_Ip;				//ip
	std::string				m_Password;			//����
};

#include "KXServer.h"

class CRedisAsyncStorer : public CNewRedisStorer, KxServer::IKxComm
{
public:
    CRedisAsyncStorer();
    ~CRedisAsyncStorer();

public:
    // ����ָ��redis������, ʹ������ʽ����
    virtual int connect(const char* ip, int port, const char* passward);
    // ����֮ǰ��ip�˿�����redis������
    virtual int reconnect();
    // ���redis����
    virtual redisContext* getRedisContext();
    
    //*********************IKxComm��������*********************
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

private:
    // �������·���
    // ����append��ʽ��ȡ���ݣ�����֮��ִ�е�redisָ��ᱻ�����ڱ���
    virtual int beginAppend();
    // append��ʽ�������endAppend������appendģʽ����ȡ����
    virtual int endAppend(std::vector<redisReply*>& results);
    // �ڷ�append��ʽ�»�ȡ����
    virtual redisReply* getReply();
};

#endif
