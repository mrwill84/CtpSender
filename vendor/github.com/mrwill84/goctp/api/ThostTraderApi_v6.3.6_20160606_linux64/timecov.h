#ifndef __TIMECOV_H__
#define __TIMECOV_H__

#include <string>

std::string format_time(const std::string& date, const std::string& time){
            std::string time_str(date);
            time_str +=" ";
            time_str += time;
           // char buf[16]={0};
           // time_str += sprintf(buf,".4f",UpdateMillisec/1000.0)
            return time_str;
}
 time_t utc_mktime(struct tm *t){
            return mktime(t) + timezone;
} 
#endif