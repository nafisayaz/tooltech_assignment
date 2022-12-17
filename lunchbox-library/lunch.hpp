

// #ifdef __cplusplus
// extern "C"
// {
// #endif

#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <algorithm>
#include <ctime>
#include <locale>
#include <iomanip>
#include <chrono>

using json = nlohmann::json;


enum DAYS{
    Sun,
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
    Sat,

};

class lunchHandler{
    public:
        static lunchHandler* get_instance(const std::string filename);
        json get_data()const;

        bool lunch_time();
        bool lunch_tm(const char* to_from);
        bool meeting_time();
        bool meeting_tm(std::string s_day, const char* to_from );
        bool is_meeting(long int currentTime);

    private:
        lunchHandler(){};
        lunchHandler(const std::string filename);
        static  lunchHandler* m_ptr;
        json    m_data;
        std::vector<std::tm> m_lunch;
        std::vector<std::tm> m_meeting;

};

lunchHandler* lunchHandler::m_ptr = nullptr;

class lunchBox{

    public:
        lunchBox();
        ~lunchBox();

        bool inform_meeting();
        bool set(const std::string current_time);

    private:
        std::tm     m_lunchtime{};
        std::tm     m_current_time{};

};

// #ifdef __cplusplus		
// }
// #endif
