
#include <iostream>
#include <memory>
#include "../lunchbox-library/lunch.hpp"

// ...
int main(int argc, char** argv){

    std::shared_ptr<lunchBox> l_ptr = std::make_shared<lunchBox>();
    l_ptr->set("2022-12-16T12:18:00Z");
    if(l_ptr->inform_meeting()){
        std::cout << "Meeting time\n";
    }else{
        std::cout << "Lunch time\n";
    }

}