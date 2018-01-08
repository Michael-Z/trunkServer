#ifndef _USER_ACTIVE_MODEL_H__
#define _USER_ACTIVE_MODEL_H__

#include "IDBModel.h"
#include "Storage.h"
#include <map>

class CUserActiveModel : public IDBModel
{
public:
	CUserActiveModel();
	~CUserActiveModel();

	virtual bool init(int uid);

	virtual bool Refresh();

	//�����̵�����
	bool SetShopActiveValue(int nActiveID,int nTimeInterval,int nField, int nValue);

	//��ȡ�̵�����
	bool GetShopActiveValue(int nActiveID, int nField, int &nValue,bool bNew = false);

	//����������ȡ��ʶ
	bool SetTaskActiveIndex(int nActiveID,int nTimeInterval, int nField, int nIndex);

	//������������
	bool SetTaskActiveValue(int nActiveID, int nTimeInterval, int nField, int nValue);

	//�����������������
	bool GetTaskActiveIndex(int nActiveID, int nField, int &nIndex, bool bNew = false);

	//���ĳ����Ĳ�������
	bool GetTaskActiveValue(int nActiveID, int nField, int &nValue, bool bNew = false);

	//��ȡĳ���¿��Ĳ�������
	static bool getMonthCardValue(int uid,int nPid, int &nValue);

private:

	//��ȡ��ͨ�����
	bool getNormalActiveData();

	//��ȡ7������
	bool get7DayActiveData();

	bool getActiveData(int nActiveID,int nActiveType);

	//��ȡ��ɫ����ʱ��
	bool getActiveCreatTime();


private:
	std::map<int, std::map<int,int> >		m_MapShopNum;						//��̵��Ӧ�ĸ������ݻID-���ID-�������
	std::map<int, std::map<int, int> >		m_MapTaskNum;						//������Ӧ����ȡ״̬
	std::map<int, std::map<int, int> >		m_MapTaskValue;						//������Ӧ��ֵ ��������-ֵ
	std::map<int, int>						m_MapMonthCard;						//�¿���ӦPid��Ӧ����ʱ��
	int										m_Uid;
	Storage*								m_pStorage;							// ���ݿ�
	int										m_nCreatTime;						// ��ɫ����ʱ��
};


#endif //_USER_ACTIVE_MODEL_H__
