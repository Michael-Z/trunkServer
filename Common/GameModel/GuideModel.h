#ifndef __STORY_MODEL__
#define __STORY_MODEL__

#include "Storage.h"
#include "IDBModel.h"

enum GuideType
{
	NewPlayerGuideType,					//������������
};

class CGuideModel : public IDBModel
{
public:
	CGuideModel();
	~CGuideModel();

	bool init(int uid);

	bool Refresh();

	bool AddGuideID(int nValue);

	bool DelGuideID(int nValue);

	std::set<int>& GetNewGuideID() { return m_SetGuide; }

protected:
	//�洢���ݵ����ݿ�
	bool SaveGuideToDB();

private:

	int                 m_nUid;
	std::string			m_strGuideKey;
	Storage             *m_pStorage;		// ���ݿ�
	std::set<int>		m_SetGuide;			//��������
};

#endif