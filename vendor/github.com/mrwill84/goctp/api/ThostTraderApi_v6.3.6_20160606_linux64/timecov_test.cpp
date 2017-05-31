#include "timecov.h"
#include <string>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <assert.h>
#include <sstream>
#include <cstdint>
#include <iostream>

int main(){
    std::string recv = format_time("2009-09-01","12:13:38");
    assert(recv == "2009-09-01 12:13:38");
    std::uint64_t i = UINT64_C(1251807218000000450);
    std::tm t;
    std::istringstream ss(recv);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    time_t t1 =utc_mktime(&t);
    std::cout<<"utc_mktime: " <<t1 <<std::endl;
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&t)+28800);
    std::uint64_t unixnano = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
    std::cout<<unixnano << " ?= " <<i <<std::endl;
    return i == unixnano;
}