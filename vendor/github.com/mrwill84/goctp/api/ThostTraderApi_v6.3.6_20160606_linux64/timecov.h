#ifndef __TIMECOV_H__
#define __TIMECOV_H__

#include <string>

std::string format_time(const std::string& date, const std::string& time){
            std::string time_str(date);
            time_str.insert(6,"-").insert(4,"-");
            time_str +="T";
            time_str += time;
            time_str +="+08:00";
            return time_str;
}

std::uint64_t utc_maketimesmape(const std::string& recv,int millic){
    std::tm t;
    strptime(recv.c_str(),"%Y-%m-%dT%H:%M:%S", &t);
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&t)+28800);
    std::uint64_t unixnano = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count()+millic*10;
    return unixnano;
}
std::uint64_t utc_now(){
    auto tp = std::chrono::system_clock::now();
    std::uint64_t unixnano = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count()+UINT64_C(28800)*UINT64_C(1000000000);
    return unixnano;
}
#endif