#ifndef __IDBMODEL_H__
#define __IDBMODEL_H__

#include "GameDef.h"

enum ModelType
{
	MODELTYPE_NONE,					//��
	MODELTYPE_SERVER,				//������
	MODELTYPE_PASSTEAM,				//ͨ�ض���
	MODELTYPE_USER,					//�û�
	MODELTYPE_BAG,					//����
	MODELTYPE_EQUIP,				//װ��
	MODELTYPE_HERO,					//Ӣ��
	MODELTYPE_SUMMONER,				//�ٻ�ʦ
	MODELTYPE_STAGE,				//�ؿ�
	MODELTYPE_TEAM,					//����
	MODELTYPE_TASK,					//����
	MODELTYPE_ACHIEVEMENT,			//�ɾ�
	MODELTYPE_GUIDE,				//��������
	MODELTYPE_MAIL,					//�����ʼ�
	MODELTYPE_INSTANCE,				//��������
	MODELTYPE_HEROTEST,				//Ӣ������
	MODELTYPE_GOLDTEST,				//�������
	MODELTYPE_TOWERTEST,			//��������
	MODELTYPE_PVP,					//pvp
	MODELTYPE_SHOP,					//�̵�
	MODELTYPE_ACTIVE,				//���������
	MODELTYPE_HEAD,					//���˽���ͷ������
	MODELTYPE_USERUNION,			//���˹�������
	MODELTYPE_MERCENARY,			//Ӷ��ϵͳ
	MODELTYPE_PVPTASK,				//pvp����
	MODELTYPE_BDACTIVE,				//����
    MODELTYPE_PVPCHEST,				//pvp����
};

class IDBModel
{
public:
	IDBModel();
	virtual ~IDBModel();

public:

	virtual bool init(int uid) { return true; }
	virtual bool Refresh() { return true; }
};

#endif //__IDBMODEL_H__