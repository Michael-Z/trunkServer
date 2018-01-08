#ifndef __BASE_MODULE_H__
#define __BASE_MODULE_H__

#include "KXServer.h"

class CBaseModule : public KxServer::IKxModule
{
public:
    CBaseModule(void){}
    virtual ~CBaseModule(void){}
    
    virtual int processLength(char* buffer, unsigned int len)
    {
        if (len < sizeof(int))
        {
            //len����Ϊ4���ֽ�, �ϲ�����������4���ֽ���ȴ�������.
            return sizeof(int);
        }
        else
        {
            //KXLOGDEBUG("CBaseModule Request Len %d", *(int*)(buffer));
            return *(int*)(buffer);
        }
    }
};

#endif
