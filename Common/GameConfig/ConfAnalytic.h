#ifndef _SUM_CONFIG_ANALYTIC_
#define _SUM_CONFIG_ANALYTIC_

#include "KxCSComm.h"

//����������
enum ConfigItemType
{
	CIT_UNVALID,		//��Ч
	CIT_STRING,			//�ַ���
	CIT_INT,			//����
	CIT_BOOL,			//����
	CIT_FLOAT,			//������
	CIT_PERCENT,		//�ٷֱ�
	CIT_JSON,			//����
	CIT_COUNT,			//����
	CIT_FUNCTION		//����
};

//��������
struct CDataFunction
{
	CDataFunction(){ NameID = 0; }
	int					NameID;
	VecInt				ParamInt;
	VecFloat			ParamFloat;
	VecVecInt			ParamIntArr;
	VecVecFloat			ParamFloatArr;
};

struct MusicInfo
{
    MusicInfo()
    {
        MusicDelay = 0.0f;
        MusicId = 0;
        IsClose = false;
        Volume = 1.0f;
        Track = 0.0f;
    }

    float MusicDelay;    // �����ӳ�
    int MusicId;         // ����ID
    bool IsClose;         // ѭ��
    float Volume;        // ����
    float Track;         // ����
};

typedef std::vector<MusicInfo> VecMusicInfos;

//���������εĽṹ��
struct ID_Num
{
    int                 ID;          //ID
    int                 num;         //����
};

struct AwardInfo
{
    int	ID;			 //������ƷID
    int	Num;		 //������Ʒ����
};

struct TimeInfo
{
	int Week;		 //��
	int Hour;		 //ʱ
	int Min;		 //��
	int Sec;		 //��
};

//���ݽ���
class CConfAnalytic
{
public:
	static int			ToInt(const std::string& str);
	static bool			ToBool(const std::string& str);
	static float		ToFloat(const std::string& str);
	static float		ToPercent(const std::string& str);
	static void			ToJsonInt(const std::string& str, VecInt& ret);
	static void			ToJsonFloat(const std::string& str, VecFloat& ret);
	static void			ToJsonStr(const std::string& str, std::vector<std::string>& ret);
    static void         ToVecMusicInfos(const std::string& str, VecMusicInfos& ret);
	static void			ToVec2(const std::string& str, Vec2& ret);
	static void			ToVecVecInt(const std::string& str, VecVecInt& ret);

    static void         ToJsonID_Num(const std::string& str, std::vector<ID_Num> &id_num);
    static void         ToAward(std::string str, std::vector<AwardInfo>& vec);
	static void			ToTime(const std::string& str, int type, TimeInfo& info);

#ifndef RunningInServer
	static void			ToColor3B(const std::string& str, Color3B& ret);
#endif 
	static void			ToFunction(const std::string& str, CDataFunction** fun);

public:
	static int			JudgeDataType(const std::string& str);
	static void			StringSplit(std::string src, std::vector<std::string>& ret, const std::string& split);
	static void			Singo(const std::string& param, CDataFunction* data);
	static void			Multi(const std::string& param, CDataFunction* data);
};

#endif