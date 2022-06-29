
#include <math.h>
#include <string.h>

#include <iostream>
#include <thread>

#include "lookup.hpp"

using namespace Fourier;

int main() {
  std::string str("192.168.2.255");
  // After construction,start the background thread lookup actuator
  Lookup lookup(&str);

  // Wait 1 seconds for the module list to populate, and then print out its
  // contents
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // set lookup stop
  lookup.setLookupFrequencyHz(0);
  std::shared_ptr<Group> group = lookup.getGroupFromFamily("Default");

  std::cout << std::endl << "group size: " << group->size() << std::endl;

  auto entry_list = lookup.getEntryList();
  for (const auto &entry : *entry_list) {
    std::cout << "Name: " << entry.name_ << std::endl;
    std::cout << "Family: " << entry.family_ << std::endl;
  }

  return 0;
}
