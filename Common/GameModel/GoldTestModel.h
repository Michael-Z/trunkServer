/*
�������ģ��
1����������ˢ��ʱ�䡢��ս������������ȡ״̬�����˺�ֵ
2����ȡ/���������Ϣ
*/

#ifndef __GOLDTRIALMODEL_H__
#define __GOLDTRIALMODEL_H__

#include <string>
#include <map>
#include "IDBModel.h"

enum EGoldTestField
{
	GT_FD_NONE = 0,
	GT_FD_USETIMES,         // ��ս����
	GT_FD_RESETSTAMP,       // ʱ���
	GT_FD_DAMAGE,           // ���˺�
	GT_FD_CHESTFLAG,        // ����״̬
    GT_FD_END,
};

class Storage;
class CGoldTestModel : public IDBModel
{
public:
	CGoldTestModel();
	~CGoldTestModel();

public:

	bool init(int uid);

	bool Refresh();

	bool GetFieldNum(int nField, int &nValue);

	bool SetFieldNum(int nField, int nValue);
	
    bool AddFieldNum(int nField, int nValue);

    bool ResetGoldTest(int stamp);

private:
	int		                m_nUid;
    Storage *		        m_pStorage;
    std::string		        m_szKey;
    std::map<int, int>      m_mapGoldTestInfo; // <field, num>
};



#endif