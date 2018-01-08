#ifndef __HTTP_HELPER_H__
#define __HTTP_HELPER_H__

#include <map>
#include <string>
#include "curl/curl.h"

class CHttpObject
{
public:
    CHttpObject()
        : m_FormPost(NULL)
    {
    }
    virtual ~CHttpObject(){}
    inline void setUrl(const std::string& url)
    {
        m_Url = url;
    }
    inline std::string getUrl() { return m_Url; }
    // ������get��ʽ�ύ�Ĳ���
    inline void setGetParam(const std::string& k, const std::string& v)
    {
        m_Gets[k] = v;
    }
    inline std::map<std::string, std::string>& getGetParams() { return m_Gets; }
    // ����Ҫ�ύ�ı�
    inline void setPostForm(const std::string& k, const std::string& v)
    {
        m_PostForm[k] = v;
    }
    inline std::map<std::string, std::string>& getPostForm() { return m_PostForm; }
    void setFormPost(struct curl_httppost* formpost) { m_FormPost = formpost; }
    struct curl_httppost* getFormPost() { return m_FormPost; }
    inline void appendData(const char* data) { m_Buffer += data; }
    // ������ɺ�Ļص���code Ϊ 0��ʾ��ȷ��������ʾ�쳣
    virtual void processCallback(int code) = 0;

protected:
    struct curl_httppost* m_FormPost;
    std::string m_Buffer;                           // ���յ�������
    std::string m_Url;                              // Ҫ�����URL
    std::map<std::string, std::string> m_PostForm;  // Ҫ�ύ�ı�
    std::map<std::string, std::string> m_Gets;      // Ҫ�ύ��get����
};

class CHttpHelper
{
private:
    CHttpHelper();
    virtual ~CHttpHelper();

public:
    static CHttpHelper* getInstance();
    static void destroy();

    bool init();
    // ���http����
    bool addHttpRequest(CHttpObject* obj);
    void update();

private:
    CURLM* m_MURLHandle;
    int m_RunningInstance;
    std::map<CURL*, CHttpObject*> m_RequestMap;
    static CHttpHelper* m_Instance;
};

#endif
