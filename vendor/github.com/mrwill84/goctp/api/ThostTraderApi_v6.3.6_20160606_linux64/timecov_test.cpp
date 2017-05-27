#include "timecov.h"
#include <string>
#include <ctime>
#include <chrono>
#include <assert.h>
#include  <cstdint>

int main(){
    std::string recv = format_time("2009-09-01","12:13:38");
    assert(recv == "2009-09-01 12:13:38");
    uint64_t i = UINT64_C(1251807218000000450);
    std::tm t;
    std::istringstream ss(input);
    ss >> std::get_time(&t, "%Y-%m-%D %H:%M:%S");
    time_t t1 =utc_mktime(&t)
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    uint64 unixnano = std::chrono::duration_cast<std::chrono::nano>(tp.time_since_epoch()).count())
    return i == unixnano;
}