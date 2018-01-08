#ifndef _GIFT_MODEL_H__
#define _GIFT_MODEL_H__

#include "IDBModel.h"
#include "Storage.h"
#include <string>

enum eGiftType			//������KEY��ʱ��������������
{
	eNoneGiftType,				//������
	eSpecPlatfType,				//ָ��ƽ̨
	eSpecChannelType,			//ָ������
	eSpecServerType,			//ָ��������
};

struct SGoodsData
{
	int nGoodsID;
	int nGoodsNum;
};

struct SGiftData
{
	std::vector<SGoodsData> vectGoods;
};

class CGiftModel : public IDBModel
{
public:
	static CGiftModel* getInstance();

	static void destroy();

	bool init(int nWorldID);

	bool Refresh();

	bool GetGiftData(int nUid, std::string strGift, int &nType, int &nParam, int &nGiftID, SGiftData &Data);

	bool DelGiftData(int nUid, std::string strGift);

	bool modifyUsrPs(int nUid, int nChannelID, const char *strPs,int PsLen);

private:
	CGiftModel();
	~CGiftModel();

	//��������Ƿ�ʹ�ù�
	bool CheckCodeIsUseFull(int nUid, int nCode);

	//���ĳ�������Ϊʹ��״̬
	bool SetCodeUseState(int nUid, int nCode);

private:
	Storage             *m_pStorage;    // ���ݿ�

	static CGiftModel *							m_pInstance;
	int					m_nWorldID;		//��Ϸ������ID
};

#endif //_GIFT_MODEL_H__

