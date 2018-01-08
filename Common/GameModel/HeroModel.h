#ifndef __HERO_MODEL_H__
#define __HERO_MODEL_H__

#include <map>
#include <string.h>
#include "IDBModel.h"
#include "Storage.h"

#define TALENT_COUNT 8

// װ����������
enum EquipPartType
{
	WEAPON = 1,					// ����
	HEADWEAR,					// ͷ��
	CLOTH,						// �·�
	SHOES,						// Ь��
	ACCESSORY,					// ��Ʒ
	TREASURE,					// ����
};

// ���ݿ�Ӣ������
struct DBHeroAttr
{
	int fragment;				//��Ƭ
	int star;					//�Ǽ�
	int level;					//�ȼ�
	int exp;					//����
    unsigned char talent[TALENT_COUNT];    //�츳
	std::map<int, int> equipId;	//װ��, ֻ��6��װ��

	DBHeroAttr()
		:fragment(0)
		, star(0)
		, level(0)
		, exp(0)
	{
        memset(talent, 0, sizeof(talent));
		for (int i = WEAPON; i < TREASURE + 1; ++i)
		{
			equipId[i] = 0;
		}
	}

};

class CRedisStorer;
class CHeroModel : public IDBModel
{
public:
	CHeroModel();
	~CHeroModel();

public:

	bool init(int uid);
	// ��ȡ����Ӣ�ۿ�Ƭ����Ϣ
	bool Refresh();

	// ����Ӣ��
	bool SetHero(int heroId, DBHeroAttr &heroAttr);
	// �������Ӣ��
	bool SetHero(std::map<int, DBHeroAttr> &heros);
	
	// ����Ӣ������, ����Ӳ��, �ýӿڲ�ѯ����������
	bool UpdateHero(int heroId, DBHeroAttr &heroAttr);
	// ����Ӣ����Ƭ
	bool UpdateHeroFragment(int heroId, int frag);
	// �����츳
	bool UpdateHeroTalent(int heroId, char* talent, int len);
	// ����װ��
	bool UpdateHeroEquip(int heroId, int part, int equipId);
	// ����װ��
	bool UpdateHeroEquip(int heroId, std::map<int, int> &equipIds);
	// �Ƴ�Ӣ��
	bool RemoveHero(int heroId);
	// Ӣ���Ƿ����
	bool ExsitHero(int heroId);
	// ���Ӣ����Ϣ
	bool GetHero(int heroId, DBHeroAttr& heroAttr, bool fromDB = false);
	// �������Ӣ����Ϣ
	std::map<int, DBHeroAttr>& GetHeros(bool fromDB = false);

private:
	// ������ת���ַ���
	bool AttrToString(DBHeroAttr & heroAttr, std::string &attrStr);
	// ���ַ���ת������
	bool StringToAttr(std::string attrStr, DBHeroAttr & heroAttr);
	
private:

	CRedisStorer *				m_pRedisStorer;	    // ���ݿ����
	int							m_nUid;			    // uid
	std::string					m_strHeroKey;	    // redisӢ��key
	std::map<int, DBHeroAttr>	m_mapHeros;		    // ����Ӣ����Ϣ
};

#endif //__HERO_MODEL_H__
