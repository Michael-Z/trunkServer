/*
 * ����Э��
*/

#ifndef __UNION_PROTOCOL_H__
#define __UNION_PROTOCOL_H__

#pragma pack(1)

enum UNIONPROTOCOL
{
    CMD_UNION_CSBEGIN,
    CMD_UNION_INFO_CS,			// ������ڹ�����Ϣ
    CMD_UNION_MEMBERS_CS,		// ������ڹ������г�Ա
    CMD_UNION_CREATE_CS,		// ��������
    CMD_UNION_APPLY_CS,			// ������빫��
    CMD_UNION_EXIT_CS,			// �˻�
    CMD_UNION_LOGLIST_CS,		// ������ڹ����¼��б�
    CMD_UNION_AUDITLIST_CS,		// ������ڹ�������б�
    CMD_UNION_AUDIT_CS,		    // �����Ϣ
    CMD_UNION_FUNCTION_CS,		// ������ز���(�߳������Ρ�Ȩ���ƽ���)
    CMD_UNION_UNIONLIST_OUT_CS,	// �����б�
    CMD_UNION_SEARCH_CS,		// ��������
    CMD_UNION_EMBLEM_CS,		// ���û��
    CMD_UNION_NAME_CS,  		// ���Ĺ�����
    CMD_UNION_NOTICE_CS,  		// ���Ĺ���
    CMD_UNION_WELFARE_CS,  		// ������ȡ
    CMD_UNION_SETAUDIT_CS,      // �������
	CMD_UNION_ALLMERCENARY_CS,	// ����Ӷ����Ϣ
	CMD_UNION_DISPATCH_CS,		// ��ǲӶ��
	CMD_UNION_RECALLMER_CS,		// �ٻ�Ӷ��
	CMD_UNION_SINGLEMER_CS,		// ���󵥸�Ӷ����ϸ��Ϣ
    CMD_UNION_CSEND,

    CMD_UNION_SCBEGIN = 100,
    CMD_UNION_INFO_SC,	        // �ط�������Ϣ
    CMD_UNION_MEMBERS_SC,		// �ط������Ա
    CMD_UNION_CREATE_SC,		// �ط���������
    CMD_UNION_APPLY_SC,			// �ط�����״̬
    CMD_UNION_EXIT_SC,			// �ط��˻�
    CMD_UNION_LOGLIST_SC,		// �ط������¼��б�
    CMD_UNION_AUDITLIST_SC,		// �ط���������б�
    CMD_UNION_AUDIT_SC,		    // �ط������Ϣ
    CMD_UNION_FUNCTION_SC,		// �ط�������ز���(�߳������Ρ�Ȩ���ƽ���)
    CMD_UNION_UNIONLIST_OUT_SC, // �ط������б�
    CMD_UNION_SEARCH_SC,		// �ط���������
    CMD_UNION_EMBLEM_SC,		// �ط����û��
    CMD_UNION_NAME_SC,  		// �ط����Ĺ�����
    CMD_UNION_NOTICE_SC,  		// �ط����Ĺ���
    CMD_UNION_WELFARE_SC,  		// �ط�������ȡ
    CMD_UNION_SETAUDIT_SC,      // �ط��������

    CMD_UNION_BEAUDIT_SC,       // �������Ӧ��
    CMD_UNION_BEFUNCTION_SC,    // ��������Ӧ��
	CMD_UNION_MERCENARY_SC,		// ����Ӷ��
    CMD_UNION_MESSAGE_SC,       // ������Ϣ֪ͨ(ǰ�˺��)
    CMD_UNION_FORWARD_SS,       // �����������������ת��������
	CMD_UNION_RECALL_SC,		// �ٻ�Ӧ��
	CMD_UNION_SINGELMER_SC,		// Ӧ�𵥸�Ӷ����ϸ��Ϣ

    CMD_UNION_WELFARE_SS = 200, // ��ȡ����ת��ServiceServier
    CMD_UNION_SCEND
};

