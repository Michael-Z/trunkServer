/*
* ֪ͨ������
* 1���ṩ����ȫ���ӿ�
*/

#ifndef __NOTICE_HELPER_H__
#define __NOTICE_HELPER_H__

#include "KxCommInterfaces.h"
#include "NoticeProtocol.h"
#include <string>

class CNoticeHelper
{
public:
    static void noticeAll(int uid, char *buff, int len, KxServer::IKxComm *target);

    // 1���ַ���֪ͨ,����XXX�˳�����
    static int getStringNoticeInfo(char *buff, int noticeId, std::string szVal);
    // 1���ַ���+1��int֪ͨ,����XXX���YY����
    static int getStringIntNoticeInfo(char *buff, int noticeId, std::string szVal, int iVal);
    static int getStringIntIntNoticeInfo(char *buff, int noticeId, std::string szVal, int iVal1, int iVal2);
    // 2���ַ���֪ͨ,����XXX�᳤תְ��YYY
    static int getStringStringNoticeInfo(char *buff, int noticeId, std::string szValOne, std::string szValTwo);

    static int getStringStringInfoLen(int szLen1, int szLen2);
    static int getStringIntIntInfoLen(int szLen);
    static int getStringIntInfoLen(int szLen);
    static int getStringInfoLen(int szLen);
    static int getIntInfoLen();

private:
    static void getStringParamContent(char *buff, int &buffLen, std::string szVal);
    static void getIntParamContent(char *buff, int &buffLen, int iVal);
};

#endif // __NOTICE_HELPER_H__
