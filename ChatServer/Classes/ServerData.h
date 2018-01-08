///////////////////////////////////////////////////////
//��������ȡ���ýṹ
//ServerData.h
//�Ƴ�
///////////////////////////////////////////////////////
#ifndef _SERVER_DATA_H__
#define _SERVER_DATA_H__

#include <string.h>

#pragma pack(1)

//��������Ϣ
struct ServerConfigData
{
	char			IP[16];
	unsigned int	Port;
	unsigned int	ServerID;				//������ID
	unsigned int	GroupID;				//��������ID
};

enum ERouteType
{
    RouteSingle,                    // ת��������������
    RouteBroadcast,                 // ת����һ�������
    RouteReturn,                    // ֱ�ӷ���
	SesssionProc		//SessionServer�¼�����
};

struct ServerRouteConfigData
{
    unsigned int GroupID;			// ��������ID
    unsigned int RouteKey;			// ·��Key
    unsigned int Permission;		// Ȩ��ID
    unsigned int RouteType;         // ·�����ͣ�0-������1-�㲥��2-ֱ�ӷ���

	ServerRouteConfigData()
	{
		memset(this, 0, sizeof(*this));
	}
};

#pragma pack()

#endif //_SERVER_DATA_H__
