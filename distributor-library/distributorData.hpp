

#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <locale>
#include <iomanip>
#include <chrono>

using json = nlohmann::json;

class DistributorData{
    public:
        static DistributorData* get_instance(const std::string filename);
        json get_data()const;


    private:
        DistributorData(){}
        DistributorData(const std::string filename);
        static  DistributorData* m_ptr;
        json    m_data;
        
};

DistributorData* DistributorData::m_ptr = nullptr;


class Scheduler{
    public:
        Scheduler(){};
        virtual ~Scheduler(){};

        std::vector<std::shared_ptr<Scheduler>> get_scheduler()const;
        virtual void push_back(std::shared_ptr<Scheduler> S_ptr);
        virtual void schedule(const std::string filename)=0;

    private:
        std::vector<std::shared_ptr<Scheduler>> m_Schedulers;
        
};

class S_distributor_t : public Scheduler{
    public:
        S_distributor_t(){};
        ~S_distributor_t (){};
        void push_back(std::shared_ptr<Scheduler> S_ptr)override;
        void schedule(const std::string name)override;

    private:
        std::tm                         m_start;
        std::tm                         m_end;
        std::string                     m_duration;
        std::string                     m_trans_id{};
        std::shared_ptr<Scheduler>      m_S_distributor;

};

class S_distributor : public Scheduler{
    public:
        S_distributor(){};
        ~S_distributor(){};
        void schedule(const std::string name)override;

    private:
        std::string     m_trans_id{};
        int             m_id{};
        int             m_transId{};
        int             m_total_limit{};
};


class Creator{
    public:
        Creator(){}
        virtual ~Creator(){}
        virtual std::shared_ptr<Scheduler> get_instance()=0;
};

class Creator_s_distributor : public Creator{
    public:
        Creator_s_distributor(){}
        ~Creator_s_distributor(){}
        std::shared_ptr<Scheduler>  get_instance()override;

};


class Creator_s_distributor_t : public Creator{
    public:
        Creator_s_distributor_t(){}
        ~Creator_s_distributor_t(){}
        std::shared_ptr<Scheduler> get_instance()override;

};

