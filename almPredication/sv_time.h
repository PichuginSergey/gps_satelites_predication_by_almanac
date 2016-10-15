#ifndef TIME_H_
#define TIME_H_

/* This class describes the current time */
class Time {

public:
	unsigned int get_year(void) const { return m_year; }
	unsigned int get_month(void) const { return m_month; }
	unsigned int get_day(void) const { return m_day; }
	unsigned int get_week_day(void) const { return m_week_day; }
			
	unsigned int get_hour(void) const { return m_hour; }
	unsigned int get_min(void) const { return m_min; }
	unsigned int get_sec(void) const { return m_sec; }
			
	void set_year(unsigned int year) {m_year = year; }
	void set_month(unsigned int month) {m_month = month; }
	void set_day(unsigned int day) {m_day = day; }
	void set_week_day(unsigned int week_day) {m_week_day = week_day; }

	void set_hour(unsigned int hour) {m_hour = hour; }
	void set_min(unsigned int min) {m_min = min; }
	void set_sec(unsigned int sec) {m_sec = sec; }

private:
	unsigned int m_year, m_month, m_day, m_week_day;
	unsigned int m_hour, m_min, m_sec;

};

extern void utc_to_gps(const Time& t, int *wk, double *sec);

#endif