#ifndef _REBOT_OP_CONFIG_H__
#define _REBOT_OP_CONFIG_H__

#include<vector>
#include<string>

#define REBOTOPCONFIG_PATH "..//RebotConfig//RebotOpConfig.csv"

struct SOpParamData
{
	int nParamType;				//��������
	int nLen;					//��������
	std::string szParam;		//��������

	SOpParamData()
	{
		nParamType = 0;
		nLen = 0;
		szParam.clear();
	}
};

struct SOpData
{
	int OpId;								//����ID
	int nMainCmd;							//����Ϣ
	int nSubCmd;							//����Ϣ
	std::vector<SOpParamData> VectParm;		//��������
};

class CRebotOpConfig
{
public:
	static CRebotOpConfig* getInstance();
	static void destroy();

	bool init();

	std::vector<SOpData>&	GetRebotOp();
protected:
	CRebotOpConfig();
	~CRebotOpConfig();

	bool LoadRebotOpConfig();

private:
	static CRebotOpConfig*	m_pInstance;
	std::vector<SOpData>	m_VecRebotOp;				//�����˲����б�
};


#endif //_REBOT_OP_CONFIG_H__
