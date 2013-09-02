#ifndef  Z_DATETIME_H
#define  Z_DATETIME_H

#include <time.h> 

typedef struct 
{
    int  year;
    int  month;
    int  day;
}  Date;

typedef struct
{
    int  hour;
    int  minute;
    int  second;
} Time;

typedef struct
{
    int   year;
    int   month;
    int   day;
    int   hour;
    int   minute;
    int   second;
    int   weekday;
    Date  date;
    Time  time;
}  Datetime;

static struct tm*  z_localtime()
{
    time_t  time_stamp = time(NULL);
    struct tm* loc_time = localtime(&time_stamp);
    
    struct tm* temp = (struct tm*)malloc(sizeof(struct tm));
     
    memcpy(temp, loc_time, sizeof(struct tm));
    
    return  temp;
}

static int  count_days(Date date)
{
    int days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int year = date.year;
        
    int count = (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
    
    count += days[date.month - 1];
    
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
       if (date.month > 2)
       {
           ++count;
       }
    }
    
    count += date.day;
    
    return  count;
}

static int  get_weekday_by_date(Date date)
{
    int count = count_days(date) % 7;
    
    return count == 0 ? 7 : count;
}

static Date  get_date()
{
    Date  date;
    
    struct tm* datetime = z_localtime();
    
    date.year  = datetime->tm_year + 1900;
    date.month = datetime->tm_mon + 1;
    date.day   = datetime->tm_mday;
    
    free(datetime);
    
    return date; 
}

static Time get_time()
{
    Time  time;
    
    struct tm* datetime = z_localtime();
    
    time.hour   = datetime->tm_hour;
    time.minute = datetime->tm_min;
    time.second = datetime->tm_sec;
    
    free(datetime);
    
    return  time; 
}

static Datetime get_datetime()
{
    Datetime  datetime;
    
    struct tm* ldatetime = z_localtime();
    
    datetime.year    = ldatetime->tm_year + 1900;
    datetime.month   = ldatetime->tm_mon + 1;
    datetime.day     = ldatetime->tm_mday;
    datetime.hour    = ldatetime->tm_hour;
    datetime.minute  = ldatetime->tm_min;
    datetime.second  = ldatetime->tm_sec;
    datetime.date    = get_date();
    datetime.time    = get_time();
    datetime.weekday = get_weekday_by_date(datetime.date);
    
    free(ldatetime);
    
    return  datetime;
}

static int  get_year()
{
    Date  date = get_date();
    
    return date.year;
}

static int get_month()
{
    Date  date = get_date();
    
    return date.month;
}

static int  get_day()
{
    Date  date = get_date();
    
    return date.day;
}

static int  get_hour()
{
    Time  time = get_time();
    
    return time.hour;
}

static int  get_minute()
{
    Time  time = get_time();
    
    return time.minute;
}

static int  get_second()
{
    Time  time = get_time();
    
    return time.second;
}

static int  get_weekday()
{
     Datetime datetime = get_datetime();
     
     return  datetime.weekday;
}

static int diff_date(Date  date1, Date date2)
{
     int  count1 = 364 * date1.year + count_days(date1);
     int  count2 = 364 * date2.year + count_days(date2);
     
     return  count1 - count2;
}

static int make_timestamp(Datetime datetime)
{
     struct tm* tm_ptr = (struct tm*)malloc(sizeof(struct tm));
     
     tm_ptr->tm_year = datetime.year - 1900;
     tm_ptr->tm_mon  = datetime.month - 1;
     tm_ptr->tm_mday = datetime.day;
     tm_ptr->tm_hour = datetime.hour;
     tm_ptr->tm_min  = datetime.minute;
     tm_ptr->tm_sec  = datetime.second;
     
     time_t  timestamp = mktime(tm_ptr);
     
     free(tm_ptr);
     
     return  timestamp;
}

#endif
