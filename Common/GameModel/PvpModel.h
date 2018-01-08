#ifndef __PVPMODEL_H__
#define __PVPMODEL_H__

#include <string>
#include <list>
#include <map>

#include "IDBModel.h"
#include "Storage.h"

enum PvpField
{
	PVP_FD_NONE,						//��

	PVP_FD_MMR,							//��ƽ����MMR
	PVP_FD_INTEGRAL,					//��ƽ������������
	PVP_FD_CONTINUSWINTIMES,			//��ƽ��������ʤ/�ܳ�
	PVP_FD_BATTLETIMES,					//��ƽ������ս������
	PVP_FD_DANCONTINUSWINTIMES,			//��ƽ������λ����ʤ��
	PVP_FD_TOTALWINTIMES,				//��ƽ������ʤ������
	PVP_FD_HISTORYHIGHESTRANK,			//��ƽ������ʷ�������
	PVP_FD_HISTORYHIGHESTINTEGRAL,		//��ƽ������ʷ��߻���
	PVP_FD_HISTORYCONTINUSWINTIMES,		//��ƽ������ʷ�����ʤ����

	CPN_FD_WEEKRESETSTAMP,				//������������ʱ��(����1����0��)
	CPN_FD_GRADINGNUM,					//����������������
	CPN_FD_GRADINGDVAL,					//�������������ֲ�
	CPN_FD_MMR,							//������MMR
	CPN_FD_INTEGRAL,					//��������������
	CPN_FD_CONTINUSWINTIMES,			//����������ʤ/�ܳ�
	CPN_FD_BATTLETIMES,					//������������ս������
	CPN_FD_DANCONTINUSWINTIMES,			//��������λ����ʤ��
	CPN_FD_TOTALWINTIMES,				//��������ʤ������
	CPN_FD_HISTORYHIGHESTRANK,			//��������ʷ�������
	CPN_FD_HISTORYHIGHESTINTEGRAL,		//��������ʷ��߻���
	CPN_FD_HISTORYCONTINUSWINTIMES,		//��������ʷ�����ʤ����

	PVPCOMM_FD_BATTLESTAMP,				//���һ����Ϸʱ���
	PVPCOMM_FD_DAYRESETSTAMP,			//������ʱ��
	PVPCOMM_FD_DAYWINTIMES,				//��ʤ��
	PVPCOMM_FD_DAYCONTINUSWINTIMES,		//������ʤ����
	PVPCOMM_FD_DAYMAXCONITNUSWINTIMES,	//�������ʤ����
	PVPCOMM_FD_DAYBATTLETIMES,			//��ս������
	PVPCOMM_FD_REWARDFLAG,				//�ս�����ȡ��ʾ��
	PVPCOMM_FD_ROBOTTIMES,				//ƥ������˴���
	PVPCOMM_FD_LASTCHESTGENTIME,		//���������ʱ��
	PVPCOMM_FD_CHESTSTATUS,				//����״̬
	PVPCOMM_FD_CHESTORDER,				//����Ʒ��˳��
	PVPCOMM_FD_DAYBUYCHESTTIMES,		//�չ��������
    PVPCOMM_FD_ROBOTINDEX1,				//ƥ����������1(����)
	PVPCOMM_FD_ROBOTINDEX2,				//ƥ����������2(��ͨ)
	PVPCOMM_FD_ROBOTINDEX3,				//ƥ����������3(�߼�)

	PVP_FD_MAX
};

enum PvpVerifyField
{
	PVPVERIFY_FD_NONE,					//��
	PVPVERIFY_FD_BATTLEID,				//ս��id
	PVPVERIFY_FD_ROBOTID				//������id
};

struct PVPReport
{
	int oppUserLv;					//սʱ�ȼ�
	int oppHeadIcon;				//���ͷ��
	int battleStamp;				//սʱʱ���
	int battleResult;				//ս�����
	int rankDV;						//������ֵ
	char oppUserName[32];			//ս���������
};

class CPvpModel : public IDBModel
{
public:
	CPvpModel();
	~CPvpModel();

	int GetUid() { return m_nUid; }

public:
	//��ʼ��
	bool init(int uid);
	//ˢ��pvp����
	bool Refresh();
	//����pvpս����ʾ��, �����ù���ʱ��
	bool SetPvpBattleId(int battleId, int robotId);
	//���pvpbattleId
	bool GetPvpBattleId(int &battleId);
	//���pvp robotId
	bool GetPvpRobotId(int &robotId);
	//ȡ������ʱ��
	bool PersistPvpBattleKey();
	//ɾ��pvp����ʱ��
	bool DeletePvpBattleKey();
	//����pvp����
	bool SetPvpField(int field, int value);
	//��������pvp����
	bool SetPvpField(std::map<int, int> &mapValues);
	//���ָ��pvpֵ
	bool GetPvpField(int field, int &value, bool bNew = false);
	//������ȡpvp����
	bool GetPvpField(std::map<int, int> &mapValues, bool bNew = false);

protected:

	bool GetRealFieldFromDB(int field, int &value);

private:

	int						m_nUid;					//��ɫID
	Storage*                m_pStorage;				//���ݿ�
	std::string				m_strPvpKey;			//pvpkey
	std::string				m_strPvpBattleKey;		//pvpս��idkey
	std::map<int, int>		m_MapPvpValues;			//pvp���ݶ�Ӧ��keyֵ
};

#endif //__PVPMODEL_H__
