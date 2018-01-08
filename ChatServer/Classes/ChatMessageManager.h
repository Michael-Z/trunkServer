/*
    ������Ϣ������
*/

#ifndef _CHAT_MESSAGE_MANAGER_H__
#define _CHAT_MESSAGE_MANAGER_H__

#include <map>
#include <list>
#include <set>

struct ChatMessageInfo
{
    char headId;                // ͷ��id
    char chatType;              // ��������EChatType
    char userLv;                // ��ҵȼ�
    char chatMessageType;       // ������Ϣ����EChatMessageType
    int userId;                 // ���id
    int sendTime;               // ����ʱ���
    int targetId;               // Ŀ��id(��������Ϊ����Id, ˽��Ϊ�Է�id)
    int extend;                 // ��չ�ֶ�
    char name[20];              // �������
    char content[128];          // ����
};

enum EChatMessageType
{
    CHAT_MESSAGE_TYPE_TEXT = 1, // �ı�
    CHAT_MESSAGE_TYPE_AUDIO,    // ����(��ʱ����)
};

enum EChatType
{
    CHAT_TYPE_PRIVATE,          // ˽��(��ʱ����)
    CHAT_TYPE_WORLD,            // ��������
    CHAT_TYPE_UNION,            // ��������(��Ҫ������˳�)
};

enum EChatRoom
{
    CHAT_ROOM_NON,
    CHAT_ROOM_WORLD,            // ����������
    CHAT_ROOM_UNION,            // ����������
};

class CChatMessageManager
{
private:
    CChatMessageManager();
    ~CChatMessageManager();

public:
    static CChatMessageManager* getInstance();
    static void destroy();

    // �������������Ϣ
    void addWorldChatMessage(ChatMessageInfo &info);
    // ��ȡ����������Ϣ
    const std::list<ChatMessageInfo>& getWorldChatMessage();
    // ��ӹ���������Ϣ
    void addUnionChatMessage(int unionId, ChatMessageInfo &info);
    // ��ȡ����������Ϣ
    void getUnionChatMessage(int unionId, std::list<ChatMessageInfo>& chatList);
    // ��ӵ������б�
    void addIllegal(int uid);
    // �ӽ����б��Ƴ�
    void removeIllegal(int uid);
    // �Ƿ���Υ����
    bool isIllegal(int uid);

private:
    static CChatMessageManager *               m_pInstance;
    std::set<int>                              m_setIllegalUsers;   // �����Ե����
    std::list<ChatMessageInfo>                 m_listWorldMessage;  //
    std::map<int, std::list<ChatMessageInfo> > m_mapUnionMessage;   // <unionid, list<> >
};


#endif //_CHAT_MESSAGE_MANAGER_H__
