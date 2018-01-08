/*
* �ʼ�ģ��
* 1.�洢��������ʱ���͵��ʼ�
* 2.�洢�����Ȼĳʱ�䷢�͵��ʼ�
* 3.�洢�Ѿ���ȡ�ֶ�(��ҳ����д)���͵��ʼ�
*   ���ݴ����ServerModel,��¼����ҲŻ��·�,�ʼ��������ٴ�ģ���Ƴ�
*/

#ifndef __MAIL_MODEL_H__
#define __MAIL_MODEL_H__

#include "Storage.h"
#include "IDBModel.h"
#include "UnionComm.h"
#include "ItemDrop.h"
#include "CommStructs.h"

#include <map>

class CMailModel : public IDBModel
{
public:
    CMailModel();
    ~CMailModel();

    bool init(int uid);
	// ˢ��
    bool Refresh();
    // ������ʼ�
    bool AddNewMail(const BaseMailInfo& info, const std::vector<DropItemInfo> &items,int &nMailID);
    // ������ʼ�(û����Ʒ)
    bool AddNewMail(const BaseMailInfo& info, int &nMailID);
    // ��ȡ�ʼ���Ϣ
    bool GetMailInfo(int mailID, BaseMailInfo &info, std::vector<DropItemInfo> &items,bool bNew = false);
    // ���ʼ�,�����ʼ�Ϊ�Ѷ�״̬
    bool ReadMail(int mailID);
    // �Ƴ��ʼ�
    bool RemoveMail(int mailID);
    // ��Ӷ�ȡ��web�ʼ�
	bool AddWebMail(int webMailID, int nState);
    // �Ƴ�web�ʼ�
    bool RemoveWebMail(int webMailID);
	// �Ƿ�ȹ�webMail
	bool IsReadWebMail(int webMailId);
    // ��ȡ�����ʼ�������Ϣ
    std::map<int, BaseMailInfo>& GetMailBasicInfo();
    // ��ȡ�����ʼ�������Ϣ
    std::map<int, std::vector<DropItemInfo> >& GetMailItemInfo();
    // ��ȡ�Ѿ���ȡ��web�ʼ�
	std::map<int, int>& GetWebMails();
	//��ȡ�����Զ����ʼ�����
	std::map<int, std::string>& GetMailContextInfo();
    // ɾ���ʼ�
    bool DeleteMail();

    // ��Ӳ���������ʼ�
    static bool AddOfflineNewMail(int uid, const BaseMailInfo& info, const std::vector<DropItemInfo> &items);

private:

    void ClearData();

    bool SaveMailData(int mailID);

	bool GetMailsFromDB();

	bool GetMailsFromDB(int nMailID);

private:

    int                                         m_nUID;             // �û�ID
	Storage*                                    m_pStorage;         // ���ݿ�
	std::string									m_NormalMailKey;	// ��ͨ�ʼ�Key
    std::map<int, BaseMailInfo>					m_MailInfoMap;      // <�ʼ�ID, �ʼ���Ϣ>
    std::map<int, std::vector<DropItemInfo> >   m_MailItemInfoMap;  // <�ʼ�ID, ���еĵ����б�>
	std::map<int,std::string>					m_MailContextMap;	// �ʼ�����
	std::map<int, int>							m_GMMailStatMap;	// GM�ʼ�״̬

};

#endif