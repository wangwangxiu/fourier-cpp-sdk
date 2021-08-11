
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "lookup.hpp"
#include "groupCommand.hpp"

int main() {
    // 广播地址
    std::string str("192.168.100.255");
    std::shared_ptr<Amber::Lookup> lookup = std::make_shared<Amber::Lookup>(&str);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lookup->setLookupFrequencyHz(0);
    auto group = lookup->getGroupFromFamily("Default");
    auto command = std::make_shared<Amber::GroupCommand>(group->size());
    
    std::cout << std::endl;
    
    auto entry_list = lookup->getEntryList();
    for (const auto &entry: *entry_list) {
        std::cout << "Name: " << entry.name_ << std::endl;
        std::cout << "Family: " << entry.family_ << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::vector<bool> v(group->size(), true);
    for (int i = 0; i < 10000; ++i) {
        command->setLatencyTest(v);
        group->sendCommand(*command);
    }
    

    return 0;
}
