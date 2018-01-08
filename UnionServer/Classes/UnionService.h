/*
* ��������
* 1.�����������(���롢��������ְ��������)
* 2.����������ս(�����������͹����Ŷ�����)
*/

#ifndef __UNION_SERVICE_H__
#define __UNION_SERVICE_H__

#include "KxCommInterfaces.h"

class CUnionService
{
public:
    /////////////////////////////// ����������� ///////////////////////////////////////////
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���󹫻���Ϣ
	static void processUnionInfoService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �����Ա�б�
	static void processMembersService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���󴴽�����
	static void processCreateService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ����������빫��
    static void processApplyService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �˻�
    static void processExitService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ������ڹ����¼��б�
    static void processLogListService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ������ڹ�������б�
    static void processAuditListService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �����Ϣ
    static void processAuditService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ������ز���(�߳������Ρ�Ȩ���ƽ���)
    static void processFunctionService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // �����б�(û�й���,���󹫻��б�)
    static void processOutUnionListService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��������
    static void processSearchService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���û��
    static void processEmblemService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���ù�����
    static void processUnionNameService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���ù���
    static void processUnionNoticeService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ��ȡ����
    static void processWelfareService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ���������Ϣ
    static void processSetAuditService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ���󹫻�ȫ��Ӷ����Ϣ
	static void processMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ��ǲӶ��
	static void processDispatchMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// �ٻ�Ӷ��
	static void processRecallMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ���󵥸�Ӷ����ϸ��Ϣ
	static void processSingleMercenaryService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // ����������ת��������
    static void processUnionForwardDataService(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__UNION_SERVICE_H__