//CMD_UNION_INFO_CS �ް���
//CMD_UNION_INFO_SC
//UnionInfo

struct OnlineMemberInfo
{
    int userId;                 // ���ID
    int lv;                     // �ȼ�
    int identity;               // ���(������,����ȼ�*10+��������)
    char pos;                   // ְλ
    char userName[20];          // �û���
};

struct UnionInfo
{
    int unionID;                // ID
    int unionLv;                // �ȼ�
    int liveness;               // ��Ծ��
    int unionRank;              // ��������
    int reputation;             // ��������
    int limitLv;                // ����ȼ�����
    int emblem;                 // ���
    int isAutoAudit;            // �Ƿ��Զ����
    int dangerousTag;           // Σ�ձ�ʶ
    int welfareTag;             // ������ȡ��ʶ
    int chairIdentity;          // �᳤���(������,����ȼ�*10+��������)
    char pos;                   // �����ߵ�ְλ
    char memberCount;           // ��Ա����
    char onlineMemberCount;     // ��������
    char unionName[20];         // ��������
    char chairmanName[20];      // �᳤����
    char notice[128];           // ����
    // OnlineMemberInfo�б�
};

//CMD_UNION_MEMBERS_CS
struct UnionMembersCS
{
    int unionID;                // ����ID
};

//CMD_UNION_MEMBERS_SC
struct UnionMembersSC
{
    int memberCount;            // ��Ա��
    // memberCount ->UnionMembersInfo
};

struct UnionMembersInfo
{
    int userID;                 // ���ID
    int totalContrib;           // �ۼƹ���
    char position;              // ְλ
    int identity;               // ���(������,����ȼ�*10+��������)
    int userLv;                 // �ȼ�
    int todayLiveness;          // ���ջ�Ծ��
    int lastLoginTime;          // ����¼ʱ���
    char userName[20];          // �������
};

//CMD_UNION_CREATE_CS
struct UnionCreateCS
{
    char unionName[20];         // ��������
};

//CMD_UNION_CREATE_SC
struct UnionCreateSC
{
    char result;                // ������� 0ʧ�� 1�ɹ�
    int extend;                 // ʧ��->��Ӧ�Ĵ����� �ɹ�->����ID
};

//CMD_UNION_APPLY_CS
struct UnionApplyCS
{
    int unionID;                // ����ID
};

//CMD_UNION_APPLY_SC
struct UnionApplySC
{
    char errorCode;             // ������
    int unionID;                // ����Ĺ���ID
};

//CMD_UNION_EXIT_CS  �ް���
//CMD_UNION_EXIT_SC
struct UnionExitSC
{
    int timeStamp;              // ʱ��� >0Ϊ�˳��ɹ�,�´����빫���ʱ���
};

//CMD_UNION_LOGLIST_CS  �ް���
//CMD_UNION_LOGLIST_SC
struct UnionLogListSC
{
    char logCount;             // �¼���
    // logCount ->UnionLogInfo
};

//CMD_UNION_AUDITLIST_CS  �ް���
//CMD_UNION_AUDITLIST_SC
struct UnionAuditListSC
{
    char auditsCount;           // �������
    // auditsCount ->AuditsInfo
};

struct AuditsInfo
{
    int userID;                 // ���ID
    int userLv;                 // ��ҵȼ�
    int identity;               // ���(������,����ȼ�*10+��������)
    char userName[20];          // �������
};

//CMD_UNION_AUDIT_CS
struct UnionAuditCS
{
    char isAgree;               //�Ƿ�ͬ�� 0��ͬ�� 1ͬ��
    char auditCount;            //��˸���
    // auditCount ->int array
};

//CMD_UNION_AUDIT_SC
struct UnionAuditSC
{
    char result;                //��ش�����
    char agreeCount;            //ͬ����빫�������
    char refuseCount;           //�ܾ����빫�������
    // agreeCount ->userid
    // refuseCount ->userid
};

//����������ͨ��ʱ������Ϣ
//CMD_UNION_BEAUDIT_SC
//OwnUnionInfo

