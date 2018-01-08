/*
 * ����ģ��
 * 1.����������Ϣ(ID,�᳤ID,��������,����ȼ���)
 * 2.��˵���Ϣ�б�
 * 3.�¼�����Ϣ�б�
 * 4.��Ա�б�
 */

#ifndef __UNION_MODEL_H__
#define __UNION_MODEL_H__

#include <map>
#include "IDBModel.h"
#include "Storage.h"
#include "RedisStorer.h"

enum EUnionField
{
    UNION_FIELD_NON,
    UNION_FIELD_UNIONNAME,          // ������
    UNION_FIELD_NOTICE,             // ���ṫ��

    UNION_FIELD_LV = 10,            // �ȼ�(10�����Ժ󲹳��µ��ֶε��¸���)
    UNION_FIELD_LIVENESS,           // ��Ծ��
    UNION_FIELD_CHAIRMAN,           // �᳤ID
    UNION_FIELD_LIMITLV,            // ���빫��ȼ�����
    UNION_FIELD_REPUTATION,         // ��������
    UNION_FIELD_EMBLEM,             // ���
    UNION_FIELD_LOGIDREF,           // ������־id
    UNION_FIELD_AUTOAUDIT,          // �Ƿ��Զ����
    UNION_FIELD_DANGEROUSTAG,       // Σ�ձ�ʶ(һ���Ծ��Ϊ0��1)
    UNION_FIELD_ORIGINLV,           // ����ԭʼ����ȼ�

    UNION_FIELD_MAX,
};

enum
{
	SRV_FD_NONE,
    SRV_FD_UNIONREF,				// ����ID����
    SRV_FD_DAYRESETSTAMP,           // ������ʱ���
	SRV_FD_END,
};

struct UnionLogInfo
{
    int eventType;                   // �¼�����(���빫�ᡢ�˳������EUnionFuncType)
    int timeStamp;                   // ʱ���
    char userName1[32];              // �������������
    char userName2[32];              // �������������
    int extend;                      // ��չ(������ƷID��)
};

class CUnionModel : public IDBModel
{
public:
    CUnionModel();
    ~CUnionModel();

    //�������������й���
    static bool CreateUnionList(int nServerID);

    //ɾ�����������й���
    static bool CloseUnionList(int nServerID);

    static std::vector<int>& GetUnionIDList() { return m_VectUnionID; }

    bool init(int unionID);
    // �¹���ģ��
    bool NewUnion(int unionID, int chairmanID, std::string unionName);
	// ˢ�¹�������
	bool Refresh();
    // ��ȡ������Ϣ
    bool GetUnionFieldVal(int field, int &value);
    // ���ù�����Ϣ
    bool SetUnionFieldVal(int field, int value);
    bool SetUnionFieldVals(std::map<int, int> info);
    bool AddUnionFieldVal(int field, int value);
    // ��ӳ�Ա
    bool AddMember(int userID, int pos);
    // ��������Ϣ
    bool AddAudit(int uid, int auditTime);
    // ����¼���Ϣ
    bool AddLog(UnionLogInfo& info);
    // �Ƴ���Ա
    bool RemoveMember(int userID);
    // �Ƴ������Ϣ
    bool RemoveAudit(int uid);
    // �Ƴ��¼���Ϣ
    bool RemoveLog(int logid);
    // ��ȡ��Ա�б�
    std::map<int, int>& GetMembers(){ return m_MemberMap; }
    void GetMembers(std::vector<int> &members);
    // ��ȡ�����Ϣ�б�
    std::map<int, int>& GetAudits(){ return m_AuditMap; }
    // ��ȡ�¼���Ϣ�б�
    std::map<int, UnionLogInfo>& GetLogs() { return m_LogMap; }
    // ���ó�Աְλ
    bool SetMemberPos(int userID, int pos);
    // ��ȡ��Աְλ
    int GetMemberPos(int userID);
    bool MemberExist(int userID);
    // ��ȡ�����Ϣ
    bool GetAuditInfo(int uid, int &auditTime);
    // ���ù�������
    bool SetUnionName(std::string unionName);
    // ���ù��ṫ��
    bool SetUnionNotice(std::string unionNotice);
    // ��ȡ��������
    std::string GetUnionName(){ return m_UnionName; }
    // ��ȡ���ṫ��
    std::string GetUnionNotice(){ return m_UnionNotice; }
    // ����ID
    int GetUnionID(){ return m_nUnionID; }
    // ɾ������
    bool DeleteUnion();

    /////////////////////////////ͨ��///////////////////////
    static bool addUnion(int nServerID, int unionID);

    static bool deleteUnion(int nServerID, int unionID);

    static bool hasUnion(int nServerID, int unionID);

    static bool getUnionField(int nServerID, int field, int &val);

    static bool setUnionField(int nServerID, int field, int val);
    // ��ȡ���¹���ID(+1)
    static bool getNewUnionId(int nServerID, int &unionId);

private:
    int                             m_nUnionID;       // ����ID
    std::string                     m_UnionName;      // ��������
    std::string                     m_UnionNotice;    // ���ṫ��
    std::map<int, int>              m_UnionInfoMap;   // ������Ϣ<field, value>
    std::map<int, int>              m_MemberMap;      // ��Ա�б�<UserID, ְλ>
    std::map<int, int>              m_AuditMap;       // ����б�<uid, time>
    std::map<int, UnionLogInfo>     m_LogMap;         // �¼��б�<logid, info>
    Storage                    *    m_pStorage;       // ���ݿ�

    static std::vector<int>         m_VectUnionID;    //����ID�б�
    static int                      m_DynUnionID;     //��̬��������IDֵ

    static std::map<int, int>       m_UnionInfo;      //�����������
};

#endif