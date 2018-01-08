#ifndef __HERO_SERVICE_H__
#define __HERO_SERVICE_H__

#include "KXServer.h"

class CHeroService
{
public:
    // Ӣ�۷���
	static void processService(int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // Ӣ�۹���(�齱)����
    static void ProcessHeroBuy(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// Ӣ�����ɷ���
	static void ProcessHeroGen(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // Ӣ��װ������
    static void ProcessHeroEquip(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // Ӣ����������
    static void ProcessHeroUpgrade(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // Ӣ�����Ƿ���
    static void ProcessHeroUpgradeStar(int uid, char *buffer, int len, KxServer::IKxComm *commun);
    // Ӣ���츳����
    static void ProcessHeroTalent(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif