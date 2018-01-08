/*
Ӣ������ģ��
1���洢����Ϊhash,fieldΪ0�洢ˢ��ʱ��(ÿ���ؿ�ˢ��ʱ��һ��)
2������λ�õ�fieldΪӢ�����������ID(>0)
*/

#ifndef __HEROTESTMODEL_H__
#define __HEROTESTMODEL_H__

#include "IDBModel.h"
#include <string>
#include <map>

enum HeroTestField
{
	HEROTEST_FD_RESETSTAMP = 0,			//ˢ��ʱ���Field
};

class Storage;
class CHeroTestModel : public IDBModel
{
public:
    CHeroTestModel();
    ~CHeroTestModel();

public:
	//��ʼ��
	bool init(int uid);
	//����ˢ��
	bool Refresh();
    // ��ȡ��ս����ʱ���
    int GetResetStamp(){ return m_nResetStamp; }
    // ��ȡĳ��������ս����
    int GetChallengeTimes(int heroTestID);
    // ��ȡ������ս�Ĵ���
    std::map<int, int>& GetAllChallengeTimes(){ return m_mapTimes; }
    // ����Ӣ������
    bool ResetHeroTest(int useStamp);
    // �����ս����
    bool AddChallengeCount(int heroTestID, int count);

private:

    int			       m_nUID;			// �û�ID
    int	               m_nResetStamp;   // ˢ��ʱ���
	std::string        m_strKey;		// key
	Storage *	       m_pStorage;		// storage

    std::map<int, int> m_mapTimes;		//��������ID ֵ������
};

#endif