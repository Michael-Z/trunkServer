#ifndef _USERUNIONMODEL_H__
#define _USERUNIONMODEL_H__

#include "IDBModel.h"
#include "StorageManager.h"
#include <set>
#include <map>

enum 
{
	USER_UNION_EXPIDITION_REWARD,				//�������ݣ��ַ���

    USER_UNION_EXPIDITION_TIME = 10,            //Զ������
	USER_UNION_ID,								//����ID
	USER_UNION_INDEX,							//��ǰ������ս�ؿ�����
    USER_UNION_CONTRIBUTION,                    //�ۼƹ���
    USER_UNION_DAYPVPLIVENESS,                  //����PVP��Ծ��
    USER_UNION_DAYSTAGELIVENESS,                //���չؿ���Ծ��
    USER_UNION_APPLYUNIONSTAMP,                 //���빫��ʱ���
    USER_UNION_DAYAPPLAYCOUNT,                  //�����������
    USER_UNION_WELFARE,                         //���������ȡ״̬
    USER_UNION_RESETSTAMP,                      //����ʱ���
	USER_UNION_MAX,								//���ֵ
};


class CUserUnionModel : public IDBModel
{
public:
	CUserUnionModel();
	~CUserUnionModel();

	bool init(int nUid);
	bool Refresh();

    bool setUserUnionValue(int nField, int nValue);
    bool setUserUnionValues(std::map<int, int> mapValue);
	bool getUserUnionValue(int nField, int &nValue);
	
	// ���Զ��
	const std::vector<int>& getUserExpiditionValue();

	bool setUserExpiditionValue(std::vector<int> RewardValue);
    bool addUserUnionValue(int nField, int nValue = 1);
    // ������빫����Ϣ
    bool addApplyInfo(int unionID, int liveTime);
    // ɾ�����빫����Ϣ
    bool removeApplyInfo(int unionID);
    // ɾ���������빫����Ϣ
    bool removeAllApplyInfo();
    // ��ȡ���빫����Ϣ
    std::map<int, int>& getApplyInfo(){ return m_MapApplyInfo; }
    // �Ƿ���Զ������
    bool hasExpiditionReward();
    // ������Ϣ(�������ݿ�)
    bool alterUserUnionValue(int nField, int nValue);
    bool alterUserUnionValueAdd(int nField, int nValue);

	//��ɫ������ʱ����ӽ�ɫ��������
	static bool setOffLineExpiditionValue(int nUid, std::vector<int> RewardValue);
    //��ȡ�����������Ϣ
    static bool getOffLineUserUnionValue(int nUid, int nField, int &nValue);
    //���ò����������Ϣ
    static bool setOffLineUserUnionValue(int nUid, int nField, int nValue);
    //�����������б�
    static bool removeOffLineAllApplyInfo(int nUid);
    //�����������б��еĹ���
    static bool removeOffLineApplyInfo(int nUid, int nUnionId);

private:
	bool saveUserRewardData();
private:

	int					 m_nUid;
	Storage *			 m_pStorage;
	std::string			 m_strUserUnionkey;
    std::map<int, int>	 m_MapValue;
    std::map<int, int>	 m_MapApplyInfo;     // ���빫����Ϣ<unionID, livetime>
	std::vector<int>	 m_VectReardValue;
};


#endif //_USERUNIONMODEL_H__
