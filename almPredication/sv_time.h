#ifndef TIME_H_
#define TIME_H_

/* This class describes the current time */
class Time {

public:
	unsigned int getYear(void) const { return year_; }
	unsigned int getMonth(void) const { return month_; }
	unsigned int getDay(void) const { return day_; }
	unsigned int getWeekDay(void) const { return weekOfDay_; }
			
	unsigned int getHour(void) const { return hour_; }
	unsigned int getMinute(void) const { return minute_; }
	unsigned int getSecond(void) const { return second_; }
			
	void setYear(unsigned int year) { year_ = year; }
	void setMonth(unsigned int month) { month_ = month; }
	void setDay(unsigned int day) { day_ = day; }
	void setWeekOfDay(unsigned int weekOfDay) { weekOfDay_ = weekOfDay; }

	void setHour(unsigned int hour) { hour_ = hour; }
	void setMinute(unsigned int minute) { minute_ = minute; }
	void setSecond(unsigned int second) { second_ = second; }

	static const unsigned int SECONDS_IN_DAY = 86400;
	static const unsigned int SECONDS_IN_HOUR = 3600;
	static const unsigned int SECONDS_IN_MINUTE = 60;
	static const unsigned int NUM_DAYS_IN_WEEK = 7;
	static const unsigned int SECONDS_IN_WEEK = NUM_DAYS_IN_WEEK*SECONDS_IN_DAY;

private:
	unsigned int year_{ 0 }, month_{ 0 }, day_{ 0 }, weekOfDay_{ 0 };
	unsigned int hour_{ 0 }, minute_{ 0 }, second_{ 0 };

};

extern void fromUtcToGpsTime(const Time& curTime, int *wk, double *seconds) noexcept;

#endif