/*
 * ����˼���Socket Accept֮�󷵻ص�Socket����
 * ͨ���˶��������ͻ���ͨѶ
 * 
 *  2013-04-20 By ��ү
 *  
 */
#ifndef __TCPCLIENTER_H__
#define __TCPCLIENTER_H__

#include "KxTCPUnit.h"

namespace KxServer {

class KxTCPClienter : public KxTCPUnit
{
public:
	KxTCPClienter();
	virtual ~KxTCPClienter();

    virtual bool init(KXCOMMID fd);

	virtual void retain();

	virtual void release();

};

}

#endif
