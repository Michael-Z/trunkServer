#ifndef __CHAPTER_SERVICE_H__
#define __CHAPTER_SERVICE_H__

#include "KxCommInterfaces.h"

class CChapterService
{
public:
	// ��սҵ����
	static void processService(int maincmd, int subcmd, int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// �ؿ���ս
	static void processChallenge(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// �ؿ���ս����
	static void processFinish(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ɨ��ҵ��
	static void processSweep(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ����ҵ��
	static void processStrategy(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// �½ڽ���
	static void processChapterReward(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// ������ս����
	static void processBuyTimes(int uid, char *buffer, int len, KxServer::IKxComm *commun);
	// �����½�
	static void processBuyChapter(int uid, char *buffer, int len, KxServer::IKxComm *commun);
};

#endif //__CHAPTER_SERVICE_H__
