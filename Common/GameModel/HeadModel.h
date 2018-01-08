#ifndef _HEAD_MODEL_H__
#define _HEAD_MODEL_H__

#include "Storage.h"
#include "IDBModel.h"
#include <set>

class CHeadModel : public IDBModel
{
public:
	CHeadModel();
	~CHeadModel();

	bool init(int uid);

	bool Refresh();

	bool AddHeadID(int nValue);

	bool IsHeadIDExist(int nHeadID);

	std::set<int>& GetAllHeadID() { return m_SetHeadID; }

protected:
	//�洢���ݵ����ݿ�
	bool SetHeadIDToDB(int nHeadID);


private:
	int                 m_nUid;
	std::string			m_strHeadKey;
	Storage             *m_pStorage;    // ���ݿ�
	std::set<int>		m_SetHeadID;

};


#endif //_HEAD_MODEL_H__
