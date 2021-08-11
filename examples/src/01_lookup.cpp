
#include <iostream>
#include <thread>
#include <math.h>
#include "lookup.hpp"

using namespace Amber;

int main() {
    Lookup lookup;

    lookup.setLookupFrequencyHz(0);

    std::shared_ptr<Group> group = lookup.getGroupFromFamily("Default");
    
    std::cout << std::endl;
    
    auto entry_list = lookup.getEntryList();
    for (const auto &entry: *entry_list) {
        std::cout << "Name: " << entry.name_ << std::endl;
        std::cout << "Family: " << entry.family_ << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
