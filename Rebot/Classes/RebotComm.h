#ifndef _REBOT_COMM_H__
#define _REBOT_COMM_H__

//�����˲���
enum emRebotOpParamType
{
	emRebotParmNone,
	emRebotParamInt,					//����
	emRebotParamStr,					//�ַ���
};

enum emRebotSpecialParam
{
	emRebotNoneSParam,
	emRebotUidSParam,					//uid
	emRebotBattleSParam,				//BattleID
	emRebotRoomSParam,					//roomID
};

//����������
enum emRebotConfig
{
	emRebotNone,
	emRebotOpInterval,			//�����˲������
	emRebotOpIp,				//����������IP
	emRebotOpPort,				//���������Ӷ˿�
	emRebotMinUid,				//��������СUID
	emRebotMaxUid,				//���������UID
};


#endif //_REBOT_COMM_H__
