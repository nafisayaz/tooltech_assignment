
#include <iostream>
#include "lunch.hpp"

lunchHandler* lunchHandler::get_instance(const std::string filename){
    if(!m_ptr){
        m_ptr = new lunchHandler(filename);
    }
    return m_ptr;
}

lunchHandler::lunchHandler(const std::string filename){
    std::ifstream f(filename);
    m_data = json::parse(f);
}

json lunchHandler::get_data()const{
    return m_data;
}

bool lunchHandler::lunch_tm(const char* to_from){

    auto now = std::chrono::system_clock::now();
    std::time_t t_m_t = std::chrono::system_clock::to_time_t(now);

    std::vector<std::string> v;
    boost::split(v,m_data["Lunch"][to_from].get<std::string>(),boost::is_any_of(":"));
    std::tm t_m = *localtime(&t_m_t);

    std::size_t pos{};
    try{
        t_m.tm_hour    =   std::stoi(v[0], &pos);
        t_m.tm_min     =   std::stoi(v[1], &pos);
        t_m.tm_sec     =   std::stoi(v[2], &pos);
    }
    catch(std::invalid_argument const& ex)
    {
        return false;
    }
    m_lunch.push_back(t_m);
    return true;

}

bool lunchHandler::lunch_time(){

    if(!lunch_tm("from")){
        return false;
    }
    if(!lunch_tm("to")){
        return false;
    }

    return true;


}

bool lunchHandler::meeting_tm(std::string s_day, const char* to_from ){

    auto now = std::chrono::system_clock::now();
    std::time_t t_m_t = std::chrono::system_clock::to_time_t(now);

    std::vector<std::string> v;
    boost::split(v,m_data["Meeting"][s_day][to_from].get<std::string>(),boost::is_any_of(":"));
    std::tm t_m = *localtime(&t_m_t);

    std::size_t pos{};

    try{
        t_m.tm_hour    =   std::stoi(v[0], &pos);
        t_m.tm_min     =   std::stoi(v[1], &pos);
        t_m.tm_sec     =   std::stoi(v[2], &pos);
    }
    catch(std::invalid_argument const& ex)
    {
        std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
        return false;
    }

    if(s_day == "Sunday"){
        t_m.tm_wday   = 0;
    }else if(s_day == "Monday"){
        t_m.tm_wday  = 1;
    }else if(s_day == "Tuesday"){
        t_m.tm_wday  = 2;
    }else if(s_day == "Wednesday"){
        t_m.tm_wday  = 3;
    }else if(s_day == "Thursday"){
        t_m.tm_wday  = 4;
    }else if(s_day == "Friday"){
        t_m.tm_wday  = 5;
    }else if(s_day == "Saturday"){
        t_m.tm_wday  = 6;
    }else{
        return false;
    }

    m_meeting.push_back(t_m);
    
    return true;

}
bool lunchHandler::meeting_time(){

    auto now = std::chrono::system_clock::now();
    std::time_t n = std::chrono::system_clock::to_time_t(now);

    for(auto&[key, value] : m_data["Meeting"].items()){
        if(!meeting_tm(key, "from")){
            return false;
        }
        if(!meeting_tm(key, "to")){
            return false;
        }
    }
    
    return true;

}
bool lunchHandler::is_meeting(long int currentTime){
    
    for(int i=0; i<m_meeting.size(); i+=2){
        if( currentTime >= std::mktime(&m_meeting[i])  and currentTime <= std::mktime(&m_meeting[i+1]) ){
            return true;
        }
    }
    return false;
}



lunchBox::lunchBox(){
    auto now = std::chrono::system_clock::now();
    std::time_t n = std::chrono::system_clock::to_time_t(now);
    m_current_time = *localtime(&n);

    std::string str(ctime(&n));
    std::vector<std::string> v_time;
    boost::split(v_time,str,boost::is_any_of(" "));

    lunchHandler* handle = lunchHandler::get_instance("../data/lunchtime.json");
    json data = handle->get_data();
    handle->lunch_time();
    handle->meeting_time();

    std::string strTime{};


}
lunchBox::~lunchBox(){}


bool lunchBox::set(const std::string current_time){

    std::istringstream ss(current_time);
    if (ss >> std::get_time(&m_current_time, "%Y-%m-%dT%H:%M:%S")){
            return true;
    }
    return false;
}
bool lunchBox::inform_meeting(){
    
    lunchHandler* handle = lunchHandler::get_instance("../data/lunchtime.json");
    json data = handle->get_data();

    
    std::tm v_from = {};
    std::tm v_to = {};
    long int v_from_int = 0;
    long int v_to_int = 0;

    std::istringstream ss_v_from(data["Valid"]["from"].get<std::string>());
    std::istringstream ss_v_to(data["Valid"]["to"].get<std::string>());
    if (ss_v_from >> std::get_time(&v_from, "%Y-%m-%dT%H:%M:%S")){
        
        v_from_int = std::mktime(&v_from);

    }
    if (ss_v_to >> std::get_time(&v_to, "%Y-%m-%dT%H:%M:%S")){
        
        v_to_int = std::mktime(&v_to);
    }

    if( std::mktime(&m_current_time) >= v_from_int  and std::mktime(&m_current_time)  <= v_to_int ){
        if(handle->is_meeting(std::mktime(&m_current_time))){
            return true;
        }else{
            return false;
        }

    }else{
        return false;
    }

    return false;
}        
