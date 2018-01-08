#ifndef _BILLNO_MODEL_H__
#define _BILLNO_MODEL_H__

#include "Storage.h"
#include "IDBModel.h"
#include <string>

class CBillnoModel : public IDBModel
{
public:
	// ��ʼ��
	bool init();
	// ˢ�¹ؿ�����
	bool Refresh();

    bool IncBillnoData(int &num);

	static CBillnoModel *getInstance();
	static void		destroy();

private:
	CBillnoModel();
	~CBillnoModel();

private:
	static CBillnoModel *				m_pInstance;
	Storage*							m_pStorage;

};


#endif //_BILLNO_MODEL_H__

