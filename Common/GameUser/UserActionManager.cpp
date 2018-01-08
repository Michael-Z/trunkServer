#include "UserActionManager.h"

CUserActionManager::CUserActionManager()
{
}

CUserActionManager::~CUserActionManager()
{
    CleanAction();
}

void CUserActionManager::AddActionListener(int actID, IUserActionListener* listener)
{
    m_mapAllListener[actID].push_back(listener);
}

bool CUserActionManager::UserAction(int actID, void *data, int len)
{
    std::map<int, std::list<IUserActionListener*> >::iterator iterAll = m_mapAllListener.find(actID);
    if (iterAll != m_mapAllListener.end())
    {
        std::list<IUserActionListener*>::iterator iterListener = iterAll->second.begin();
        for (; iterListener != iterAll->second.end();)
        {
            // �Ƿ��Ƴ���������
            if ((*iterListener)->onAction(actID, data, len))
            {
                // �����Ƴ������������ɾ�,Ŀǰֱ��ɾ��
                delete *iterListener;
                iterListener = iterAll->second.erase(iterListener);
            }
            else
            {
                iterListener++;
            }
        }

        return true;
    }

    return false;
}

bool CUserActionManager::CleanAction()
{
	for (std::map<int, std::list<IUserActionListener*> >::iterator ator = m_mapAllListener.begin(); ator != m_mapAllListener.end(); ++ator)
	{
		std::list<IUserActionListener*>& ActionListerList = ator->second;

		for (std::list<IUserActionListener*>::iterator iter = ActionListerList.begin(); iter != ActionListerList.end(); ++iter)
		{
			delete *iter;
		}

		ActionListerList.clear();
	}

	m_mapAllListener.clear();

	return true;
}
