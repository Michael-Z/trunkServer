/*
 * storagemanager�� ����baseframe�ڣ� 
 * �������baseframe��Ŀ���ý���ʹ��
 * 2014-02-10
 */

#ifndef _STORAGE_MANAGER_H__
#define _STORAGE_MANAGER_H__

#include <string>
#include <map>
#include "Storage.h"

// ����model��ص�storageId
enum StorageID
{
    // all of redis
	STORAGEID_NONE = 0,			   //��
	STORAGEID_SERVER,			   //�����������Ϣ
	STORAGEID_USER,				   //�û�����
	STORAGEID_BAG,				   //��������
	STORAGEID_EQUIP,			   //װ������
	STORAGEID_HERO,				   //Ӣ������
	STORAGEID_SUMMONER,			   //�ٻ�ʦ����
	STORAGEID_STAGE,			   //�ؿ�����
    STORAGEID_TEAM,                //��������
    STORAGEID_TASK,                //��������
    STORAGEID_ACHIEVEMENT,         //�ɾ�����
	STORAGEID_STORY,			   //��������
	STORAGEID_PASSTEAM,			   //ͨ�ض���
    STORAGEID_UNION,               //��������
    STORAGEID_MAIL,                //�ʼ�����
	STORAGEID_ACTIVITY,			   //�����
	STORAGEID_GOLDTEST,			   //�������
	STORAGEID_HEROTEST,			   //Ӣ������
	STORAGEID_TOWERTEST,		   //��������
	STORAGEID_PVP,				   //PVP
	STORAGE_SHOP,				   //�̵�����
	STORAGE_STAT,				   //ͳ������
	STORAGEID_GLOBALMAIL,		   //ȫ���ʼ�
	STORAGE_CHECK,				   //��½//���
	STORAGE_PAY,				   //��ֵ��֤
};

#define STORAGE_XML_FILE  "..//GameConfig//module_config.xml"
typedef std::map<int, Storage*>     MapStorage;

class StorageManager
{
private:
    StorageManager();
    ~StorageManager();

public:
    static StorageManager* getInstance();
    static void destroy();

public:
	//ͨ�����ñ��ʼ��
	bool InitWithXML(std::string xmlFile);
    //��ȡ�洢���ݿ����
    Storage* GetStorage(int storageId);
    //��ô洢����
    IStorer* GetStorer(int storageId, int uid);
	//ͨ��IP
	IStorer* GetStorer(std::string ip, int port);
    //����洢���ݿ����
	bool InsertStorer(STORER_TYPE type, int storageId, std::string ip, int port, DBRule rule);
    
private:

    static StorageManager *				m_pInstance;
	MapStorage							m_mapStorage;	// <storageId, storage>
	std::map<std::string, IStorer *>	m_mapStorers;	// <ip:port, storer>
};

#endif //_STORAGE_MANAGER_H__
