#ifndef __TIMECOV_H__
#define __TIMECOV_H__

#include <string>

std::string format_time(const std::string& date, const std::string& time){
            std::string time_str(date);
            time_str +=" ";
            time_str += time;
            return time_str;
}

std::uint64_t utc_maketimesmape(const std::string& recv,int millic){
    std::tm t;
    std::istringstream ss(recv);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&t)+28800);
    std::uint64_t unixnano = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count()+millic*10;
    return unixnano;
}
#endif