#ifndef _STORAGE_H__
#define _STORAGE_H__

#include <vector>
#include <string>

enum STORER_TYPE
{
    STORE_NONE,
    STORE_TMEM,
    STORE_MYSQL,
    STORE_CACHE,
    STORE_REDIS,
    STORE_OTHER
};

class IStorer;
struct DBRule
{
    int range_minvalue;          // id��Сֵ
    int range_maxvalue;          // id���ֵ
	IStorer * storer;		     // ���ݿ�ʵ��
};

class Storage
{
public:
    Storage();
    virtual ~Storage();
    
public:

	virtual bool InitStorage(STORER_TYPE type);

    virtual IStorer* GetStorer(int uid);
    
	virtual bool InsertStorer(DBRule dbRule);

    virtual STORER_TYPE GetStoreType() { return m_type;}

	virtual int GetModeSize() { return m_DBRulelist.size(); }
  
protected:

	STORER_TYPE				m_type;			// ��storge�����ݿ�����
	std::vector<DBRule>		m_DBRulelist;	// ���ݿ����
};

#endif //_STORAGE_H__
