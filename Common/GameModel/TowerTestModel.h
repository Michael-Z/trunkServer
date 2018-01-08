#ifndef __TOWERTESTMODEL_H__
#define __TOWERTESTMODEL_H__

#include "IDBModel.h"
#include "StorageManager.h"
#include <map>
#include <vector>

enum ETowerTestField
{
	TOWER_FD_NONE = 0,
	TOWER_FD_TIMES,					//��ս����
	TOWER_FD_TIMESTAMP,				//�ָ�ʱ���
	TOWER_FD_FLOOR,					//��ǰ¥��
	TOWER_FD_STAGEID,				//¥��ѡ���Ĺؿ�ID
	TOWER_FD_FLOORSTATE,			//¥��״̬
	TOWER_FD_EVENTPARAM,			//�¼�����
	TOWER_FD_INTEGRAL,				//����
	TOWER_FD_CRYSTAL,				//��սˮ����
	TOWER_FD_STARS,					//��������
	TOWER_FD_OUTERBONUSLIST,		//�ⲿbuff�б�
	TOWER_FD_END,					//������ʶ
};								

enum ETowerFloorState
{
	FLOORSTATE_FIGHTING = 0,		//��ս״̬
	FLOORSTATE_OUTERBONUS,			//�ⲿbuff�¼�״̬
	FLOORSTATE_TREASURE,			//�����¼�״̬
};

class CTowerTestModel : public IDBModel
{
public:
	CTowerTestModel();
	~CTowerTestModel();

public:

	bool init(int uid);

	bool Refresh();

	bool SetTowerTestField(std::map<int, int> &values);

	bool SetTowerTestField(int field, int value);

	bool GetTowerTestField(std::map<int, int> &values);

	int GetTowerTestField(int field);

	bool AddOuterBonus(int outerId);

	bool AddOuterBonus(std::vector<int> &outerIds);

	bool ResetTowerTest();

	std::vector<int> & GetOuterBonusList();

private:

	int					 m_nUid;
	Storage *			 m_pStorage;
	std::string			 m_strTowerkey;
	std::vector<int>	 m_mapOuterBonusList;
	std::map<int, int>   m_mapTowerData;
};

#endif //__TOWERTESTMODEL_H__
