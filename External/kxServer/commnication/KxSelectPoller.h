/*
 * ʵ�ֿ�ƽ̨��Select
 * ��ÿ��Poll��ѯ��ʱ�򣬻Ὣ�쳣�Ķ������ӵ�ɾ���б���
 * ����ѯ��֮��һ����ɾ��
 *
 *  2013-04-20 By ��ү
 *  
 */
#ifndef __SELECTPOLLER_H__
#define __SELECTPOLLER_H__

#include <set>
#include <map>

#include "KxCore.h"
#include "KxCommInterfaces.h"
#include "KxTimeVal.h"

namespace KxServer {

class KxSelectPoller : public IKxCommPoller
{
public:
	KxSelectPoller();
	virtual ~KxSelectPoller();

    // ������ѯ�������ش������¼���
    // ��ѯʧ�ܷ���-1
    virtual int poll();

    // ע��һ��IKxCommͨѶ���󣬲�retain
    // �����ظ�ע�ᣬ�ظ�ע���൱�ڵ���modifyCommObject
    // ���ӳɹ�(�����ظ����)����0��ʧ�ܷ���-1
    virtual int addCommObject(IKxComm* obj, int type);

    // �޸�IKxCommͨѶ�������������¼�
    // δע���IKxCommͨѶ������ø÷�����ʧ��
    // �޸ĳɹ�����0��ʧ�ܷ���-1
    virtual int modifyCommObject(IKxComm* obj, int type);

    // �Ƴ��Ѿ�ע���IKxCommͨѶ����
    // �Ƴ��ɹ�����0��ʧ�ܷ���-1
    virtual int removeCommObject(IKxComm* obj);

    IKxComm* getComm(KXCOMMID cid);

    inline void setTimeOut(float timeOut){ m_TimeOut.setFromfloat(timeOut); m_IsBlock = false; }
    inline void setTimeOut(long sec, long usec) { m_TimeOut = kxTimeVal(sec, usec); m_IsBlock = false; }
    inline void setPollBlock(bool isBlock) { m_IsBlock = isBlock; }

private:
	void clear();

private:
    int m_MaxCount;
    bool m_IsBlock;
    fd_set m_InSet;
    fd_set m_OutSet;
    fd_set m_ExceptSet;
	kxTimeVal m_TimeOut;

    std::map<KXCOMMID, IKxComm*> m_PollMap;
    std::set<IKxComm*> m_RemoveSet;
};

}

#endif