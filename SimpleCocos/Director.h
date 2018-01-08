#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include "Ref.h"

class Node;
class Director : public Ref
{
public:
    Director(void);
    ~Director(void);

public:
    // ��ʼ����Ҫ���
    bool init();
    // ��ʼ��֡����
    void initFrames(int fps, int maxFrames = 0xffffff, int curFrame = -1);
    // ����������Ϸ
    void runWithScene(Node *pScene);
    // ������Ϸ, ���սڵ�, �����ڴ�
    void end();
    // ��Ϸ��ѯ��֡, ��������node����onEnter��ʼ���ڵ�
    void mainLoop();
    // ��ѯһ��ʱ����, ���ڶ���֡, ��director���ӽڵ��Լ�ȷ��.
    void loopDelta(float delta);
//     // ��ѯ���ڼ�֡ (��ʱ����)
//     void loopToFrame(int toFrame);
//     // ��ѯn֡ (��ʱ����)
//     void loopFrames(int frames);
    // �����Ϸ����ѯ�ڵ�
    Node* getRunningScene();

private:
    Node*       m_pScene;               // ��Ҫ���еĳ���
    Node*       m_pRunningScene;        // �����еĳ���
    int         m_nMaxFrames;           // ��֡��
    int         m_nCurFrame;            // ��ǰ֡��
    int         m_nFPS;                 // ÿ�����֡
    float       m_fDelta;               // ÿ֡ʱ��
    bool        m_bPaused;              // �Ƿ���ͣ��Ϸ
};

#endif //__DIRECTOR_H__