//CMD_UNION_FUNCTION_CS
struct UnionFunctionCS
{
    char funcType;              // ��������
    int userID;                 // ���ID
};

//CMD_UNION_FUNCTION_SC
struct UnionFunctionSC
{
    char result;                // 0ʧ�� 1�ɹ�
    char funcType;              // ��������
    int userID;                 // ���ID
};

//��������Ӧ��
//CMD_UNION_BEFUNCTION_SC
struct UnionBeFunctionSC
{
    char funcType;              // ��������
    char unionName[20];         // ��������
};

//CMD_UNION_UNIONLIST_OUT_CS
struct UnionListOutCS
{
    char type;                  // �������ͣ�������δ��Ա
    char count;                 // ����Ĺ������
    int orignNum;               // �������ڼ���ʼ
};

struct UnionListInfo
{
    int unionID;                // ID
    int unionLv;                // �ȼ�
    int limitLv;                // ����ȼ�����
    int emblem;                 // ���
    int identity;               // �᳤���(������,����ȼ�*10+��������)
    char memberCount;           // ��Ա����
    char unionName[20];         // ��������
    char chairmanName[20];      // �᳤����
    char notice[128];           // ����
};

//CMD_UNION_UNIONLIST_OUT_SC
struct UnionListOutSC
{
    char unionCount;            // ���ع������
    // unionCount ->UnionListInfo
};

//CMD_UNION_SEARCH_CS
struct UnionSearchCS
{
    char searchName[20];        // ��������,������ID������
};

//CMD_UNION_SEARCH_SC
struct UnionSearchSC
{
    char unionCount;            // ���ع������
    // unionCount ->UnionListInfo
};

//CMD_UNION_EMBLEM_CS
struct UnionEmblemCS
{
    int emblemId;              // ���id
};

//CMD_UNION_EMBLEM_SC
struct UnionEmblemSC
{
    int emblemId;              // ���idΪ0ʧ��
};

//CMD_UNION_NAME_CS
struct UnionNameCS
{
    char unionName[20];         // ��������
};

//CMD_UNION_NAME_SC
struct UnionNameSC
{
    char result;                // ������� 0ʧ�� 1�ɹ�
};

//CMD_UNION_NOTICE_CS
struct UnionNoticeCS
{
    char notice[128];           // ����
};

//CMD_UNION_NOTICE_SC
struct UnionNoticeSC
{
    char result;                // ������� 0ʧ�� 1�ɹ�
};

//CMD_UNION_WELFARE_CS
struct UnionWelfareCS
{
    char welfareType;           // ��������
};

//CMD_UNION_WELFARE_SC
struct UnionWelfareSC
{
    char welfareType;           //��ȡ����������
    int itemCount;			    //��Ʒ����
    //... DropItemInfo
};

//CMD_UNION_SETAUDIT_CS
struct UnionSetAuditCS
{
    int limitLv;                //���Ƶȼ�
    int isAuto;                 //�Ƿ��Զ�ͨ��
};

//CMD_UNION_SETAUDIT_SC
struct UnionSetAuditSC
{
    char result;                //���0ʧ�� 1�ɹ�
    int limitLv;                //���Ƶȼ�
    int isAuto;                 //�Ƿ��Զ�ͨ��
};

enum MercenaryProtocol
{
	ApplyAllMercenaryInfo,
	DispatchMercenary,
	RecallMercenary,
	ApplySingleMercenary,
};

//CMD_UNION_MESSAGE_SC
struct UnionMessageSC
{
    int type;                   //���� 0��Ծ�� 1���
};

//CMD_UNION_FORWARD_SS
struct UnionForwardSS
{
    int type;                   //����EUnionForwardData
    int val;                    //ֵ
    char szExtend[20];          //�ַ���
};

//CMD_UNION_WELFARE_SS
struct UnionWelfareSS
{
    char welfareType;           //��������
    int dropId;                 //����id
};

#pragma pack()

#endif //__UNION_PROTOCOL_H__