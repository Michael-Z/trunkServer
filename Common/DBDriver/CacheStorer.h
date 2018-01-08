#ifndef _CACHESTORER_H_
#define _CACHESTORER_H_
/*
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include "libmemcached/memcached.h"

/*
Memcached������������״̬   by������ 2011-11-23 19:52:06
*MEMCACHED_SUCCESS                  �ɹ�
*MEMCACHED_FAILURE                  ʧ��
*MEMCACHED_WRITE_FAILURE            ����ʧ��
*MEMCACHED_READ_FAILURE             ��ȡʧ��1
*MEMCACHED_UNKNOWN_READ_FAILURE     ��ȡʧ��2
*MEMCACHED_TIMEOUT                  ��ʱ
*MEMCACHED_NOTFOUND                 �Ҳ���key
*MEMCACHED_STORED                   �Ѿ��洢
*MEMCACHED_NOTSTORED                δ�洢
*MEMCACHED_DATA_EXISTS              �����Ѿ�����
*/
/*
class CCacheStorer
{
public:
    CCacheStorer();
    ~CCacheStorer();
    
    //���һ̨���������
    bool addServer(const std::string addr, int port);
    //������������
    void FreeServer();
    
    //���ó�ʱʱ��
    bool SetExpiration(time_t time);
    //��ȡ��ʱʱ��
    time_t GetExpiration();
    //���ñ�־
    bool SetFlags(uint32_t flg);
    //��ȡ�������״̬
    inline memcached_return GetState(){return m_ReturnValue;};
    
    //����key-value
    bool Set(const char* key, const char* value, size_t len);
    //��ȡkey-value
    bool Get(const char* key, char* value, size_t valuelen, size_t &len);
    //����key-value
    bool Add(const char* key, const char* value, size_t len);
    //ɾ��key-value
    bool Delete(const char* key);
    //�滻key-vaule
    bool Replace(const char* key, const char* value, size_t len);
    //׷������
    bool Append(const char* key, const char* value, size_t len);
    //����
    bool Increment(const char* key, unsigned int offset, uint64_t* value);
    //�Լ�
    bool Decrement(const char* key, unsigned int offset, uint64_t* value);

private:
    inline memcached_st * SelectCache();

private:
    std::vector<memcached_st *>     m_MemPtrList;      //MEMCACHE�������
    memcached_server_st*            m_ServerPtr;       //�������о�� 
    memcached_st*                   m_MemPtr;          //Memcached����ָ��
    memcached_return                m_ReturnValue;     //��������ֵ
    time_t                          m_Expiration;      //��ʱʱ��
    uint32_t                        m_Flags;           //��ʶ
    int                             m_RetryTimes;      //���Դ���
};
*/
#endif //_CACHESTORER_H_
