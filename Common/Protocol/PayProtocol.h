/*
*   ֧��Э��
*
*   2016-6-29 by ��ү
*/
#ifndef __PAY_PROTOCOL_H__
#define __PAY_PROTOCOL_H__

#pragma pack(1)

enum PAYROTOCOL
{
    CMD_PAY_CSBEGIN,
    CMD_PAY_YSDK_CS,
    CMD_PAY_CSEND,

    CMD_PAY_SCBEGIN = 100,
    CMD_PAY_YSDK_SC,
    CMD_PAY_SCEND,
};

//////////////////////////////////��Ѷ֧��////////////////////////////////////////

struct PayYSDKSC
{
	int result;         // ��ֵ��� 0 ʧ�� -1�����ڣ�1-�ɹ�
	int nVipLv;			//Vip�ȼ�
	int nVipNum;		//Vip��ֵ����
	int nDiamond;		//����ʯ��
	int nPid;			//��ֵ��Ʒ���
};

#pragma pack()

#endif