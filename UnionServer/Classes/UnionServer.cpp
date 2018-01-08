#include "UnionServer.h"
#include "LogConsoleHandler.h"
#include "UnionClient.h"
#include "UnionListen.h"
#include "UnionModule.h"
#include "ConfManager.h"
#include "StorageManager.h"
#include "UnionModel.h"
#include "UnionManager.h"
#include <string>
#include "GameUserManager.h"
#include "RankModel.h"
#include "LogFileHandler.h"

using namespace KxServer;

CUnionServer * CUnionServer::m_pInstance = NULL;

CUnionServer::CUnionServer()
{
}

CUnionServer::~CUnionServer()
{
}

void CUnionServer::AddModelType()
{
	//��ʼ��
	CGameUserManager::getInstance()->init(getTimerManager());
	//���ظ÷�������������ģ��
	CGameUserManager::getInstance()->addModelType(MODELTYPE_USER);
    CGameUserManager::getInstance()->addModelType(MODELTYPE_EQUIP);
    CGameUserManager::getInstance()->addModelType(MODELTYPE_HERO);
	CGameUserManager::getInstance()->addModelType(MODELTYPE_USERUNION);
	CGameUserManager::getInstance()->addModelType(MODELTYPE_MERCENARY);
	CGameUserManager::getInstance()->addModelType(MODELTYPE_SUMMONER);
}

// ��������ʼ��
bool CUnionServer::onServerInit()
{
    if (!KxBaseServer::onServerInit())
    {
        return false;
    }

    KxLogger::getInstance()->setShowTime(true);
    KxLogger::getInstance()->setShowDate(true);
    KxLogger::getInstance()->addHandler(1, new KxLogConsoleHandler());
	KxLogFileHandler* pFileHandle = new KxLogFileHandler();
	char szLogPath[128] = {};
	snprintf(szLogPath, sizeof(szLogPath), "SUM_Union_%d", m_ServerInfo.serverId);
	pFileHandle->setFileName(szLogPath);
	pFileHandle->setFastModel(false);
	KxLogger::getInstance()->addHandler(2, pFileHandle);

    //1.��ʼ����ѯ��
    m_Poller = new KxSelectPoller();

    //2.��ȡ�����ļ�����Server�����ÿͻ������Ӵ����Module
    CUnionListen* listener = new CUnionListen();
	if (!listener->init() 
		|| !listener->listen(m_ServerInfo.port, (char*)m_ServerInfo.host.c_str()))
    {
        return false;
    }

    CUnionModule *pUnionModule= new CUnionModule();
    listener->setClientModule(pUnionModule);
    m_Poller->addCommObject(listener, listener->getPollType());
	KXSAFE_RELEASE(listener);
	KXSAFE_RELEASE(pUnionModule);

    // ��ʼ�����ݿ�
    if (!StorageManager::getInstance()->InitWithXML(STORAGE_XML_FILE))
    {
        return false;
    }
    // �������
    if (!CConfManager::getInstance()->init())
    {
        return false;
    }

	AddModelType();

	CRankModel::getInstance()->init();
    CUnionManager::getInstane()->init();

	KXLOGDEBUG("==========Start UnionServer serverid %d ip %s, port %d", m_ServerInfo.serverId, m_ServerInfo.host.c_str(), m_ServerInfo.port);
    return true;
}

CUnionServer * CUnionServer::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CUnionServer();
    }

    return m_pInstance;
}

void CUnionServer::destroy()
{
    if (m_pInstance != NULL)
    {
        CUnionManager::destroy();
        delete m_pInstance;
        m_pInstance = NULL;
    }
}
