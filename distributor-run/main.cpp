

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

#include "../distributor-library/distributorData.hpp"

using json = nlohmann::json;


int main(){

    std::shared_ptr<Creator> ptr = std::make_shared<Creator_s_distributor>();
    std::shared_ptr<Scheduler>  S_ptr = ptr->get_instance();

    std::shared_ptr<Creator> ptr_t = std::make_shared<Creator_s_distributor_t>();
    std::shared_ptr<Scheduler> S_ptr_t = ptr_t->get_instance();
    S_ptr_t->push_back(S_ptr);
    
}

