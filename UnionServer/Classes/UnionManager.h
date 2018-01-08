/*
 * ���������
 * 1.�����б�(ʵ��)
 * 2.���(ɾ��)�¹���
 * 3.��ȡĳ������ID�Ĺ���ʵ��
 */

#ifndef __UNION_MANAGER_H__
#define __UNION_MANAGER_H__

#include <map>
#include "Union.h"
#include "KXServer.h"

class CUnionManager : public KxServer::KxTimerObject
{
private:
    CUnionManager();
    ~CUnionManager();

public:
    static CUnionManager* getInstane();
    static void destroy();
    
    bool init();
    // �����¹���
    bool createUnion(int unionID, int chairmanID, std::string unionName);
    // �Ƴ�����(��ɾ�����ݿ�����)
    bool removeUnion(int unionID);
    // ɾ������(ɾ�����ݿ�����)
    bool deleteUnion(int unionID);
    // ����ID��ȡ����
    CUnion* getUnion(int unionID);
    // ��ȡ���й���
    std::map<int, CUnion*> getAllUnion();
    // ��ȡ�����б�
    std::vector<int>& getUnionSort();
    // ��ȡ��Ӧ��ģ��
    CUnionModel *getUnionModel(int unionID);
	//��ȡ��Ӧ�Ĺ���Ӷ��ģ��
	CUnionMercenaryModel *getUnionMercenaryModel(int unionID);

    virtual void onTimer(const KxServer::kxTimeVal& now);

    virtual void setTimer(int sec, int nNum = 0);

private:
    //��ʼ�����й�����Ϣ
    void initUnions(int nServerID);
    // ���ݿ�Ĺ���
    bool addUnion(int unionID);
    // ���б��Ƴ�
    void removeFromList(int unionID);
    // ����ÿ�մ���
    void daily();
    // ��ɢ����
    void dismissUnion(std::list<int> unions);
    // ת�û᳤
    bool assignChairman(int unionId);
    // �ط��������������ô���
    void resetUnionsDayData();

private:
    std::map<int, CUnion*>    m_UnionMap;    // �����б�<����id, ����>
    std::vector<int>          m_UnionSort;   // �����б�(�ȼ�>����>ID)
    int                       m_nSearchTime; // ��¼��ѯʱ���

    
    static CUnionManager *    m_Instance;
};

#endif