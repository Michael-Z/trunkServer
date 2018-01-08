#ifndef _ERROR_CODE_PROTOCOL_H__
#define _ERROR_CODE_PROTOCOL_H__

#pragma pack(1)

enum ERRORCODEPROTOCOL
{
	ERRORCODE_PROTOCOL = 1,					//����������Ϣ
};

enum ErrorCode
{
///////////////////////100ս��////////////////////////////////////////
	//ERROR_BATTLE_
	ERROR_BATTLE_EXIST = 100,				//��ɫ��������ս��
	ERROR_BATTLE_TIMESNOTENOUGH = 101,		//ս����������
	ERROR_BATTLE_STATEERROR = 102,			//ս��״̬�쳣
	ERROR_BATTLE_MAXTOWER= 103,				//����¥�����
	ERROR_BATTLE_STARTERROR = 104,			//�������ǲ���
	ERROR_BATTLE_ENERGYERROR = 105,			//��������
	// pvp
	ERROR_BATTLE_PVPHAVEROOM = 150,			//pvp��������
	ERROR_BATTLE_LOGIC = 199,

///////////////////////200����///////////////////////////////////////
	//��������
	ERROR_BAG_ISNOTENOUGH = 200,			//��������
	ERROR_BAG_ITEMNOTEXIST = 201,			//���߲�����
	ERROR_BAG_EQUIPNOTEXIST = 202,			//װ��������
	ERROR_BAG_GOODSNOTENOUGH = 203,			//���߲���
	ERROR_BAG_MAX_BAGNUM	 = 204,			//��������
/////////////////////////300Ӣ��/////////////////////////////////////////////
	ERROR_HERO_BAGNOTENOUGH		= 300,			//��������
	ERROR_HERO_NOTEXIST			= 301,			//Ӣ�۲�����
	ERROR_HERO_EQUIPNOTEXIST	= 302,			//Ӣ��û�д�װ��
	ERROR_HERO_MAXSTART			= 303,			//Ӣ������
	ERROR_HERO_LEVELNOTENOUGH   = 304,			//Ӣ�۵ȼ�����
	ERROR_HERO_FRAGNOTENOUGH    = 305,			//Ӣ����Ƭ����
	ERROR_HERO_COMPLETE			= 306,			//Ӣ��Ϊ����
	ERROR_HERO_SKILLBOOK		= 307,			//�����鲻��Ӧ
	ERROR_HERO_LEVELLIMIT       = 308,			//Ӣ�۵ȼ�����
//////////////////////////350�츳///////////////////////////////////////////////////
	ERROR_SKILL_STARTFAIL		= 350,			//�츳����ʧ��
	ERROR_SKILL_ISEXIST			= 351,			//�츳�Ѿ�������
////////////////////////��Դ400////////////////////////////////////////
	ERROR_GOLD_NOTENOUGH		= 400,			//��Ǯ����
	ERROR_DIAMOND_NOTENOUGH		= 402,			//��ʯ����
	ERROR_ENERGY_NOTENOUTH		= 403,			//��������
	ERROR_TOWERCOIN_NOTENOUGH	= 404,			//�����Ҳ���
	ERROR_PVPCOIN_NOTENOUGH		= 405,			//�����Ҳ���
	ERROR_VIP_NOTENOUGH			= 406,			//VIP����
	ERROR_LEVEL_NOTENOUGH		= 407,			//�ȼ�����
	ERROR_SHILIANTIMES_NOTENOUGH = 408,			//������������
	ERROR_GOLDBUYTIMES_NOTENOUGH = 409,			//��ҹ����������
	ERROR_ENERGYBUYTIMES_NOTENOUGH = 410,		//���������������
	ERROR_EITLETIMES_NOTENOUGH = 411,			//��Ӣ�ؿ���ս��������
	ERROR_UNIONCONTRIB_NOTENOUGH = 412,			//���ṱ�ײ���
	ERROR_ELTLEBUYTIMES_NONENOUGH = 413,		//��Ӣ�ؿ������������
	ERROR_SHOPFRESHTIMES_NOTENOUGH = 414,		//�̵�ˢ�´�������
///////////////////////////�ʼ�450//////////////////////////////////
	ERROR_EMAIL_HAVEGET		= 450,				//�ʼ�����ȡ
////////////////////////////����500//////////////////////////////////////
	ERROR_TASK_HAVEGET		= 500,				//��������ȡ
	ERROR_TASK_NOTFINISH	= 502,				//����δ���
	ERROR_TASK_NOTEXIST		= 503,				//���񲻴���
//////////////////////////////�ɾ�550/////////////////////////////////////////
	ERROR_ACHIEVE_HAVEGET	= 550,				//�ɾ�����ȡ
	ERROR_ACHIEVE_NOTFINISH	= 551,				//�ɾ�δ���
	ERROR_ACHIVEV_NOTEXIST	= 552,				//�ɾͲ�����
///////////////////////////////�ٻ�ʦ600/////////////////////////////////////////////
	ERROR_SUMMEONER_NOTEXIST = 600,				//�ٻ�ʦ������
///////////////////////////////����650/////////////////////////////////////////////
	ERROR_SHILIAN_TIMESNOTENOUGH = 650,			//������������
	ERROR_SHILIAN_NOTSTART	= 651,				//����δ����
////////////////////////////////�ؿ�700///////////////////////////////////////////////////
	ERROR_STAGE_NOTSTART = 700,					//�ؿ�δ����
	ERROR_STAGE_NOTUNLOCK = 701,				//�ؿ�δ����
	ERROR_STAGE_FINISH = 702,					//�ؿ������, �����ٴ���ս
/////////////////////////////////����////////////////////////////////////////////////////////
	ERROR_UNION_EXPIDITION_OVER = 750,			//Զ������
	ERROR_UNION_EXPIDITION_STAGEPASS,			//�ؿ��Ѿ�ͨ��
	ERROR_UNION_EXPIDITION_REARDERROR,			//����δ���ţ���������
////////////////////////////////���900//////////////////////////////////////////////
	ERROR_GIFTCODE_INVALID = 900,				//�����Ƿ�
	ERROR_GIFTPLATFORM_INVALID = 901,			//ƽ̨�Ƿ�
	ERROR_GIFTCHANNELID_INVALID = 902,			//�����Ƿ�
	ERROR_GIFTSERVERID_INVALID = 903,			//�������Ƿ�
/////////////////////////////////�/////////////////////////////////////
	ERROR_MONTHCAR_OUTDATE = 950,				// �¿�����
	ERROR_MONTHCARD_NOTEXIST,					//û�й����¿�
	ERROR_CURDAY_CANTGET,						//�¿����ղ�����ȡ
};

struct ErrorCodeData
{
	int nCode;
};


#pragma pack()

#endif //_ERROR_CODE_PROTOCOL_H__
