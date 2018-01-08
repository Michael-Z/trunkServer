#include "HttpHelper.h"
#include "KXServer.h"

CHttpHelper* CHttpHelper::m_Instance = NULL;
using namespace std;
using namespace KxServer;

CHttpHelper::CHttpHelper()
: m_MURLHandle(NULL)
, m_RunningInstance(0)
{
}

CHttpHelper::~CHttpHelper()
{
    curl_multi_cleanup(m_MURLHandle);
}

CHttpHelper* CHttpHelper::getInstance()
{
    if (NULL == m_Instance)
    {
        m_Instance = new CHttpHelper();
    }
    return m_Instance;
}

void CHttpHelper::destroy()
{
    if (NULL != m_Instance)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

bool CHttpHelper::init()
{
    if (m_MURLHandle == NULL)
    {
        m_MURLHandle = curl_multi_init();
    }
    return true;
}

size_t writeFun(void* ptr, size_t size, size_t nmemb, void *userdata)
{
    ((CHttpObject*)(userdata))->appendData((char*)ptr);
    return nmemb;
}

bool CHttpHelper::addHttpRequest(CHttpObject* obj)
{
    if (NULL == obj)
    {
        return false;
    }

    CURL *handle = curl_easy_init();
    // ����URL
    std::map<std::string, std::string>& gets = obj->getGetParams();
    if (gets.size() > 0)
    {
        std::string url = obj->getUrl();
        char buffer[128] = { 0 };
        char separator = '?';
        for (std::map<std::string, std::string>::iterator iter = gets.begin();
            iter != gets.end(); ++iter)
        {
            snprintf(buffer, sizeof(buffer), "%c%s=%s", separator, iter->first.c_str(), iter->second.c_str());
            url += buffer;
            separator = '&';
        }
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    }
    else
    {
        curl_easy_setopt(handle, CURLOPT_URL, obj->getUrl().c_str());
    }
    // ���ñ�
    std::map<std::string, std::string>& form = obj->getPostForm();
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    for (std::map<std::string, std::string>::iterator iter = form.begin();
        iter != form.end(); ++iter)
    {
        curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, iter->first.c_str(),
            CURLFORM_COPYCONTENTS, iter->second.c_str(),
            CURLFORM_END);
    }
    if (NULL != formpost)
    {
        curl_easy_setopt(handle, CURLOPT_HTTPPOST, formpost);
        // ����������������
        obj->setFormPost(formpost);
    }
    // ���ý��ջص�
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeFun);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, obj);
    // ��ӵ�m_MURLHandle��
    curl_multi_add_handle(m_MURLHandle, handle);
	curl_multi_perform(m_MURLHandle, &m_RunningInstance);
    m_RequestMap[handle] = obj;
    return true;
}

void CHttpHelper::update()
{
    if (m_RunningInstance > 0)
    {
        struct timeval timeout;
        int rc = -1;
        int msgs_in_queue;
        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        int maxfd = -1;
        //long curl_timeo = -1;
        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);
        timeout.tv_sec = 0;
        timeout.tv_usec = 10;

        // ��CURLM��ȡ��fdset��������maxfd
        curl_multi_fdset(m_MURLHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
        if (maxfd != -1)
        {
            // select�����Ƿ��ж�д�¼���������ʱ��select������������������timeout��ָ����ʱ��
            // �����ϣ��select���������Խ�timeout��������Ա����������Ϊ0������NULL��ʾһֱ����
            // �������ӿɶ����дʱ��select���������ؿɶ�д���ӵ������������ʱ�򷵻�0������-1��ʾ�쳣
            rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
        }
        else
        {
            // ���绹δ��ʼ���ã��´�����
			while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(m_MURLHandle, &m_RunningInstance));
            return;
        }

        // ����select�Ľ��������
		switch (rc)
		{
			// select �����˴���
		case -1:
			m_RunningInstance = 0;
			KXLOGERROR("select() returns error, this is badness\n");
			break;
			// �����ݿɶ����д������curl_multi_perform�������ݶ�д
			// curl_multi_perform�Ὣʣ������ִ�е����������still_running��
			// CURL������õĻص�
		case 0:
			return;
		default:
			while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(m_MURLHandle, &m_RunningInstance));
			break;
		}

        CURLMsg * msg = NULL;
        do
        {
            msg = curl_multi_info_read(m_MURLHandle, &msgs_in_queue);
            if (msg)
            {
                // ��ӡ��������������curl����
                KXLOGDEBUG("%d http request finish result %d ", msg->easy_handle, msg->data.result);
                std::map<CURL*, CHttpObject*>::iterator iter = m_RequestMap.find(msg->easy_handle);

                if (iter != m_RequestMap.end())
                {
                    CHttpObject* obj = iter->second;
                    // ִ�лص�
                    obj->processCallback(static_cast<int>(msg->data.result));
                    // ���������
                    if (obj->getFormPost() != NULL)
                    {
                        curl_formfree(obj->getFormPost());
                    }
                    m_RequestMap.erase(iter);
                    delete obj;
                }
                curl_multi_remove_handle(m_MURLHandle, msg->easy_handle);
                curl_easy_cleanup(msg->easy_handle);
            }
		} while (msg != NULL);
    }
}
