#ifndef _STAT_MODEL_H__
#define _STAT_MODEL_H__

#include <string>
#include "Storage.h"
#include "IDBModel.h"
#include "RedisStorer.h"

class CStatModel : public IDBModel
{
public:
	static CStatModel *getInstance();
	static void		  destroy();

	virtual bool init();

	bool MessageStart(int nUid, std::string szType);
	//Type��ʶ�Ƿ�Ϊ���һ�����ݣ�0��ʶ���ǣ�1-��ʶ��
	bool MessageDataAdd(std::string szDataKey, std::string szDataValue, int Type = 0);
	bool MessageEnd(std::string szTime);

private:
	CStatModel();
	~CStatModel();
	bool MessageTypeAdd(std::string szType);
private:
	std::string		m_MessageData;			//ͳ�����ݿ�
	int				m_ModNum;				//ȡģֵ
	static CStatModel*		m_pInstance;	//ͳ��ģ��ʵ��
	std::string		m_StatUidKey;			//ͳ��������Key
	CRedisStorer *	m_pRedisStorer;
};


#endif //_STAT_MODEL_H__
