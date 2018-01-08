#ifndef __TIMECALCTOOL_H__
#define __TIMECALCTOOL_H__

#include <time.h>

struct DayTime
{
	int hour;
	int minutes;
	int second;
};

struct MonthTime
{
	int month;
	int day;
	DayTime dayTime;
};

struct WeekTime
{
	int weekDay;
	DayTime dayTime;
};

struct YearTime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

class CTimeCalcTool
{
public:
	//��ѯʱ���������ʱ���Ƿ���ָ����ʱ��
	static bool isDayTimeOver(time_t prev, DayTime &dayTime);
	//��ѯʱ���������ʱ���Ƿ���ָ������ʱ��
	static bool isWeekTimeOver(time_t prev, WeekTime &weekTime);
	//��ѯʱ���������ʱ���Ƿ���ָ����ʱ��
	static bool isMonthTimeOver(time_t prev, MonthTime &month);
	//��ѯʱ���������ʱ���Ƿ����second��
	static bool isSecondOver(time_t prev, int second);
	//��ѯ�����Ƿ�Ϊָ����ʱ����
	static bool isDayTime(DayTime &beginTime, DayTime &endTime);
	//��ѯ�����Ƿ���ָ��������
	static bool isYearTime(YearTime &beginYearTime, YearTime &endYearTime);
	//��ѯ�Ƿ񵽵�, �����ؼ���
	static int overDay(time_t prev, DayTime &dayTime);
	//��ѯ�Ƿ�ָ����������, �����ؼ���
	static int overWeek(time_t prev, WeekTime &weekTime);
	//��ѯ�Ƿ�ָ������, �����ؼ���
	static int overSecond(time_t prev, int second);
    //����ĳ��ʱ������¸���ʱ���ֵ�ʱ���
    static int nextTimeStamp(time_t prev, int nextMin, int nextHour);
    //����ĳ��ʱ������ܼ���ʱ���ֵ�ʱ��� wDay��1~7
    static int nextTimeStamp(time_t prev, int nextMin, int nextHour, int wDay);
	//����prev��min��֮���ʱ���
	static int nextTimeStamp(time_t prev, int min);
	//����������賿min���ӵ�ʱ���
	static int nextTimeStampToZero(time_t prev, int min);
	//���ص�ǰʱ������ڵ���0��0�ֵ�ʱ���
	static int curTimeZero(time_t curt);
	//��ȡ��ǰ�ܵڼ���0��0�ֵ�ʱ���
	static int curDayTimeZero(int nDay);
};

#endif //__TIMECALCTOOL_H__
