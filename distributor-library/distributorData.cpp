
#include "distributorData.hpp"

DistributorData* DistributorData::get_instance(const std::string filename){
    if(!m_ptr){
        m_ptr = new DistributorData(filename);
    }
    return m_ptr;
}

json DistributorData::get_data()const{
    return m_data;
}

DistributorData::DistributorData(const std::string filename){
    std::ifstream f(filename);
    m_data = json::parse(f);
}


std::vector<std::shared_ptr<Scheduler>> Scheduler::get_scheduler()const{
    return m_Schedulers;
}

void Scheduler::push_back(std::shared_ptr<Scheduler> S_ptr_t){
    m_Schedulers.push_back(S_ptr_t);
}

void S_distributor::schedule(const std::string trans_id){
    m_trans_id = trans_id;
	m_id = std::rand();
    m_transId = std::rand();
    m_total_limit = 1 + std::rand()/((RAND_MAX)/100000);

    DistributorData * d_ptr = DistributorData::get_instance("../data/distributor_data.json");
    json data = d_ptr->get_data();

    auto stack = data["Stack"]["option1"].get<int>();
    auto houseId = data["HouseID"]["id"].get<std::vector<int>>();
    auto supply = data["Supply"]["option2"].get<std::string>();
    auto limit = data["Limit"]["value"].get<int>();
    auto type = data["Limit"]["type"].get<std::string>();


    json output_data;
    output_data["ProfileName"] = m_trans_id;
    output_data["ProfileId"] = m_id;
    output_data["ScheduleId"] = m_transId;

    std::vector<int> out_limit;
    for(int l = 0; l<stack and m_total_limit >=limit; l++){
        out_limit.push_back(limit);
        m_total_limit -= limit;
    }
    output_data["HouseId"] = m_transId;
    output_data["supplyLimit"] = out_limit;

    std::freopen("../data/distributor_output.json", "w", stdout);
    std::cout << output_data;

}

void S_distributor_t::schedule(const std::string trans_id){
    auto now = std::chrono::system_clock::now();
    std::time_t t_m_now = std::chrono::system_clock::to_time_t(now);
    m_start = *localtime(&t_m_now);

    m_trans_id = trans_id;
    m_S_distributor->schedule(trans_id);

    auto end = std::chrono::system_clock::now();
    std::time_t t_m_end = std::chrono::system_clock::to_time_t(now);
    m_end = *localtime(&t_m_end);

    std::chrono::duration<double> diff = end - now;

    m_duration = boost::lexical_cast<std::string>(diff.count());

}

void S_distributor_t::push_back(std::shared_ptr<Scheduler> S_ptr){
    m_S_distributor = std::move(S_ptr);

    schedule(boost::lexical_cast<std::string>(1 + std::rand()/((RAND_MAX)/10000)));
}


std::shared_ptr<Scheduler> Creator_s_distributor::get_instance(){
    return std::shared_ptr<Scheduler>(new S_distributor());
    
}

std::shared_ptr<Scheduler> Creator_s_distributor_t::get_instance(){
    return std::shared_ptr<Scheduler>(new S_distributor_t());
}

