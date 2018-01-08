#include "CacheStorer.h"
/*
using namespace std;

CCacheStorer::CCacheStorer():
    m_ServerPtr(NULL),
    m_MemPtr(NULL),
    m_ReturnValue(MEMCACHED_SUCCESS),
    m_Expiration(0),
    m_Flags(0)
{
    m_RetryTimes = 3;
    srand(time(NULL));
}

CCacheStorer::~CCacheStorer()
{

}

void CCacheStorer::FreeServer()
{
    //��MEMCACHED�Ͽ��ѽ���������,������������б�
    memcached_server_free(m_ServerPtr);
    //���ڶ��Memcachedʵ������
    vector<memcached_st*>::iterator ator;
    for(ator = m_MemPtrList.begin(); ator != m_MemPtrList.end();ator++)
    {
        memcached_free(*ator);
    } 
}


//����MEMCACHE������Ϣ���������������
bool CCacheStorer::addServer(const std::string addr, int port)
{
    CServerBase::userlog_.LOG_P_LEVEL(LOG_DEBUG, "Add Server Ip %s : %d\n", addr.c_str(), port);

    char address[256] = {0};
    sprintf(address,"%s:%d", addr.c_str(), port);
  
    m_MemPtr = memcached_create(NULL);
    if(NULL == m_MemPtr)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::addServer() m_MemPtr is NULL\n");
        return false;
    }
  
    m_ServerPtr = memcached_servers_parse(address);
    m_ReturnValue = memcached_server_push(m_MemPtr, m_ServerPtr);
  
    if(MEMCACHED_SUCCESS != m_ReturnValue)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::addServer() Couldn't add server: %s Ip %s : %d\n",
        memcached_strerror(m_MemPtr, m_ReturnValue), addr.c_str(), port);
        return false;
    }
  
    //�ͷ�m_ServerPtr?
    memcached_server_list_free(m_ServerPtr);
    m_ServerPtr = NULL;
  
    //ʹ�ö�����Э�鴫�佫���Ӹ�Ч
    memcached_behavior_set(m_MemPtr, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 0);
    //��ӵ�������
    m_MemPtrList.push_back(m_MemPtr);

    return true;
}

memcached_st * CCacheStorer::SelectCache()
{
    int size = m_MemPtrList.size();
    int index = rand()%size;
    CServerBase::userlog_.LOG_P_LEVEL(LOG_DEBUG, "Cache Size %d Index %I64d \n", size, index);
    return m_MemPtrList[index];
}

//����key-value
bool CCacheStorer::Set(const char* key, const char* value, size_t len)
{
    if(NULL == key || NULL == value)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Set() Error\n");
        return false;
    }
    
    m_MemPtr = SelectCache();
    if(m_MemPtr == NULL)
    {
        return false;
    }
    
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_set(m_MemPtr, key, strlen(key), value, len, m_Expiration, 0);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_DEBUG, "Error: CCacheStorer::Set() %s len %zd Expiration %d Flags %d \n", key, len, m_Expiration, 0);
            return true;
        }
        //���»�������������
        //m_MemPtr = SelectCache();
    }
    
    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error CCacheStorer::Set error: %s errno %d key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), m_MemPtr->cached_errno, key);
    return false;
}

//��ȡkey-value
bool CCacheStorer::Get(const char* key, char* value, size_t valuelen, size_t &len)
{
    
    if(NULL == key || NULL == m_MemPtr)
    {
          CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Get() Error\n");
          return false;
    }
  
    m_MemPtr = SelectCache();
  
    //���ݵ���ʵ����
    char *ptr = NULL;

    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        ptr = memcached_get(m_MemPtr, key, strlen(key), &len, &m_Flags, &m_ReturnValue);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            if(NULL != ptr )
            {
                if(len > valuelen)
                {
                    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Get() Buffer Is Too Small %zd, Need %zd\n", len, valuelen);
                    return false;
                }
                //�ѽ����������
                memcpy(value, ptr, len);
                free(ptr);
            }
            return true;
        }
        //����Ҳ������key
        else if(MEMCACHED_NOTFOUND == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR,"Error: CCacheStorer::Get() %s %s len %zd key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), ptr, valuelen, key);
            return false;
        }
    
        //���»�������������
        //m_MemPtr = SelectCache();
    } 
  
    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Get() %s %s len %zd key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), ptr, valuelen, key);

  return false;
}

//����key-value
bool CCacheStorer::Add(const char* key, const char* value, size_t len)
{
    
    if(NULL == m_MemPtr || NULL == key || NULL == value)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Add() Error\n");
        return false;
    }
  
    m_MemPtr = SelectCache();
  
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_add(m_MemPtr, key, strlen(key), value, len, m_Expiration, 0);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            return true;
        }
        //������key�Ѿ�����
        else if(MEMCACHED_STORED == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Add() %s key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), key);
            return false;
        }
    
        //���»�������������
        //m_MemPtr = SelectCache();
    }
  
    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Add() %s key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), key);
    return false;
}

//ɾ��key-value
bool CCacheStorer::Delete(const char* key)
{
    if(NULL == m_MemPtr || NULL == key)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Delete()\n");
        return false;
    }
    
    m_MemPtr = SelectCache();
    
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_delete(m_MemPtr, key, strlen(key), m_Expiration);
        if(MEMCACHED_SUCCESS == m_ReturnValue || MEMCACHED_NOTFOUND == m_ReturnValue)
        {
            return true;
        }
    }

    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Delete() %s errno %d key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), m_MemPtr->cached_errno, key);
    return false;
}

//�滻key-vaule
bool CCacheStorer::Replace(const char* key, const char* value, size_t len)
{
    if(NULL == m_MemPtr || NULL == key || NULL == value)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Replace() Error\n");
        return false;
    }

    m_MemPtr = SelectCache();
  
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_replace(m_MemPtr, key, strlen(key), value, len, m_Expiration, 0);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            return true;
        }
        else if(MEMCACHED_NOTFOUND == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Replace() error: %s\n", memcached_strerror(m_MemPtr, m_ReturnValue));
            return false;
        }
    }

#ifdef DEBUG
    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Replace() error: %s\n", memcached_strerror(m_MemPtr, m_ReturnValue));
#endif
    return false;
}

//׷������
bool CCacheStorer::Append(const char* key, const char* value, size_t len)
{
    if(NULL == m_MemPtr || NULL == key || NULL == value)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Append() Error Ptr Is NULL \n");
        return false;
    }

    m_MemPtr = SelectCache();
    
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_append(m_MemPtr, key, strlen(key), value,  len, m_Expiration, 0);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Append() error: %s key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), key);
            return true;
        }
        else if(MEMCACHED_NOTFOUND == m_ReturnValue || MEMCACHED_NOTSTORED == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Append() error: %s key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), key);
            return false;
        }
    }
  
    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Append() error: %s key %s\n", memcached_strerror(m_MemPtr, m_ReturnValue), key);
    return false;
}

//����
bool CCacheStorer::Increment(const char* key, unsigned int offset, uint64_t* value)
{
    if(NULL == m_MemPtr || NULL == key)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Increment() Error\n");
        return false;
    }

    m_MemPtr = SelectCache();    
  
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_increment(m_MemPtr, key, strlen(key), offset, value);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            return true;
        }
        //���û�����key
        else if(MEMCACHED_NOTFOUND == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Increment() error: %s\n", memcached_strerror(m_MemPtr, m_ReturnValue));
            return false;
        }
    }

    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Increment() error: %s\n", memcached_strerror(m_MemPtr, m_ReturnValue));
    return false;
}

//�Լ�
bool CCacheStorer::Decrement(const char* key, unsigned int offset, uint64_t* value)
{

    if(NULL == m_MemPtr || NULL == key)
    {
        CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Decrement() Error\n");
        return false;
    }

    m_MemPtr = SelectCache();
  
    for(int retrytimes = 0; retrytimes < m_RetryTimes; ++retrytimes)
    {
        m_ReturnValue = memcached_decrement(m_MemPtr, key, strlen(key), offset, value);
        CServerBase::userlog_.LOG_P_LEVEL(LOG_DEBUG, "CCacheStorer::Decrement() %s len %zd offset %u value %d\n",
            key, strlen(key), offset, *value);
        if(MEMCACHED_SUCCESS == m_ReturnValue)
        {
            return true;
        }
        //���û�����key
        else if(MEMCACHED_NOTFOUND == m_ReturnValue)
        {
            CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Decrement() error: %s\n", memcached_strerror(m_MemPtr, m_ReturnValue));
            return false;
        }
    
        //���»�������������
        //m_MemPtr = SelectCache();
    }
    
    CServerBase::userlog_.LOG_P_LEVEL(LOG_ERROR, "Error: CCacheStorer::Decrement() error: %s\n", memcached_strerror(m_MemPtr, m_ReturnValue));
    return false;
}

//��ȡ��ʱʱ��
time_t CCacheStorer::GetExpiration()
{
    return m_Expiration;
}

//���ó�ʱʱ��
bool CCacheStorer::SetExpiration(time_t time)
{
    m_Expiration = time;
    return true;
}

//���ñ�־
bool CCacheStorer::SetFlags(uint32_t flg) 
{
    m_Flags = flg;
    return true;
}
*/
