#ifndef __Mail_HELPER_H__
#define __Mail_HELPER_H__

#include <vector>
#include <string>

struct DropItemInfo;
struct BaseMailInfo;
class CMailHelper
{
public:
    // �����ʼ����ҷ���
    static bool saveAndSendBagFullMail(int uid, std::vector<DropItemInfo> &dropItem);
    // ������ͨ�ʼ�
	static bool sendNormalMail(int uid, const BaseMailInfo &noramlMailInfo, int &nMailID);
	//PVPÿ���ʼ�
	static bool sendPvpRankMail(int uid, std::vector<DropItemInfo> &dropItem);
    // �����ʼ�
    static bool sendUnionMail(int uid, int configID, std::string unionName);
	//�����ʼ�
	static bool sendTowerMail(int uid, std::vector<DropItemInfo> &dropItem);
};

#endif //__Mail_HELPER_H__
