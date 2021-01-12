#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#include <iostream>

// struct tm str_to_date(std::string time){
//     struct tm tm_s;

// }

// Converts datetime in format "1970-01-01" to UNIX Epoch
std::string date_to_epoch(std::string time){
    
    std::tm tm = {};
    std::stringstream ss(time);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    auto t = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    long p1 = std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count();
    std::string time_in_string = std::to_string(p1);

    std::cout << time_in_string << std::endl;;

    return time_in_string;
}