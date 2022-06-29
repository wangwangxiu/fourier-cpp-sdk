
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "groupCommand.hpp"
#include "groupFeedback.hpp"
#include "lookup.hpp"

int main(int argc, char *argv[]) {
  // Try and get the requested group.
  std::shared_ptr<Fourier::Group> group;
  {
    std::string str("192.168.2.255");
    Fourier::Lookup lookup(&str);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    lookup.setLookupFrequencyHz(0);
    group = lookup.getGroupFromFamily("Default");
    if (!group) {
      std::cout << "No group found!" << std::endl;
      return -1;
    }
  }

  fourierSetLogLevel("ERROR");

  Fourier::GroupFeedback feedback(group->size());

  Fourier::GroupCommand group_command(group->size());

  std::vector<float> enable_status(group->size(),
                                   std::numeric_limits<float>::quiet_NaN());
  for (int i = 0; i < group->size(); ++i) {
    enable_status[i] = 1;
  }
  group_command.enable(enable_status);
  group->sendCommand(group_command);
  // std::this_thread::sleep_for(std::chrono::milliseconds(20));

  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> t(std::chrono::system_clock::now() - start);
  double duration = 1;
  std::vector<float> v_pos;
  std::vector<PosPtInfo> pos_pt_infos;
  while (t.count() < duration) {
    t = std::chrono::system_clock::now() - start;

    group->getNextFeedback(feedback);
    for (size_t mod_idx = 0; mod_idx < feedback.size(); ++mod_idx) {
      if (feedback[mod_idx]->position !=
          std::numeric_limits<float>::quiet_NaN()) {
        PosPtInfo info = {0};
        info.pos = feedback[mod_idx]->position + .1;
        pos_pt_infos.push_back(info);
        std::cout << "pos:" << feedback[mod_idx]->position << "  "
                  << "vel:" << feedback[mod_idx]->velocity << "  "
                  << "cur:" << feedback[mod_idx]->current << "  ";
      }
      std::cout << std::endl;
    }

    group_command.setInputPositionPt(pos_pt_infos);
    group->sendCommand(group_command);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    pos_pt_infos.clear();
  }

  for (int i = 0; i < group->size(); ++i) {
    enable_status[i] = 0;
  }
  group_command.enable(enable_status);
  group->sendCommand(group_command);

  return 0;
}
