#ifndef __SUMMON_CARD_MODEL__
#define __SUMMON_CARD_MODEL__

#include <set>
#include "Storage.h"
#include "IDBModel.h"
#include <algorithm>

class CSummonModel : public IDBModel
{
public:
    CSummonModel();
    ~CSummonModel();

public:

    bool init(int uid);
	// ˢ������ٻ�ʦ����
	bool Refresh();
	// �Ƿ�ӵ���ٻ�ʦ
	bool HaveSummoner(int cardId,bool bNew = false);
    // ���һ���ٻ�ʦ��Ƭ
    bool AddSummon(const int& cardId);
    // ��ȡ���е��ٻ�ʦ��Ƭ��Ϣ
	std::vector<int> &GetAllSummon();
	// ɾ������ٻ�ʦ����
    bool DeleteSummon();

private:
    int                 m_nUID;
    std::string         m_strSummonKey;
    std::vector<int>	m_VectAllSummon; // �ٻ�ʦ�б�
    Storage             *m_pStorage;    // ���ݿ�
};

#endif