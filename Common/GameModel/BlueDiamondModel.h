#ifndef _BLUDE_DIAMOND_MODEL_H__
#define _BLUDE_DIAMOND_MODEL_H__

#include "IDBModel.h"
#include "Storage.h"
#include <map>

//��������
enum eBDActiveType
{
	eBDActive_NoneType,					//��
	eBDActive_DailyType,				//ÿ�����
	eBDActive_NewPType,					//�������
	eBDActive_IncrLvType,				//�������
	eBDActive_CPayType,					//�������
};

//��������
enum eBDType
{
	eBDNormalType = 0x1,			//��������
	eDBYearType = 0x2,				//�������
	eDBVipType = 0x4,				//��������
};

enum eBDUserType
{
	eBDNoneUserType,				//��ͨ�û�
	eBDNormalUserType = 1,			//��ͨ�����û�
	eBDNYearUserType = 2,			//��������û�
	eBDLuxuryType = 3,				//���������û�
	eBDLuxuryYType = 4,				//������������û�
};

class CBlueDiamondModel : public IDBModel
{
public:
	CBlueDiamondModel();
	~CBlueDiamondModel();

	virtual bool init(int uid);

	virtual bool Refresh();

	//��ȡQQ��������
	bool getQQActiveValue(int nActiveID, int nField, int &nIndex, bool bNew = false);

	//����QQ��������
	bool setQQActiveValue(int nActiveID, int nField, int nIndex);

	//����QQ��������
	bool reSetQQActiveValue();

private:

	bool getActiveData(int nActiveID, int nTaskID);

	std::map<int, std::map<int, int> >		m_MapQQIndexNum;					//QQ������Ӧ����ȡ״̬
	int										m_Uid;
	Storage*								m_pStorage;							// ���ݿ�
};

#endif 
