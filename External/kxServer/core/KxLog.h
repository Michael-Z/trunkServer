/*
*   ��־��¼��
*   ��ΪKxLogger�Լ�IKxLogHandler������
*   KxLogger������־�Ĺ��ˣ����ݿ����Լ����ã���������IKxLogHandler
*   IKxLogHandler����ʵ����־��Ϣ�Ĵ���������д�ļ������������̨������������ʾ��
*
*   2015-04-24 By ��ү
*/
#ifndef __KX_LOG_H__
#define __KX_LOG_H__

#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

#if KX_TARGET_PLATFORM == KX_PLATFORM_LINUX
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include <stdarg.h>
#endif

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// ��־����
enum KXLOG_LEVEL
{
    KXLOG_INFO,         // ��Ϣ����
    KXLOG_DEBUG,        // ���Լ���
    KXLOG_ERROR         // ���󼶱�
};

// ��־����������
// ʵ����־�Ĵ���
class IKxLogHandler
{
public:
    IKxLogHandler()
        :m_Tag(1)
    { }
    virtual ~IKxLogHandler() { }
    // �����¼��־ʱ�ص���������־��Ϣ
    virtual bool onLog(int level, const std::string& log) = 0;
    // ����ӽ�KxLoggerʱ�ص�
    virtual void onEnter() { }
    // ��KxLogger���Ƴ�ʱ�ص�
    virtual void onExit() { }
    // ���������־ʱ�ص�
    virtual void onClean() { }

    inline int getHandleId()
    {
        return m_HandlerId;
    }

    inline void setHandleId(int handleId)
    {
        m_HandlerId = handleId;
    }

    int getTag() 
    { 
        return m_Tag; 
    }

    inline void setTag(int tag)
    {
        m_Tag = tag;
    }

	virtual std::string getFileName()
	{
		return "";
	}

protected:
    int m_HandlerId;
    int m_Tag;
};

// ��־���󣨵�����
// ������־�Լ�IKxLogHandler���
class KxLogger
{
private:
    KxLogger();
    virtual ~KxLogger();

public:
    static KxLogger* getInstance();
    static void destroy();

    // ���һ����־������
    bool addHandler(int handleId, IKxLogHandler* handler);
    // ����handler�����handleId���һ����־������
    bool addHandler(IKxLogHandler* handler);
    // �Ƴ�һ����־������
    bool removeHandler(int handleId);
    // ����handleId����ȡһ����־������
    IKxLogHandler* getHandler(int id);
    // ������е���־������
    void clearHandler();

    // д��һ����־��������־���������д���
    void writelog(int tag, int level, const char* file, const char* func, int line, const char* log, ...);
    // �����־��ֻ�ǵ�����־��������onClean����
    void clearLog();

    // ������־���Լ���
    inline void setLogLevel(int lv) { m_LogLevel = lv; };
    inline int getLogLevel() { return m_LogLevel; }

    // �������ȡ��־����ʾ��Ϣ
    inline bool isShowLevel() { return m_LevelOption; }
    inline bool isShowTime() { return m_TimeOption; }
    inline bool isShowDate() { return m_DateOption; }
    inline bool isShowFile() { return m_FileOption; }
    inline bool isShowFunc() { return m_FuncOption; }
    inline bool isShowLine() { return m_LineOption; }
    inline void setShowLevel(bool b) { m_LevelOption = b; }
    inline void setShowTime(bool b) { m_TimeOption = b; }
    inline void setShowDate(bool b) { m_DateOption = b; }
    inline void setShowFile(bool b) { m_FileOption = b; }
    inline void setShowFunc(bool b) { m_FuncOption = b; }
    inline void setShowLine(bool b) { m_LineOption = b; }

	inline void HexDumpImp(const void *pdata, unsigned int len)
	{
		if (pdata == 0 || len == 0)
		{
			return;
		}

		int cnt = 0;
		int n = 0;
		int cnt2 = 0;
		const char *data = (const char *)pdata;
		IKxLogHandler* pHandle = getHandler(2);
		if (pHandle == NULL)
		{
			return;
		}
		std::string NetStatMem;
		NetStatMem = pHandle->getFileName() + "NetMem.log";
		ofstream fileos(NetStatMem.c_str(), ios::app);
		if (!fileos)return;
		fileos << "Address               Hexadecimal values                  Printable\n";
		cout << "Address               Hexadecimal values                  Printable\n";
		fileos << "-------  -----------------------------------------------  -------------\n";
		cout << "-------  -----------------------------------------------  -------------\n";
		fileos << "\n";
		cout << "\n";
		unsigned char buffer[20];
		unsigned int rpos = 0;

		while (1)
		{
			if (len <= rpos)
			{
				break;
			}
			if (len >= rpos + 16)
			{
				memcpy(buffer, data + rpos, 16);
				rpos += 16;
				cnt = 16;
			}
			else
			{
				memcpy(buffer, data + rpos, len - rpos);
				cnt = len - rpos;
				rpos = len;
			}
			if (cnt <= 0)
			{
				return;
			}

			fileos << setw(7) << (int)rpos << "  ";
			cout << setw(7) << (int)rpos << "  ";

			cnt2 = 0;
			for (n = 0; n < 16; n++)
			{
				cnt2 = cnt2 + 1;
				if (cnt2 <= cnt)
				{
					fileos << hex << setw(2) << setfill('0') << (unsigned int)buffer[n];
					cout << hex << setw(2) << setfill('0') << (unsigned int)buffer[n];
				}
				else
				{
					fileos << "  ";
					cout << "  ";
				}
				fileos << "  ";
				cout << " ";
			}

			fileos << setfill(' ');
			cout << setfill(' ');

			fileos << " ";
			cout << " ";
			cnt2 = 0;
			for (n = 0; n < 16; n++)
			{
				cnt2 = cnt2 + 1;
				if (cnt2 <= cnt)
				{
					if (buffer[n] < 32 || 126 < buffer[n])
					{
						fileos << '.';
						cout << '.';
					}
					else
					{
						fileos << buffer[n];
						cout << buffer[n];
					}
				}
			}
			fileos << "\n";
			cout << "\n";
			fileos << dec;
			cout << dec;
		}
		fileos.close();
		return;
	}

private:
    void updateFormat(int level, const char* log, const char* file, const char* func, int line);

private:
    int m_LogLevel;
    bool m_LevelOption;
    bool m_TimeOption;
    bool m_DateOption;
    bool m_FileOption;
    bool m_FuncOption;
    bool m_LineOption;
    std::string m_StringFormat;
    std::map<int, IKxLogHandler*> m_Handlers;
    static KxLogger* m_Instance;
};

#define KXLOG(tag, level, log, ...) KxLogger::getInstance()->writelog(tag, level, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGDEBUG(log, ...) KxLogger::getInstance()->writelog(1, KXLOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGINFO(log, ...) KxLogger::getInstance()->writelog(1, KXLOG_INFO, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGERROR(log, ...) KxLogger::getInstance()->writelog(1, KXLOG_ERROR, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGBATTLE(...)       do {} while (0)
//#define KXLOGBATTLE(log, ...) KxLogger::getInstance()->writelog(1<<1, KXLOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)

#endif
