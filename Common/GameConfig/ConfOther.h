#ifndef __CONF_OTHER_H__
#define __CONF_OTHER_H__

#include "ConfManager.h"

/////////////////////////////////�������/////////////////////////////////////////
enum EResType
{
	RT_ARMATURE = 1,						// cocostudio �����ļ�
	RT_SPINE,								// spine�����ļ�
    RT_CSB2,                                // CSB2.0�����ļ�
};

// ��Դ����
struct SResPathItem
{
	EResType            ResType;			// 1Ϊarmature,2Ϊspine,3Ϊcsb2.0��Ч,4Ϊ��Чbank
	int					AnimationID;		// ��ԴID
	std::string         ResName;            // ��Դ����
	std::string			Path;				// spineΪjson·��,armatureΪcsb·��
	std::string         AtlasPath;			// רΪspineͼ��ʹ��,armatureΪ��
	std::string			Skin;				// רΪspineƤ��ʹ��
};

//��ɫ��Ӧ��Դ
struct SRoleResItem
{
	int RoleID;		// ��ɫID
	int RoleStar;	// ��ɫ�Ǽ�
	VecInt ResIDs;	// ��Ӧ��ԴID
    std::vector<std::string> MusicRess;	// ��Ӧ��Ч��Դ
};

////////////////////////////////�������//////////////////////////////////////////
class CConfAnimationRes : public CConfBase
{
public:
	virtual bool LoadCSV(const std::string& str);
};

class CConfRoleResPreload : public CConfBase
{
public:
    virtual ~CConfRoleResPreload();
	virtual bool LoadCSV(const std::string& str);

public:
	SRoleResItem* getData(int id, int star)
	{
		std::map<int, std::map<int, SRoleResItem*> >::iterator iter = m_mapRoleResItem.find(id);
		if (iter != m_mapRoleResItem.end())
		{
			std::map<int, SRoleResItem*>::iterator iterItem = iter->second.find(star);
			if (iterItem != iter->second.end())
			{
				return iterItem->second;
			}
		}
		return NULL;
	}

	std::map<int, std::map<int, SRoleResItem*> >& getData()
	{
		return m_mapRoleResItem;
	};

private:
	std::map<int, std::map<int, SRoleResItem*> > m_mapRoleResItem;
};

////////////////////////////////��ѯ���//////////////////////////////////////////
inline const SResPathItem* queryConfSResInfo(const int& id)
{
	CConfAnimationRes *confRes = dynamic_cast<CConfAnimationRes*>(
		CConfManager::getInstance()->getConf(CONF_RESPATH));
	return static_cast<SResPathItem*>(confRes->getData(id));
}

inline const SRoleResItem* queryConfSRoleResInfo(const int& id, const int& star)
{
	CConfRoleResPreload *confRoleRes = dynamic_cast<CConfRoleResPreload*>(
		CConfManager::getInstance()->getConf(CONF_ROLERES));
	return confRoleRes->getData(id, star);
}

#endif
