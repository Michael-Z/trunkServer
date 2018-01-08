/*
* ���������ӿ�
*   Ŀǰ������/�ɾ���̳�
*/

#ifndef __USER_ACTION_LISTENER_H__
#define __USER_ACTION_LISTENER_H__

// ��������
enum EListenerAction
{
    ELA_BEGIN,
    ELA_PASS_STAGE,                   // ͨ��ָ��/����ؿ�
    ELA_HERO_EQUIP,                   // Ϊ����Ӣ�۴���װ��
    ELA_HERO_LEVEL_UP,                // Ϊ����Ӣ������
    ELA_HERO_UPGRADE_STAR,            // Ϊ����Ӣ������
    ELA_HERO_UPGRADE_SKILL,           // Ϊ����Ӣ�ۼ�������
    ELA_DRAW_CARD,                    // �鿨N��
    ELA_USE_EXP_BOOK,                 // ʹ�����⾭�鿨
    ELA_BUY_GOLD_TIMES,               // �����ʯ�����ң�X��
    ELA_DAILY_ENERGY,                 // ��ȡÿ������
    
    ELA_USER_LEVEL_UP,                // ��ҵȼ��ﵽX��(�����ض�����ͷǼ�������)
    ELA_OWN_HERO,                     // ӵ��N��X��Ӣ��
    ELA_HERO_UPGRADE_STAR_TO_X,       // ��������Ӣ�۵�X��
    ELA_HERO_UPGRADE_SKILL_TO_X,      // ��������Ӣ�ۼ��ܵ�X��
    ELA_OWN_EQUIP,                    // ӵ��X����ɫXXװ��
    ELA_OWN_DIFFEREN_HERO,            // ӵ��X����ͬ��Ӣ��
    ELA_OWN_SUMMONER_COUNT,           // ӵ��X���ٻ�ʦ
    ELA_OWN_ASSIGN_SUMMONER,          // ���XXX�ٻ�ʦ
    ELA_STAGE_STAR,                   // ��ҹؿ����XXX������
    ELA_ACCUMULATE_GOLD,              // �ۼƻ��XXXXX���

    ELA_HERO_TEST,                    // ���N��Ӣ������
    ELA_GOLD_TEST,                    // ���N�ν������
    ELA_TOWER_TEST_FLOOR,             // ���N����������
    ELA_PVP,                          // ���N�ξ���
    ELA_UNION_TASK,                   // ���N�ι�������
    ELA_FB_STAGE,                     // ���N�θ����ؿ�
    ELA_MONTH_CARD,                   // �¿���ȡ
    ELA_PASSED_STAGE,                 // ͨ��ָ���ؿ�

	ELA_USER_FIRSTLOGIN,			  // �û��״ε�½ 

    ELA_HAVE_UNION = 30,              // ����/���빫��
    ELA_BUY_ITEM_COUNT,               // �̵��ۼƹ���N����Ʒ
    ELA_HAVE_HERO_X_STAR,             // ӵ��N��X�Ǽ���Ӣ��
    ELA_USE_SAME_RACE,                // �ۼ�ֻʹ��X����Ӣ��ȡ��N��ʤ��
    ELA_PVP_KEEP_WIN,                 // ������ȡ��N��ʤ
    ELA_PVP_SCORE,                    // ���������ִﵽN��
    ELA_PVP_RANK,                     // ��������ʷ�����ﵽN������
	ELA_CONTINUELOGIN = 40,			  // ������½����
	ELA_TOTALLOGIN = 41,			  // �ۼƵ�½����
    ELA_UNION_LIVENESS,               // �����Ծ
    ELA_UNION_DISPATCH_MERCENARY,     // ������ǲӶ��
    ELA_UNION_USE_MERCENARY,          // ����ʹ��Ӷ��

	ELA_FUND = 46,					  // ���𴥷��¼�
	ELA_DIAMONDCOSTNUM = 47,		  // ��ʯ���Ĵ����¼�
	ELA_GOLDCOSTNUM,				  // ��������ۼ���
	ELA_PVPCOINCOSTNUM,				  // PVP�������ۼ���
	ELA_TOWERCOINCOSTNUM,			  // ���������ۼ���
	ELA_ENERGY_BUYTIMES,			  // �����������

	ELA_RMVBCOST = 100,				 //����ҳ�ֵ
	ELA_CHAMPION = 101,				 //������
	ELA_PAYDIAMOND = 102,			// �ۼƳ�ֵ��ʯ��

	ELA_OLTIME	= 201,				//�ۼ�����ʱ��
	ELA_ACTIVELIFTCOST,				//��������
	ELA_SHOPFRESHTIMES,				//�̵�ˢ�´���
	ELA_EQUIPBUILD,					//װ������
	ELA_OWNCOLORHERO,				//ӵ��Xɫ��Ӣ��
	ELA_FINISHACTIVETASK = 207,		//��ɳɳ�������Ŀ
	ELA_OWNLEVELHERO,				//ӵ��ָ���ȼ���Ӣ����Ŀ
    ELA_END,
};


//param1 PVP�¼���1��ʤ����2��ʧ��
struct ActionCommonInfo
{
    int param1;
    int param2;
};

class IUserActionListener
{
public:
    virtual bool onAction(int actID, void *data, int len) = 0;
};

#endif