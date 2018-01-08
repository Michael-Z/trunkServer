/*
* ���ݻ���������
* �������л��ͷ����л�
*
* 2015-5-21 By ��ү
*/
#ifndef __BUFFERDATA_H__
#define __BUFFERDATA_H__

#include <stdio.h>
#include <memory.h>
#include <assert.h>

//���л�����һ��ֵ,�����л�����Ӧ��λ�ö�ȡ��ֵ,���ж��Ƿ�ƥ��,���ƥ����û�д�λ, ��ƥ�������Ϣ
//���л�һ��ֵ,��������ƥ��
#define SERIALIZE_MATCH(_buffData_)					\
{													\
	(_buffData_).writeData(12345678);				\
}
//���Է����л���ֵ�Ƿ�ƥ��
#define UN_SERIALIZE_MATCH(_buffData_)											\
{																				\
	int _readValue_ = 0;														\
	(_buffData_).readData(_readValue_);											\
	LOG("readValue= %d, File %s, Line %d", _readValue_, __FILE__, __LINE__);	\
	assert("�����л���ƥ��" && (12345678 ==_readValue_));							\
}

#define SERIALIZE_MATCH_VALUE(_buffData_, _value_)	\
{													\
	(_buffData_).writeData(_value_);				\
}
//���Է����л���ֵ�Ƿ�ƥ��
#define UN_SERIALIZE_MATCH_VALUE(_buffData_, _value_)							\
{																				\
	int _readValue_ = 0;														\
	(_buffData_).readData(_readValue_);											\
	LOG("readValue= %d, File %s, Line %d", _readValue_, __FILE__, __LINE__);	\
	assert("�����л���ƥ��" && (_value_ ==_readValue_));							\
}

class CBufferData
{
public:
    CBufferData();
    virtual ~CBufferData();

    bool init(int bufferSize);
    bool init(char* buffer, unsigned int dataLength);

    void clean();

    template<typename T>
    bool writeData(T data)
    {
        if (!checkBufferSize(sizeof(data)))
        {
            return false;
        }
        else
        {
            *reinterpret_cast<T*>(m_Buffer + m_DataLength) = data;
            m_DataLength += sizeof(data);
            return true;
        }
    }
    bool writeData(const void* data, unsigned int length);
    // ����һ��յ�buffer����Ҫ����ռλʹ��
    bool writeEmptyBuffer(unsigned int length);

    template<typename T>
    bool readData(T& data)
    {
        if (!checkDataLength(sizeof(data)))
        {
            return false;
        }

        data = *reinterpret_cast<T*>(m_Buffer + m_Offset);
        m_Offset += sizeof(data);
        return true;
    }
    bool readData(void* data, unsigned int dataSize);

    inline void resetOffset() { m_Offset = 0; }
    inline void updateOffset(unsigned int offset) { m_Offset = offset; }
    inline unsigned int getOffset() { return m_Offset; }
    inline unsigned int getDataLength() { return m_DataLength; }
    inline unsigned int getBufferSize() { return m_BufferSize; }

	char *getBuffer(){ return m_Buffer; };

private:
    bool checkDataLength(unsigned int dataLength);
    bool checkBufferSize(unsigned int newLength);

private:
    bool m_IsInit;              // ��ֹ�ظ���ʼ��
    bool m_IsReadMode;          // ��дģʽ
    unsigned int m_Offset;      // ��ǰ��ȡƫ��
    unsigned int m_DataLength;  // ��ǰʵ�����ݳ���
    unsigned int m_BufferSize;  // ��������С
    char* m_Buffer;
};

// �����л���ɷ����л��ӿ�
class ISerializable
{
    virtual bool serialize(CBufferData& data) = 0;
    virtual bool unserialize(CBufferData& data) = 0;
};

#endif
