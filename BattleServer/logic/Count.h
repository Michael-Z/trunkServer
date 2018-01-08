#ifndef _SUM_COUNT_
#define _SUM_COUNT_

#include "KxCSComm.h"

//�˺�����
enum eHurtType
{
    kUnCrit = 1,        // ��ͨ����
    kCrit,              // ����
    kAddBlood,          // ����
    kMiss,              // ����
    kUnKnow,
};

//�����˺�����
enum CountDamageType
{
	CDT_UNVALID,	//��Ч��
	CDT_PHYSICAL,   //�����˺�
	CDT_MAGIC,		//ħ���˺�
	CDT_REAL,		//��ʵ�˺�
	CDT_TREAT,		//�����˺�
    CDT_PERCENT,    //�ٷֱ��˺�
};

//�����ж�����
enum CountJudgeType
{
	CJT_UNVALID,	//��Ч��
	CJT_BIGGER,		//����
	CJT_EQUAL,		//����
	CJT_SMALLER,	//С��
};

//�����������
enum CountChangeType
{
	CCT_UNVALID,	// ��Ч��
    CCT_RESET,      // ���û�ԭ
    CCT_PERCENT,    // �ٷֱ�
	CCT_NUMBER,		// ��ֵ
	CCT_EQUAL,		// ��ֵ
};

class CRole;
class CountConfItem;
class CCount
{
public:
	// ��ͨ����
	static bool roleExecute(int countID, CRole* src, CRole* aim);
	// ��Ƭ����
	static bool cardExecute(int countID, CRole* aim);
    // ���Ž���Ч��
    static void playCountEffect(CRole* role, eHurtType hurtType, int hurtValue);
protected:
	// V4.0 ex
	static void extra(const CountConfItem* item, CRole* aim);
	// �˺�����
	static void damage(const CountConfItem* item, CRole* src, CRole* aim);
	// �Ƚ��ж�
	static bool judge(int cardAttr, int countCondition, int countParam);
	// �����ж�
	static bool judge(int cardAttr, const VecInt& countCondition);
	// ״̬�ж�
	static bool judge(int cardAttr, int countCondition);
	// ״̬����
	static void change(bool& cardAttr, bool countValue);

private:
};

#endif
